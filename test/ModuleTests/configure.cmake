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
        MaskRenderingStageTest
		PointMarkerHelperTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		ModuleTests/CuttingPlanesStageTest.hpp
		ModuleTests/DRRStageTest.hpp
		ModuleTests/FrameRendererHelperTest.hpp
		ModuleTests/VolumeGridTest.hpp
		ModuleTests/MeshColorCodingStageTest.hpp
		ModuleTests/MIPStageTest.hpp
		ModuleTests/OpaqueRenderingStageTest.hpp
		ModuleTests/TransparentRenderingStageTest.hpp
		ModuleTests/DVRStageTest.hpp
		ModuleTests/MaskRenderingStageTest.hpp
		ModuleTests/PointMarkerHelperTest.hpp
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
		ModuleTests/MaskRenderingStageTest.cpp
		ModuleTests/PointMarkerHelperTest.cpp
	)
