#################################################
# Configure
#################################################
set(JSON_NAME       "json")
set(JSON_REPO_URL   "https://github.com/nlohmann/json")
set(JSON_REPO_TAG   "v3.12.0")
set(JSON_ROOT_DIR   "${PROJECT_EXTERN_ROOT_DIR}/${JSON_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${JSON_NAME}
	GIT_REPOSITORY  ${JSON_REPO_URL}
	GIT_TAG         ${JSON_REPO_TAG}
	SOURCE_DIR      ${JSON_ROOT_DIR}
)

# Tie this to CommonLibSSE for REX::JSON
target_include_directories(
	${COMMONLIBSSE_NAME}
	PUBLIC
		"${JSON_ROOT_DIR}/single_include"
)

target_compile_definitions(
	${COMMONLIBSSE_NAME} PRIVATE
		REX_OPTION_JSON=1
)
