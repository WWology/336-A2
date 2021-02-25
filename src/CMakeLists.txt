add_executable(${PROJECT_NAME}
	main.cpp
	Callbacks.cpp
	Shader.cpp
	IndexBuffer.cpp
	VertexBuffer.cpp
	VertexArray.cpp
	Renderer.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	GraphicsSDK
)

set(AntTweakBar_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/AntTweakBar/include")

target_include_directories(${PROJECT_NAME} PUBLIC
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${assimp_INCLUDE_DIR}
	${AntTweakBar_INCLUDE_DIR}
)

pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE ${PROJECT_NAME}
	MODE GA:1,2 OP
	LOG ${PROJECT_NAME}.err
	HIDE_HELP
	ARGS -e ${CMAKE_SOURCE_DIR}/lib
)