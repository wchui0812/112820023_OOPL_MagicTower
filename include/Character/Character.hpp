#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <vector>
#include <map>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Character : public Util::GameObject {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    explicit Character(const std::string&) {
        std::map<Direction, std::string> dirCode = {
            {Direction::UP, "u"}, {Direction::DOWN, "d"},
            {Direction::LEFT, "tl"}, {Direction::RIGHT, "tr"}
        };

        // 預載每個面向的 3 張圖 (假設編號是 1, 2, 3)
        for (auto const& [dir, code] : dirCode) {
            for (int i = 1; i <= 3; ++i) {
                // 格式：RESOURCE_DIR "/Image/Character/Player/player_u1.png"
                std::string path = RESOURCE_DIR "/Image/Character/Player/player_" + code + std::to_string(i) + ".png";
                m_AnimationFrames[dir].push_back(std::make_shared<Util::Image>(path));
            }
        }

        m_CurrentDir = Direction::DOWN;
        m_Drawable = m_AnimationFrames[m_CurrentDir][0];// 預設站立 (索引 0 對應編號 1)
        m_ZIndex = 5.0f; // 設定較高的 ZIndex 確保角色在地圖（背景）上方
    }

    //void SetImage(const std::string& path) {
        //m_Drawable = std::make_shared<Util::Image>(path);
    //}

    void NextFrame() {
        m_FrameIndex = (m_FrameIndex + 1) % 3; // 在 0, 1, 2 之間循環
        m_Drawable = m_AnimationFrames[m_CurrentDir][m_FrameIndex];
    }

    void SetDirection(Direction dir) {
        if (m_CurrentDir != dir) {
            m_CurrentDir = dir;
            m_FrameIndex = 0; // 換方向時回到第一幀
        } else {
            NextFrame(); // 同方向連續移動則切換下一幀
        }
        m_Drawable = m_AnimationFrames[m_CurrentDir][m_FrameIndex];
    }

    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

    [[nodiscard]] const glm::vec2& GetPosition() const {
        return m_Transform.translation;
    }

    void Move(const glm::vec2& delta) {
        m_Transform.translation += delta;
    }

    void Update();

private:
    // 使用 std::map 儲存預載好的圖片
    Direction m_CurrentDir;
    int m_FrameIndex = 0;
    std::map<Direction, std::vector<std::shared_ptr<Util::Image>>> m_AnimationFrames;
};

#endif