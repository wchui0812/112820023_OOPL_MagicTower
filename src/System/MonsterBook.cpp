#include "System/MonsterBook.hpp"
#include "Character/Player.hpp"
#include "Map/Map.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

#include <algorithm>
#include <unordered_set>

namespace {
constexpr float kPanelScale = 0.73f;
constexpr float kPanelZ = 80.0f;
constexpr float kRowStartY = 250.0f;
constexpr float kRowSpacing = 180.0f;
constexpr float kEnemyFrameScale = 0.75f;
constexpr float kEnemyImageScale = 1.55f;
constexpr float kArrowScale = 1.2f;

std::shared_ptr<TextObject> MakeText(
    int size,
    const std::string& text,
    float zIndex,
    const glm::vec2& pos,
    const Util::Color& color = Util::Color::FromName(Util::Colors::WHITE)
) {
    auto textObject = std::make_shared<TextObject>(size, text, zIndex, color);
    textObject->m_Transform.translation = pos + glm::vec2(textObject->GetSize().x / 2.0f, 0.0f);
    return textObject;
}
}

MonsterBook::MonsterBook() {
    m_Background = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/Black.bmp");
    m_EnemyFrame = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/EnemyFrame.bmp");
    m_LeftArrowWhite = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/left_arrow_white.png");
    m_LeftArrowGray = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/left_arrow_gray.png");
    m_RightArrowWhite = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/right_arrow_white.png");
    m_RightArrowGray = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/right_arrow_gray.png");
    m_Drawable = m_Background;

    m_ZIndex = kPanelZ;
    m_Transform.translation = {142.0f, 0.0f};

    m_EmptyText = std::make_shared<TextObject>(28, "目前樓層沒有怪物", m_ZIndex + 2.0f);
    m_EmptyText->m_Transform.translation = m_Transform.translation;

    m_HintText = std::make_shared<TextObject>(22, "L 返回", m_ZIndex + 2.0f);
    m_HintText->m_Transform.translation = m_Transform.translation + glm::vec2(250.0f, -285.0f);

    SetVisible(false);
}

void MonsterBook::Open(const Map& map, const Player& player) {
    if (!player.m_HasMonsterBook) return;

    m_Page = 0;
    m_InputCooldown = m_InputDelay;
    m_AnimationTimer = 0.0f;
    m_ShowAltFrame = false;
    m_Active = true;
    SetVisible(true);
    RebuildEntries(map, player);
    RefreshTexts();
}

void MonsterBook::Update(float deltaTime) {
    if (!m_Active) return;

    m_AnimationTimer += deltaTime;
    if (m_AnimationTimer >= m_AnimationInterval) {
        m_AnimationTimer = 0.0f;
        m_ShowAltFrame = !m_ShowAltFrame;
    }

    if (m_InputCooldown > 0.0f) {
        m_InputCooldown -= deltaTime;
        return;
    }

    const int pageCount = std::max(1, static_cast<int>((m_Entries.size() + m_PageSize - 1) / m_PageSize));
    if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
        m_InputCooldown = m_InputDelay;
        m_Page = std::min(m_Page + 1, pageCount - 1);
        RefreshTexts();
    } else if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
        m_InputCooldown = m_InputDelay;
        m_Page = std::max(m_Page - 1, 0);
        RefreshTexts();
    } else if (Util::Input::IsKeyDown(Util::Keycode::L)) {
        m_Active = false;
        SetVisible(false);
    }
}

void MonsterBook::Draw() {
    if (!m_Active) return;

    Util::Transform backgroundTransform = m_Transform;
    backgroundTransform.scale = {kPanelScale, kPanelScale};
    m_Background->Draw(Util::ConvertToUniformBufferData(
        backgroundTransform,
        m_Background->GetSize(),
        m_ZIndex
    ));

    if (m_Entries.empty()) {
        m_EmptyText->Draw();
    }

    const int startIndex = m_Page * m_PageSize;
    const int endIndex = std::min(startIndex + m_PageSize, static_cast<int>(m_Entries.size()));
    for (int i = startIndex; i < endIndex; i++) {
        const int row = i - startIndex;
        const float rowY = kRowStartY - static_cast<float>(row) * kRowSpacing;
        const glm::vec2 imageCenter = m_Transform.translation + glm::vec2(-270.0f, rowY - 30.0f);

        Util::Transform frameTransform;
        frameTransform.translation = imageCenter;
        frameTransform.scale = {kEnemyFrameScale, kEnemyFrameScale};
        m_EnemyFrame->Draw(Util::ConvertToUniformBufferData(
            frameTransform,
            m_EnemyFrame->GetSize(),
            m_ZIndex + 1.0f
        ));

        Util::Transform imageTransform;
        imageTransform.translation = imageCenter;
        imageTransform.scale = {kEnemyImageScale, kEnemyImageScale};
        auto image = m_ShowAltFrame ? m_Entries[i].image2 : m_Entries[i].image1;
        image->Draw(Util::ConvertToUniformBufferData(
            imageTransform,
            image->GetSize(),
            m_ZIndex + 2.0f
        ));
    }

    for (auto& text : m_Texts) {
        text->Draw();
    }

    auto leftArrow = m_LeftArrowWhite;
    auto rightArrow = m_RightArrowWhite;

    Util::Transform leftTransform;
    leftTransform.translation = m_Transform.translation + glm::vec2(210.0f, -245.0f);
    leftTransform.scale = {kArrowScale, kArrowScale};
    leftArrow->Draw(Util::ConvertToUniformBufferData(
        leftTransform,
        leftArrow->GetSize(),
        m_ZIndex + 2.0f
    ));

    Util::Transform rightTransform;
    rightTransform.translation = m_Transform.translation + glm::vec2(265.0f, -245.0f);
    rightTransform.scale = {kArrowScale, kArrowScale};
    rightArrow->Draw(Util::ConvertToUniformBufferData(
        rightTransform,
        rightArrow->GetSize(),
        m_ZIndex + 2.0f
    ));

    m_HintText->Draw();
}

void MonsterBook::RebuildEntries(const Map& map, const Player& player) {
    m_Entries.clear();
    std::unordered_set<std::string> seenNames;

    for (const auto& enemy : map.GetEnemies()) {
        if (!enemy) continue;

        const auto& stats = enemy->GetStats();
        if (seenNames.count(stats.name) > 0) continue;
        seenNames.insert(stats.name);

        MonsterEntry entry;
        entry.stats = stats;
        entry.attackCount = CalculateAttackCount(player, stats);
        entry.estimatedDamage = CalculateEstimatedDamage(player, stats, entry.attackCount);
        entry.canWin = entry.attackCount > 0 && player.m_Hp > entry.estimatedDamage;
        entry.image1 = std::make_shared<Util::Image>(stats.imagePath1);
        entry.image2 = std::make_shared<Util::Image>(stats.imagePath2.empty() ? stats.imagePath1 : stats.imagePath2);
        m_Entries.push_back(entry);
    }
}

void MonsterBook::RefreshTexts() {
    m_Texts.clear();

    const int startIndex = m_Page * m_PageSize;
    const int endIndex = std::min(startIndex + m_PageSize, static_cast<int>(m_Entries.size()));
    for (int i = startIndex; i < endIndex; i++) {
        const int row = i - startIndex;
        const auto& entry = m_Entries[i];
        const auto& stats = entry.stats;
        const float rowY = kRowStartY - static_cast<float>(row) * kRowSpacing;
        const glm::vec2 base = m_Transform.translation + glm::vec2(-290.0f, rowY);

        const std::string damageText = entry.canWin
            ? std::to_string(entry.estimatedDamage)
            : "無法戰鬥";
        const std::string attackCountText = entry.attackCount > 0
            ? std::to_string(entry.attackCount)
            : "--";
        const auto damageColor = entry.canWin
            ? Util::Color::FromName(Util::Colors::WHITE)
            : Util::Color::FromName(Util::Colors::RED);
        const auto nameColor = Util::Color::FromName(Util::Colors::GREEN);

        m_Texts.push_back(MakeText(24, "名稱: " + stats.name, m_ZIndex + 2.0f, base + glm::vec2(90.0f, 35.0f), nameColor));
        m_Texts.push_back(MakeText(22, "體力: " + std::to_string(stats.hp), m_ZIndex + 2.0f, base + glm::vec2(90.0f, 0.0f)));
        m_Texts.push_back(MakeText(22, "攻擊力: " + std::to_string(stats.atk), m_ZIndex + 2.0f, base + glm::vec2(280.0f, 0.0f)));
        m_Texts.push_back(MakeText(22, "防禦力: " + std::to_string(stats.def), m_ZIndex + 2.0f, base + glm::vec2(450.0f, 0.0f)));
        m_Texts.push_back(MakeText(22, "攻擊次數: " + attackCountText, m_ZIndex + 2.0f, base + glm::vec2(90.0f, -40.0f)));
        m_Texts.push_back(MakeText(22, "估計傷害: " + damageText, m_ZIndex + 2.0f, base + glm::vec2(280.0f, -40.0f), damageColor));
        m_Texts.push_back(MakeText(22, "經驗值: " + std::to_string(stats.exp), m_ZIndex + 2.0f, base + glm::vec2(90.0f, -80.0f)));
        m_Texts.push_back(MakeText(22, "金幣: " + std::to_string(stats.coin), m_ZIndex + 2.0f, base + glm::vec2(280.0f, -80.0f)));
    }
}

int MonsterBook::CalculateAttackCount(const Player& player, const Enemy::Stats& stats) const {
    const int damageToEnemy = player.m_Atk - stats.def;
    if (damageToEnemy <= 0) return 0;
    return (stats.hp + damageToEnemy - 1) / damageToEnemy;
}

int MonsterBook::CalculateEstimatedDamage(const Player& player, const Enemy::Stats& stats, int attackCount) const {
    if (attackCount <= 0) return 0;
    const int damageToPlayer = std::max(0, stats.atk - player.m_Def);
    return Enemy::CalculateOpeningDamage(stats, player.m_Hp) + (attackCount - 1) * damageToPlayer;
}
