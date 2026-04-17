#ifndef NPC_EVENT_MANAGER_HPP
#define NPC_EVENT_MANAGER_HPP

#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include "NPC/NPC.hpp"

class NPCEventManager {
public:
    // 核心函式：根據 NPC 類型觸發事件
    static void DispatchEvent(NPC::Type type, Player& player, Map& map, std::shared_ptr<NPC> npc);

private:
    // 將不同 NPC 的邏輯拆分成私有函式，方便管理
    static void HandleFairyEvent(Player& player, Map& map, std::shared_ptr<NPC> npc);
    static void HandleElderEvent(Player& player, Map& map);
};

#endif