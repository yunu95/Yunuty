#pragma once
#include "YunutyEngine.h"
#include "MapObject.h"

class ImageScroller;
namespace gs_map
{
    class MapImage : public MapObject
    {
    public:
        enum class LayerType
        {
            ForeCharacters, BackCharacters, BehindBackground
        };
        MapImage() { mapImages.insert(this); }
        ~MapImage()
        {
            mapImages.erase(this);
        }
        // 파일 입출력 로직
        static void Save(wstring mapRoot);
        static void Load(wstring mapRoot);
        static MapImage* CreateMapImage(Vector2d location, LayerType layerType);
        static MapImage* CreateMapImage(Vector2d location, MapImage* original);
        static void DeleteMapImage(MapImage* point);
        void SetLayerType(LayerType layerType);
        void SetScrollRate(double scrollRate);
        void PlaceTop();
        void PlaceBottom();
    protected:
        virtual void Update() override;
    private:
        static unordered_set<MapImage*> mapImages;
        static D2DRectangle* selectedRectangle;
        static MapImage* selectedImage;
        static Vector3d selectedDeltaPosition;
        void PropagateTransform();
        void ApplyScrollLogic();
        // 카메라가 x만큼 오른쪽으로 움직이면, 이미지는 변위 * (1-scrollRate) 만큼 오른쪽으로 움직인다.
        double scrollRate = 1.0;
        // 이미지의 위치 = scrollConstant + (1-scrollRate)
        double scrollConstant = 0;
        LayerType layerType;
        D2DSprite* sprite = nullptr;
        ImageScroller* imageScroller = nullptr;
        BoxCollider2D* boxCollider = nullptr;
        friend MapTool;
    };
}