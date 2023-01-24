#include "YunuD2DGraphicCore.h"
#include "D2DSprite.h"
void D2DSprite::Render(D2D1::Matrix3x2F transform)
{
    YunuD2D::YunuD2DGraphicCore::GetInstance()->DrawSprite(spriteFilePath, transform,color,drawRect.width,drawRect.height);
}
