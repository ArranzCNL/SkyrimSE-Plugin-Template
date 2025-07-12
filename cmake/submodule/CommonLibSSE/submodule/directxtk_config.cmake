#################################################
# Configure
#################################################
set(DIRECTXTK_NAME     "DirectXTK")
set(DIRECTXTK_REPO_URL "https://github.com/microsoft/DirectXTK")
set(DIRECTXTK_REPO_TAG "mar2025")
set(DIRECTXTK_ROOT_DIR "${COMMONLIBSSE_EXTERN_ROOT_DIR}/${DIRECTXTK_NAME}")
#################################################
# Fetch Project
#################################################
FetchContent_Populate_No_Submodules(
	${DIRECTXTK}
	GIT_REPOSITORY ${DIRECTXTK_REPO_URL}
	GIT_TAG        ${DIRECTXTK_REPO_TAG}
	SOURCE_DIR     ${DIRECTXTK_ROOT_DIR}
)
