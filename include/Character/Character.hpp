#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::string& path) {
        m_Drawable = std::make_shared<Util::Image>(path);
        m_ZIndex = 5.0f; // 設定較高的 ZIndex 確保角色在地圖（背景）上方
    }

    void SetImage(const std::string& path) {
        m_Drawable = std::make_shared<Util::Image>(path);
    }

    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    void Move(const glm::vec2& delta) {
        m_Transform.translation += delta;
    }

};

#endif