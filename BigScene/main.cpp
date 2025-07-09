#ifdef WIN
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


#include "BigSceneApp.h"



#ifdef WIN
int main()
//int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#elif LINUX
int main()
#elif MACOS
int main()
#endif
{
    BigSceneApp app = BigSceneApp();

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << "RUNTIME ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}