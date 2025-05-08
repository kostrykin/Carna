include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
        BufferedIntensityVolumeTest
		BufferedNormalMap3DTest
		mathTest
        ColorMapTest
		ColorTest
		HUVTest
		VolumeGridHelperTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/BufferedIntensityVolumeTest.hpp
		UnitTests/BufferedNormalMap3DTest.hpp
		UnitTests/mathTest.hpp
		UnitTests/ColorMapTest.hpp
		UnitTests/ColorTest.hpp
		UnitTests/HUVTest.hpp
		UnitTests/VolumeGridHelperTest.hpp
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/BufferedIntensityVolumeTest.cpp
		UnitTests/BufferedNormalMap3DTest.cpp
		UnitTests/mathTest.cpp
		UnitTests/ColorMapTest.cpp
		UnitTests/ColorTest.cpp
		UnitTests/HUVTest.cpp
		UnitTests/VolumeGridHelperTest.cpp
	)
