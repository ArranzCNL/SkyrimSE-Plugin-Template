#################################################
# Configure
#################################################
set(SPDLOG_NAME      "spdlog")
set(SPDLOG_REPO_URL  "https://github.com/gabime/spdlog")
set(SPDLOG_REPO_TAG  "v1.17.0")
set(SPDLOG_ROOT_DIR  "${COMMONLIBSSE_EXTERN_ROOT_DIR}/${SPDLOG_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${SPDLOG_NAME}
	GIT_REPOSITORY  ${SPDLOG_REPO_URL}
	GIT_TAG         ${SPDLOG_REPO_TAG}
	SOURCE_DIR      ${SPDLOG_ROOT_DIR}
)
#################################################
# Find files
#################################################
# Find include files
file(
	GLOB_RECURSE      SPDLOG_HEADERS
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${SPDLOG_ROOT_DIR}/include/*.h"
		"${SPDLOG_ROOT_DIR}/include/*.hpp"
)
# Find source files
file(
	GLOB_RECURSE      SPDLOG_SOURCES
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${SPDLOG_ROOT_DIR}/src/*.cpp"
		"${SPDLOG_ROOT_DIR}/src/*.cxx"
)
# Find clang-format file
file(
	GLOB              SPDLOG_CLANG_FORMAT
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${SPDLOG_ROOT_DIR}/*.clang-format"
)
#################################################
# Set hierarchy for filters
#################################################
foreach(source ${SPDLOG_HEADERS})
	get_filename_component(source_abs "${source}" ABSOLUTE)
	file(RELATIVE_PATH rel_path "${SPDLOG_ROOT_DIR}/include" "${source_abs}")
	get_filename_component(dir "${rel_path}" PATH)
	string(REPLACE "/" "\\" filter "include\\${dir}")
	source_group("${filter}" FILES "${source}")
endforeach()

foreach(source ${SPDLOG_SOURCES})
	get_filename_component(source_abs "${source}" ABSOLUTE)
	file(RELATIVE_PATH rel_path "${SPDLOG_ROOT_DIR}/src" "${source_abs}")
	get_filename_component(dir "${rel_path}" PATH)
	string(REPLACE "/" "\\" filter "src\\${dir}")
	source_group("${filter}" FILES "${source}")
endforeach()
# Add library using the variables
add_library(
	${SPDLOG_NAME} STATIC
		${SPDLOG_HEADERS}
		${SPDLOG_SOURCES}
		${SPDLOG_CLANG_FORMAT}
)

set_target_properties(${SPDLOG_NAME} PROPERTIES FOLDER "Dependencies/${COMMONLIBSSE_NAME}/Dependencies")

target_include_directories(
	${SPDLOG_NAME} PUBLIC
		${SPDLOG_ROOT_DIR}/include
)

target_compile_definitions(
	${SPDLOG_NAME} PUBLIC
		SPDLOG_COMPILED_LIB
)

if(MSVC)
	target_compile_options(
		"${SPDLOG_NAME}" PUBLIC
			$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>
	)
endif()
