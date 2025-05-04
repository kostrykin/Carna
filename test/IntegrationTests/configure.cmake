include_directories( ${CMAKE_PROJECT_DIR}IntegrationTests )

list( APPEND TESTS
		FrameRendererIntegrationTest
		ParallaxStageIntegrationTest
        MaskRenderingStageIntegrationTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		IntegrationTests/FrameRendererIntegrationTest.hpp
		IntegrationTests/ParallaxStageIntegrationTest.hpp
        IntegrationTests/MaskRenderingStageIntegrationTest.hpp
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		IntegrationTests/FrameRendererIntegrationTest.cpp
		IntegrationTests/ParallaxStageIntegrationTest.cpp
        IntegrationTests/MaskRenderingStageIntegrationTest.cpp
	)
