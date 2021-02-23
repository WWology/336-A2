add_executable(${PROJECT_NAME}
	main.cpp
	Callbacks.cpp
	Shader.cpp
	IndexBuffer.cpp
	VertexBuffer.cpp
	VertexArray.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	GraphicsSDK
)

pvs_studio_add_target(TARGET Tutorial2d.analyze ALL
	OUTPUT FORMAT errorfile
	ANALYZE ${PROJECT_NAME}
	MODE GA:1,2 OP
	LOG Tutorial2d.err
	HIDE_HELP
)