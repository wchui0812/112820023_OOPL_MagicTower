#include "System/GameFlowScene.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

#include <algorithm>
#include <fstream>

GameFlowScene::GameFlowScene() {
    m_LoginImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Scene/scene0.BMP");
    m_BlackBackground = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Special/Black.bmp");

    for (int i = 1; i <= 4; ++i) {
        m_LoadingImages.push_back(std::make_shared<Util::Image>(
            std::string(RESOURCE_DIR "/Image/Scene/loading") + std::to_string(i) + ".BMP"
        ));
    }

    LoadStartingText();
    LoadEndingText();

    m_GameOverTitleText = std::make_shared<TextObject>(54, "Game Over！", 90.0f);
    m_GameOverSubtitleText = std::make_shared<TextObject>(28, "無法戰鬥", 90.0f);
    m_GameOverHintText = std::make_shared<TextObject>(28, "Press R to restart", 90.0f);
    m_WinTitleText = std::make_shared<TextObject>(58, "Win！", 90.0f);
    m_WinSubtitleText = std::make_shared<TextObject>(28, "最終 Boss 擊殺成功", 90.0f);
    m_WinHintText = std::make_shared<TextObject>(28, "Press Space to continue", 90.0f);
}

void GameFlowScene::Update(float deltaTime) {
    if (m_Phase == Phase::GameOver &&
        Util::Input::IsKeyDown(Util::Keycode::R)) {
        m_RestartRequested = true;
        return;
    }

    if (m_Phase == Phase::Login) {
        if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
            m_Phase = Phase::Loading;
            m_Timer = 0.0f;
            m_LoadingIndex = 0;
        }
        return;
    }

    if (m_Phase == Phase::Loading) {
        UpdateLoading(deltaTime);
        return;
    }

    if (m_Phase == Phase::Intro) {
        UpdateIntro(deltaTime);
        return;
    }

    if (m_Phase == Phase::Win) {
        UpdateWin(deltaTime);
        return;
    }

    if (m_Phase == Phase::Ending) {
        UpdateEnding(deltaTime);
    }
}

void GameFlowScene::Draw() {
    if (m_Phase == Phase::Login) {
        DrawLogin();
    } else if (m_Phase == Phase::Loading) {
        DrawLoading();
    } else if (m_Phase == Phase::Intro) {
        DrawIntro();
    } else if (m_Phase == Phase::GameOver) {
        DrawGameOver();
    } else if (m_Phase == Phase::Win) {
        DrawWin();
    } else if (m_Phase == Phase::Ending) {
        DrawEnding();
    }
}

void GameFlowScene::StartPlaying() {
    m_Phase = Phase::Playing;
}

void GameFlowScene::StartGameOver() {
    m_Phase = Phase::GameOver;
    m_Timer = 0.0f;
}

void GameFlowScene::StartWin() {
    m_Phase = Phase::Win;
    m_Timer = 0.0f;
}

void GameFlowScene::StartEnding() {
    m_Phase = Phase::Ending;
    m_Timer = 0.0f;
    m_EndingScrollY = kMapCenterY - kMapSize / 2.0f;
}

void GameFlowScene::LoadStartingText() {
    std::ifstream file(RESOURCE_DIR "/Text/Starting.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            m_StartingLines.push_back(line);
        }
    }

    for (const auto& text : m_StartingLines) {
        m_IntroTexts.push_back(std::make_shared<TextObject>(20, text, 90.0f));
    }
}

void GameFlowScene::LoadEndingText() {
    std::ifstream file(RESOURCE_DIR "/Text/Ending.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            m_EndingLines.push_back(line);
        }
    }

    for (const auto& text : m_EndingLines) {
        m_EndingTexts.push_back(std::make_shared<TextObject>(20, text, 90.0f));
    }
}

void GameFlowScene::UpdateLoading(float deltaTime) {
    m_Timer += deltaTime;
    if (m_Timer < m_LoadingFrameTime) return;

    m_Timer = 0.0f;
    m_LoadingIndex++;
    if (m_LoadingIndex >= static_cast<int>(m_LoadingImages.size())) {
        m_Phase = Phase::Intro;
        m_IntroScrollY = kMapCenterY - kMapSize / 2.0f;
    }
}

void GameFlowScene::UpdateIntro(float deltaTime) {
    m_IntroScrollY += m_IntroScrollSpeed * deltaTime;

    const float lineSpacing = 38.0f;
    const float clipBottom = kMapCenterY - kMapSize / 2.0f + kTextClipPadding;
    const float clipTop = kMapCenterY + kMapSize / 2.0f - kTextClipPadding;
    const float lastVisibleLineY = m_IntroTexts.empty()
        ? clipBottom
        : m_IntroScrollY - lineSpacing * static_cast<float>(m_IntroTexts.size() - 1);
    const float lastLineY = m_IntroScrollY - lineSpacing * static_cast<float>(m_IntroTexts.size());

    if (lastVisibleLineY >= clipBottom && Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_Phase = Phase::Playing;
        return;
    }

    if (lastLineY > clipTop) {
        m_Phase = Phase::Playing;
    }
}

void GameFlowScene::UpdateWin(float deltaTime) {
    m_Timer += deltaTime;
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        StartEnding();
    }
}

void GameFlowScene::UpdateEnding(float deltaTime) {
    m_EndingScrollY += m_EndingScrollSpeed * deltaTime;

    const float lineSpacing = 38.0f;
    const float clipTop = kMapCenterY + kMapSize / 2.0f - kTextClipPadding;
    const float lastLineY = m_EndingScrollY - lineSpacing * static_cast<float>(m_EndingTexts.size());
    if (lastLineY > clipTop || Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_Phase = Phase::Finished;
    }
}

void GameFlowScene::DrawLogin() {
    DrawCenteredImage(m_LoginImage, 80.0f);
}

void GameFlowScene::DrawLoading() {
    if (m_LoadingImages.empty()) return;

    const int index = std::min(m_LoadingIndex, static_cast<int>(m_LoadingImages.size()) - 1);
    DrawCenteredImage(m_LoadingImages[index], 80.0f);
}

void GameFlowScene::DrawIntro() {
    DrawMapSizedImage(m_BlackBackground, 80.0f);

    const float lineSpacing = 38.0f;
    const float clipBottom = kMapCenterY - kMapSize / 2.0f + kTextClipPadding;
    const float clipTop = kMapCenterY + kMapSize / 2.0f - kTextClipPadding;
    for (std::size_t i = 0; i < m_IntroTexts.size(); ++i) {
        const float y = m_IntroScrollY - lineSpacing * static_cast<float>(i);
        if (y < clipBottom || y > clipTop) continue;

        DrawLeftAlignedText(*m_IntroTexts[i], {kMapStartX + 35.0f, y});
    }
}

void GameFlowScene::DrawGameOver() {
    DrawResultScreen(*m_GameOverTitleText, *m_GameOverSubtitleText, *m_GameOverHintText);
}

void GameFlowScene::DrawWin() {
    DrawResultScreen(*m_WinTitleText, *m_WinSubtitleText, *m_WinHintText);
}

void GameFlowScene::DrawEnding() {
    DrawMapSizedImage(m_BlackBackground, 80.0f);

    const float lineSpacing = 38.0f;
    const float clipBottom = kMapCenterY - kMapSize / 2.0f + kTextClipPadding;
    const float clipTop = kMapCenterY + kMapSize / 2.0f - kTextClipPadding;
    for (std::size_t i = 0; i < m_EndingTexts.size(); ++i) {
        const float y = m_EndingScrollY - lineSpacing * static_cast<float>(i);
        if (y < clipBottom || y > clipTop) continue;

        DrawLeftAlignedText(*m_EndingTexts[i], {kMapStartX + 35.0f, y});
    }
}

void GameFlowScene::DrawResultScreen(TextObject& title, TextObject& subtitle, TextObject& hint) {
    DrawMapSizedImage(m_BlackBackground, 80.0f);
    DrawCenteredText(title, {kMapCenterX, kMapCenterY + 100.0f});
    DrawCenteredText(subtitle, {kMapCenterX, kMapCenterY + 20.0f});
    DrawCenteredText(hint, {kMapCenterX, kMapCenterY - 100.0f});
}

void GameFlowScene::DrawCenteredImage(
    const std::shared_ptr<Util::Image>& image,
    float zIndex,
    const glm::vec2& scale
) {
    if (!image) return;

    Util::Transform transform;
    transform.translation = {0.0f, 0.0f};
    transform.scale = scale;
    image->Draw(Util::ConvertToUniformBufferData(transform, image->GetSize(), zIndex));
}

void GameFlowScene::DrawMapSizedImage(const std::shared_ptr<Util::Image>& image, float zIndex) {
    if (!image) return;

    const glm::vec2 imageSize = image->GetSize();
    if (imageSize.x <= 0.0f || imageSize.y <= 0.0f) return;

    Util::Transform transform;
    transform.translation = {kMapCenterX, kMapCenterY};
    transform.scale = {
        kMapSize / imageSize.x,
        kMapSize / imageSize.y
    };

    image->Draw(Util::ConvertToUniformBufferData(transform, imageSize, zIndex));
}

void GameFlowScene::DrawCenteredText(TextObject& text, const glm::vec2& center) {
    text.m_Transform.translation = center;
    text.Draw();
}

void GameFlowScene::DrawLeftAlignedText(TextObject& text, const glm::vec2& leftCenter) {
    text.m_Transform.translation = leftCenter + glm::vec2(text.GetSize().x / 2.0f, 0.0f);
    text.Draw();
}
