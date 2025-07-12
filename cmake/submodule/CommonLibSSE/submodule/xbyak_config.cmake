#################################################
# Configure
#################################################
set(XBYAK_NAME      "xbyak")
set(XBYAK_REPO_URL  "https://github.com/herumi/xbyak")
set(XBYAK_REPO_TAG  "v7.23.1")
set(XBYAK_ROOT_DIR  "${COMMONLIBSSE_EXTERN_ROOT_DIR}/${XBYAK_NAME}")
#################################################
# Fetch Project
#################################################
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Populate(
	${XBYAK_NAME}
	GIT_REPOSITORY  ${XBYAK_REPO_URL}
	GIT_TAG         ${XBYAK_REPO_TAG}
	SOURCE_DIR      ${XBYAK_ROOT_DIR}
)
