#ifndef BATTLE_ANIMATION_HPP
#define BATTLE_ANIMATION_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

class BattleAnimation : public Util::GameObject {
public:
    BattleAnimation() {
        // 假設你有 attack1.png, attack2.png, attack3.png 等特效圖
        m_Animation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Effect/attack_animation_1.bmp",
                RESOURCE_DIR "/Image/Effect/attack_animation_2.bmp",
                RESOURCE_DIR "/Image/Effect/attack_animation_3.bmp"
            },
            true, 200, false, 0 // 每幀 100ms，不循環播放
        );
        m_Drawable = m_Animation;
        m_ZIndex = 20.0f; // 確保在所有人之上
        SetVisible(false);
    }

    void Play(const glm::vec2& pos) {
        m_Transform.translation = pos;
        SetVisible(true);
        m_Animation->Play();
    }

    bool IsFinished() {
        return m_Animation->GetState() == Util::Animation::State::ENDED;
    }

    void Reset() { SetVisible(false); }

    bool IsVisible() const {
        return m_Visible;
    }

private:
    std::shared_ptr<Util::Animation> m_Animation;
};

#endif