#include "Map/Map.hpp"
#include "System/BattleScene.hpp"
#include "System/RewardMessage.hpp"
#include "Character/Player.hpp"
#include "Util/Time.hpp"
#include "Util/TransformUtils.hpp"
#include <algorithm>

BattleScene::BattleScene() {
    m_BackgroundImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Enemy/Fighting.bmp");
    // 2. 將圖片設定給 GameObject 的 m_Drawable，這樣才能被渲染
    m_Drawable = m_BackgroundImage;

    m_VsText = std::make_shared<TextObject>(55, "VS", 51.0f);
    m_VsText->m_Transform.translation = {0.0f, 20.0f};

    // 3. 設定視窗位置與層級
    m_Transform.translation = {0.8f, 0.6f}; // 放在畫面正中央
    m_ZIndex = 50.0f;

    int fontSize = 32;
    float z = 51.0f;

    m_EnemyHpLabel = std::make_shared<TextObject>(fontSize, "體力：", z);
    m_EnemyAtkLabel = std::make_shared<TextObject>(fontSize, "攻擊力：", z);
    m_EnemyDefLabel = std::make_shared<TextObject>(fontSize, "防禦力：", z);

    m_PlayerHpLabel = std::make_shared<TextObject>(fontSize, "：體力", z);
    m_PlayerAtkLabel = std::make_shared<TextObject>(fontSize, "：攻擊力", z);
    m_PlayerDefLabel = std::make_shared<TextObject>(fontSize, "：防禦力", z);

    m_EnemyNameText = std::make_shared<TextObject>(fontSize, "Enemy", z);
    m_EnemyHpText = std::make_shared<TextObject>(fontSize, "0", z);
    m_EnemyAtkText = std::make_shared<TextObject>(fontSize, "0", z);
    m_EnemyDefText = std::make_shared<TextObject>(fontSize, "0", z);

    m_PlayerNameText = std::make_shared<TextObject>(fontSize, "勇者", z);
    m_PlayerHpText = std::make_shared<TextObject>(fontSize, "0", z);
    m_PlayerAtkText = std::make_shared<TextObject>(fontSize, "0", z);
    m_PlayerDefText = std::make_shared<TextObject>(fontSize, "0", z);

    float farX = 210.0f;    // 標籤位置
    float nearX = 120.0f;   // 數值位置
    float startY = 70.0f;
    float spacing = 55.0f;

    // --- 左側 (怪物) ---
    m_EnemyHpLabel->m_Transform.translation = {-farX, startY};
    m_EnemyHpText->m_Transform.translation  = {-nearX, startY};

    m_EnemyAtkLabel->m_Transform.translation = {-farX, startY - spacing};
    m_EnemyAtkText->m_Transform.translation  = {-nearX, startY - spacing};

    m_EnemyDefLabel->m_Transform.translation = {-farX, startY - spacing * 2};
    m_EnemyDefText->m_Transform.translation  = {-nearX, startY - spacing * 2};

    // --- 右側 (勇者) ---[cite: 1]
    m_PlayerHpText->m_Transform.translation  = {nearX, startY};
    m_PlayerHpLabel->m_Transform.translation = {farX, startY};

    m_PlayerAtkText->m_Transform.translation  = {nearX, startY - spacing};
    m_PlayerAtkLabel->m_Transform.translation = {farX, startY - spacing};

    m_PlayerDefText->m_Transform.translation  = {nearX, startY - spacing * 2};
    m_PlayerDefLabel->m_Transform.translation = {farX, startY - spacing * 2};

    // 頭像框位置修正 (對齊你圖片中的藍框)[cite: 1]
    m_EnemyNameText->m_Transform.translation = {-340.0f, 120.0f};
    m_PlayerNameText->m_Transform.translation = {355.0f, 120.0f};

    // 3. 設定初始可見度
    m_ZIndex = 50.0f;
    m_Active = false;
    this->SetVisible(false);

    m_RewardMessage = std::make_shared<RewardMessage>();
}

void BattleScene::StartBattle(Player& player, Map& map, std::shared_ptr<Enemy> enemy) {
    m_Player = &player;
    m_Map = &map;
    m_Enemy = enemy;
    m_EnemyStats = enemy->GetStats();

    m_EnemyAvatar = std::make_shared<Util::Image>(m_EnemyStats.imagePath1);
    m_PlayerAvatar = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Character/Player/player_d1.png");

    m_EnemyNameText->SetText(m_EnemyStats.name);
    m_EnemyAtkText->SetText(std::to_string(m_EnemyStats.atk));
    m_EnemyDefText->SetText(std::to_string(m_EnemyStats.def));

    m_PlayerAtkText->SetText(std::to_string(m_Player->m_Atk));
    m_PlayerDefText->SetText(std::to_string(m_Player->m_Def));

    m_CurrentPlayerHp = player.m_Hp;
    m_CurrentEnemyHp = m_EnemyStats.hp;
    
    m_Active = true;
    m_State = State::FIGHTING;
    m_Timer = 0.0f;

    this->SetVisible(true);
}

void BattleScene::Update(float deltaTime) {
    if (!m_Active) return;

    if (m_State == State::FIGHTING) {
        m_Timer += deltaTime;
        if (m_Timer >= m_AttackInterval) {
            m_Timer = 0.0f;

            // 1. 勇者攻擊怪物
            int damageToEnemy = std::max(0, m_Player->m_Atk - m_EnemyStats.def);
            m_CurrentEnemyHp -= damageToEnemy;

            // 2. 檢查怪物是否死亡
            if (m_CurrentEnemyHp <= 0) {
                m_State = State::FINISHED;
                this->SetVisible(false);

                // 更新 Player 數據
                m_Player->m_Hp = m_CurrentPlayerHp;
                m_Player->m_Coin += m_EnemyStats.coin;
                m_Player->m_Exp += m_EnemyStats.exp;

                if (m_Map && m_Enemy) {
                    m_Map->RemoveEnemy(m_Enemy);
                    m_Enemy.reset();
                }

                std::string rewardMsg = "擊殺成功! 獲得 金幣: " + std::to_string(m_EnemyStats.coin) +
                                           "  經驗值: " + std::to_string(m_EnemyStats.exp);
                m_RewardMessage->Show(rewardMsg);
                return;
            }

            // 3. 怪物反擊勇者
            int damageToPlayer = std::max(0, m_EnemyStats.atk - m_Player->m_Def);
            m_CurrentPlayerHp -= damageToPlayer;

            m_PlayerHpText->SetText(std::to_string(m_CurrentPlayerHp));
            m_EnemyHpText->SetText(std::to_string(m_CurrentEnemyHp));
        }
    }
    else if (m_State == State::FINISHED) {
        // 在這裡更新橫幅，偵測按鍵
        m_RewardMessage->Update(deltaTime);

        // 當橫幅不可見時（玩家按下 Z 鍵後），才真正關閉整個 BattleScene
        if (!m_RewardMessage->IsVisible()) {
            m_Active = false;
            m_State = State::IDLE;
        }
    }
}

void BattleScene::Draw() {
    if (!m_Active) return;

    // 1. 繪製背景視窗
    // 利用 ConvertToUniformBufferData 轉換座標系統給渲染器[cite: 1]
    if (m_State == State::FIGHTING && m_Visible) {
        auto bgMatrices = Util::ConvertToUniformBufferData(
        m_Transform,
        m_BackgroundImage->GetSize(),
        m_ZIndex
        );
        m_BackgroundImage->Draw(bgMatrices);
        m_VsText->Draw();

        m_EnemyHpLabel->Draw();
        m_EnemyAtkLabel->Draw();
        m_EnemyDefLabel->Draw();
        m_PlayerHpLabel->Draw();
        m_PlayerAtkLabel->Draw();
        m_PlayerDefLabel->Draw();

        // 畫數值與名稱[cite: 1, 4]
        m_EnemyHpText->Draw();
        m_EnemyAtkText->Draw();
        m_EnemyDefText->Draw();
        m_PlayerHpText->Draw();
        m_PlayerAtkText->Draw();
        m_PlayerDefText->Draw();

        m_EnemyNameText->Draw();
        m_PlayerNameText->Draw();

        // 2. 繪製怪物頭像 (左側)
        if (m_EnemyAvatar) {
            Util::Transform enemyT;
            enemyT.translation = {-345.0f, 38.0f}; // 根據背景調整
            enemyT.scale = {2.0f, 2.0f};
            m_EnemyAvatar->Draw(Util::ConvertToUniformBufferData(enemyT, m_EnemyAvatar->GetSize(), m_ZIndex + 1));
        }

        // 3. 繪製勇者頭像 (右側)
        if (m_PlayerAvatar) {
            Util::Transform playerT;
            playerT.translation = {345.0f, 38.0f};
            playerT.scale = {2.0f, 2.0f};
            m_PlayerAvatar->Draw(Util::ConvertToUniformBufferData(playerT, m_PlayerAvatar->GetSize(), m_ZIndex + 1));
        }
    }

    // 4. 繪製文字 (利用 TextObject 內建的 Draw)
    //m_EnemyHpText->SetText(std::to_string(m_CurrentEnemyHp));
    //m_PlayerHpText->SetText(std::to_string(m_CurrentPlayerHp));

    if (m_State == State::FINISHED) {
        m_RewardMessage->Draw();
    }
}