#include "System/UIStatus.hpp"

#include "Util/Renderer.hpp"

UIStatus::UIStatus() {
    float textX = -285.0f;
    float startY = 210.0f;
    float spacing = 43.0f;
    float defaultZ = 10.0f;
    int fontSize = 32;

    m_LevelText = std::make_shared<TextObject>(fontSize, "1", defaultZ);
    m_LevelText->m_Transform.translation = {textX, startY};

    m_HpText = std::make_shared<TextObject>(fontSize, "1000", defaultZ);
    m_HpText->m_Transform.translation = {textX, startY - spacing};

    m_AtkText = std::make_shared<TextObject>(fontSize, "10", defaultZ);
    m_AtkText->m_Transform.translation = {textX, startY - spacing * 2};

    m_DefText = std::make_shared<TextObject>(fontSize, "10", defaultZ);
    m_DefText->m_Transform.translation = {textX, startY - spacing * 3 + 3};

    m_ExpText = std::make_shared<TextObject>(fontSize, "0", defaultZ);
    m_ExpText->m_Transform.translation = {textX, startY - spacing * 4 + 7};

    m_FloorText = std::make_shared<TextObject>(fontSize, "0", defaultZ);
    m_FloorText->m_Transform.translation = {155, 333};

    // 鑰匙區域座標設定
    float keyX = -285.0f;
    float keyStartY = -115.0f;
    float keySpacing = 57.0f;

    m_YellowKeyText = std::make_shared<TextObject>(fontSize, "0", defaultZ);
    m_YellowKeyText->m_Transform.translation = {keyX, keyStartY};

    m_BlueKeyText = std::make_shared<TextObject>(fontSize, "0", defaultZ);
    m_BlueKeyText->m_Transform.translation = {keyX, keyStartY - keySpacing};

    m_RedKeyText = std::make_shared<TextObject>(fontSize, "0", defaultZ);
    m_RedKeyText->m_Transform.translation = {keyX, keyStartY - keySpacing * 2};

    m_MoneyText = std::make_shared<TextObject>(fontSize, "0", defaultZ);
    m_MoneyText->m_Transform.translation = {keyX, keyStartY - keySpacing * 3};

    if (m_LevelText) {
        LOG_DEBUG("UIStatus: LevelText initialized at x: {}, y: {}",
                  m_LevelText->m_Transform.translation.x,
                  m_LevelText->m_Transform.translation.y);
    }
}

void UIStatus::Update(const Player& player, const Map& map) {
    // 使用 TextObject::SetText 更新內容
    m_LevelText->SetText(std::to_string(player.m_Level));
    m_HpText->SetText(std::to_string(player.m_Hp));
    m_AtkText->SetText(std::to_string(player.m_Atk));
    m_DefText->SetText(std::to_string(player.m_Def));
    m_ExpText->SetText(std::to_string(player.m_Exp));
    m_FloorText->SetText(map.GetDisplayLevelName());

    m_YellowKeyText->SetText(std::to_string(player.m_YellowKeys));
    m_BlueKeyText->SetText(std::to_string(player.m_BlueKeys));
    m_RedKeyText->SetText(std::to_string(player.m_RedKeys));
    m_MoneyText->SetText(std::to_string(player.m_Coin));
}

void UIStatus::Draw() {
    m_LevelText->Draw();
    m_HpText->Draw();
    m_AtkText->Draw();
    m_DefText->Draw();
    m_ExpText->Draw();
    m_FloorText->Draw();
    m_YellowKeyText->Draw();
    m_BlueKeyText->Draw();
    m_RedKeyText->Draw();
    m_MoneyText->Draw();
}
