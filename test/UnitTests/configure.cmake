include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
		mathTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/mathTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/mathTest.cpp
	)
