#pragma once
#include <Windows.h>

struct Globals
{
    // System
    bool Run = false;
    bool ShowMenu = false;
    int MenuKey = VK_INSERT;
    RECT GameSize;
    POINT GamePoint;

    // AimBot
    bool AimBot = false;
    float AimFov = 150.f;
    float Smooth = 1.f;

    // Visual
    bool ESP = true;
    bool TeamESP = false;

    // V-Options
    bool ESP_Box = true;
    bool ESP_Line = false;
    bool ESP_Distance = true;
    bool ESP_HealthBar = true;
    bool ESP_Name = true;
    bool ESP_Skeleton = true;
    float ESP_MaxDistance = 500.f;
};

extern Globals g;
extern bool IsKeyDown(int VK);
