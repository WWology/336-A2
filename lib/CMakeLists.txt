find_package(OpenGL REQUIRED)

set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED)

find_package(glfw3 3.3 REQUIRED)

set(glm_DIR "glm/cmake/glm")
find_package(glm REQUIRED)

find_library(AntTweakBar
	libAntTweakBar.a
	NAMES AntTweakBar anttweakbar ANTTWEAKBAR
	PATHS "AntTweakBar/lib/"
	DOC "AntTweakBar Library"
	REQUIRED
)

find_package(assimp REQUIRED)

add_library(GraphicsSDK INTERFACE)

target_link_libraries(GraphicsSDK INTERFACE
	OpenGL::GL
	GLEW::glew_s
	glfw
	glm::glm
	${AntTweakBar}
	assimp
)