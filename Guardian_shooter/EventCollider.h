#pragma once
#include <fstream>
#include "EnemyType.h"
#include "MapObject.h"


#ifdef _DEBUG
namespace gs_map
{
    class EventCollider :
        public MapObject
    {
    public:
        friend class MapTool;

        EventCollider();
        ~EventCollider();
        static bool SaveInfo();
        static bool LoadInfo();
        virtual void Invoke();
        virtual void Apply();
    protected:
        virtual void OnCollisionEnter2D(const Collision2D& collision) override;
    };
}
#endif
