include_directories( ${CMAKE_PROJECT_DIR}ModuleTests )

list( APPEND TESTS
		CuttingPlanesStageTest
		DRRStageTest
		FrameRendererHelperTest
		HUVolumeGridTest
		MeshColorCodingStageTest
		MIPStageTest
		OpaqueRenderingStageTest
		ParallaxStageTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		ModuleTests/CuttingPlanesStageTest.h
		ModuleTests/DRRStageTest.h
		ModuleTests/FrameRendererHelperTest.h
		ModuleTests/HUVolumeGridTest.h
		ModuleTests/MeshColorCodingStageTest.h
		ModuleTests/MIPStageTest.h
		ModuleTests/OpaqueRenderingStageTest.h
		ModuleTests/ParallaxStageTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		ModuleTests/CuttingPlanesStageTest.cpp
		ModuleTests/DRRStageTest.cpp
		ModuleTests/FrameRendererHelperTest.cpp
		ModuleTests/HUVolumeGridTest.cpp
		ModuleTests/MeshColorCodingStageTest.cpp
		ModuleTests/MIPStageTest.cpp
		ModuleTests/OpaqueRenderingStageTest.cpp
		ModuleTests/ParallaxStageTest.cpp
	)
