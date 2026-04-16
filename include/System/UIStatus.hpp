#ifndef UISTATUS_HPP
#define UISTATUS_HPP

#include "TextObject.hpp" // 使用專案現成的 TextObject
#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include <memory>
#include <string>

class UIStatus {
public:
    UIStatus();
    void Update(const Player& player, const Map& map);
    void Draw();

private:
    // 使用 TextObject 替代 Util::Text 以獲得更好的操作介面
    std::shared_ptr<TextObject> m_LevelText;
    std::shared_ptr<TextObject> m_HpText;
    std::shared_ptr<TextObject> m_AtkText;
    std::shared_ptr<TextObject> m_DefText;
    std::shared_ptr<TextObject> m_ExpText;
    std::shared_ptr<TextObject> m_FloorText;

    std::shared_ptr<TextObject> m_YellowKeyText;
    std::shared_ptr<TextObject> m_BlueKeyText;
    std::shared_ptr<TextObject> m_RedKeyText;
    std::shared_ptr<TextObject> m_MoneyText;

    const std::string m_FontPath = RESOURCE_DIR "/Font/Cubic_11.ttf";
};

#endif