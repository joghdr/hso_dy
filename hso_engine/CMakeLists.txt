cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" HSO_VERSION)
string(STRIP "${HSO_VERSION}" HSO_VERSION)
project(HSODrellYan VERSION ${HSO_VERSION} LANGUAGES CXX C)
set(ENGINE_TARGET "hso-dy")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(HSOLayout)

if(NOT HSO_LAYOUT_LOADED)
    message(FATAL_ERROR "HSOLayout not found or corrupted.")
endif()

set(
  CORE_SOURCES
  src/behavior.cpp
  src/collinear_factorization.cpp
  src/csec_DY_css_hso_integrands_cuba.cpp
  src/CSkernel.cpp
  src/data_class.cpp
  src/data_loader.cpp
  src/evo.cpp
  src/FCN.cpp
  src/math_functions.cpp
  src/models_fcn-tmd-gaussx1-CS-gauss.cpp
  src/objects_data.cpp
  src/objects_stat.cpp
  src/objects_theory.cpp
  src/output_dir_tree.cpp
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


  add_library(${ENGINE_TARGET}_obj OBJECT ${CORE_SOURCES})
  set(OBJ_DIR "$<TARGET_PROPERTY:${ENGINE_TARGET}_obj,BINARY_DIR>/CMakeFiles/${ENGINE_TARGET}_obj.dir")
  target_include_directories(${ENGINE_TARGET}_obj PUBLIC "${CMAKE_SOURCE_DIR}/include")

  add_library(${ENGINE_TARGET} STATIC $<TARGET_OBJECTS:${ENGINE_TARGET}_obj>)
  target_include_directories(${ENGINE_TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")

  target_link_libraries(${ENGINE_TARGET} PUBLIC ${ENGINE_TARGET}_obj)

option(HSO_DOCKER_ONLY "skip dependency check, done by Docker" OFF)

if(NOT HSO_DOCKER_ONLY)
  include(HSODependencyFinder)

  find_package(GSL REQUIRED)
  find_package(Minuit2 CONFIG REQUIRED)
  hso_find_dependency(LHAPDF lhapdf "LHAPDF/LHAPDF.h")
  hso_find_dependency(cuba cuba "cuba.h")

  target_link_libraries(${ENGINE_TARGET}_obj PUBLIC
      GSL::gsl
      Minuit2::Minuit2
      LHAPDF::LHAPDF
      cuba::cuba
  )
endif()

add_executable(HSODrellYanFitter programs/HSODrellYanFitter.cpp)
target_link_libraries(HSODrellYanFitter PRIVATE  ${ENGINE_TARGET})
install(TARGETS HSODrellYanFitter DESTINATION ${CMAKE_BINARY_DIR}/bin)

add_executable(HSODrellYanHessian programs/HSODrellYanHessian.cpp)
target_link_libraries(HSODrellYanHessian PRIVATE ${ENGINE_TARGET})
install(TARGETS HSODrellYanHessian DESTINATION ${CMAKE_BINARY_DIR}/bin)

############################################
############################################
#################  CUSTOM TARGETS     ######
############################################
############################################

add_custom_target(update_registry_globals
  COMMAND ${CMAKE_COMMAND} -E env ${HSO_ENV_BLOCK}
          bash "tests/static_analysis/update_registry_globals.sh"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMENT "***[HSO] Updating Registry of Globals."
  DEPENDS HSODrellYanFitter
  VERBATIM
)


############################################
############################################
#################  TESTS     ###############
############################################
############################################

if(NOT TEST_CORES)
    set(TEST_CORES 5)
endif()

enable_testing()

set(TEST_SYNTAX
      sa_test_S2-1
      sa_test_S3-1
    )
foreach(test_unit ${TEST_SYNTAX} )
  add_test(
    NAME "${test_unit}"
    COMMAND bash "${HSO_TEST_SA_DIR}/${test_unit}.sh"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" )
  set_tests_properties(
    "${test_unit}" PROPERTIES
    LABELS "syntax")
  set_property(
    TEST "${test_unit}" APPEND PROPERTY
    ENVIRONMENT ${HSO_ENV_BLOCK})
endforeach()

set(TEST_STATIC
      sa_test_B1-B2-1
      sa_test_B3-1
    )
foreach(test_unit ${TEST_STATIC} )
  add_test(
    NAME "${test_unit}"
    COMMAND bash "${HSO_TEST_SA_DIR}/${test_unit}.sh"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

  set_tests_properties(
    "${test_unit}" PROPERTIES
    LABELS "static"
  )
  set_property(
    TEST "${test_unit}" APPEND PROPERTY
    ENVIRONMENT ${HSO_ENV_BLOCK})
endforeach()

set(TEST_SYSTEM
      sv_test_fit_fast
    )
foreach(test_unit ${TEST_SYSTEM} )
  add_test(
    NAME "${test_unit}"
    COMMAND ${CMAKE_COMMAND} -E env EXEC=$<TARGET_FILE:HSODrellYanFitter>
            bash "${HSO_TEST_SV_SCRIPTS_DIR}/${test_unit}.sh"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set_tests_properties(
    "${test_unit}" PROPERTIES
    PROCESSORS "${TEST_CORES}"
    RUN_SERIAL TRUE
    LABELS "system")
  set_property(
    TEST "${test_unit}" APPEND PROPERTY
    ENVIRONMENT "CUBACORES=${TEST_CORES}"
                ${HSO_ENV_BLOCK})
endforeach()

  ############################################
  ############################################
  #################  Docker Images     #######
  ############################################
  ############################################

add_custom_target(hso_docker_image_dev
    COMMAND
            docker build .
            --target hso-dy-dev
            --build-arg HSO_IMAGE_NAME=${ENGINE_TARGET}
            --build-arg HSO_VERSION=${HSO_VERSION}
            -t ${ENGINE_TARGET}-dev:${HSO_VERSION}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    USES_TERMINAL
)

add_custom_target(hso_docker_image_deps
    COMMAND
            docker build .
            --target hso-dy-deps
            --build-arg HSO_IMAGE_NAME=${ENGINE_TARGET}
            --build-arg HSO_VERSION=${HSO_VERSION}
            -t ${ENGINE_TARGET}-deps:${HSO_VERSION}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS hso_docker_image_dev
    USES_TERMINAL
)

add_custom_target(hso_docker_image_engine
    COMMAND
            docker build .
            --target hso-dy
            --build-arg HSO_IMAGE_NAME=${ENGINE_TARGET}
            --build-arg HSO_VERSION=${HSO_VERSION}
            -t ${ENGINE_TARGET}:${HSO_VERSION}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS hso_docker_image_deps
    USES_TERMINAL
)
