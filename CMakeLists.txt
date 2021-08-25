cmake_minimum_required(VERSION 3.8)

project(SimpleTestingPipeline)

include(ExternalProject)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake ${SimpleTestingPipeline_SOURCE_DIR})

set(EXTERNAL_INSTALL_LOCATION ${SimpleTestingPipeline_SOURCE_DIR}/3rdparty)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SimpleTestingPipeline_SOURCE_DIR}/bin)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
if (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") 
endif()

ExternalProject_Add(
    googletest_repo
    GIT_REPOSITORY https://github.com/google/googletest.git
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include include src)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

execute_process(COMMAND ${SimpleTestingPipeline_SOURCE_DIR}/install_deps.sh ${Python3_EXECUTABLE} WORKING_DIRECTORY ${SimpleTestingPipeline_SOURCE_DIR})

set(CMAKE_PROGRAM_PATH "${CMAKE_PROGRAM_PATH}" ${SimpleTestingPipeline_SOURCE_DIR}/PythonEnvironment/bin)

include(CodeCoverage)
add_custom_target(coverage_report)

function(run_coverage test_sources sources coverage_target target_name coverage_name)
    set(test_name "${target_name}-test")
    message(INFO sources "${sources}")
    add_executable(${test_name} ${sources} ${test_sources})


    target_link_libraries(
		${test_name} gtest_main gtest gmock Threads::Threads #gcovr
	)

    setup_target_for_coverage_gcovr_html(
	    NAME ${coverage_name} 
	    EXECUTABLE ${test_name} 
    )
    add_dependencies(${test_name} googletest_repo)

    add_dependencies(coverage_report ${coverage_target})
endfunction()

add_subdirectory(src)
add_subdirectory(tests)
