#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Map/Map.hpp"
#include "Character/Character.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    Map m_GameMap;
    std::shared_ptr<Character> m_Player;
    State m_CurrentState = State::START;

private:

};

#endif
