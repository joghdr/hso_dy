cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION_STR)
string(STRIP "${VERSION_STR}" VERSION_STR)
project(HSODrellYan VERSION ${VERSION_STR} LANGUAGES CXX C)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(HSODependencyLinker)
set(ENGINE_TARGET "hso_engine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(AUDIT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/audit")


find_package(GSL)
find_package(OpenMP)
find_package(Minuit2 CONFIG QUIET)
hso_find_dependency(LHAPDF lhapdf "LHAPDF/LHAPDF.h")
hso_find_dependency(cuba cuba "cuba.h")

set(ALL_DEPENDENCIES_FOUND TRUE)
set(REQUIRED_LIBS  GSL OpenMP Minuit2 LHAPDF cuba XXX )
foreach(LIB ${REQUIRED_LIBS})
  if(NOT ${LIB}_FOUND)
    set(ALL_DEPENDENCIES_FOUND FALSE)
    message("*** [HSO] Required dependency ${LIB} not found.")
  else()
  endif()
endforeach()

get_filename_component(BUILD_DIR_NAME "${CMAKE_BINARY_DIR}" NAME)
if(NOT ALL_DEPENDENCIES_FOUND)
  add_custom_target(skip_build
    COMMAND ${CMAKE_COMMAND} -E touch SKIP_TESTS
    COMMENT "[HSO] Some dependencies not found. Local build and verify are disabled."
  )
  add_custom_target(try_build DEPENDS skip_build)
else()
  add_custom_target(perform_build
    COMMAND ${CMAKE_COMMAND} -E remove SKIP_TESTS
    DEPENDS HSODrellYanFitter
  )
  add_custom_target(try_build
    DEPENDS perform_build
  )
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

file(MAKE_DIRECTORY "${AUDIT_OUTPUT_DIR}")
file(WRITE "${AUDIT_OUTPUT_DIR}/target_name.txt" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/HSODrellYanFitter")
file(WRITE "${AUDIT_OUTPUT_DIR}/audit_obj_path.txt" "${CMAKE_BINARY_DIR}/CMakeFiles/${ENGINE_TARGET}.dir/src")
file(WRITE "${AUDIT_OUTPUT_DIR}/audit_bin_path.txt" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/HSODrellYanFitter")


# Ensure the directory exists


