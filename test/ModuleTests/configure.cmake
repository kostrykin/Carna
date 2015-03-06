include_directories( ${CMAKE_PROJECT_DIR}ModuleTests )

list( APPEND TESTS
		OffscreenRenderingTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		ModuleTests/OffscreenRenderingTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		ModuleTests/OffscreenRenderingTest.cpp
	)
