#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <PROXY/Proxies.h>

#include <REX/REX/Singleton.h>

#include <xbyak/xbyak.h>

// Additional headers
#include <Psapi.h>

using namespace std::literals;

namespace stl {

	using namespace SKSE::stl;

	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);

	template <class T>
	void asm_replace(std::uintptr_t a_from)
	{
		asm_replace(a_from, T::size, reinterpret_cast<std::uintptr_t>(T::func));
	}

	template <class T>
	void write_prologue_call(const std::uintptr_t a_src, const std::size_t a_copyBytes)
	{
		auto& trampoline = SKSE::GetTrampoline();
		const auto opCode = *reinterpret_cast<const std::uint8_t*>(a_src);

		if (opCode == 0xE8) {
			SKSE::AllocTrampoline(0xE);
			T::func = trampoline.write_call<5>(a_src, T::thunk);
			return;
		}
		else if (opCode == 0xE9) {
			SKSE::AllocTrampoline(0xE);
			T::func = trampoline.write_branch<5>(a_src, T::thunk);
			return;
		}
		// FIX ME: write_call/branch 6.
		else if (opCode == 0xFF) {
			auto address = a_src - REL::Module::get().base();
			std::wstring programName = REL::Module::get().filename().data();

			HMODULE hModule = nullptr;
			wchar_t modName[MAX_PATH]{};
			GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCWSTR>(_ReturnAddress()), &hModule);
			GetModuleBaseNameW(GetCurrentProcess(), hModule, modName, MAX_PATH);

			std::wstring msg = std::format(L"Unable to hook:\n    {}+{:07X}"sv, programName.c_str(), address);
			MessageBoxW(nullptr, msg.c_str(), modName, MB_ICONWARNING | MB_OK);
			return;
		}
		else {
			struct Patch : Xbyak::CodeGenerator {

				Patch(const std::uintptr_t src, const std::size_t copyBytes)
				{
					for (size_t i = 0; i < copyBytes; i++) {
						db(*reinterpret_cast<std::uint8_t*>(src + i));
					}
					jmp(qword[rip]);
					dq(src + copyBytes);
				}
			};
			Patch patch(a_src, a_copyBytes);
			patch.ready();

			SKSE::AllocTrampoline(patch.getSize() + 0xE);
			trampoline.write_branch<5>(a_src, T::thunk);

			const auto alloc = trampoline.allocate(patch.getSize());
			std::memcpy(alloc, patch.getCode(), patch.getSize());
			T::func = reinterpret_cast<std::uintptr_t>(alloc);
		}
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(0xE);
		T::func = trampoline.write_call<T::size>(a_src, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[T::vTableIndex] };
		T::func = vtbl.write_vfunc(T::vFunctionIndex, T::thunk);
	}

#if defined(SKYRIM_SUPPORT_NG)
	#define RELOCATION_OFFSET(a_se, a_ae) REL::VariantOffset(a_se, a_ae, 0x0).offset()
#else
	#if defined(SKYRIM_SUPPORT_AE)
		#define RELOCATION_OFFSET(se, ae) ae
	#else
		#define RELOCATION_OFFSET(se, ae) se
	#endif
#endif

}
