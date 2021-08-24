set(TARGET_NAME "test")
set(COVERAGE_NAME "coverage")
set(COVERAGE_TARGET "coverage-test")
FILE(GLOB Tests_SRC "*.cpp")
set(TEST_SOURCES ${Tests_SRC})
FILE(GLOB SOURCES "../include/*.h")
run_coverage(
	"${TEST_SOURCES}"
	"${SOURCES}"
	"${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
