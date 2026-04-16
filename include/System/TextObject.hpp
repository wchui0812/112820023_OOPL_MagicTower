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

    void Draw();

private:
    std::shared_ptr<Util::Text> m_Text;
};

#endif
