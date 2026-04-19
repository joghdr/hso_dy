cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION_STR)
string(STRIP "${VERSION_STR}" VERSION_STR)
project(HSODrellYan VERSION ${VERSION_STR} LANGUAGES CXX C)
set(ENGINE_TARGET "hso_dy")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(
  CORE_SOURCES
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

include(HSODependencyLinker)

find_package(GSL REQUIRED)
find_package(Minuit2 CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
hso_find_dependency(LHAPDF lhapdf "LHAPDF/LHAPDF.h")
hso_find_dependency(cuba cuba "cuba.h")

add_library(${ENGINE_TARGET}_obj OBJECT ${CORE_SOURCES})
set(OBJ_DIR "$<TARGET_PROPERTY:${ENGINE_TARGET}_obj,BINARY_DIR>/CMakeFiles/${ENGINE_TARGET}_obj.dir")
target_include_directories(${ENGINE_TARGET}_obj PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_library(${ENGINE_TARGET} STATIC $<TARGET_OBJECTS:${ENGINE_TARGET}_obj>)
target_include_directories(${ENGINE_TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(${ENGINE_TARGET} PUBLIC ${ENGINE_TARGET}_obj)
target_link_libraries(${ENGINE_TARGET}_obj PUBLIC
    GSL::gsl
    Minuit2::Minuit2
    LHAPDF::LHAPDF
    cuba::cuba
    Boost::boost
)

add_executable(HSODrellYanFitter programs/HSODrellYanFitter.cpp)
target_link_libraries(HSODrellYanFitter PRIVATE ${ENGINE_TARGET})
install(TARGETS HSODrellYanFitter DESTINATION bin)

add_executable(HSODrellYanHessian programs/HSODrellYanHessian.cpp)
target_link_libraries(HSODrellYanHessian PRIVATE ${ENGINE_TARGET})
install(TARGETS HSODrellYanHessian DESTINATION bin)

add_custom_target(update_registry_globals
  COMMAND "${CMAKE_COMMAND}" -E env
          "HSO_ROOT=${HSO_ROOT}"
          "APP_PATH=$<TARGET_FILE:HSODrellYanFitter>"
          "BINARIES_PATH=${OBJ_DIR}"
          bash "${HSO_ROOT}/tests/static_analysis/update_registry_globals.sh"
  WORKING_DIRECTORY "${HSO_ROOT}"
  COMMENT "***[HSO] Updating Registry of Globals."
  DEPENDS HSODrellYanFitter
  VERBATIM
)

set(AUDIT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/audit")
add_custom_command(
  TARGET HSODrellYanFitter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${AUDIT_OUTPUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_BINARY_DIR}/SKIP_TESTS"
  COMMENT "*** [HSO] Generating post-build audit metadata"
  VERBATIM)

############################################
############################################
#################  TESTS     ###############
############################################
############################################

enable_testing()

set(TEST_SYNTAX
      sa_test_S1-1
      sa_test_S2-1
      sa_test_S3-1
      sa_test_S5-1
    )
foreach(test_unit ${TEST_SYNTAX} )
  add_test(
    NAME "${test_unit}"
    COMMAND bash "tests/static_analysis/${test_unit}.sh"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" )
  set_tests_properties(
    "${test_unit}" PROPERTIES
    ENVIRONMENT "HSO_ROOT=${HSO_ROOT};HSO_BUILD_DIR=${HSO_BUILD_DIR};HSO_BUILD_TYPE=${HSO_BUILD_TYPE}")
  set_tests_properties("${test_unit}" PROPERTIES LABELS "syntax" )
endforeach()

set(TEST_STATIC
      sa_test_B1-B2-1
      sa_test_B3-1
    )
foreach(test_unit ${TEST_STATIC} )
  add_test(
    NAME "${test_unit}"
    COMMAND bash "tests/static_analysis/${test_unit}.sh"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set_property(
    TEST "${test_unit}" APPEND PROPERTY
    ENVIRONMENT "HSO_ROOT=${HSO_ROOT};HSO_BUILD_DIR=${HSO_BUILD_DIR};HSO_BUILD_TYPE=${HSO_BUILD_TYPE}")
  set_property(
    TEST "${test_unit}" APPEND PROPERTY
    ENVIRONMENT "APP_PATH=$<TARGET_FILE:HSODrellYanFitter>;BINARIES_PATH=${OBJ_DIR}")
  set_tests_properties("${test_unit}" PROPERTIES LABELS "static" )
endforeach()

set(TEST_SYSTEM
      sv_test_fit_fast
    )
foreach(test_unit ${TEST_SYSTEM} )
  add_test(
    NAME "${test_unit}"
    COMMAND bash "tests/system_validation/${test_unit}/run.sh"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set_tests_properties(
    "${test_unit}" PROPERTIES
    PROCESSORS 5
    RUN_SERIAL TRUE
    ENVIRONMENT "CUBACORES=5;HSO_ROOT=${HSO_ROOT};HSO_BUILD_DIR=${HSO_BUILD_DIR};HSO_BUILD_TYPE=${HSO_BUILD_TYPE};EXEC=$<TARGET_FILE:HSODrellYanFitter>")
  set_tests_properties(
    "${test_unit}" PROPERTIES
    LABELS "system")
endforeach()

# leverage test patterns for building docker image
add_test(
  NAME build_docker_image
  COMMAND bash "scripts/build_docker_image.sh"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" )
set_tests_properties(
  build_docker_image PROPERTIES
  ENVIRONMENT "HSO_ROOT=${HSO_ROOT};HSO_BUILD_DIR=${HSO_BUILD_DIR};HSO_BUILD_TYPE=${HSO_BUILD_TYPE};IMAGE_NAME=${ENGINE_TARGET};VERSION=${VERSION_STR}"
  LABELS "infrastructure")


