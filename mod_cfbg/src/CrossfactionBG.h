#ifndef CFBG_H
#define CFBG_H

#include "Player.h"

class CFBG
{
public:
    void CFJoinBattleGround(Player* player);
    void CFLeaveBattleGround(Player* player);
    void SetFakeValues(Player* player);
    void FakeDisplayID(Player* player);
    void ReplaceRacials(Player* player, bool native);
};

#define sCrossFaction Oregon::Singleton<CFBG>::Instance()
#endif