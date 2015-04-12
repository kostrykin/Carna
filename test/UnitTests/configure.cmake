include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
		BufferedHUVolumeTest
		BufferedNormalMap3DTest
		mathTest
		ColorTest
		VolumeGridHelperTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/BufferedHUVolumeTest.h
		UnitTests/BufferedNormalMap3DTest.h
		UnitTests/mathTest.h
		UnitTests/ColorTest.h
		UnitTests/VolumeGridHelperTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/BufferedHUVolumeTest.cpp
		UnitTests/BufferedNormalMap3DTest.cpp
		UnitTests/mathTest.cpp
		UnitTests/ColorTest.cpp
		UnitTests/VolumeGridHelperTest.cpp
	)
