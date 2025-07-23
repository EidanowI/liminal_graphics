#add_executable (LiminalGraphics WIN32  "main.cpp")----------
add_executable (LiminalGraphics "main.cpp" "../Dependencies/glad/src/glad.c" "Engine/Graphics/Mesh/Mesh.h" "Engine/Graphics/Mesh/Mesh.cpp" "Engine/Graphics/Textures/Textures.h" "Engine/Graphics/Textures/Textures.cpp" "Engine/Graphics/ShaderProgram/ShaderProgram.h" "Engine/Graphics/ShaderProgram/ShaderProgram.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LiminalGraphics PROPERTY CXX_STANDARD 20)
endif()


include_directories("../Dependencies/GLFW/include")
include_directories("../Dependencies")
include_directories("../Dependencies/glad/include")

target_link_directories(LiminalGraphics PRIVATE "../Dependencies/GLFW/")
target_link_libraries(LiminalGraphics glfw3 lodepng imgui)


if (WIN32)
    target_compile_definitions(LiminalGraphics PRIVATE WIN)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(LiminalGraphics PRIVATE LINUX)
elseif (APPLE)
    target_compile_definitions(LiminalGraphics PRIVATE MACOS)
endif()

target_compile_definitions(LiminalGraphics PRIVATE DEBUG)