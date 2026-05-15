#ifndef MAGICTOWER_REWARDMESSAGE_HPP
#define MAGICTOWER_REWARDMESSAGE_HPP

#include "Util/GameObject.hpp"    // 基礎遊戲物件類別
#include "Util/Image.hpp"         // 處理橫幅背景圖片
#include "System/TextObject.hpp"  // 處理獲得獎勵或道具的文字顯示
#include "Util/Input.hpp"         // 偵測玩家按下 Z 或 Enter 關閉訊息
#include <memory>
#include <string>

class RewardMessage : public Util::GameObject {
public:
    RewardMessage();
    void Show(const std::string& message); // 顯示訊息
    void ShowItem(const std::string& message);
    void Update(float delta_time); // 偵測按鍵關閉
    void Draw();
    bool IsVisible() const {
        return m_Visible;
    }


private:
    std::shared_ptr<Util::Image> m_Background;
    std::shared_ptr<Util::Image> m_ItemIcon;
    std::shared_ptr<TextObject> m_Text;
    bool m_Visible = false;

    float m_DisplayTimer = 0.0f;
    const float m_MaxDisplayTime = 1.0f;
};

#endif //MAGICTOWER_REWARDMESSAGE_HPP