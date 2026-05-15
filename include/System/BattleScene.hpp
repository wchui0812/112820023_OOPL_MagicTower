#ifndef BATTLE_SCENE_HPP
#define BATTLE_SCENE_HPP

#include "Util/GameObject.hpp"
#include "System/TextObject.hpp"
#include "System/RewardMessage.hpp"
#include "Enemy/Enemy.hpp"
#include <memory>

class Player;
class Map;

class BattleScene : public Util::GameObject {
public:
    enum class State { IDLE, FIGHTING, FINISHED };

    BattleScene();
    
    // 初始化戰鬥數據並開啟視窗
    void StartBattle(Player& player, Map& map, std::shared_ptr<Enemy> enemy);
    
    void Update(float deltaTime);
    void Draw(); // 繪製背景與戰鬥數據文字
    
    bool IsActive() const { return m_Active; }
    State GetState() const { return m_State; }

private:
    bool m_Active = false;
    State m_State = State::IDLE;
    float m_Timer = 0.0f;
    const float m_AttackInterval = 0.5f; // 每次扣血的間隔時間

    // 引用外部資料
    Player* m_Player = nullptr;
    Map* m_Map = nullptr;
    std::shared_ptr<Enemy> m_Enemy = nullptr;
    Enemy::Stats m_EnemyStats;

    // 暫存目前的戰鬥數值
    int m_CurrentPlayerHp = 0;
    int m_CurrentEnemyHp = 0;

    std::shared_ptr<Util::Image> m_BackgroundImage;

    std::shared_ptr<TextObject> m_VsText;

    std::shared_ptr<Util::Image> m_PlayerAvatar;
    std::shared_ptr<Util::Image> m_EnemyAvatar;

    // UI 元件
    std::shared_ptr<TextObject> m_EnemyNameText;
    std::shared_ptr<TextObject> m_EnemyHpText;
    std::shared_ptr<TextObject> m_EnemyAtkText;
    std::shared_ptr<TextObject> m_EnemyDefText;

    std::shared_ptr<TextObject> m_PlayerNameText;
    std::shared_ptr<TextObject> m_PlayerHpText;
    std::shared_ptr<TextObject> m_PlayerAtkText;
    std::shared_ptr<TextObject> m_PlayerDefText;

    std::shared_ptr<TextObject> m_EnemyHpLabel;
    std::shared_ptr<TextObject> m_EnemyAtkLabel;
    std::shared_ptr<TextObject> m_EnemyDefLabel;

    std::shared_ptr<TextObject> m_PlayerHpLabel;
    std::shared_ptr<TextObject> m_PlayerAtkLabel;
    std::shared_ptr<TextObject> m_PlayerDefLabel;

    std::shared_ptr<RewardMessage> m_RewardMessage;
};

#endif