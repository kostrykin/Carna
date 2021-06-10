include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
		BufferedHUVolumeTest
        BufferedIntensityVolumeTest
		BufferedNormalMap3DTest
		mathTest
		ColorTest
		HUVTest
		VolumeGridHelperTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/BufferedHUVolumeTest.h
		UnitTests/BufferedIntensityVolumeTest.h
		UnitTests/BufferedNormalMap3DTest.h
		UnitTests/mathTest.h
		UnitTests/ColorTest.h
		UnitTests/HUVTest.h
		UnitTests/VolumeGridHelperTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/BufferedHUVolumeTest.cpp
		UnitTests/BufferedIntensityVolumeTest.cpp
		UnitTests/BufferedNormalMap3DTest.cpp
		UnitTests/mathTest.cpp
		UnitTests/ColorTest.cpp
		UnitTests/HUVTest.cpp
		UnitTests/VolumeGridHelperTest.cpp
	)
