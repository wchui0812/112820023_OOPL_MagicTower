#ifndef FLOOR_TELEPORT_HPP
#define FLOOR_TELEPORT_HPP

#include "System/TextObject.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#include <memory>

class Map;
class Player;

class FloorTeleport : public Util::GameObject {
public:
    FloorTeleport();

    void Open(const Map& map, const Player& player);
    void Update(float deltaTime, Map& map, Player& player);
    void Draw();

    bool IsActive() const { return m_Active; }

private:
    void MoveSelection(int delta);
    void ConfirmSelection(Map& map, Player& player);
    void RefreshTexts();

    bool m_Active = false;
    int m_SelectedLevel = 0;
    int m_MinLevel = 0;
    int m_MaxLevel = 0;
    float m_InputCooldown = 0.0f;
    const float m_InputDelay = 0.18f;

    std::shared_ptr<Util::Image> m_Background;
    std::shared_ptr<Util::Image> m_Frame;
    std::shared_ptr<Util::Image> m_UpArrowWhite;
    std::shared_ptr<Util::Image> m_UpArrowGray;
    std::shared_ptr<Util::Image> m_DownArrowWhite;
    std::shared_ptr<Util::Image> m_DownArrowGray;

    std::shared_ptr<TextObject> m_FloorNumberText;
    std::shared_ptr<TextObject> m_SelectHintText;
    std::shared_ptr<TextObject> m_QuitHintText;
};

#endif
