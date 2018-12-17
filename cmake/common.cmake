function(addCppHeader prjName targetList relativeBasePath filename)
    SET(HEADER_GENERATOR_FILE "${PROJECT_SOURCE_DIR}/Cmake/header.h.in")
    #Compose the file variables
    SET(CURRENT_SOURCE_FILE_FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${filename}")
    SET(CURRENT_SOURCE_FILE_REL_PATH "${relativeBasePath}/${filename}")
    STRING(REPLACE "//" "/" CURRENT_SOURCE_FILE_REL_PATH ${CURRENT_SOURCE_FILE_REL_PATH})
    get_filename_component(CURRENT_SOURCE_FILE_NAME ${filename} NAME)
    get_filename_component(CURRENT_SOURCE_FILE_DIR ${filename} DIRECTORY)
    get_filename_component(CURRENT_SOURCE_FILE_EXT ${filename} EXT)
    get_filename_component(CURRENT_SOURCE_FILE_NAME_ONLY ${filename} NAME_WE)

    SET(TARGET_FILENAME "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_SOURCE_FILE_REL_PATH}")
    SET(FILE_HEADER_GUARD "${prjName}_${FILE_HEADER_GUARD_PATH}_${CURRENT_SOURCE_FILE_NAME_ONLY}_H_")
    string(REPLACE "/" "_" FILE_HEADER_GUARD "${FILE_HEADER_GUARD}")
    string(REPLACE "__" "_" FILE_HEADER_GUARD "${FILE_HEADER_GUARD}")
    string(TOUPPER "${FILE_HEADER_GUARD}" FILE_HEADER_GUARD)

    
    if(CURRENT_SOURCE_FILE_EXT STREQUAL ".h")
        if(NOT EXISTS "${TARGET_FILENAME}") 
            configure_file("${HEADER_GENERATOR_FILE}" "${TARGET_FILENAME}")
            message("header file ${TARGET_FILENAME} generated.")
        endif()
        if("${${targetList}}" STREQUAL "")
            SET(NEW_LIST "${CURRENT_SOURCE_FILE_REL_PATH}")
        else()
            SET(NEW_LIST "${${targetList}};${CURRENT_SOURCE_FILE_REL_PATH}")
        endif()
        set("${targetList}" "${NEW_LIST}" PARENT_SCOPE)
    endif()
endfunction()

function(addCppSource prjName targetList relativeBasePath filename)
    SET(GENERATOR_FILE "${PROJECT_SOURCE_DIR}/Cmake/source.cpp.in")
    if("${relativeBasePath}" STREQUAL "")
        SET(CURRENT_SOURCE_FILE_REL_PATH "${filename}")
    else()
        SET(CURRENT_SOURCE_FILE_REL_PATH "${relativeBasePath}/${filename}")
    endif()
    
    STRING(REPLACE "//" "/" CURRENT_SOURCE_FILE_REL_PATH ${CURRENT_SOURCE_FILE_REL_PATH})
    SET(TARGET_FILENAME "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_SOURCE_FILE_REL_PATH}")
    get_filename_component(CURRENT_SOURCE_FILE_EXT ${filename} EXT)
    if(CURRENT_SOURCE_FILE_EXT STREQUAL ".cpp")
        if(NOT EXISTS "${TARGET_FILENAME}")
            configure_file("${GENERATOR_FILE}" "${TARGET_FILENAME}")
            message("source file ${TARGET_FILENAME} generated.")
        endif()
        if("${${targetList}}" STREQUAL "")
            SET(NEW_LIST "${CURRENT_SOURCE_FILE_REL_PATH}")
        else()
            SET(NEW_LIST "${${targetList}};${CURRENT_SOURCE_FILE_REL_PATH}")
        endif()
        set("${targetList}" "${NEW_LIST}" PARENT_SCOPE)
    endif()    
endfunction()


macro(buildSourceGroups files)
    foreach(file ${${files}})
        get_filename_component(CURRENT_FILE_REL_DIR "${file}" DIRECTORY)
        string(REPLACE "/" "\\" GROUP_NAME ${CURRENT_FILE_REL_DIR})
        source_group("${GROUP_NAME}" FILES "${file}")
    endforeach()    
endmacro()


