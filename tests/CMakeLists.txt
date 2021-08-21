set(TARGET_NAME "test")
set(COVERAGE_NAME "coverage")
set(COVERAGE_TARGET "coverage-test")
set(TEST_SOURCES ../include/TestCase.h TestCase)

run_coverage(
	"${TEST_SOURCES}"
	""
	"${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
