#include "System/TextObject.hpp"
#include "Util/TransformUtils.hpp" // 必須引入這個標頭檔

TextObject::TextObject(const int &size, const std::string &text, const float &zindex) {
    m_Text = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Font/Cubic_11.ttf",
        size,
        text,
        Util::Color::FromName(Util::Colors::WHITE)
    );
    m_ZIndex = zindex;
}

void TextObject::SetText(const std::string &text) {
    // 這裡的大小建議與建構子保持一致，或是儲存一個 m_Size 變數
    m_Text = std::make_shared<Util::Text>(
        RESOURCE_DIR "/Font/Cubic_11.ttf", 32, text,
        Util::Color::FromName(Util::Colors::WHITE)
    );
}

void TextObject::Draw() {
    if (m_Text) {
        // 關鍵修正：使用 ConvertToUniformBufferData 轉換矩陣
        // 1. 取得文字的原始尺寸 (GetSize())
        // 2. 傳入當前的 m_Transform 與 m_ZIndex
        auto matrices = Util::ConvertToUniformBufferData(
            m_Transform,
            m_Text->GetSize(),
            m_ZIndex
        );

        // 呼叫底層渲染
        m_Text->Draw(matrices);
    }
}