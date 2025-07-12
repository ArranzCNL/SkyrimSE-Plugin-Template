#################################################
# Configure
#################################################
set(SIMPLEINI_NAME       "simpleini")
set(SIMPLEINI_REPO_URL   "https://github.com/brofield/simpleini")
set(SIMPLEINI_REPO_TAG   "v4.22")
set(SIMPLEINI_ROOT_DIR   "${PROJECT_EXTERN_ROOT_DIR}/${SIMPLEINI_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${SIMPLEINI_NAME}
	GIT_REPOSITORY  ${SIMPLEINI_REPO_URL}
	GIT_TAG         ${SIMPLEINI_REPO_TAG}
	SOURCE_DIR      ${SIMPLEINI_ROOT_DIR}
)

# Tie this to CommonLibSSE for REX::INI
target_include_directories(
	${COMMONLIBSSE_NAME}
	PUBLIC
		"${SIMPLEINI_ROOT_DIR}"
)

target_compile_definitions(
	${COMMONLIBSSE_NAME} PRIVATE
		REX_OPTION_INI=1
)
