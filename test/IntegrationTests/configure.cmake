include_directories( ${CMAKE_PROJECT_DIR}IntegrationTests )

list( APPEND TESTS
        DRRMaskRenderingStageIntegrationTest
        DVRMaskRenderingStageIntegrationTest
		FrameRendererIntegrationTest
		ParallaxStageIntegrationTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
        IntegrationTests/DRRMaskRenderingStageIntegrationTest.hpp
        IntegrationTests/DVRMaskRenderingStageIntegrationTest.hpp
		IntegrationTests/FrameRendererIntegrationTest.hpp
		IntegrationTests/ParallaxStageIntegrationTest.hpp
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
        IntegrationTests/DRRMaskRenderingStageIntegrationTest.cpp
        IntegrationTests/DVRMaskRenderingStageIntegrationTest.cpp
		IntegrationTests/FrameRendererIntegrationTest.cpp
		IntegrationTests/ParallaxStageIntegrationTest.cpp
	)
