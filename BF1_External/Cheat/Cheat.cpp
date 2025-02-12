#include "Cheat.h"

void Cheat::UpdateList()
{
    while (g.Run)
    {
        std::vector<CPlayer> TempList;

        for (int i = 0; i < 70; i++)
        {
            CPlayer player = CPlayer();
            player.GetPlayer(i);
            if (!player.Update())
                continue;

            if (strcmp(player.Name.c_str(), LocalName) == 0)
            {
                local = player;
                continue;
            }

            TempList.push_back(player);
        }

        PlayerList = TempList;
        TempList.clear();
        Sleep(1000);
    }
}