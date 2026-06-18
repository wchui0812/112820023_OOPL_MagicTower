#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include "Map/Stair.hpp"
#include "System/CollisionHandler.hpp"
#include "System/BattleScene.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"


Player::Player() {
    std::map<Direction, std::string> dirCode = {
        {Direction::UP, "u"}, {Direction::DOWN, "d"},
        {Direction::LEFT, "tl"}, {Direction::RIGHT, "tr"}
    };

    for (auto const& [dir, code] : dirCode) {
        for (int i = 1; i <= 3; ++i) {
            std::string path = RESOURCE_DIR "/Image/Character/Player/player_" + code + std::to_string(i) + ".png";
            m_AnimationFrames[dir].push_back(std::make_shared<Util::Image>(path));
        }
    }

    m_Drawable = m_AnimationFrames[m_CurrentDir][0];
    m_ZIndex = 5.0f;
}

void Player::Update(
    Map& map,
    BattleScene& battleScene,
    RewardMessage& rewardMessage,
    NPCDialog& npcDialog,
    ShopScene& shopScene
) {
    float moveDist = 56.0f;
    glm::vec2 currentPos = m_Transform.translation;
    glm::vec2 targetPos = currentPos;
    glm::vec2 moveVec = {0, 0};

    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        SetDirection(Direction::UP);
        targetPos.y += moveDist;
        moveVec = {0, 1};
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        SetDirection(Direction::DOWN);
        targetPos.y -= moveDist;
        moveVec = {0, -1};
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
        SetDirection(Direction::LEFT);
        targetPos.x -= moveDist;
        moveVec = {-1, 0};
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
        SetDirection(Direction::RIGHT);
        targetPos.x += moveDist;
        moveVec = {1, 0};
    }

    if (targetPos == currentPos) return;

    int tileType = map.GetTileType(targetPos.x, targetPos.y);


    if (CollisionHandler::HandleCollision(*this, map, targetPos, battleScene, rewardMessage, npcDialog, shopScene)) {

        m_Transform.translation = targetPos;

        Stair::CheckAndTransport(m_Transform.translation, map, moveVec);
    }
}

void Player::SetDirection(Direction dir) {
    if (m_CurrentDir != dir) {
        m_CurrentDir = dir;
        m_FrameIndex = 0;
    } else {
        NextFrame();
    }
    m_Drawable = m_AnimationFrames[m_CurrentDir][m_FrameIndex];
}

void Player::NextFrame() {
    m_FrameIndex = (m_FrameIndex + 1) % 3;
    m_Drawable = m_AnimationFrames[m_CurrentDir][m_FrameIndex];
}

void Player::Move(const glm::vec2& delta) {
    m_Transform.translation += delta;
}
