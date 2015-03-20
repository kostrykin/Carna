include_directories( ${CMAKE_PROJECT_DIR}IntegrationTests )

list( APPEND TESTS
		FrameRendererIntegrationTest
		ParallaxStageIntegrationTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		IntegrationTests/FrameRendererIntegrationTest.h
		IntegrationTests/ParallaxStageIntegrationTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		IntegrationTests/FrameRendererIntegrationTest.cpp
		IntegrationTests/ParallaxStageIntegrationTest.cpp
	)
