#include "Enemy/Enemy.hpp"

// 定義怪物的初始數據表
const std::unordered_map<Enemy::Type, Enemy::Stats> Enemy::s_EnemyDataTable = {
    {Enemy::Type::GREEN_SLIME,     {"綠色史萊姆", 50, 20, 1, 1, 1, RESOURCE_DIR "/Image/Character/Enemy/GreenSlime1.png", RESOURCE_DIR "/Image/Character/Enemy/GreenSlime2.png"}},
    {Enemy::Type::RED_SLIME,       {"紅色史萊姆", 70, 15, 2, 2, 2, RESOURCE_DIR "/Image/Character/Enemy/RedSlime1.png",RESOURCE_DIR "/Image/Character/Enemy/RedSlime2.png"}},
    {Enemy::Type::BIG_SLIME,       {"大史萊姆", 200, 35, 10, 5, 5, RESOURCE_DIR "/Image/Character/Enemy/BigSlime1.png",RESOURCE_DIR "/Image/Character/Enemy/BigSlime2.png"}},
    {Enemy::Type::BAT,             {"小蝙蝠", 100, 20, 5, 3, 3, RESOURCE_DIR "/Image/Character/Enemy/Bat1.png",RESOURCE_DIR "/Image/Character/Enemy/Bat2.png"}},
    {Enemy::Type::SKELETON_C,      {"骷髏人", 110, 25, 5, 5, 4, RESOURCE_DIR "/Image/Character/Enemy/SkeletonC1.png",RESOURCE_DIR "/Image/Character/Enemy/SkeletonC2.png"}},
    {Enemy::Type::SKELETON_B,      {"骷髏士兵", 150, 40, 20, 8, 6, RESOURCE_DIR "/Image/Character/Enemy/SkeletonB1.png",RESOURCE_DIR "/Image/Character/Enemy/SkeletonB2.png"}},
    {Enemy::Type::PRIEST_C,        {"初級法師", 125, 50, 25, 10, 7, RESOURCE_DIR "/Image/Character/Enemy/PriestC1.png",RESOURCE_DIR "/Image/Character/Enemy/PriestC2.png"}},
    {Enemy::Type::BIG_BAT,         {"大蝙蝠", 150, 65, 30, 10, 8, RESOURCE_DIR "/Image/Character/Enemy/BigBat1.png",RESOURCE_DIR "/Image/Character/Enemy/BigBat2.png"}},
    {Enemy::Type::ZOMBIE,          {"獸人", 300, 75, 45, 13, 10, RESOURCE_DIR "/Image/Character/Enemy/Zombie1.png",RESOURCE_DIR "/Image/Character/Enemy/Zombie2.png"}},
    {Enemy::Type::SKELETON_A,        {"骷髏隊長", 400, 90, 50, 15, 12, RESOURCE_DIR "/Image/Character/Enemy/SkeletonA1.png",RESOURCE_DIR "/Image/Character/Enemy/SkeletonA2.png"}},
    {Enemy::Type::ROCK,            {"石頭人", 500, 115, 65, 15, 15, RESOURCE_DIR "/Image/Character/Enemy/Rock1.png",RESOURCE_DIR "/Image/Character/Enemy/Rock2.png"}},
    {Enemy::Type::MAGICIAN_B,      {"初級巫師", 250, 120, 70, 20, 17, RESOURCE_DIR "/Image/Character/Enemy/MagicianB1.png",RESOURCE_DIR "/Image/Character/Enemy/MagicianB2.png"}},
    {Enemy::Type::GATE_KEEPER_C,   {"初級衛兵", 450, 150, 90, 22, 19, RESOURCE_DIR "/Image/Character/Enemy/GateKeeperC1.png",RESOURCE_DIR "/Image/Character/Enemy/GateKeeperC2.png"}},
    {Enemy::Type::RED_BAT,         {"紅蝙蝠", 550, 160, 90, 25, 20, RESOURCE_DIR "/Image/Character/Enemy/RedBat1.png",RESOURCE_DIR "/Image/Character/Enemy/RedBat2.png"}},
    {Enemy::Type::MAGICIAN_A,        {"高級巫師", 500, 400, 260, 47, 45, RESOURCE_DIR "/Image/Character/Enemy/MagicianA1.png",RESOURCE_DIR "/Image/Character/Enemy/MagicianA2.png"}},
    {Enemy::Type::SLIME_LORD,      {"史萊姆王", 700, 250, 125, 32, 30, RESOURCE_DIR "/Image/Character/Enemy/SlimeLord1.png",RESOURCE_DIR "/Image/Character/Enemy/SlimeLord2.png"}},
    {Enemy::Type::PRIEST_A,          {"高級法師", 100, 200, 110, 30, 25, RESOURCE_DIR "/Image/Character/Enemy/PriestA1.png",RESOURCE_DIR "/Image/Character/Enemy/PriestA2.png"}},
    {Enemy::Type::MAGIC_SERGEANT_D,{"白衣武士", 1300, 300, 150, 40, 35, RESOURCE_DIR "/Image/Character/Enemy/MagicSergeantD1.png",RESOURCE_DIR "/Image/Character/Enemy/MagicSergeantD2.png"}},
    {Enemy::Type::ZOMBIE_KNIGHT,   {"獸人武士", 900, 450, 330, 50, 50, RESOURCE_DIR "/Image/Character/Enemy/ZombieKnight1.png",RESOURCE_DIR "/Image/Character/Enemy/ZombieKnight2.png"}},
    {Enemy::Type::GATE_KEEPER_B,   {"冥衛兵", 1250, 500, 400, 55, 55, RESOURCE_DIR "/Image/Character/Enemy/GateKeeperB1.png",RESOURCE_DIR "/Image/Character/Enemy/GateKeeperB2.png"}},
    {Enemy::Type::GATE_KEEPER_A,     {"高級衛兵",  1500,560,460,60,60,RESOURCE_DIR "/Image/Character/Enemy/GateKeeperA1.png",RESOURCE_DIR "/Image/Character/Enemy/GateKeeperA2.png"}},
    {Enemy::Type::SWORDS_MAN,      {"雙手劍士",1200,620,520,65,75, RESOURCE_DIR "/Image/Character/Enemy/SwordsMan1.png",RESOURCE_DIR "/Image/Character/Enemy/SwordsMan2.png"}},
    {Enemy::Type::KNIGHT,          {"騎士",850,350,200,45,40, RESOURCE_DIR "/Image/Character/Enemy/Knight1.png",RESOURCE_DIR "/Image/Character/Enemy/Knight2.png"}},
    {Enemy::Type::IRON_KNIGHT_A,     {"騎士隊長",900,750,6650,77,70, RESOURCE_DIR "/Image/Character/Enemy/IronKnightA1.png",RESOURCE_DIR "/Image/Character/Enemy/IronKnightA2.png"}},
    {Enemy::Type::SOUL_SKELETON_KNIGHT,{"冥戰士",2000,680,590,70,65, RESOURCE_DIR "/Image/Character/Enemy/SoulSkeletonKnight1.png",RESOURCE_DIR "/Image/Character/Enemy/SoulSkeletonKnight2.png"}},
    {Enemy::Type::DARK_KNIGHT,     {"靈武士",1200,980,900,88,75, RESOURCE_DIR "/Image/Character/Enemy/DarkKnight1.png",RESOURCE_DIR "/Image/Character/Enemy/DarkKnight2.png"}},
    {Enemy::Type::DARK_MAGICIAN,   {"靈法師",1500,830,730,80,70, RESOURCE_DIR "/Image/Character/Enemy/DarkMagician1.png",RESOURCE_DIR "/Image/Character/Enemy/DarkMagician2.png"}},
    {Enemy::Type::SOUL_SKELETON,   {"冥隊長",3333,1200,1133,112,100, RESOURCE_DIR "/Image/Character/Enemy/SoulSkeleton1.png",RESOURCE_DIR "/Image/Character/Enemy/SoulSkeleton2.png"}},
    {Enemy::Type::MAGIC_SERGEANT_A,  {"紅衣魔王",15000,1000,1000,100,100, RESOURCE_DIR "/Image/Character/Enemy/MagicSergeantA1.png",RESOURCE_DIR "/Image/Character/Enemy/MagicSergeantA2.png"}},
    {Enemy::Type::SLIME_MAN,       {"影子戰士", 3100,1150,1050,92,80, RESOURCE_DIR "/Image/Character/Enemy/SlimeMan1.png",RESOURCE_DIR "/Image/Character/Enemy/SlimeMan2.png"}},
    {Enemy::Type::VAMPIRE,         {"冥靈魔王",30000,1700,1500,250,220, RESOURCE_DIR "/Image/Character/Enemy/Vampire1.png",RESOURCE_DIR "/Image/Character/Enemy/Vampire2.png"}},
    {Enemy::Type::OCTOPUS,         {"血影",99999,5000,4000,0,0, RESOURCE_DIR "/Image/Character/Enemy/Octopus8.png",RESOURCE_DIR "/Image/Character/Enemy/Octopus8.png"}},
    {Enemy::Type::DRAGON,         {"魔龍", 99999,9999,5000,0,0, RESOURCE_DIR "/Image/Character/Enemy/Dragon8.png",RESOURCE_DIR "/Image/Character/Enemy/Octopus8.png"}}
};

Enemy::Enemy(Type type) 
    : BackgroundImage(s_EnemyDataTable.at(type).imagePath1), m_Type(type) {
    
    // 從數據表中拷貝數值到當前實例
    m_Stats = s_EnemyDataTable.at(type);
    
    // 設定預設縮放與層級 (確保跟地圖格子大小一致)
    this->SetScale({1.75f,1.75f});
    this->m_ZIndex = 5.0f; // 設在背景之上，UI 之下
}

void Enemy::UpdateImage(bool showAltFrame) {
    // 檢查目前顯示的是否已經是目標幀，避免每一幀都重複讀取檔案
    if (m_CurrentFrameIsAlt == showAltFrame) return;

    m_CurrentFrameIsAlt = showAltFrame;

    // 修正點：BackgroundImage 內部是透過 m_Drawable 來渲染的
    // 我們直接更換 m_Drawable 指向的 Image 物件
    if (showAltFrame) {
        this->m_Drawable = std::make_shared<Util::Image>(m_Stats.imagePath2);
    } else {
        this->m_Drawable = std::make_shared<Util::Image>(m_Stats.imagePath1);
    }
}