//#include "YunuD2DGraphicCore.h"
#include "RA2NavigationField2D.h"

using namespace YunutyEngine;

void RA2NavigationField2D::LoadTerrainData(wstring filepath)
{
    const YunutyPixelInfos pixelInfos = YunutyEngine::D2D::GetPixelInfos(filepath);
    for (int row = 0; row < tileHeight; row++)
    {
        for (int col = 0; col < tileWidth; col++)
        {
            int pixelRow = (0.999999999999 - row / double(tileHeight)) * pixelInfos.height;
            int pixelCol = (col / double(tileWidth)) * pixelInfos.width;
            auto pixel = pixelInfos[pixelRow][pixelCol];
            if (pixelInfos[pixelRow][pixelCol].r == 255)
                tileMap[row][col].state = Tile::State::Closed;
            else
                tileMap[row][col].state = Tile::State::Open;
        }
    }
}
