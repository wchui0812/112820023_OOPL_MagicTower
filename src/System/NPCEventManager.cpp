#include "System/NPCEventManager.hpp"
#include "Util/Logger.hpp"

void NPCEventManager::DispatchEvent(NPC::Type type, Player& player, Map& map, std::shared_ptr<NPC> npc) {
    switch (type) {
        case NPC::Type::FAIRY:
            HandleFairyEvent(player, map, npc);
            break;
        case NPC::Type::ELDER:
            HandleElderEvent(player, map);
            break;
            // 未來可以輕鬆擴充商店、小偷等事件
        default:
            LOG_INFO("This NPC has no event.");
            break;
    }
}

void NPCEventManager::HandleFairyEvent(Player& player, Map& map, std::shared_ptr<NPC> npc) {
    LOG_INFO("Fairy Event Triggered!");

    // 1. 給鑰匙
    player.m_YellowKeys += 1;
    player.m_BlueKeys += 1;
    player.m_RedKeys += 1;

    // 2. 移動仙子 (向左移一格)
    map.MoveNPC(npc, -1, 0);

    // 3. 可以在這裡加入對話框觸發邏輯
}

void NPCEventManager::HandleElderEvent(Player& player, Map& map) {
    LOG_INFO("Elder gives advice: 'The dragon is strong, watch your HP!'");
}