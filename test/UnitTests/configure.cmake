include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
		BufferedHUVolumeTest
		mathTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/BufferedHUVolumeTest.h
		UnitTests/mathTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/BufferedHUVolumeTest.cpp
		UnitTests/mathTest.cpp
	)
