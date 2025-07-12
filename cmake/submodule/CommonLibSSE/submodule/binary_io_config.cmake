#################################################
# Configure
#################################################
set(BINARY_IO_NAME     "binary_io")
set(BINARY_IO_REPO_URL "https://github.com/Ryan-rsm-McKenzie/binary_io")
set(BINARY_IO_REPO_TAG "2.0.6")
set(BINARY_IO_ROOT_DIR "${COMMONLIBSSE_EXTERN_ROOT_DIR}/${BINARY_IO_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${BINARY_IO_NAME}
	GIT_REPOSITORY  ${BINARY_IO_REPO_URL}
	GIT_TAG         ${BINARY_IO_REPO_TAG}
	SOURCE_DIR      ${BINARY_IO_ROOT_DIR}
)
#################################################
# Find files
#################################################
# Find include files
file(
	GLOB_RECURSE      BINARY_IO_HEADERS
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${BINARY_IO_ROOT_DIR}/include/*.h"
		"${BINARY_IO_ROOT_DIR}/include/*.hpp"
)
# Find source files
file(
	GLOB_RECURSE      BINARY_IO_SOURCES
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${BINARY_IO_ROOT_DIR}/src/*.cpp"
		"${BINARY_IO_ROOT_DIR}/src/*.cxx"
)
# Find clang-format file
file(
	GLOB              BINARY_IO_CLANG_FORMAT
	LIST_DIRECTORIES  false
	CONFIGURE_DEPENDS
		"${BINARY_IO_ROOT_DIR}/*.clang-format"
)
#################################################
# Set hierarchy for filters
#################################################
foreach(source ${BINARY_IO_HEADERS})
	get_filename_component(source_abs "${source}" ABSOLUTE)
	file(RELATIVE_PATH rel_path "${BINARY_IO_ROOT_DIR}/include" "${source_abs}")
	get_filename_component(dir "${rel_path}" PATH)
	string(REPLACE "/" "\\" filter "include\\${dir}")
	source_group("${filter}" FILES "${source}")
endforeach()

foreach(source ${BINARY_IO_SOURCES})
	get_filename_component(source_abs "${source}" ABSOLUTE)
	file(RELATIVE_PATH rel_path "${BINARY_IO_ROOT_DIR}/src" "${source_abs}")
	get_filename_component(dir "${rel_path}" PATH)
	string(REPLACE "/" "\\" filter "src\\${dir}")
	source_group("${filter}" FILES "${source}")
endforeach()
# Add library using the variables
add_library(
	${BINARY_IO_NAME} STATIC
		${BINARY_IO_HEADERS}
		${BINARY_IO_SOURCES}
		${BINARY_IO_CLANG_FORMAT}
)

set_target_properties(${BINARY_IO_NAME} PROPERTIES FOLDER "Dependencies/${COMMONLIBSSE_NAME}/Dependencies")

target_include_directories(
	${BINARY_IO_NAME} PUBLIC
		${BINARY_IO_ROOT_DIR}/include
)

if(MSVC)
	target_compile_options(
		"${BINARY_IO_NAME}" PRIVATE
			/wd4005 # macro redefinition
	)
endif()
