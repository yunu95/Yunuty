#include "pch.h"
#include "PixelInfos.h"

using namespace YunuD2D;

PixelInfos::PixelInfos(IWICBitmap* pBitmap)
{
    UINT width, height;
    pBitmap->GetSize(&width, &height);
    IWICBitmapLock* pLock = nullptr;
    WICRect rc = { 0,0,width,height };
    this->width = width;
    this->height = height;
    HRESULT hr = pBitmap->Lock(&rc, WICBitmapLockRead, &pLock);

    UINT cbBufferSize;
    BYTE* pBuffer = nullptr;
    pLock->GetDataPointer(&cbBufferSize, &pBuffer);

    for (UINT y = 0; y < height; y++)
    {
        pixels.push_back(vector<Pixel>());
        for (UINT x = 0; x < width; x++)
            pixels[y].push_back(Pixel(
                pBuffer[y * width * 4 + x * 4 + 2],
                pBuffer[y * width * 4 + x * 4 + 1],
                pBuffer[y * width * 4 + x * 4],
                pBuffer[y * width * 4 + x * 4 + 3]
            ));
    }
    pLock->Release();
}
