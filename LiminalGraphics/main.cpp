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
    App app;

    app.Run();

    return 0;
}