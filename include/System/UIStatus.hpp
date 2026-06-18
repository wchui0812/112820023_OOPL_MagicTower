#ifndef UISTATUS_HPP
#define UISTATUS_HPP

#include "TextObject.hpp"
#include "Character/Player.hpp"
#include "Map/Map.hpp"

#include <memory>

class UIStatus {
public:
    UIStatus();
    void Update(const Player& player, const Map& map);
    void Draw();


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
};

#endif
