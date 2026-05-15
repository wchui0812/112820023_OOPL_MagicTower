#ifndef TEXTOBJECT_HPP
#define TEXTOBJECT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include <memory>
#include <string>

class TextObject : public Util::GameObject{
public:
    TextObject(const int &size, const std::string &text, const float &zindex);

    void SetText(const std::string &text);
    glm::vec2 GetSize() const;

    void Draw();

private:
    int m_Size;
    std::shared_ptr<Util::Text> m_Text;
};

#endif
