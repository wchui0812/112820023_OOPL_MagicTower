#ifndef NOTICE_SCENE_HPP
#define NOTICE_SCENE_HPP

#include "System/TextObject.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#include <memory>
#include <string>
#include <vector>

class NoticeScene : public Util::GameObject {
public:
    NoticeScene();

    void Open();
    void Update(float deltaTime);
    void Draw();

    bool IsActive() const { return m_Active; }

private:
    void BuildTexts();
    void AddText(int size, const std::string& text, const glm::vec2& leftCenter);

    bool m_Active = false;
    float m_InputCooldown = 0.0f;
    const float m_InputDelay = 0.18f;

    std::shared_ptr<Util::Image> m_Background;
    std::vector<std::shared_ptr<TextObject>> m_Texts;

    static constexpr float kPanelScale = 0.73f;
    static constexpr float kPanelZ = 80.0f;
};

#endif
