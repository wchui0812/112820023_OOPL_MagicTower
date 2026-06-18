#include "System/NoticeScene.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

NoticeScene::NoticeScene() {
    m_Background = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/Black.bmp");
    m_Drawable = m_Background;
    m_ZIndex = kPanelZ;
    m_Transform.translation = {142.0f, 0.0f};

    BuildTexts();
    SetVisible(false);
}

void NoticeScene::Open() {
    m_Active = true;
    m_InputCooldown = m_InputDelay;
    SetVisible(true);
}

void NoticeScene::Update(float deltaTime) {
    if (!m_Active) return;

    if (m_InputCooldown > 0.0f) {
        m_InputCooldown -= deltaTime;
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::N) ||
        Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
        Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        m_Active = false;
        SetVisible(false);
    }
}

void NoticeScene::Draw() {
    if (!m_Active) return;

    Util::Transform backgroundTransform = m_Transform;
    backgroundTransform.scale = {kPanelScale, kPanelScale};
    m_Background->Draw(Util::ConvertToUniformBufferData(
        backgroundTransform,
        m_Background->GetSize(),
        m_ZIndex
    ));

    for (auto& text : m_Texts) {
        text->Draw();
    }
}

void NoticeScene::BuildTexts() {
    m_Texts.clear();

    const glm::vec2 origin = m_Transform.translation + glm::vec2(-245.0f, 225.0f);
    constexpr float line = 35.0f;

    AddText(34, "一般按鍵", origin);
    AddText(24, "Space / Enter: 確認、跳過對話", origin + glm::vec2(0.0f, -line * 1.4f));
    AddText(24, "Arrow: 角色移動、選單選擇", origin + glm::vec2(0.0f, -line * 2.4f));
    AddText(24, "R: 重新開始", origin + glm::vec2(0.0f, -line * 3.4f));
    AddText(24, "ESC: 離開遊戲", origin + glm::vec2(0.0f, -line * 4.4f));

    AddText(34, "特殊按鍵", origin + glm::vec2(0.0f, -line * 6.2f));
    AddText(24, "J: 樓層飛升", origin + glm::vec2(0.0f, -line * 7.6f));
    AddText(20, "   （獲得風之羅盤後，可以自由飛往去過的樓層）", origin + glm::vec2(0.0f, -line * 8.5f));
    AddText(24, "L: 怪物清單", origin + glm::vec2(0.0f, -line * 9.7f));
    AddText(20, "   （獲得聖光徽後，可以查看怪物基本情況及能力）", origin + glm::vec2(0.0f, -line * 10.6f));

    AddText(20, "N : 關閉提醒", origin + glm::vec2(400.0f, -line * 14.0f));
}

void NoticeScene::AddText(int size, const std::string& text, const glm::vec2& leftCenter) {
    auto textObject = std::make_shared<TextObject>(size, text, m_ZIndex + 2.0f);
    textObject->m_Transform.translation = leftCenter + glm::vec2(textObject->GetSize().x / 2.0f, 0.0f);
    m_Texts.push_back(textObject);
}
