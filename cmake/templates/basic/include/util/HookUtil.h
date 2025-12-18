#pragma once

#if defined(SKSE_SUPPORT_XBYAK)

#include <xbyak/xbyak.h>

namespace stl {

    class Hook {

    public:
        /// <summary>
        /// Registers a short call/jump hook at the specified address.
        /// Automatically detects instruction size (5 or 6 bytes) and stores trampoline in T::func,
        /// redirecting execution to T::thunk.
        /// </summary>
        /// <typeparam name="T">Type containing the trampoline (func) and thunk pointer.</typeparam>
        /// <param name="a_target">Address of the target instruction to hook.</param>
        template <class T>
        static void call(const std::uintptr_t a_target)
        {
            m_entries.emplace_back(make_entry<T>(a_target, detect_instruction_size(a_target)));
        }

        /// <summary>
        /// Registers a function hook for the specified target function.
        /// Detects if the function has already been hooked:
        /// - If a standard call/jump is present, chains the hook on top of existing hooks.
        /// - Otherwise, enables manualPatch to copy the full prologue and branch to T::thunk.
        /// Stores the trampoline in T::func.
        /// </summary>
        /// <typeparam name="T">Type containing the trampoline (func) and thunk pointer.</typeparam>
        /// <param name="a_target">Address of the target function.</param>
        /// <param name="a_prologueSize">Number of bytes to copy if manualPatch is required.</param>
        template <class T>
        static void function(const std::uintptr_t a_target, const std::size_t a_prologueSize)
        {
            // Detect if the function has already been hooked.
            auto detectSize = detect_instruction_size(a_target);

            m_entries.emplace_back(Entry{
                a_target,
                reinterpret_cast<std::uintptr_t*>(&T::func),
                detectSize ? detectSize : a_prologueSize,
                false,
                detectSize ? false : true,
                reinterpret_cast<void*>(&T::thunk)
            });
        }

        /// <summary>
        /// Registers an inline patch at the specified address.
        /// Executes custom patch code in T::thunk without allocating a trampoline.
        /// </summary>
        /// <typeparam name="T">Type containing the thunk pointer to execute the patch.</typeparam>
        /// <param name="a_target">Address of the instruction block to patch.</param>
        /// <param name="a_patchSize">Size of the code block to patch.</param>
        template <class T>
        static void patch(std::uintptr_t a_target, std::size_t a_patchSize)
        {
            m_entries.emplace_back(Entry{
                a_target,
                nullptr,
                a_patchSize,
                false,
                false,
                nullptr,
                &T::thunk
            });
        }

        /// <summary>
        /// Replaces the instruction block at the specified address with a direct jump to T::func.
        /// Automatically fills the overwritten region with INT3 and ensures the jump fits.
        /// </summary>
        /// <typeparam name="T">Type containing the trampoline pointer (func).</typeparam>
        /// <param name="a_target">Start address of the instructions to replace.</param>
        /// <param name="a_replaceSize">Number of bytes in the original block to overwrite.</param>
        template <class T>
        static void replace(const std::uintptr_t a_target, const std::size_t a_replaceSize)
        {
            REL::safe_fill(a_target, REL::INT3, a_replaceSize);

            struct Patch : Xbyak::CodeGenerator
            {
                Patch(std::uintptr_t a_dst) : Xbyak::CodeGenerator(64)
                {
                    mov(rax, a_dst);
                    jmp(rax);
                }
            };
            Patch patch{reinterpret_cast<std::uintptr_t>(T::func)};
            patch.ready();

            const auto patchSize = patch.getSize();
            assert(patchSize <= a_replaceSize && "replace: patch too large for overwritten region");

            std::span<const std::uint8_t> patchSpan{ reinterpret_cast<const std::uint8_t*>(patch.getCode()), patchSize };
            REL::safe_write(a_target, patchSpan);
        }

        /// <summary>
        /// Hooks a virtual function by replacing an entry in a vtable.
        /// Stores the original trampoline in T::func and increments the virtual hook count.
        /// </summary>
        /// <typeparam name="F">Class type containing the target vtable.</typeparam>
        /// <typeparam name="T">Type containing the trampoline (func) and thunk pointer.</typeparam>
        /// <param name="a_tableIndex">Index of the vtable to patch.</param>
        /// <param name="a_functionIndex">Index of the virtual function to hook.</param>
        template <class F, class T>
        static void virtual_function(const std::size_t a_tableIndex, const std::size_t a_functionIndex)
        {
            REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[a_tableIndex] };
            T::func = vtbl.write_vfunc(a_functionIndex, T::thunk);
            m_vTableCount++;
        }

        /// <summary>
        /// Returns the number of registered non-virtual hooks.
        /// </summary>
        /// <returns>Number of non-virtual hooks in the hook list.</returns>
        static std::size_t count() noexcept
        {
            return m_entries.size();
        }

        /// <summary>
        /// Returns the number of registered virtual function hooks.
        /// </summary>
        /// <returns>Number of virtual function hooks installed.</returns>
        static std::size_t virtualCount() noexcept
        {
            return m_vTableCount;
        }

        /// <summary>
        /// Installs all registered hooks and patches in the order they were added.
        /// Allocates a single contiguous trampoline block for all hooks that require it.
        /// Automatically handles:
        /// - Call/jump hooks with auto-detected instruction size
        /// - Manual prologue patches
        /// - Inline patches using handlers
        /// After installation, all entries are cleared to prevent reinstallation.
        /// </summary>
        static void install()
        {
            auto& trampoline = SKSE::GetTrampoline();
            std::size_t totalSize{ 0 };

            for (auto& entry : m_entries) {

                if (!entry.func && !entry.handler) {
                    continue;
                }

                if (!entry.manualPatch && !entry.handler) {
                    totalSize += TRAMPOLINE_OVERHEAD;
                }
                else {
                    totalSize += entry.size + TRAMPOLINE_OVERHEAD;
                }
            }

            if (totalSize > 0) {
                SKSE::AllocTrampoline(totalSize);
            }
            else {
                m_entries.clear();
                return;
            }

            // Install each hook
            for (auto& entry : m_entries) {

                if (entry.handler) {
                    entry.handler(entry.target);
                    continue;
                }

                if (!entry.func) {
                    continue;
                }

                if (!entry.manualPatch) {

                    if (entry.isCall) {
                        if (entry.size == 5) *entry.func = trampoline.write_call<5>(entry.target, entry.thunk);
                        else if (entry.size == 6) *entry.func = trampoline.write_call<6>(entry.target, entry.thunk);
                    }
                    else {
                        if (entry.size == 5) *entry.func = trampoline.write_branch<5>(entry.target, entry.thunk);
                        else if (entry.size == 6) *entry.func = trampoline.write_branch<6>(entry.target, entry.thunk);
                    }
                }
                else {
                    struct Patch : Xbyak::CodeGenerator
                    {
                        Patch(const std::uintptr_t a_src, const std::size_t a_copyBytes)
                        {
                            auto p = reinterpret_cast<const std::uint8_t*>(a_src);
                            for (size_t i = 0; i < a_copyBytes; i++) {
                                db(p[i]);
                            }

                            jmp(ptr[rip]);
                            dq(a_src + a_copyBytes);
                        }
                    };
                    Patch patch(entry.target, entry.size);
                    patch.ready();

                    auto alloc = trampoline.allocate(patch.getSize());
                    std::memcpy(alloc, patch.getCode(), patch.getSize());
                    *entry.func = reinterpret_cast<std::uintptr_t>(alloc);
                    // Install branch from original to thunk
                    trampoline.write_branch<5>(entry.target, entry.thunk);
                }
            }
            m_entries.clear();
        }

    private:
        struct Entry
        {
            /// <summary>
            /// Address of the original function or instruction block to hook or patch.
            /// </summary>
            std::uintptr_t target{};

            /// <summary>
            /// Pointer to the trampoline storage (T::func) for call/jump hooks.
            /// For patches, this is nullptr.
            /// </summary>
            std::uintptr_t* func{};

            /// <summary>
            /// Number of bytes to copy for a manual patch, or allocated trampoline size for inline patches.
            /// </summary>
            std::size_t size{};

            /// <summary>
            /// True if the hook is a call instruction; false if it is a jump.
            /// Only relevant for auto-detected call/jump hooks.
            /// </summary>
            bool isCall{};

            /// <summary>
            /// True if the hook requires manually copying the prologue before branching to the thunk.
            /// Typically enabled if instruction size detection fails or chain hooking is needed.
            /// </summary>
            bool manualPatch{};

            /// <summary>
            /// Pointer to the thunk function to redirect execution to.
            /// For patch hooks, this may be nullptr.
            /// </summary>
            void* thunk{};

            /// <summary>
            /// Optional handler invoked for inline patches or custom code replacement.
            /// Receives the target address as a parameter.
            /// </summary>
            std::function<void(std::uintptr_t)> handler{};
        };

        static constexpr std::size_t TRAMPOLINE_OVERHEAD = 14;
        static inline std::vector<Entry> m_entries;
        static inline std::size_t m_vTableCount{ 0 };

        template <class T>
        static Entry make_entry(const std::uintptr_t a_target, const std::size_t a_size)
        {
            return Entry{
                a_target,
                reinterpret_cast<std::uintptr_t*>(&T::func),
                a_size,
                is_call_opcode(a_target),
                !(a_size == 5 || a_size == 6),
                reinterpret_cast<void*>(&T::thunk)
            };
        }

        /// <summary>
        /// Detects the size (in bytes) of the instruction at the given address, specifically for 
        /// call/jump instructions. Returns 5 or 6 for recognized CALL/JMP opcodes, or 0 if the 
        /// instruction is not a recognized call/jump.
        /// </summary>
        /// <param name="a_target">The address of the instruction to inspect.</param>
        /// <returns>The size of the instruction in bytes, or 0 if unknown.</returns>
        static std::size_t detect_instruction_size(const std::uintptr_t a_target)
        {
            const auto p = reinterpret_cast<const std::uint8_t*>(a_target);
            std::uint8_t opcode = *p;

            switch (opcode) {
                case 0xE8: // relative CALL
                case 0xE9: // relative JMP
                    return 5;

                case 0xFF: {
                    std::uint8_t modrm = *(p + 1);
                    std::uint8_t reg = (modrm >> 3) & 0x7;
                    std::uint8_t mod = (modrm >> 6) & 0x3;

                    // Memory indirect CALL/JMP
                    if ((reg == 2 || reg == 4) && mod != 0b11) return 6;
                    if (modrm == 0x15 || modrm == 0x25) return 6;
                    break;
                }
            }
            return 0;
        }

        /// <summary>
        /// Determines if the instruction at the given address is a CALL instruction.
        /// Supports relative CALL/JMP and memory-indirect forms.
        /// </summary>
        /// <param name="a_target">The address of the instruction to inspect.</param>
        /// <returns>True if the instruction is a CALL; false if a JMP or unrecognized.</returns>
        static bool is_call_opcode(const std::uintptr_t a_target)
        {
            const auto p = reinterpret_cast<const std::uint8_t*>(a_target);
            std::uint8_t opcode = *p;

            switch (opcode) {
                case 0xE8: return true;
                case 0xE9: return false;

                case 0xFF: {
                    std::uint8_t modrm = *(p + 1);
                    std::uint8_t reg = (modrm >> 3) & 0x7;

                    if (reg == 2) return true;
                    if (reg == 4) return false;
                    if (modrm == 0x15) return true;
                    if (modrm == 0x25) return false;
                    break;
                }
            }
            return false;
        }
    };
}

#endif
