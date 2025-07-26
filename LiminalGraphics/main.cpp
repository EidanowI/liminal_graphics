#include <cstdlib>
#ifdef WIN
#include <Windows.h>
#endif

#include "App.h"



#ifdef WIN
int main()
//int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#elif LINUX
int main()
#elif MACOS
int main()
#endif
{
    #ifdef USE_POWERFULL_GPU
    setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
    setenv("__NV_PRIME_RENDER_OFFLOAD_PROVIDER", "NVIDIA-G0", 1);
    setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
    setenv("__VK_LAYER_NV_optimus", "NVIDIA_only", 1);
    setenv("DRI_PRIME", "1", 1);
    #endif
    
    App app;

    app.Run();

    return 0;
}
