#include "App/App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_Player = std::make_shared<Character>(RESOURCE_DIR "/Image/Character/Player/player_d1.png");
    m_Player->SetScale({2.40625f, 2.40625f});
    m_Player->SetPosition({0.0f, 0.0f});

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    m_GameMap.Draw();

    // 設定每一格移動的距離 (根據您之前的設定是 77)
    float moveDist = 77.0f;

    // 鍵盤輸入偵測
    // 使用 IsKeyDown：只有在按下的那一幀會觸發
    if (Util::Input::IsKeyDown(Util::Keycode::W) || Util::Input::IsKeyDown(Util::Keycode::UP)) {
        m_Player->SetImage(RESOURCE_DIR "/Image/Character/Player/player_u1.png"); // 轉向上
        m_Player->Move({0, moveDist});
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::S) || Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        m_Player->SetImage(RESOURCE_DIR "/Image/Character/Player/player_d1.png"); // 轉向下
        m_Player->Move({0, -moveDist});
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::A) || Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
        m_Player->SetImage(RESOURCE_DIR "/Image/Character/Player/player_tl1.png"); // 轉向左
        m_Player->Move({-moveDist, 0});
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::D) || Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
        m_Player->SetImage(RESOURCE_DIR "/Image/Character/Player/player_tr1.png"); // 轉向右
        m_Player->Move({moveDist, 0});
    }


    static Util::Renderer renderer;
    renderer.AddChild(m_Player);
    renderer.Update();
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
