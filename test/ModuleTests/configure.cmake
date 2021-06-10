include_directories( ${CMAKE_PROJECT_DIR}ModuleTests )

list( APPEND TESTS
		CuttingPlanesStageTest
		DRRStageTest
		FrameRendererHelperTest
		VolumeGridTestUInt8
		VolumeGridTestUInt16
		MeshColorCodingStageTest
		MIPStageTest
		OpaqueRenderingStageTest
		TransparentRenderingStageTest
		DVRStageTest
		PointMarkerHelperTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		ModuleTests/CuttingPlanesStageTest.h
		ModuleTests/DRRStageTest.h
		ModuleTests/FrameRendererHelperTest.h
		ModuleTests/VolumeGridTest.h
		ModuleTests/MeshColorCodingStageTest.h
		ModuleTests/MIPStageTest.h
		ModuleTests/OpaqueRenderingStageTest.h
		ModuleTests/TransparentRenderingStageTest.h
		ModuleTests/DVRStageTest.h
		ModuleTests/PointMarkerHelperTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		ModuleTests/CuttingPlanesStageTest.cpp
		ModuleTests/DRRStageTest.cpp
		ModuleTests/FrameRendererHelperTest.cpp
		ModuleTests/VolumeGridTest.cpp
		ModuleTests/MeshColorCodingStageTest.cpp
		ModuleTests/MIPStageTest.cpp
		ModuleTests/OpaqueRenderingStageTest.cpp
		ModuleTests/TransparentRenderingStageTest.cpp
		ModuleTests/DVRStageTest.cpp
		ModuleTests/PointMarkerHelperTest.cpp
	)
