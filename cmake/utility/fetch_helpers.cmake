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

    # Clone repo shallow without checking out
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

    # Checkout specific commit/tag/branch
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

# Similar to FetchContent_Populate but for a single file (raw URL download)
function(FetchContent_Populate_Single_File)
    cmake_parse_arguments(
        FILE
        ""
        "GIT_REPOSITORY;GIT_FILE;DESTINATION;EXPECTED_HASH;FORCE_UPDATE;GIT_PROGRESS"
        ""
        ${ARGN}
    )

    if(NOT FILE_GIT_REPOSITORY)
        message(FATAL_ERROR "[FetchContent_Populate_Single_File] GIT_REPOSITORY is required")
    endif()

    if(NOT FILE_GIT_FILE)
        message(FATAL_ERROR "[FetchContent_Populate_Single_File] GIT_FILE is required")
    endif()

    if(NOT FILE_DESTINATION)
        message(FATAL_ERROR "[FetchContent_Populate_Single_File] DESTINATION is required")
    endif()

    # Full path to the file
    set(_dest_dir "${FILE_DESTINATION}")
    set(_dest_file "${_dest_dir}/${FILE_GIT_FILE}")

    file(MAKE_DIRECTORY "${_dest_dir}")

    # Determine whether we need to download
    set(_download FALSE)
    if(FILE_FORCE_UPDATE)
        string(TOUPPER "${FILE_FORCE_UPDATE}" _forceupdate_value)
        if(_forceupdate_value STREQUAL "TRUE")
            set(_download TRUE)
            if(EXISTS "${_dest_file}")
                message(STATUS "[FetchContent_Populate_Single_File] FORCE_UPDATE enabled — removing existing file")
                file(REMOVE "${_dest_file}")
            endif()
        endif()
    elseif(NOT EXISTS "${_dest_file}")
        set(_download TRUE)
    else()
        file(SIZE "${_dest_file}" _file_size)
        if(_file_size EQUAL 0)
            set(_download TRUE)
        endif()
    endif()

    if(_download)
        message(STATUS "[FetchContent_Populate_Single_File] Downloading ${FILE_GIT_FILE}")

        # Build arguments for file(DOWNLOAD)
        set(_download_args
            "${FILE_GIT_REPOSITORY}"
            "${_dest_file}"
            TIMEOUT 60
            TLS_VERIFY ON
            STATUS _status
        )

        if(FILE_EXPECTED_HASH)
            list(APPEND _download_args EXPECTED_HASH "${FILE_EXPECTED_HASH}")
        endif()

        if(FILE_GIT_PROGRESS)
            string(TOUPPER "${FILE_GIT_PROGRESS}" _progress_value)
            if(_progress_value STREQUAL "TRUE")
                list(APPEND _download_args SHOW_PROGRESS)
            endif()
        endif()

        # Download the file
        file(DOWNLOAD ${_download_args})

        list(GET _status 0 _status_code)
        list(GET _status 1 _status_msg)

        if(NOT _status_code EQUAL 0)
            message(FATAL_ERROR "[FetchContent_Populate_Single_File] Download failed: ${_status_msg}")
        endif()
    else()
        message(STATUS "[FetchContent_Populate_Single_File] ${FILE_GIT_FILE} already exists at ${_dest_file}")
    endif()
endfunction()
