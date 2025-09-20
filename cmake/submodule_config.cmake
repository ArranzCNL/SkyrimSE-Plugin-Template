#################################################
# CommonLibSSE
#################################################
if(COMMONLIB_VARIANT STREQUAL "vs2022-po3")
	include(submodule/CommonLibSSE/dev-powerof3_config)
elseif(COMMONLIB_VARIANT STREQUAL "vs2022-ng")
	include(submodule/CommonLibSSE/ng-alandtse_config)
else()
	message(FATAL_ERROR "Unsupported COMMONLIB_VARIANT: ${COMMONLIB_VARIANT}")
endif()
#################################################
# Place other submodules here
#################################################
if(INSTALL_IMGUI)
	include(submodule/imgui_config)
endif()

# Xbyak - A JIT assembler for x86/x64 architectures
include(submodule/xbyak_config)

# JSON Support
#include(submodule/nlohmann-json_config)

# SimpleIni Support
#include(submodule/simpleini_config)

# Toml Support
#include(submodule/toml11_config)
