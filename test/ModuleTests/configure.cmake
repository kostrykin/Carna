include_directories( ${CMAKE_PROJECT_DIR}ModuleTests )

list( APPEND TESTS
		CuttingPlanesStageTest
		DRRStageTest
		MIPStageTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		ModuleTests/CuttingPlanesStageTest.h
		ModuleTests/DRRStageTest.h
		ModuleTests/MIPStageTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		ModuleTests/CuttingPlanesStageTest.cpp
		ModuleTests/DRRStageTest.cpp
		ModuleTests/MIPStageTest.cpp
	)
