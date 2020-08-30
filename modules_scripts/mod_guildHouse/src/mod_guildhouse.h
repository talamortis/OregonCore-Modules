#ifndef GUILDHOUSE_H
#define GUILDHOUSE_H

#include "Player.h"
#include "Guild.h"
#include "Creature.h"

class GuildHouse
{
public:
    bool SelectGuildHouse(Guild* guild, Player* player, Creature* creature);
    bool SellGuildHouse(Player* player, Guild* guild);
    bool BuyGuildHouse(Player* player, Guild* guild, uint32 action);
    void TeleportToGuildHouse(Guild* guild, Player* player, Creature* creature);
    void SpawnNPC(uint32 entry, Player* player, uint32 cost);
    void SpawnObject(uint32 entry, Player* player, uint32 cost);
    void DeleteCreature(Player* player);
    bool ShowGameObjectPortals(Player* player, Item* item);
    bool ShowClassTrainers(Player* player, Item* item);
    bool ShowMainMenu(Player* player, Item* item);
    bool ShowGameObjectMenu(Player* player, Item* Item);
    bool ShowCreatureMainMenu(Player* player, Item* item);
    bool ShowProfessionTrainer(Player* player, Item* item);
    bool ShowMiscMenu(Player* player, Item* item);
    bool OnListNearObjects(Player* player, Item* item);

private:
    uint32 map;
    float posX;
    float posY;
    float posZ;
    uint32 zoneId;
    uint32 action;
};

#define sGuildHouse Oregon::Singleton<GuildHouse>::Instance()
#endif // !GUILDHOUSE_H
