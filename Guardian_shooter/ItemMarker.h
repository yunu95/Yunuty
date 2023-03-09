#pragma once
#include <fstream>
#include "Item.h"
#include "MapObject.h"


namespace gs_map
{
    class MapTool;
    class ItemMarker :
        public MapObject
    {
    public:
        friend class MapTool;

        ItemMarker();
        ~ItemMarker();
        // 파일 입출력 로직
        static void Save(wstring mapRoot);
        static void Load(wstring mapRoot);
        static ItemMarker* CreateItemMarker(Vector2d location, ItemType itemType);
        static void DeleteItemMarker(ItemMarker* marker);
        void SetOpacity(double opacity);
        virtual void Invoke();
        virtual void OnRestart();
        virtual void OnEngagingMapEditMode();
        virtual void OnDisengagingMapEditMode();
        ItemType itemType = ItemType::HealPack;
        bool alreadySummoned = false;

    protected:
        virtual void Update() override;
        virtual void OnCollisionEnter2D(const Collision2D& collision) override;
        virtual void OnCollisionStay2D(const Collision2D& collision) override;
    private:
        int markerIndex;
        D2DAnimatedSprite* markerSprite = nullptr;
        static ItemMarker* selectedItemMarker;
        static vector<ItemMarker*> itemMarkers;
        static ifstream fin;
        static ofstream fout;
    };
}
