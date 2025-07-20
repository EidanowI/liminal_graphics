#add_executable (BigScene WIN32  "main.cpp")----------
add_executable (BigScene "main.cpp" "../Dependencies/glad/src/glad.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BigScene PROPERTY CXX_STANDARD 20)
endif()


include_directories("../Dependencies/GLFW/include")
include_directories("../Dependencies")
include_directories("../Dependencies/glad/include")

target_link_directories(BigScene PRIVATE "../Dependencies/GLFW/")
target_link_libraries(BigScene glfw3)


if (WIN32)
    target_compile_definitions(BigScene PRIVATE WIN)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(BigScene PRIVATE LINUX)
elseif (APPLE)
    target_compile_definitions(BigScene PRIVATE MACOS)
endif()