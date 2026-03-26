#ifndef STAIR_HPP
#define STAIR_HPP

#include "Util/Logger.hpp"
#include <glm/vec2.hpp>

// 前置宣告：告訴編譯器 Map 是一個類別，先不用去看它的詳細內容
class Map;

class Stair {
public:
    // 因為這裡只用到 Map 的引用 (Map&)，所以前置宣告就夠了
    static void CheckAndTransport(glm::vec2& playerPos, Map& map, const glm::vec2& offsetDir);
};

#endif