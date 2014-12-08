include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
		SceneTest
		CounterTest
		HistogramTest
		VolumeHistogramTest
		HistogramClassDetectorTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/SceneTest.h
		UnitTests/CounterTest.h
		UnitTests/HistogramTest.h
		UnitTests/VolumeHistogramTest.h
		UnitTests/HistogramClassDetectorTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/SceneTest.cpp
		UnitTests/CounterTest.cpp
		UnitTests/HistogramTest.cpp
		UnitTests/VolumeHistogramTest.cpp
		UnitTests/HistogramClassDetectorTest.cpp
	)
