cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION_STR)
string(STRIP "${VERSION_STR}" VERSION_STR)
project(HSODrellYan VERSION ${VERSION_STR} LANGUAGES CXX C)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(HSODependencyLinker)
set(ENGINE_TARGET "hso_engine")
set(AUDIT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/audit")

set(REQUIRED_LIBS
    GSL OpenMP Minuit2 LHAPDF cuba
    # xGSL xOpenMP xMinuit2 xLHAPDF xcuba
    )
find_package(GSL)
find_package(OpenMP)
find_package(Minuit2 CONFIG QUIET)
hso_find_dependency(LHAPDF lhapdf "LHAPDF/LHAPDF.h")
hso_find_dependency(cuba cuba "cuba.h")

set(MISSING_LIBS "")
set(ALL_DEPENDENCIES_FOUND TRUE)
foreach(LIB ${REQUIRED_LIBS})
  if(NOT ${LIB}_FOUND)
    set(ALL_DEPENDENCIES_FOUND FALSE)
    list(APPEND MISSING_LIBS ${LIB})
    message("*** [HSO] Required dependency ${LIB} not found.")
  else()
  endif()
endforeach()

if(NOT ALL_DEPENDENCIES_FOUND)
  list(JOIN MISSING_LIBS "\n" MISSING_LIBS_LOG)
  file(WRITE "${CMAKE_BINARY_DIR}/hso_missing_deps.log" "${MISSING_LIBS_LOG}\n"
      )
  add_custom_target(skip_build
  COMMAND ${CMAKE_COMMAND} -E touch SKIP_TESTS
  COMMENT "*** [HSO] Local build and verify are disabled (missing dependencies)."
  )
  add_custom_target(try_build DEPENDS skip_build)
  if(EXISTS "${CMAKE_BINARY_DIR}/audit")
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/audit")
  endif()
  if(EXISTS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    file(REMOVE_RECURSE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  endif()
  file(GLOB STALE_LIBS "${CMAKE_BINARY_DIR}/*.a")
  if(STALE_LIBS)
      file(REMOVE ${STALE_LIBS})
  endif()
  if(EXISTS "${CMAKE_BINARY_DIR}/CMakeFiles/${ENGINE_TARGET}.dir")
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/CMakeFiles/${ENGINE_TARGET}.dir")
  endif()
else()
  add_custom_target(perform_build
    COMMAND ${CMAKE_COMMAND} -E remove SKIP_TESTS
    DEPENDS HSODrellYanFitter HSODrellYanHessian
  )
  add_custom_target(try_build DEPENDS perform_build)
  if(EXISTS "${CMAKE_BINARY_DIR}/hso_missing_deps.log")
    file(REMOVE "${CMAKE_BINARY_DIR}/hso_missing_deps.log")
  endif()
endif()

set(CORE_SOURCES
  src/behavior.cpp
  src/collinear_factorization.cpp
  src/csec_DY_css_hso_integrands_cuba.cpp
  src/CSkernel.cpp
  src/data_class.cpp
  src/evo.cpp
  src/FCN.cpp
  src/math_functions.cpp
  src/models_fcn-tmd-gaussx1-CS-gauss.cpp
  src/objects_data.cpp
  src/objects_stat.cpp
  src/objects_theory.cpp
  src/pdf_cutoff_from_msbar.cpp
  src/pqcd.cpp
  src/read_data.cpp
  src/read_kin.cpp
  src/read_para.cpp
  src/scale_setting.cpp
  src/stat_class.cpp
  src/theory_class.cpp
  src/tmdPDF.cpp
  src/utilities.cpp
  src/WDY_hso.cpp
)

add_library(${ENGINE_TARGET} STATIC ${CORE_SOURCES})
target_include_directories(${ENGINE_TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(${ENGINE_TARGET} PRIVATE GSL::gsl)
target_link_libraries(${ENGINE_TARGET} PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(${ENGINE_TARGET} PUBLIC Minuit2::Minuit2 )
target_link_libraries(${ENGINE_TARGET} PUBLIC LHAPDF::LHAPDF  )
target_link_libraries(${ENGINE_TARGET} PUBLIC cuba::cuba)

add_executable(HSODrellYanFitter programs/HSODrellYanFitter.cpp)
target_link_libraries(HSODrellYanFitter PRIVATE ${ENGINE_TARGET})
install(TARGETS HSODrellYanFitter DESTINATION bin)

add_executable(HSODrellYanHessian programs/HSODrellYanHessian.cpp)
target_link_libraries(HSODrellYanHessian PRIVATE ${ENGINE_TARGET})
install(TARGETS HSODrellYanHessian DESTINATION bin)

add_custom_command(TARGET HSODrellYanFitter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${AUDIT_OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:HSODrellYanFitter>" > "${AUDIT_OUTPUT_DIR}/target_name.txt"
    COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_BINARY_DIR}/CMakeFiles/${ENGINE_TARGET}.dir/src" > "${AUDIT_OUTPUT_DIR}/audit_obj_path.txt"
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:HSODrellYanFitter>" > "${AUDIT_OUTPUT_DIR}/audit_bin_path.txt"
    COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_BINARY_DIR}/SKIP_TESTS"
    COMMENT "*** [HSO] Generating post-build audit metadata"
    VERBATIM
  )



