#include "Cheat.h"

int BoxType = 1;
int LinePos = 2;
const char* BoxTypeList[] = { "Simple", "Cornered" };
const char* LinePosList[] = { "Top", "Center", "Bottom" };
Vector2 AimTargetPos{};

// Info
void Cheat::RenderInfo()
{
    if (g.AimBot)
        Circle(ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom / 2.f), g.AimFov, ImColor(1.f, 1.f, 1.f, 1.f));
}

// Menu
void Cheat::RenderMenu()
{
    ImGui::SetNextWindowSize(ImVec2(650.f, 400.f));
    ImGui::Begin("BF1-External [ unknowncheats.me ]", &g.ShowMenu);

    ImGui::BeginChild("##V-Child-01", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 16.f, ImGui::GetContentRegionAvail().y));

    ImGui::Text("AimBot");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Checkbox("AimBot", &g.AimBot);

    ImGui::NewLine();

    ImGui::Text("Visual");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Checkbox("ESP", &g.ESP);
    ImGui::Checkbox("Team ESP", &g.TeamESP);

    ImGui::NewLine();
    ImGui::Text("ESP Options");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Checkbox("Box", &g.ESP_Box);
    ImGui::Checkbox("Line", &g.ESP_Line);
    ImGui::Checkbox("Distance", &g.ESP_Distance);
    ImGui::Checkbox("HealthBar", &g.ESP_HealthBar);
    ImGui::Checkbox("Skeleton", &g.ESP_Skeleton);
    ImGui::Checkbox("Name", &g.ESP_Name);

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##V-Child-02", ImVec2(ImGui::GetContentRegionAvail()));

    ImGui::Text("Aim Config");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::SliderFloat("FOV", &g.AimFov, 100.f, 300.f);
    ImGui::SliderFloat("Smooth", &g.Smooth, 1.f, 20.f);

    ImGui::NewLine();

    ImGui::Text("ESP Config");
    ImGui::Separator();
    ImGui::Spacing();
   
    ImGui::Combo("BoxType", &BoxType, BoxTypeList, IM_ARRAYSIZE(BoxTypeList));
    ImGui::Combo("Line Position", &LinePos, LinePosList, IM_ARRAYSIZE(LinePosList));
    ImGui::SliderFloat("Distance", &g.ESP_MaxDistance, 50.f, 2000.f);

    ImGui::NewLine();

    ImGui::Text("Color");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::ColorEdit4("Normal ", &Col_ESP_Normal.Value.x);
    ImGui::ColorEdit4("Visible", &Col_ESP_Visible.Value.x);
    ImGui::ColorEdit4("Team", &Col_ESP_Team.Value.x);

    ImGui::EndChild();
}

void AimBot(Vector2 TargetPos)
{
    // AimBot
    Vector2 Mid = { g.GameSize.right / 2.f, g.GameSize.bottom / 2.f };
    int cX = (Mid.x - TargetPos.x) / g.Smooth;
    int cY = (Mid.y - TargetPos.y) / g.Smooth;

    mouse_event(MOUSEEVENTF_MOVE, -cX, -cY, 0, 0);
}

// ESP
void Cheat::RenderESP()
{
    CPlayer* pLocal = &local;
    if (!pLocal->Update())
        return;

    UpdateData(); // for W2S
    float FOV = 0.f;
    float MinDistance = 9999.f;
    AimTargetPos = {};
    Vector2 Center = { g.GameSize.right / 2.f, g.GameSize.bottom / 2.f };

    for (auto player : PlayerList)
    {
        CPlayer *pEntity = &player;
        if (!pEntity->Update())
            continue;

        // GetDistance
        float distance = GetDistance(pLocal->Position, pEntity->Position);

        // SomeChecks
        if (pEntity->InVehicle())
            continue;
        else if (!g.TeamESP && pEntity->Team == pLocal->Team)
            continue;
        else if (distance > g.ESP_MaxDistance)
            continue;

        // WorldToScreen
        Vector2 g_Screen{};
        if (!WorldToScreen(pEntity->Position, g_Screen))
            continue;

        // SetColor
        ImColor pColor = pEntity->IsVisible() ? Col_ESP_Visible : Col_ESP_Normal;
        if (pEntity->Team == pLocal->Team)
            pColor = Col_ESP_Team;

        // Box and more用
        Vector3 BoxTop = pEntity->Position + pEntity->GetAABB().Max;
        Vector2 vTop{};
        if (!WorldToScreen(BoxTop, vTop))
            continue;

        // Sizes
        float Height = g_Screen.y - vTop.y;
        float Width = Height / 4.f; // Half

        // Line
        if (g.ESP_Line)
        {
            ImVec2 StartPos{};

            switch (LinePos)
            {
            case 0:
                StartPos = ImVec2(g.GameSize.right / 2.f, g.GameSize.top);
                break;
            case 1:
                StartPos = ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom / 2.f);
                break;
            case 2:
                StartPos = ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom);
                break;
            }

            DrawLine(StartPos, ImVec2(g_Screen.x, g_Screen.y), pColor, 1.f);
        }

        // Box
        if (g.ESP_Box)
        {
            float bScale = Width / 1.6f;

            switch (BoxType)
            {
            case 0:
                DrawLine(ImVec2(g_Screen.x + Width, vTop.y), ImVec2(g_Screen.x - Width, vTop.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, g_Screen.y), ImVec2(g_Screen.x - Width, g_Screen.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, vTop.y), ImVec2(g_Screen.x + Width, g_Screen.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - Width , vTop.y), ImVec2(g_Screen.x - Width, g_Screen.y), pColor, 1.f);
                break;
            case 1:
                DrawLine(ImVec2(g_Screen.x - Width, vTop.y), ImVec2(g_Screen.x - Width + bScale, vTop.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, vTop.y), ImVec2(g_Screen.x + Width - bScale, vTop.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - Width, vTop.y), ImVec2(g_Screen.x - Width, vTop.y + bScale), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, vTop.y), ImVec2(g_Screen.x + Width, vTop.y + bScale), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - Width, g_Screen.y), ImVec2(g_Screen.x - Width, g_Screen.y - bScale), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, g_Screen.y), ImVec2(g_Screen.x + Width, g_Screen.y - bScale), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - Width, g_Screen.y), ImVec2(g_Screen.x - Width + bScale, g_Screen.y), pColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, g_Screen.y), ImVec2(g_Screen.x + Width - bScale, g_Screen.y), pColor, 1.f);
                break;
            default:
                break;
            }
        }

        // Healthbar
        if (g.ESP_HealthBar)
            HealthBar(g_Screen.x - Width - 4, g_Screen.y, 2.f, -Height, pEntity->Health, pEntity->MaxHealth);

        // Distance
        if (g.ESP_Distance)
        {
            std::string DistStr = std::to_string((int)distance) + "m";
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(DistStr.c_str()).x / 2.f), g_Screen.y + 1.f), ImColor(1.f, 1.f, 1.f, 1.f), DistStr.c_str());
        }

        // Name
        if (g.ESP_Name)
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(pEntity->Name.c_str()).x / 2.f), vTop.y - 14.f), ImColor(1.f, 1.f, 1.f, 1.f), pEntity->Name.c_str());
    
        // Skeleton
        if (g.ESP_Skeleton)
        {
            int aSkeleton[][2] =
            { { NECK, SPINE },{ NECK, LEFTSHOULDER },{ LEFTSHOULDER, LEFTELBOWROLL }, { LEFTELBOWROLL, LEFTHAND },
             { NECK, RIGHTSHOULDER },{ RIGHTSHOULDER, RIGHTELBOWROLL }, { RIGHTELBOWROLL, RIGHTHAND },
             { SPINE, LEFTKNEEROLL }, { LEFTKNEEROLL, LEFTFOOT },  { SPINE, RIGHTKNEEROLL }, { RIGHTKNEEROLL, RIGHTFOOT },
            };

            Vector2 head{}, neck{};
            Vector3 head_pos = pEntity->GetBonePositionByID(HEAD);
            Vector3 neck_pos = pEntity->GetBonePositionByID(NECK);

            if (head_pos != Vector3() && neck_pos != Vector3())
            {
                if (!WorldToScreen(head_pos, head) || !WorldToScreen(neck_pos, neck))
                    continue;

                float HeadToNeck = neck.y - head.y;
                ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head.x, head.y - (HeadToNeck / 2.f)), HeadToNeck * 1.5f, pColor, 0.f);
            }

            for (int j = 0; j < 11; j++)
            {
                Vector3 Bone1 = pEntity->GetBonePositionByID(aSkeleton[j][0]);
                Vector3 Bone2 = pEntity->GetBonePositionByID(aSkeleton[j][1]);

                if (Bone1 == Vector3(0.f, 0.f, 0.f) || Bone2 == Vector3(0.f, 0.f, 0.f))
                    break;

                Vector2 Out1{}, Out2{};
                if (WorldToScreen(Bone1, Out1) && WorldToScreen(Bone2, Out2))
                    DrawLine(ImVec2(Out1.x, Out1.y), ImVec2(Out2.x, Out2.y), pColor, 1.f);
            }
        }

        // AimBot
        if (g.AimBot)
        {
            if (!IsKeyDown(VK_RBUTTON))
                continue;
            else if (pLocal->InVehicle())
                continue;
            else if (!pEntity->IsVisible())
                continue;
            else if (distance > 100.f)
                continue;

            bool Ready = false;

            // in FOV?
            std::vector<int>list = { HEAD, NECK, SPINE, SPINE1, SPINE2, LEFTELBOWROLL, LEFTHAND, RIGHTELBOWROLL, RIGHTHAND, LEFTKNEEROLL, LEFTFOOT, RIGHTKNEEROLL, RIGHTFOOT };
            for (auto bone : list)
            {
                Vector2 screen{};
                Vector3 game_pos = pEntity->GetBonePositionByID(bone);
                if (game_pos == Vector3(0.f, 0.f, 0.f))
                    continue;
                else if (!WorldToScreen(game_pos, screen))
                    continue;

                FOV = abs((Center - screen).Length());

                if (FOV < g.AimFov)
                {
                    Ready = true;
                    break;
                }
            }

            // Aim
            if (Ready)
            {
                Vector2 targetScreen{};
                if (!WorldToScreen(pEntity->GetBonePositionByID(SPINE2), targetScreen))
                    continue;

                if (MinDistance > distance)
                {
                    MinDistance = distance;
                    AimTargetPos = targetScreen;
                }
            }
        }
    }

    if (g.AimBot && AimTargetPos != Vector2(0.f, 0.f))
    {
        if (IsKeyDown(VK_RBUTTON) || IsKeyDown(VK_RBUTTON))
            AimBot(AimTargetPos);
    }
}