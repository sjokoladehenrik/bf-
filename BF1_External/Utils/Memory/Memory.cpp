#include "Memory.h"

bool Memory::Init()
{
    TargetHwnd = FindWindowW(NULL, L"Battlefield™ 1");

    if (!TargetHwnd)
    {
        MessageBoxA(nullptr, "Waiting BF1...", "", MB_OK | MB_TOPMOST);

        while (!TargetHwnd)
        {
            TargetHwnd = FindWindowW(NULL, L"Battlefield™ 1");

            Sleep(500);
        }
    }

    GetWindowThreadProcessId(TargetHwnd, &PID);
    pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    if (!pHandle)
    {
        MessageBoxA(nullptr, "Failed to get pHandle", "ERROR", MB_OK | MB_TOPMOST | MB_ICONERROR);

        return false;
    }

    return true;
}

Memory m;