#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "App/BackgroundImage.hpp"

#include <string>
#include <unordered_map>

class Enemy : public BackgroundImage {
public:
    enum class Type {
        GREEN_SLIME,
        RED_SLIME,
        BAT,
        SKELETON_C,
        BIG_SLIME,
        SKELETON_B,
        PRIEST_C,
        BIG_BAT,
        ZOMBIE,
        SKELETON_A,
        ROCK,
        MAGICIAN_B,
        GATE_KEEPER_C,
        RED_BAT,
        MAGICIAN_A,
        SLIME_LORD,
        PRIEST_A,
        MAGIC_SERGEANT_D,
        ZOMBIE_KNIGHT,
        GATE_KEEPER_B,
        GATE_KEEPER_A,
        SWORDS_MAN,
        KNIGHT,
        IRON_KNIGHT_A,
        SOUL_SKELETON_KNIGHT,
        DARK_KNIGHT,
        DARK_MAGICIAN,
        SOUL_SKELETON,
        MAGIC_SERGEANT_A,
        SLIME_MAN,
        VAMPIRE,
        OCTOPUS,
        DRAGON
    };

    struct Stats {
        std::string name;
        int hp;
        int atk;
        int def;
        int coin;
        int exp;
        std::string imagePath1;
        std::string imagePath2;
        int openingDamage = 0;
        int openingDamageDivisor = 0;

    };

    Enemy(Type type);

    void UpdateImage(bool showAltFrame);
    void ApplyLevelStats(int level);
    int GetOpeningDamage(int playerHp) const;
    static int CalculateOpeningDamage(const Stats& stats, int playerHp);

    // 取得當前怪物的數值
    const Stats& GetStats() const { return m_Stats; }
    Type GetType() const { return m_Type; }

private:
    Type m_Type;
    Stats m_Stats;

    // 靜態數據表：存放所有種類敵人的初始數值
    static const std::unordered_map<Type, Stats> s_EnemyDataTable;

    bool m_CurrentFrameIsAlt = false;
};

#endif
