#include "NPC/NPC.hpp"
#include "Util/Image.hpp"

// 定義 NPC 數據表
const std::unordered_map<NPC::Type, NPC::Stats> NPC::s_NPCDataTable = {
    {NPC::Type::FAIRY,   {"仙子",
     RESOURCE_DIR "/Image/Character/NPC/Fairy1.png",
     RESOURCE_DIR "/Image/Character/NPC/Fairy2.png"}},
    {NPC::Type::ELDER,   {"智者",
        RESOURCE_DIR "/Image/Character/NPC/Elder1.png",
        RESOURCE_DIR "/Image/Character/NPC/Elder2.png"}},
    {NPC::Type::MERCHANT,  {"商人", 
        RESOURCE_DIR "/Image/Character/NPC/Merchant1.png", 
        RESOURCE_DIR "/Image/Character/NPC/Merchant2.png"}},
    {NPC::Type::THIEF,     {"小偷", 
        RESOURCE_DIR "/Image/Character/NPC/Thief1.png", 
        RESOURCE_DIR "/Image/Character/NPC/Thief2.png"}},
    {NPC::Type::PRINCESS,     {"公主",
        RESOURCE_DIR "/Image/Character/NPC/Princess1.png",
        RESOURCE_DIR "/Image/Character/NPC/Princess2.png"}}
};

NPC::NPC(Type type) 
    : BackgroundImage(s_NPCDataTable.at(type).imagePath1), m_Type(type) {
    
    m_Stats = s_NPCDataTable.at(type);
    
    // 設定縮放與層級，建議與敵人一致
    this->SetScale({1.75f, 1.75f});
    this->m_ZIndex = 5.0f; 
}

void NPC::UpdateImage(bool showAltFrame) {
    // 檢查狀態是否改變，避免每一幀都重新載入圖片
    if (m_CurrentFrameIsAlt == showAltFrame) return;
    
    m_CurrentFrameIsAlt = showAltFrame;
    
    // 根據 PTSD 框架修正：更換 m_Drawable 內的影像資源
    if (showAltFrame) {
        this->m_Drawable = std::make_shared<Util::Image>(m_Stats.imagePath2);
    } else {
        this->m_Drawable = std::make_shared<Util::Image>(m_Stats.imagePath1);
    }
}