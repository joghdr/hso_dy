include(FindPackageHandleStandardArgs)


function(hso_find_dependency LIBRARY LIBRARY_PC LIBRARY_ANCHOR_H )

  if(NOT LIBRARY)
    message(FATAL_ERROR "*** [HSO] hso_find_dependency called without a library name.")
  endif()

  if(NOT LIBRARY)
    message(FATAL_ERROR "hso_find_dependency called without a library name.")
    return()
  endif()

  set(${LIBRARY}_FOUND FALSE)
  set(${LIBRARY}_INCLUDE_DIRS "")
  set(${LIBRARY}_LIBRARIES "")
  set(${LIBRARY}_VERSION "0.0.0")

  ########## Search for package with pkg-config
  find_package(PkgConfig QUIET)
  if(PKG_CONFIG_FOUND)

    pkg_check_modules(${LIBRARY}_PKG ${LIBRARY_PC} QUIET)

      if(${LIBRARY}_PKG_FOUND)
        set (${LIBRARY}_FOUND TRUE)
        set (${LIBRARY}_VERSION ${${LIBRARY}_PKG_VERSION})
        set (${LIBRARY}_INCLUDE_DIRS ${${LIBRARY}_PKG_INCLUDE_DIRS})

        if(${LIBRARY}_PKG_LINK_LIBRARIES)
          set(${LIBRARY}_LIBRARIES ${${LIBRARY}_PKG_LINK_LIBRARIES})
        else()
          set(${LIBRARY}_LIBRARIES ${${LIBRARY}_PKG_LIBRARIES})
        endif()

      endif()

  endif()

  ########## Search with find_path if pkg-config failed
  if(NOT ${LIBRARY}_FOUND)

    find_path(${LIBRARY}_INCLUDE_DIRS_FALLBACK
      PATH_SUFFIXES include include/${LIBRARY}
      NAMES ${LIBRARY_ANCHOR_H}
    )

    find_library(${LIBRARY}_LIBRARIES_FALLBACK
      PATH_SUFFIXES lib lib64
      NAMES ${LIBRARY}
    )

    if(${LIBRARY}_INCLUDE_DIRS_FALLBACK AND ${LIBRARY}_LIBRARIES_FALLBACK)
      set(${LIBRARY}_FOUND TRUE)
      set(${LIBRARY}_INCLUDE_DIRS ${${LIBRARY}_INCLUDE_DIRS_FALLBACK})
      set(${LIBRARY}_LIBRARIES ${${LIBRARY}_LIBRARIES_FALLBACK})
    endif()

    if(${LIBRARY}_FOUND)
    endif()

  endif()


  if(${LIBRARY}_FOUND AND NOT TARGET ${LIBRARY}::${LIBRARY} )
    add_library(${LIBRARY}::${LIBRARY} INTERFACE IMPORTED GLOBAL)
    set_target_properties(${LIBRARY}::${LIBRARY} PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${${LIBRARY}_INCLUDE_DIRS}"
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${${LIBRARY}_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${${LIBRARY}_LIBRARIES}"
    )

  endif()


  find_package_handle_standard_args(${LIBRARY}
      REQUIRED_VARS ${LIBRARY}_INCLUDE_DIRS ${LIBRARY}_LIBRARIES
      VERSION_VAR ${LIBRARY}_VERSION
    )

  set(${LIBRARY}_FOUND ${${LIBRARY}_FOUND} PARENT_SCOPE)

  return()
endfunction()

