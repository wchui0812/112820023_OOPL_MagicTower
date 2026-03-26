#include "Map/Map.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"

#include <fstream>
#include <iostream>

Map::Map() 
    : m_Wall(RESOURCE_DIR "/Image/Road/wall_b.bmp"),
      m_Floor(RESOURCE_DIR "/Image/Road/road.bmp"),
      m_Lava(RESOURCE_DIR "/Image/Road/lava.bmp"),
      m_Shine(RESOURCE_DIR "/Image/Road/wall_shine.bmp"),
      m_UpStairs(RESOURCE_DIR "/Image/Background/Stair/upstair.bmp"),   // 5
      m_DownStairs(RESOURCE_DIR "/Image/Background/Stair/downstair.bmp") // 4
{
    float newScale = 0.78f;
    m_Wall.SetScale({newScale, newScale});
    m_Floor.SetScale({newScale, newScale});
    m_Lava.SetScale({newScale, newScale});
    m_Shine.SetScale({newScale, newScale});
    m_UpStairs.SetScale({newScale, newScale});
    m_DownStairs.SetScale({newScale, newScale});

    m_TileSize = 60.0f;

    for (int i = 0; i < 25; ++i) {
        // 動態組合路徑：D:/.../Resources/Levels/map0.txt, map1.txt ...
        std::string filePath = RESOURCE_DIR "/Layer/layer" + std::to_string(i) + ".txt";

        // 呼叫原本的 LoadLevel 函式
        LoadLevel(filePath);
    }

    m_CurrentLevel = 0;
}

void Map::LoadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("無法開啟地圖檔: {}", filePath);
        return;
    }

    std::vector<std::vector<int>> tempLevel(11, std::vector<int>(11));
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            // 直接將檔案中的數字讀入二維陣列
            if (!(file >> tempLevel[i][j])) {
                LOG_ERROR("地圖檔案格式錯誤: {}", filePath);
                // 處理格式不符的情況
            }
        }
    }
    m_MapData.push_back(tempLevel);
    file.close();

    LOG_INFO("成功載入樓層: {}", filePath);
}

int Map::GetTileType(float x, float y) const {
    float totalSize = 11.0f * m_TileSize;
    float offsetX = -totalSize / 2.0f;
    float offsetY = -totalSize / 2.0f;

    // 換算成索引
    int col = static_cast<int>((x - offsetX) / m_TileSize);
    int row = 10 - static_cast<int>((y - offsetY) / m_TileSize);

    // 邊界安全檢查
    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return -1; // 超出範圍
    }

    return m_MapData[m_CurrentLevel][row][col];
}

void Map::Draw() {
    if (m_MapData.empty()) return;
    Util::Renderer renderer;

    // 1. 動態計算偏移量
    // 地圖總寬度是 (格數 11 * 每格大小 77)，除以 2 得到中心偏移
    // 加上負號讓座標系往左下平移，使地圖中心對準視窗 (0,0)
    float totalSize = 11.0f * m_TileSize;
    float offsetX = -totalSize / 2.0f;
    float offsetY = -totalSize / 2.0f;

    auto& currentMap = m_MapData[m_CurrentLevel];

    for (int i = 0; i < 11; ++i) {     // 列 (Y軸)
        for (int j = 0; j < 11; ++j) { // 行 (X軸)
            
            // 計算每個格子在螢幕上的座標
            float posX = j * m_TileSize + offsetX + (m_TileSize / 2.0f);
            float posY = (10 - i) * m_TileSize + offsetY + (m_TileSize / 2.0f);

            int tileType = currentMap[i][j];

            if (currentMap[i][j] == 1) {
                m_Wall.SetPosition({posX, posY});
                renderer.AddChild(std::make_shared<BackgroundImage>(m_Wall));
            }
            else if (tileType == 2) {
                m_Lava.SetPosition({posX, posY});
                renderer.AddChild(std::make_shared<BackgroundImage>(m_Lava));
            }
            else if (tileType == 3) {
                m_Shine.SetPosition({posX, posY});
                renderer.AddChild(std::make_shared<BackgroundImage>(m_Shine));
            }
            else if (tileType == 5) { // 上樓梯
                m_UpStairs.SetPosition({posX, posY});
                renderer.AddChild(std::make_shared<BackgroundImage>(m_UpStairs));
            } else if (tileType == 4) { // 下樓梯
                m_DownStairs.SetPosition({posX, posY});
                renderer.AddChild(std::make_shared<BackgroundImage>(m_DownStairs));
            }
            else {
                m_Floor.SetPosition({posX, posY});
                renderer.AddChild(std::make_shared<BackgroundImage>(m_Floor));
            }
        }
    }
    renderer.Update();
}

bool Map::IsWalkable(float x, float y) const {
    // 1. 計算偏移量（與 Draw 函式一致）
    float totalSize = 11.0f * m_TileSize;
    float offsetX = -totalSize / 2.0f;
    float offsetY = -totalSize / 2.0f;

    // 將螢幕座標轉換為「相對於地圖左下角」的距離
    float relativeX = x - offsetX;
    float relativeY = y - offsetY;

    // 2. 將螢幕座標 (x, y) 轉換為陣列索引 (row, col)
    int col = static_cast<int>(relativeX / m_TileSize);
    int row = 10 - static_cast<int>(relativeY / m_TileSize);

    // 3. 邊界檢查：超出 11x11 範圍視為不能走
    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return false;
    }

    // 取得當前樓層的格子數值
    int tileType = m_MapData[m_CurrentLevel][row][col];

    // 4. 通行判斷：地板(0)、下樓梯(4)、上樓梯(5)
    return (tileType == 0 || tileType == 4 || tileType == 5);
}

glm::vec2 Map::FindTilePosition(int targetType) const {
    auto& currentMap = m_MapData[m_CurrentLevel];
    float totalSize = 11.0f * m_TileSize;
    float offsetX = -totalSize / 2.0f;
    float offsetY = -totalSize / 2.0f;

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            if (currentMap[i][j] == targetType) {
                // 換算回螢幕座標
                float posX = j * m_TileSize + offsetX + (m_TileSize / 2.0f);
                float posY = (10 - i) * m_TileSize + offsetY + (m_TileSize / 2.0f);
                return {posX, posY};
            }
        }
    }
    return {0.0f, 0.0f}; // 若沒找到則回傳中心點
}