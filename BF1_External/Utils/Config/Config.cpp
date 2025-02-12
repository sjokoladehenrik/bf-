#include "Config.h"

Globals g;

bool IsKeyDown(int VK)
{
    return (GetAsyncKeyState(VK) & 0x8000) != 0;
}