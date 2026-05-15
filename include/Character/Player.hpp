#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <string>
#include <vector>

#include "Map/Map.hpp"
#include "System/BattleAnimation.hpp"
#include "System/BattleScene.hpp"
#include "System/NPCDialog.hpp"
#include "System/RewardMessage.hpp"
#include "System/ShopScene.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Player : public Util::GameObject {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    explicit Player(); // 建構子改到 .cpp 實作

    void Update(
        Map& map,
        BattleAnimation& anim,
        BattleScene& battleScene,
        RewardMessage& rewardMessage,
        NPCDialog& npcDialog,
        ShopScene& shopScene
    );

    void SetDirection(Direction dir);
    void Move(const glm::vec2& delta);
    
    // 工具函式
    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    [[nodiscard]] const glm::vec2& GetPosition() const {
        return m_Transform.translation;
    }

    /*int m_Level = 1;
    int m_Hp = 1000;
    int m_Atk = 10;
    int m_Def = 10;
    int m_Coin = 0;
    int m_Exp = 0;
    int m_YellowKeys = 0;
    int m_BlueKeys = 0;
    int m_RedKeys = 0;*/

    int m_Level = 30;
    int m_Hp = 9999;
    int m_Atk = 999;
    int m_Def = 999;
    int m_Coin = 600;
    int m_Exp = 0;
    int m_YellowKeys = 30;
    int m_BlueKeys = 30;
    int m_RedKeys = 30;


private:
    void NextFrame();

    Direction m_CurrentDir = Direction::DOWN;
    int m_FrameIndex = 0;
    float m_MoveDist = 56.0f;

    int GetYellowKeyCount() const { return m_YellowKeys; }

    // 儲存所有預載圖片的容器
    std::map<Direction, std::vector<std::shared_ptr<Util::Image>>> m_AnimationFrames;
};

#endif
