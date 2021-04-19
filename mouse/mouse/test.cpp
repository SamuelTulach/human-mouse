#include "human_mouse.h"

HumanMouse mouse;

int main()
{
    mouse.SetTarget(200, 200);
    mouse.Start();

    while (!(GetKeyState(VK_F4) & 0x8000))
    {
        if (GetKeyState(VK_F3) & 0x8000)
        {
            SetCursorPos(rand() % 2560 - 1, rand () % 1440 - 1);
            Sleep(100);
        }

        if (GetKeyState(VK_F2) & 0x8000)
        {
            mouse.SetTarget(rand() % 2560 - 1, rand() % 1440 - 1);
        }
    }

    mouse.Stop();

    return 0;
}

