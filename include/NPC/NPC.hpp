#ifndef NPC_HPP
#define NPC_HPP

#include "App/BackgroundImage.hpp"
#include <string>
#include <unordered_map>

class NPC : public BackgroundImage {
public:
    enum class Type {
        FAIRY,
        ELDER,   // 智者（老人）
        MERCHANT,  // 商人
        THIEF,      // 小偷
        PRINCESS
    };

    struct Stats {
        std::string name;
        std::string imagePath1;
        std::string imagePath2;
    };

    NPC(Type type);
    void UpdateImage(bool showAltFrame);

private:
    Type m_Type;
    Stats m_Stats;
    bool m_CurrentFrameIsAlt = false;
    static const std::unordered_map<Type, Stats> s_NPCDataTable;
};

#endif