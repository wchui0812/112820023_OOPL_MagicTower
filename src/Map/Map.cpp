#include "Map/Map.hpp"
#include "Util/Renderer.hpp"

Map::Map() 
    : m_Wall(RESOURCE_DIR "/Image/Road/wall_b.bmp"),   // 對應你的實際路徑
      m_Floor(RESOURCE_DIR "/Image/Road/road.bmp") // 對應你的實際路徑
{}

void Map::Draw() {
    static Util::Renderer renderer;

    // 1. 動態計算偏移量
    // 地圖總寬度是 (格數 11 * 每格大小 77)，除以 2 得到中心偏移
    // 加上負號讓座標系往左下平移，使地圖中心對準視窗 (0,0)
    float offsetX = -(11.0f * m_TileSize) / 2.0f;
    float offsetY = -(11.0f * m_TileSize) / 2.0f;

    for (int i = 0; i < 11; ++i) {     // 列 (Y軸)
        for (int j = 0; j < 11; ++j) { // 行 (X軸)
            
            // 計算每個格子在螢幕上的座標
            float posX = j * m_TileSize + offsetX;
            float posY = i * m_TileSize + offsetY;

            if (m_MapData[i][j] == 1) {
                m_Wall.SetPosition({posX, posY});
                m_Wall.Draw();
            } else {
                m_Floor.SetPosition({posX, posY});
                m_Floor.Draw();
            }
        }
    }
    renderer.Update();
}