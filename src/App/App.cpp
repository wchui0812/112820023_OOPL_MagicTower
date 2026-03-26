#include "App/App.hpp"
#include "Character/Player.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_Player = std::make_shared<Player>();
    m_Player->SetScale({1.875f, 1.875f});
    m_Player->SetPosition({0.0f, 0.0f});

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    m_GameMap.Draw();

    m_Player->Update(m_GameMap);

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
