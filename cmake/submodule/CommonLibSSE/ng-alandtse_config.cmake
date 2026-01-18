#################################################
# Configure
#################################################
set(COMMONLIBSSE_NAME            "CommonLibSSE")
set(COMMONLIBSSE_REPO_URL        "https://github.com/alandtse/CommonLibVR")
set(COMMONLIBSSE_REPO_TAG        "0aeb311") # Commit on Jan 17, 2026
set(COMMONLIBSSE_ROOT_DIR        "${PROJECT_EXTERN_ROOT_DIR}/${COMMONLIBSSE_NAME}")
set(COMMONLIBSSE_EXTERN_ROOT_DIR "${COMMONLIBSSE_ROOT_DIR}/extern")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${COMMONLIBSSE_NAME}
	GIT_REPOSITORY          ${COMMONLIBSSE_REPO_URL}
	GIT_TAG                 ${COMMONLIBSSE_REPO_TAG}
	SOURCE_DIR              ${COMMONLIBSSE_ROOT_DIR}
)
#################################################
# Dependencies
#################################################
include(${CMAKE_CURRENT_LIST_DIR}/submodule/directxtk_config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/submodule/spdlog_config.cmake)
#################################################
# Patch Project
#################################################
set(PATCH_FILES
	# pch
	"pch/PCH.cpp"
	"pch/PCH.h"
	# include
	"include/PROXY/Proxies.h"
	"include/PROXY/ActorProxy.h"
	"include/PROXY/ControlMapProxy.h"
	"include/PROXY/PlayerCharacterProxy.h"
	"include/PROXY/RendererProxy.h"
	
	"include/RE/E/ExtraLevCreaModifier.h"
	# src
)

foreach(FILE_PATH IN LISTS PATCH_FILES)
	get_filename_component(FILENAME "${FILE_PATH}" NAME)
	get_filename_component(SUBDIR "${FILE_PATH}" DIRECTORY)

	set(PATCH_SRC "${CMAKE_CURRENT_LIST_DIR}/patch/${SUBDIR}/${FILENAME}")
	set(PATCH_DST "${COMMONLIBSSE_ROOT_DIR}/${SUBDIR}")

	file(COPY "${PATCH_SRC}" DESTINATION "${PATCH_DST}")
endforeach()
#################################################
# Find files
#################################################
# Include files
file(
	GLOB_RECURSE      COMMONLIBSSE_HEADERS
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${COMMONLIBSSE_ROOT_DIR}/include/*.h"
		"${COMMONLIBSSE_ROOT_DIR}/include/*.hpp"
)
# Source files
file(
	GLOB_RECURSE      COMMONLIBSSE_SOURCES
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${COMMONLIBSSE_ROOT_DIR}/src/*.cpp"
		"${COMMONLIBSSE_ROOT_DIR}/src/*.cxx"
)
# Precompiled header files
file(
	GLOB              COMMONLIBSSE_PCH
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${COMMONLIBSSE_ROOT_DIR}/pch/*.*"
)
# Clang-format file
file(
	GLOB              COMMONLIBSSE_CLANG_FORMAT
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${COMMONLIBSSE_ROOT_DIR}/*.clang-format"
)
# Natvis file
file(
	GLOB              COMMONLIBSSE_NATVIS
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${COMMONLIBSSE_ROOT_DIR}/*.natvis"
)
#################################################
# Generate Skyrim.h
#################################################
function(generate_skyrim_header output_file)
	# Gather all headers under include/RE/, including both RE/*.h and RE/*/*.h
	file(GLOB_RECURSE all_headers RELATIVE "${COMMONLIBSSE_ROOT_DIR}/include" CONFIGURE_DEPENDS
		"${COMMONLIBSSE_ROOT_DIR}/include/RE/*.h"
		"${COMMONLIBSSE_ROOT_DIR}/include/RE/*.hpp"
	)

	set(includes "")
	foreach(h IN LISTS all_headers)
		string(REPLACE "\\" "/" rel "${h}")
		# Skip the file we're about to generate
		if(rel STREQUAL "RE/Skyrim.h")
			continue()
		endif()
		list(APPEND includes "#include \"${h}\"")
	endforeach()

	list(SORT includes)
	string(REPLACE ";" "\n" includes_str "${includes}")

	set(header_text "#pragma once\n\n#include \"SKSE/Impl/PCH.h\"\n\n${includes_str}\n")
	file(WRITE "${output_file}" "${header_text}")
endfunction()

generate_skyrim_header("${COMMONLIBSSE_ROOT_DIR}/include/RE/Skyrim.h")
#################################################
# Set hierarchy for filters
#################################################
function(group_sources BASE_PATH SOURCE_LIST FILTER_PREFIX)
	foreach(source IN LISTS ${SOURCE_LIST})
		get_filename_component(source_abs "${source}" ABSOLUTE)
		file(RELATIVE_PATH rel_path "${BASE_PATH}" "${source_abs}")
		get_filename_component(dir "${rel_path}" PATH)
		string(REPLACE "/" "\\" filter "${FILTER_PREFIX}\\${dir}")
		source_group("${filter}" FILES "${source}")
	endforeach()
endfunction()

group_sources("${COMMONLIBSSE_ROOT_DIR}/include" COMMONLIBSSE_HEADERS "include")
group_sources("${COMMONLIBSSE_ROOT_DIR}/src"     COMMONLIBSSE_SOURCES "src")
group_sources("${COMMONLIBSSE_ROOT_DIR}/pch"     COMMONLIBSSE_PCH     "pch")
#################################################
add_library(
	${COMMONLIBSSE_NAME} STATIC
		${COMMONLIBSSE_HEADERS}
		${COMMONLIBSSE_SOURCES}
		${COMMONLIBSSE_PCH}
		${COMMONLIBSSE_CLANG_FORMAT}
		${COMMONLIBSSE_NATVIS}
)

target_compile_definitions(
	"${COMMONLIBSSE_NAME}" PUBLIC
		WINVER=0x0601	# windows 7, minimum supported version by skyrim special edition
		_WIN32_WINNT=0x0601
		ENABLE_SKYRIM_SE=1
		ENABLE_SKYRIM_AE=1
		SKYRIM_SUPPORT_NG=1
)

target_compile_features(
	"${COMMONLIBSSE_NAME}" PRIVATE
		cxx_std_23
)

if(MSVC)
	target_compile_options(
		"${COMMONLIBSSE_NAME}" PUBLIC
			# warnings -> errors
			/we4715	# 'function': not all control paths return a value

			# disable warnings
			/wd4005 # macro redefinition
			/wd4061 # enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
			/wd4068 # unknown pragma
			/wd4200 # nonstandard extension used : zero-sized array in struct/union
			/wd4201 # nonstandard extension used : nameless struct/union
			/wd4265 # 'type': class has virtual functions, but its non-trivial destructor is not virtual; instances of this class may not be destructed correctly
			/wd4266 # 'function': no override available for virtual member function from base 'type'; function is hidden
			/wd4324 # 'structname': structure was padded due to alignment specifier
			/wd4371 # 'classname': layout of class may have changed from a previous version of the compiler due to better packing of member 'member'
			/wd4514 # 'function': unreferenced inline function has been removed
			/wd4582 # 'type': constructor is not implicitly called
			/wd4583 # 'type': destructor is not implicitly called
			/wd4623 # 'derived class': default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
			/wd4625 # 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
			/wd4626 # 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
			/wd4710 # 'function': function not inlined
			/wd4711 # function 'function' selected for inline expansion
			/wd4820 # 'bytes' bytes padding added after construct 'member_name'
			/wd4996 # 'deprecated function': required due to Microsoft STL itself using its own deprecated functions
			/wd5026 # 'type': move constructor was implicitly defined as deleted
			/wd5027 # 'type': move assignment operator was implicitly defined as deleted
			/wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
			/wd5053 # support for 'explicit(<expr>)' in C++17 and earlier is a vendor extension
			/wd5204 # 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
			/wd5220 # 'member': a non-static data member with a volatile qualified type no longer implies that compiler generated copy / move constructors and copy / move assignment operators are not trivial
	)
endif()

target_link_libraries(
	"${COMMONLIBSSE_NAME}" PRIVATE
		advapi32.lib
		bcrypt.lib
		d3d11.lib
		d3dcompiler.lib
		dbghelp.lib
		dxgi.lib
		ole32.lib
		version.lib
	
	PUBLIC
		${SPDLOG_NAME}
)

set_target_properties(${COMMONLIBSSE_NAME} PROPERTIES FOLDER "Dependencies/${COMMONLIBSSE_NAME}")

target_include_directories(
	${COMMONLIBSSE_NAME} PRIVATE
		${COMMONLIBSSE_ROOT_DIR}/pch
		
	PUBLIC
		${COMMONLIBSSE_ROOT_DIR}/include
		${SPDLOG_ROOT_DIR}/include
		${XBYAK_ROOT_DIR}
		${DIRECTXTK_ROOT_DIR}/Inc
)
# Setup precompiled header, cmake does it weird.
if(MSVC)
	set_target_properties(${COMMONLIBSSE_NAME} PROPERTIES COMPILE_FLAGS "/YuPCH.h")
	set_source_files_properties(${COMMONLIBSSE_ROOT_DIR}/pch/PCH.cpp PROPERTIES COMPILE_FLAGS "/YcPCH.h")
	target_compile_options(${COMMONLIBSSE_NAME} PRIVATE /FI"PCH.h")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMONLIBSSE_NAME})
