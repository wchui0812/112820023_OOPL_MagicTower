#include "System/FloorTeleport.hpp"
#include "Character/Player.hpp"
#include "Map/Map.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

#include <algorithm>

namespace {
constexpr float kPanelScale = 0.74f;
constexpr int kMaxNormalLevel = 24;
constexpr int kLayer22 = 22;
constexpr int kLayer23 = 23;
constexpr int kLayer23_1 = 25;
constexpr int kLayer23_3 = 26;
constexpr int kLayer24 = 24;
constexpr float kTileSize = 56.0f;
}

FloorTeleport::FloorTeleport() {
    m_Background = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/Black.bmp");
    m_Frame = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/Fly.bmp");
    m_UpArrowWhite = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/up_arrow_white.png");
    m_UpArrowGray = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/up_arrow_gray.png");
    m_DownArrowWhite = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/down_arrow_white.png");
    m_DownArrowGray = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/down_arrow_gray.png");

    m_ZIndex = 80.0f;
    m_Transform.translation = {141.0f, 0.0f};
    m_Drawable = m_Background;

    m_FloorNumberText = std::make_shared<TextObject>(64, "0", m_ZIndex + 2.0f);
    m_SelectHintText = std::make_shared<TextObject>(22, "-Space-", m_ZIndex + 2.0f);
    m_QuitHintText = std::make_shared<TextObject>(22, "-Quit-(J)", m_ZIndex + 2.0f);

    SetVisible(false);
}

void FloorTeleport::Open(const Map& map, const Player& player) {
    if (!player.m_HasWindCompass) return;

    m_MinLevel = 1;
    m_MaxLevel = std::min({player.m_MaxReachedFloor, map.GetLevelCount() - 1, kMaxNormalLevel});

    int currentLevelForSelection = map.GetCurrentLevel();
    if (currentLevelForSelection == kLayer23_1 || currentLevelForSelection == kLayer23_3) {
        currentLevelForSelection = kLayer23;
    }

    m_SelectedLevel = std::clamp(currentLevelForSelection, m_MinLevel, m_MaxLevel);
    m_InputCooldown = m_InputDelay;
    m_Active = true;
    SetVisible(true);
    RefreshTexts();
}

void FloorTeleport::Update(float deltaTime, Map& map, Player& player) {
    if (!m_Active) return;

    if (m_InputCooldown > 0.0f) {
        m_InputCooldown -= deltaTime;
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        MoveSelection(1);
    } else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        MoveSelection(-1);
    } else if (Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
               Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        ConfirmSelection(map, player);
    } else if (Util::Input::IsKeyDown(Util::Keycode::J)) {
        m_Active = false;
        SetVisible(false);
    }
}

void FloorTeleport::Draw() {
    if (!m_Active) return;

    Util::Transform backgroundTransform = m_Transform;
    backgroundTransform.scale = {kPanelScale, kPanelScale};
    m_Background->Draw(Util::ConvertToUniformBufferData(
        backgroundTransform,
        m_Background->GetSize(),
        m_ZIndex
    ));

    Util::Transform frameTransform = m_Transform;
    frameTransform.scale = {kPanelScale, kPanelScale};
    m_Frame->Draw(Util::ConvertToUniformBufferData(
        frameTransform,
        m_Frame->GetSize(),
        m_ZIndex + 1.0f
    ));

    auto& upArrow = m_SelectedLevel < m_MaxLevel ? m_UpArrowWhite : m_UpArrowGray;
    Util::Transform upTransform;
    upTransform.translation = m_Transform.translation + glm::vec2(0.0f, 245.0f);
    upTransform.scale = {0.75f, 0.75f};
    upArrow->Draw(Util::ConvertToUniformBufferData(
        upTransform,
        upArrow->GetSize(),
        m_ZIndex + 2.0f
    ));

    auto& downArrow = m_SelectedLevel > m_MinLevel ? m_DownArrowWhite : m_DownArrowGray;
    Util::Transform downTransform;
    downTransform.translation = m_Transform.translation + glm::vec2(0.0f, -245.0f);
    downTransform.scale = {0.75f, 0.75f};
    downArrow->Draw(Util::ConvertToUniformBufferData(
        downTransform,
        downArrow->GetSize(),
        m_ZIndex + 2.0f
    ));

    m_FloorNumberText->Draw();
    m_SelectHintText->Draw();
    m_QuitHintText->Draw();
}

void FloorTeleport::MoveSelection(int delta) {
    m_InputCooldown = m_InputDelay;
    m_SelectedLevel = std::clamp(m_SelectedLevel + delta, m_MinLevel, m_MaxLevel);
    RefreshTexts();
}

void FloorTeleport::ConfirmSelection(Map& map, Player& player) {
    const int oldLevel = map.GetCurrentLevel();
    map.SetLevel(m_SelectedLevel);
    player.m_MaxReachedFloor = std::max(player.m_MaxReachedFloor, m_SelectedLevel);

    const glm::vec2 landingOffsets[] = {
        {0.0f, -kTileSize},
        {0.0f, kTileSize},
        {-kTileSize, 0.0f},
        {kTileSize, 0.0f},
    };

    auto findLandingNear = [&](const glm::vec2& targetPos) {
        for (const auto& offset : landingOffsets) {
            const glm::vec2 candidate = targetPos + offset;
            if (map.GetTileType(candidate.x, candidate.y) == 0) {
                return candidate;
            }
        }

        return targetPos;
    };

    auto setLandingNearTile = [&](int row, int col) {
        player.SetPosition(findLandingNear(map.GetTileCenter(row, col)));
        m_Active = false;
        SetVisible(false);
    };

    if (m_SelectedLevel == kLayer24) {
        const glm::vec2 layer24Landing = map.GetTileCenter(10, 5);
        if (map.GetTileType(layer24Landing.x, layer24Landing.y) == 0) {
            player.SetPosition(layer24Landing);
        } else {
            player.SetPosition(findLandingNear(layer24Landing));
        }
        m_Active = false;
        SetVisible(false);
        return;
    }

    if (m_SelectedLevel == kLayer22) {
        if (oldLevel == kLayer23_1) {
            setLandingNearTile(5, 0);
            return;
        }
        if (oldLevel == kLayer23) {
            setLandingNearTile(10, 5);
            return;
        }
        if (oldLevel == kLayer23_3) {
            setLandingNearTile(5, 10);
            return;
        }
    }

    const int preferredStair = m_SelectedLevel > oldLevel ? 4 : 5;
    glm::vec2 stairPos = map.FindTilePosition(preferredStair);
    if (stairPos == glm::vec2(0.0f, 0.0f)) {
        stairPos = map.FindTilePosition(preferredStair == 4 ? 5 : 4);
    }

    glm::vec2 landingPos = stairPos;
    for (const auto& offset : landingOffsets) {
        const glm::vec2 candidate = stairPos + offset;
        if (map.GetTileType(candidate.x, candidate.y) == 0) {
            landingPos = candidate;
            break;
        }
    }

    player.SetPosition(landingPos);

    m_Active = false;
    SetVisible(false);
}

void FloorTeleport::RefreshTexts() {
    m_FloorNumberText->SetText(std::to_string(m_SelectedLevel));
    m_FloorNumberText->m_Transform.translation = m_Transform.translation + glm::vec2(-55.0f, 0.0f);

    m_SelectHintText->m_Transform.translation = m_Transform.translation + glm::vec2(245.0f, -185.0f);
    m_QuitHintText->m_Transform.translation = m_Transform.translation + glm::vec2(260.0f, -285.0f);
}
