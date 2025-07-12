# Similar to FetchContent_Populate without Submodules
function(FetchContent_Populate_No_Submodules)
    cmake_parse_arguments(
        CLONE
        ""
        "GIT_REPOSITORY;GIT_TAG;SOURCE_DIR;GIT_PROGRESS"
        ""
        ${ARGN}
    )

    if(NOT CLONE_GIT_REPOSITORY)
        message(FATAL_ERROR "[FetchContent_Populate_No_Submodules] GIT_REPOSITORY is required")
    endif()

    if(NOT CLONE_GIT_TAG)
        message(FATAL_ERROR "[FetchContent_Populate_No_Submodules] GIT_TAG is required")
    endif()

    if(NOT CLONE_SOURCE_DIR)
        message(FATAL_ERROR "[FetchContent_Populate_No_Submodules] SOURCE_DIR is required")
    endif()

    if(EXISTS "${CLONE_SOURCE_DIR}/.git")
        message(STATUS "[FetchContent_Populate_No_Submodules] Repo already cloned at ${CLONE_SOURCE_DIR}")
        return()
    endif()

    file(MAKE_DIRECTORY "${CLONE_SOURCE_DIR}")

    # Step 1: Clone repo shallow without checking out
    set(GIT_CLONE_COMMAND
        git clone
        --no-single-branch
        "${CLONE_GIT_REPOSITORY}"
        "${CLONE_SOURCE_DIR}"
    )

    if(CLONE_GIT_PROGRESS)
        string(TOUPPER "${CLONE_GIT_PROGRESS}" _progress_value)
        if(_progress_value STREQUAL "TRUE")
            list(APPEND GIT_CLONE_COMMAND --progress)
        endif()
    endif()

    message(STATUS "[FetchContent_Populate_No_Submodules] Cloning ${CLONE_GIT_REPOSITORY} → ${CLONE_SOURCE_DIR}")
    execute_process(
        COMMAND ${GIT_CLONE_COMMAND}
        RESULT_VARIABLE _res
        OUTPUT_VARIABLE _out
        ERROR_VARIABLE  _err
    )

    if(NOT _res EQUAL 0)
        message(FATAL_ERROR "[FetchContent_Populate_No_Submodules] Git clone failed:\n${_err}")
    endif()

    # Step 2: Checkout specific commit/tag/branch
    execute_process(
        COMMAND git checkout ${CLONE_GIT_TAG}
        WORKING_DIRECTORY "${CLONE_SOURCE_DIR}"
        RESULT_VARIABLE _checkout_res
        OUTPUT_VARIABLE _checkout_out
        ERROR_VARIABLE _checkout_err
    )
    if(NOT _checkout_res EQUAL 0)
        message(FATAL_ERROR "[FetchContent_Populate_No_Submodules] Git checkout failed:\n${_checkout_err}")
    endif()
endfunction()
