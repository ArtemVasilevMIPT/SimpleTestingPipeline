cmake_minimum_required(VERSION 3.8)

project(AdditionalFiles)

FILE(GLOB Libs "${SimpleTestingPipeline_SOURCE_DIR}/include/*.h")
 

add_library(
	test_library SHARED source.cpp ${Libs}
)
