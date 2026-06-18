#ifndef GAME_FLOW_SCENE_HPP
#define GAME_FLOW_SCENE_HPP

#include "System/TextObject.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#include <memory>
#include <string>
#include <vector>

class GameFlowScene : public Util::GameObject {
public:
    enum class Phase {
        Login,
        Loading,
        Intro,
        Playing,
        GameOver,
        Win,
        Ending,
        Finished
    };

    GameFlowScene();

    void Update(float deltaTime);
    void Draw();

    bool CanUpdateGame() const { return m_Phase == Phase::Playing; }
    bool ShouldShowStatusUI() const {
        return m_Phase == Phase::Playing ||
               m_Phase == Phase::Win ||
               m_Phase == Phase::Ending;
    }
    bool ShouldDrawMap() const {
        return m_Phase == Phase::Intro ||
               m_Phase == Phase::Playing ||
               m_Phase == Phase::GameOver ||
               m_Phase == Phase::Win ||
               m_Phase == Phase::Ending;
    }
    bool IsFinished() const { return m_Phase == Phase::Finished; }
    bool ConsumeRestartRequested() {
        const bool requested = m_RestartRequested;
        m_RestartRequested = false;
        return requested;
    }

    void StartPlaying();
    void StartGameOver();
    void StartWin();
    void StartEnding();

private:
    void LoadStartingText();
    void LoadEndingText();
    void UpdateLoading(float deltaTime);
    void UpdateIntro(float deltaTime);
    void UpdateWin(float deltaTime);
    void UpdateEnding(float deltaTime);
    void DrawLogin();
    void DrawLoading();
    void DrawIntro();
    void DrawGameOver();
    void DrawWin();
    void DrawEnding();
    void DrawResultScreen(TextObject& title, TextObject& subtitle, TextObject& hint);
    void DrawCenteredImage(const std::shared_ptr<Util::Image>& image, float zIndex, const glm::vec2& scale = {1.0f, 1.0f});
    void DrawMapSizedImage(const std::shared_ptr<Util::Image>& image, float zIndex);
    void DrawCenteredText(TextObject& text, const glm::vec2& center);
    void DrawLeftAlignedText(TextObject& text, const glm::vec2& leftCenter);

    Phase m_Phase = Phase::Login;
    float m_Timer = 0.0f;
    float m_IntroScrollY = -330.0f;
    float m_EndingScrollY = -330.0f;
    bool m_RestartRequested = false;

    std::shared_ptr<Util::Image> m_LoginImage;
    std::shared_ptr<Util::Image> m_BlackBackground;
    std::vector<std::shared_ptr<Util::Image>> m_LoadingImages;
    std::vector<std::string> m_StartingLines;
    std::vector<std::string> m_EndingLines;

    int m_LoadingIndex = 0;
    std::vector<std::shared_ptr<TextObject>> m_IntroTexts;
    std::vector<std::shared_ptr<TextObject>> m_EndingTexts;
    std::shared_ptr<TextObject> m_GameOverTitleText;
    std::shared_ptr<TextObject> m_GameOverSubtitleText;
    std::shared_ptr<TextObject> m_GameOverHintText;
    std::shared_ptr<TextObject> m_WinTitleText;
    std::shared_ptr<TextObject> m_WinSubtitleText;
    std::shared_ptr<TextObject> m_WinHintText;

    const float m_LoadingFrameTime = 0.35f;
    const float m_IntroScrollSpeed = 36.0f;
    const float m_EndingScrollSpeed = 32.0f;
    static constexpr float kMapStartX = -165.0f;
    static constexpr float kMapStartY = 308.0f;
    static constexpr float kMapSize = 56.0f * 11.0f;
    static constexpr float kMapCenterX = kMapStartX + kMapSize / 2.0f;
    static constexpr float kMapCenterY = kMapStartY - kMapSize / 2.0f;
    static constexpr float kTextClipPadding = 56.0f;
};

#endif
