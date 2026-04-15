function(hso_find_dependency LIBRARY LIBRARY_PC LIBRARY_ANCHOR_H )

  if(NOT LIBRARY)
    message(FATAL_ERROR "hso_find_package_resilient called without a library name.")
    return()
  endif()

  set(${LIBRARY}_FOUND_GLOBAL FALSE PARENT_SCOPE)
  set(${LIBRARY}_INCLUDE_DIRS_GLOBAL "" PARENT_SCOPE)
  set(${LIBRARY}_LIBRARIES_GLOBAL "" PARENT_SCOPE)
  set(${LIBRARY}_FOUND FALSE)
  set(${LIBRARY}_INCLUDE_DIRS_NORM "")
  set(${LIBRARY}_LIBRARIES_NORM "")

  find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(${LIBRARY}_PKG ${LIBRARY_PC} QUIET)
        if(${LIBRARY}_PKG_FOUND)
          set (${LIBRARY}_FOUND TRUE)
          set (${LIBRARY}_INCLUDE_DIRS_NORM ${${LIBRARY}_PKG_INCLUDE_DIRS})
          set (${LIBRARY}_INCLUDE_DIRS_GLOBAL ${${LIBRARY}_INCLUDE_DIRS_NORM} PARENT_SCOPE)
          set (${LIBRARY}_FOUND_GLOBAL TRUE PARENT_SCOPE)

          if(${LIBRARY}_PKG_LINK_LIBRARIES)
            set(${LIBRARY}_LIBRARIES_NORM ${${LIBRARY}_PKG_LINK_LIBRARIES})
          else()
            set(${LIBRARY}_LIBRARIES_NORM ${${LIBRARY}_PKG_LIBRARIES})
          endif()
          set(${LIBRARY}_LIBRARIES_GLOBAL ${${LIBRARY}_LIBRARIES_NORM} PARENT_SCOPE)

          # message(STATUS "--------------------------------------------")
          # message(STATUS "${LIBRARY}: Found and configured (PkgConfig)")
          # message(STATUS "  - Libraries: ${${LIBRARY}_LIBRARIES_NORM}")
          # message(STATUS "  - Includes:  ${${LIBRARY}_INCLUDE_DIRS_NORM}")
          # message(STATUS "--------------------------------------------")
          return()
        endif()
    endif()

  set(_SEARCH_PATHS /usr /usr/local /opt/${LIBRARY} /opt/${LIBRARY_PC})
  set(_PATHS_SUFFIXES lib lib64 )
  find_path (
    ${LIBRARY}_INCLUDE_DIRS_MANUAL
    NAMES ${LIBRARY_ANCHOR_H}
    PATHS ${_SEARCH_PATHS}
    PATH_SUFFIXES include
  )
  find_library(
    ${LIBRARY}_LIBRARIES_MANUAL
    NAMES ${LIBRARY}
    PATHS ${_SEARCH_PATHS}
    PATH_SUFFIXES ${_PATHS_SUFFIXES}
  )
  if(${LIBRARY}_INCLUDE_DIRS_MANUAL AND ${LIBRARY}_LIBRARIES_MANUAL)
    set(${LIBRARY}_FOUND TRUE)
    set(${LIBRARY}_INCLUDE_DIRS_NORM ${${LIBRARY}_INCLUDE_DIRS_MANUAL})
    set(${LIBRARY}_LIBRARIES_NORM ${${LIBRARY}_LIBRARIES_MANUAL})

    set(${LIBRARY}_FOUND_GLOBAL TRUE PARENT_SCOPE)
    set(${LIBRARY}_INCLUDE_DIRS_GLOBAL ${${LIBRARY}_INCLUDE_DIRS_NORM} PARENT_SCOPE)
    set(${LIBRARY}_LIBRARIES_GLOBAL ${${LIBRARY}_LIBRARIES_NORM} PARENT_SCOPE)

    # message(STATUS "--------------------------------------------")
    # message(STATUS "${LIBRARY}: Found and configured (manual search)")
    # message(STATUS "  - Libraries: ${${LIBRARY}_LIBRARIES_NORM}")
    # message(STATUS "  - Includes:  ${${LIBRARY}_INCLUDE_DIRS_NORM}")
    # message(STATUS "--------------------------------------------")
    return()
  endif()

  message(FATAL_ERROR "${LIBRARY} not found")
  return()
endfunction()

function(hso_link_include_dependency TARGET LIBRARY)

  set(_FOUND "${${LIBRARY}_FOUND_GLOBAL}")
  set(_CMAKE_INSTALLED  "${${LIBRARY}_CMAKE_INSTALLED_GLOBAL}")
  set(_LIBRARIES "${${LIBRARY}_LIBRARIES_GLOBAL}")
  set(_INCLUDE_DIRS "${${LIBRARY}_INCLUDE_DIRS_GLOBAL}")

  if(NOT _FOUND)
    message(FATAL_ERROR "${LIBRARY} not found")
    return()
  endif()

  target_link_libraries(${TARGET} PRIVATE ${_LIBRARIES})
  target_include_directories(${TARGET} SYSTEM PRIVATE ${_INCLUDE_DIRS})
  # message(STATUS "--------------------------------------------")
  # message(STATUS "${LIBRARY}: Linked to ${TARGET}")
  # message(STATUS "  - Libraries: ${_LIBRARIES}")
  # message(STATUS "  - Includes:  ${_INCLUDE_DIRS}")
  # message(STATUS "--------------------------------------------")
  return()
  # return()

endfunction()
