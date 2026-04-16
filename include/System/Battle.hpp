#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "Character/Player.hpp"
#include "Enemy/Enemy.hpp"

class Battle {
public:
    // 回傳值代表玩家是否能在這場戰鬥中獲勝（不死亡且打得動）
    static bool CanWin(const Player& player, const Enemy& enemy);

    // 執行戰鬥：扣除玩家血量、增加金幣與經驗值
    static void ExecuteBattle(Player& player, const Enemy& enemy);
};

#endif