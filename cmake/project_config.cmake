# Collapse ALL_BUILD, INSTALL, ZERO_CHECK under CMakePredefinedTargets
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Remove ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)
# Set default startup project
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
# Set paths
set(PROJECT_ROOT_DIR          "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}")
set(PROJECT_INCLUDE_ROOT_DIR  "${PROJECT_ROOT_DIR}/include")
set(PROJECT_SOURCE_ROOT_DIR   "${PROJECT_ROOT_DIR}/src")
set(PROJECT_PCH_ROOT_DIR      "${PROJECT_ROOT_DIR}/pch")
set(PROJECT_RESOURCE_ROOT_DIR "${PROJECT_ROOT_DIR}/resource")
set(PROJECT_EXTERN_ROOT_DIR   "${PROJECT_ROOT_DIR}/extern")
# Set defaults for version components if not defined
set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(VERSION_TWEAK ${PROJECT_VERSION_TWEAK})

foreach(ver VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_TWEAK)
	if(NOT ${ver})
		set(${ver} 0)
	endif()
endforeach()
# Configure version.h
configure_file(${CMAKE_CURRENT_LIST_DIR}/templates/version.h.in ${PROJECT_INCLUDE_ROOT_DIR}/version.h @ONLY)

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
# Set linker flags
if(MSVC)
	add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG:FASTLINK>")
	add_link_options("$<$<NOT:$<CONFIG:Debug>>:/INCREMENTAL:NO>")
	add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
	add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")
	add_link_options("$<$<NOT:$<CONFIG:Debug>>:/LTCG>")
endif()
# Find include files
file(
	GLOB_RECURSE      PROJECT_HEADERS
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${PROJECT_INCLUDE_ROOT_DIR}/*.h"
		"${PROJECT_INCLUDE_ROOT_DIR}/*.hpp"
)
# Find source files
file(
	GLOB_RECURSE      PROJECT_SOURCES
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${PROJECT_SOURCE_ROOT_DIR}/*.cpp"
		"${PROJECT_SOURCE_ROOT_DIR}/*.cxx"
)
# Find precompiled header files
file(
	GLOB              PROJECT_PCH
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${PROJECT_PCH_ROOT_DIR}/*.*"
)
# Find resource file
file(
	GLOB              PROJECT_RESOURCE
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${PROJECT_RESOURCE_ROOT_DIR}/*.rc"
)
# Find clang-format file
file(
	GLOB              PROJECT_CLANG_FORMAT
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${PROJECT_ROOT_DIR}/*.clang-format"
)
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

group_sources("${PROJECT_INCLUDE_ROOT_DIR}"  PROJECT_HEADERS  "include")
group_sources("${PROJECT_SOURCE_ROOT_DIR}"   PROJECT_SOURCES  "src")
group_sources("${PROJECT_PCH_ROOT_DIR}"      PROJECT_PCH      "pch")
group_sources("${PROJECT_RESOURCE_ROOT_DIR}" PROJECT_RESOURCE "resource")
#################################################
add_library(
	${PROJECT_NAME} MODULE
		${PROJECT_HEADERS}
		${PROJECT_SOURCES}
		${PROJECT_PCH}
		${PROJECT_RESOURCE}
		${PROJECT_CLANG_FORMAT}
)
# Set include directories
target_include_directories(
	${PROJECT_NAME} PRIVATE
		${PROJECT_INCLUDE_ROOT_DIR}
		${PROJECT_PCH_ROOT_DIR}
		${PROJECT_EXTERN_ROOT_DIR}
)
# Setup precompiled header, cmake does it weird.
if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/YuPCH.h")
	set_source_files_properties(${PROJECT_PCH_ROOT_DIR}/PCH.cpp PROPERTIES COMPILE_FLAGS "/YcPCH.h")
	target_compile_options(${PROJECT_NAME} PRIVATE /FI"PCH.h")
endif()

# Define unified output directory for all configurations
set_target_properties(
	${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "$<IF:$<CONFIG:Debug>,${PROJECT_SOURCE_DIR}/.bin/x64-debug,${PROJECT_SOURCE_DIR}/.bin/x64-release>"
		LIBRARY_OUTPUT_DIRECTORY "$<IF:$<CONFIG:Debug>,${PROJECT_SOURCE_DIR}/.bin/x64-debug,${PROJECT_SOURCE_DIR}/.bin/x64-release>"
		ARCHIVE_OUTPUT_DIRECTORY "$<IF:$<CONFIG:Debug>,${PROJECT_SOURCE_DIR}/.lib/x64-debug,${PROJECT_SOURCE_DIR}/.lib/x64-release>"
)
