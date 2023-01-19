#include "SpeedMeter.h"

void SpeedMeter::Update()
{
    if (GetGameObject()->GetComponent<YunutyEngine::RigidBody2D>() && GetGameObject()->GetComponent<D2D::D2DText>())
    {
        GetGameObject()->GetComponent<D2D::D2DText>()->fontSize = 30;
        GetGameObject()->GetComponent<D2D::D2DText>()->width = 300;
        GetGameObject()->GetComponent<D2D::D2DText>()->height = 300;
        wstringstream sstream(L"");
        sstream << (int)GetGameObject()->GetComponent<YunutyEngine::RigidBody2D>()->velocity.x;
        sstream << ',';
        sstream << (int)GetGameObject()->GetComponent<YunutyEngine::RigidBody2D>()->velocity.y;

        GetGameObject()->GetComponent<D2D::D2DText>()->text = sstream.str();
    }
}
