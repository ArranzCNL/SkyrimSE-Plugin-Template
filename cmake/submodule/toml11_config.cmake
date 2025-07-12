#################################################
# Configure
#################################################
set(TOML11_NAME       "toml11")
set(TOML11_REPO_URL   "https://github.com/ToruNiina/toml11")
set(TOML11_REPO_TAG   "v4.4.0")
set(TOML11_ROOT_DIR   "${PROJECT_EXTERN_ROOT_DIR}/${TOML11_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${TOML11_NAME}
	GIT_REPOSITORY  ${TOML11_REPO_URL}
	GIT_TAG         ${TOML11_REPO_TAG}
	SOURCE_DIR      ${TOML11_ROOT_DIR}
)

# Tie this to CommonLibSSE for REX::TOML
target_include_directories(
	${COMMONLIBSSE_NAME}
	PUBLIC
		"${TOML11_ROOT_DIR}/single_include"
)

target_compile_definitions(
	${COMMONLIBSSE_NAME} PRIVATE
		REX_OPTION_TOML=1
)
