#ifndef MONSTER_BOOK_HPP
#define MONSTER_BOOK_HPP

#include "Enemy/Enemy.hpp"
#include "System/TextObject.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#include <memory>
#include <string>
#include <vector>

class Map;
class Player;

class MonsterBook : public Util::GameObject {
public:
    MonsterBook();

    void Open(const Map& map, const Player& player);
    void Update(float deltaTime);
    void Draw();

    bool IsActive() const { return m_Active; }

private:
    struct MonsterEntry {
        Enemy::Stats stats;
        int attackCount = 0;
        int estimatedDamage = 0;
        bool canWin = false;
        std::shared_ptr<Util::Image> image1;
        std::shared_ptr<Util::Image> image2;
    };

    void RebuildEntries(const Map& map, const Player& player);
    void RefreshTexts();
    int CalculateAttackCount(const Player& player, const Enemy::Stats& stats) const;
    int CalculateEstimatedDamage(const Player& player, const Enemy::Stats& stats, int attackCount) const;

    bool m_Active = false;
    float m_InputCooldown = 0.0f;
    const float m_InputDelay = 0.18f;
    float m_AnimationTimer = 0.0f;
    const float m_AnimationInterval = 0.35f;
    bool m_ShowAltFrame = false;
    int m_Page = 0;
    const int m_PageSize = 3;

    std::shared_ptr<Util::Image> m_Background;
    std::shared_ptr<Util::Image> m_EnemyFrame;
    std::shared_ptr<Util::Image> m_LeftArrowWhite;
    std::shared_ptr<Util::Image> m_LeftArrowGray;
    std::shared_ptr<Util::Image> m_RightArrowWhite;
    std::shared_ptr<Util::Image> m_RightArrowGray;
    std::shared_ptr<TextObject> m_EmptyText;
    std::shared_ptr<TextObject> m_HintText;
    std::vector<MonsterEntry> m_Entries;
    std::vector<std::shared_ptr<TextObject>> m_Texts;
};

#endif
