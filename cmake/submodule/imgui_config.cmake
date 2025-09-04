#################################################
# Configure
#################################################
set(IMGUI_NAME      "imgui")
set(IMGUI_REPO_URL  "https://github.com/ocornut/imgui")
set(IMGUI_REPO_TAG  "v1.92.2b")
set(IMGUI_ROOT_DIR  "${PROJECT_EXTERN_ROOT_DIR}/${IMGUI_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${IMGUI_NAME}
	GIT_REPOSITORY  ${IMGUI_REPO_URL}
	GIT_TAG         ${IMGUI_REPO_TAG}
	SOURCE_DIR      ${IMGUI_ROOT_DIR}
)
#################################################
# Find files
#################################################
set(
	IMGUI_HEADERS
		"${IMGUI_ROOT_DIR}/imconfig.h"
		"${IMGUI_ROOT_DIR}/imgui.h"
		"${IMGUI_ROOT_DIR}/imgui_internal.h"
		"${IMGUI_ROOT_DIR}/imstb_rectpack.h"
		"${IMGUI_ROOT_DIR}/imstb_textedit.h"
		"${IMGUI_ROOT_DIR}/imstb_truetype.h"
		"${IMGUI_ROOT_DIR}/backends/imgui_impl_dx11.h"
		"${IMGUI_ROOT_DIR}/backends/imgui_impl_win32.h"
		"${IMGUI_ROOT_DIR}/misc/cpp/imgui_stdlib.h"
)

set(
	IMGUI_SOURCES
		"${IMGUI_ROOT_DIR}/imgui.cpp"
		"${IMGUI_ROOT_DIR}/imgui_demo.cpp"
		"${IMGUI_ROOT_DIR}/imgui_draw.cpp"
		"${IMGUI_ROOT_DIR}/imgui_tables.cpp"
		"${IMGUI_ROOT_DIR}/imgui_widgets.cpp"
		"${IMGUI_ROOT_DIR}/backends/imgui_impl_dx11.cpp"
		"${IMGUI_ROOT_DIR}/backends/imgui_impl_win32.cpp"
		"${IMGUI_ROOT_DIR}/misc/cpp/imgui_stdlib.cpp"
)
#################################################
# Set hierarchy for filters
#################################################
foreach(header ${IMGUI_HEADERS})
	file(RELATIVE_PATH rel_path "${IMGUI_ROOT_DIR}" "${header}")
	get_filename_component(dir "${rel_path}" PATH)
	string(REPLACE "/" "\\" filter "include\\${dir}")
	source_group("${filter}" FILES "${header}")
endforeach()

foreach(source ${IMGUI_SOURCES})
	file(RELATIVE_PATH rel_path "${IMGUI_ROOT_DIR}" "${source}")
	get_filename_component(dir "${rel_path}" PATH)
	string(REPLACE "/" "\\" filter "src\\${dir}")
	source_group("${filter}" FILES "${source}")
endforeach()
# Add library using the variables
add_library(
	${IMGUI_NAME} STATIC
		${IMGUI_HEADERS}
		${IMGUI_SOURCES}
)

set_target_properties(${IMGUI_NAME} PROPERTIES FOLDER "Dependencies")

target_include_directories(
	${IMGUI_NAME}
	PUBLIC
		${IMGUI_ROOT_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${IMGUI_NAME})
