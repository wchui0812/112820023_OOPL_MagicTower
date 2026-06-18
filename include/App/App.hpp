#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Map/Map.hpp"
#include "Character/Player.hpp"
#include "NPC/NPCDialog.hpp"
#include "Shop/ShopScene.hpp"
#include "System/UIStatus.hpp"
#include "System/BattleScene.hpp"
#include "System/RewardMessage.hpp"
#include "System/FloorTeleport.hpp"
#include "System/GameFlowScene.hpp"
#include "System/MonsterBook.hpp"
#include "System/NoticeScene.hpp"


class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    void RestartGame();

    Map m_GameMap;
    std::shared_ptr<Player> m_Player;
    std::shared_ptr<BackgroundImage> m_MainBackground;
    UIStatus m_UI;

    State m_CurrentState = State::START;

    std::shared_ptr<BattleScene> m_BattleScene;
    std::shared_ptr<NPCDialog> m_NPCDialog;
    std::shared_ptr<RewardMessage> m_RewardMessage;
    std::shared_ptr<ShopScene> m_ShopScene;
    std::shared_ptr<FloorTeleport> m_FloorTeleport;
    std::shared_ptr<MonsterBook> m_MonsterBook;
    std::shared_ptr<NoticeScene> m_NoticeScene;
    std::shared_ptr<GameFlowScene> m_GameFlowScene;

};

#endif
