#pragma once
#include "YunutyEngine.h"
#include "MapObject.h"

namespace gs_map
{
    class CameraSticky : public Component
    {
    public:
        bool followScale = true;
    protected:
        void Update()
        {
            GetTransform()->SetWorldPosition(D2DCamera::GetMainD2DCamera()->GetTransform()->GetWorldPosition());
            if (followScale)
                GetTransform()->scale = (D2DCamera::GetMainD2DCamera()->GetTransform()->GetWorldScale());
        }
    private:
    };
}
