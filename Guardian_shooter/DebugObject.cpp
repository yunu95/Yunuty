#include "DebugObject.h"
#include "Guardian_shooter.h"

//#define _MAPTOOL

unordered_set<DebugObject*> DebugObject::debugObjects;
bool DebugObject::debugMode = true;

DebugObject::DebugObject()
{
    debugObjects.insert(this);
};
DebugObject::~DebugObject()
{
    debugObjects.erase(this);
};
void DebugObject::EnableDebugmode()
{
    if (debugMode)
        return;

    debugMode = true;
    for (auto each : debugObjects)
        each->onDebugEnabled();
}
void DebugObject::DisableDebugmode()
{
    if (!debugMode)
        return;

    debugMode = false;
    for (auto each : debugObjects)
        each->onDebugDisabled();
}
void DebugObject::ToggleDebugmode()
{
    if (debugMode)
        DisableDebugmode();
    else
        EnableDebugmode();
}
void DebugObject::CreateDebugRectImage(GameObject* parent, double width, double height, D2D1::ColorF color, double border, bool filled)
{
#ifdef _MAPTOOL
    auto imageObj = parent->GetScene()->AddGameObject(parent);
    auto debugComp = imageObj->AddComponent<DebugObject>();
    auto image = imageObj->AddComponent<D2DRectangle>();
    debugComp->onDebugEnabled = [imageObj]() {imageObj->SetSelfActive(true); };
    debugComp->onDebugDisabled = [imageObj]() {imageObj->SetSelfActive(false); };
    image->width = width;
    image->height = height;
    image->color = color;
    image->border = border;
    image->filled = filled;
#endif
}
void DebugObject::CreateColliderImage(BoxCollider2D* collider, D2D1::ColorF color, double border, bool filled)
{
#ifdef _MAPTOOL
    auto imageObj = collider->GetGameObject()->AddGameObject();
    auto debugComp = imageObj->AddComponent<DebugObject>();
    auto image = imageObj->AddComponent<D2DRectangle>();
    debugComp->onDebugEnabled = [imageObj]() {imageObj->SetSelfActive(true); };
    debugComp->onDebugDisabled = [imageObj]() {imageObj->SetSelfActive(false); };
    debugComp->onUpdate = [collider, image]()
    {
        image->width = collider->GetWidth();
        image->height = collider->GetHeight();
    };
    image->color = color;
    image->border = border;
    image->filled = filled;
#endif
}
void DebugObject::CreateDebugCircleImage(GameObject* parent, double radius, D2D1::ColorF color, double border, bool filled)
{
#ifdef _MAPTOOL
    auto imageObj = parent->GetScene()->AddGameObject(parent);
    auto debugComp = imageObj->AddComponent<DebugObject>();
    auto image = imageObj->AddComponent<D2DCircle>();
    imageObj->GetTransform()->position = Vector3d::zero;
    debugComp->onDebugEnabled = [imageObj]() {imageObj->SetSelfActive(true); };
    debugComp->onDebugDisabled = [imageObj]() {imageObj->SetSelfActive(false); };
    image->radius = radius;
    image->color = color;
    image->border = border;
    image->filled = filled;
#endif
}
void DebugObject::CreateDebugText(GameObject* parent, function<wstring()> stringFunctor, Vector3d relativePosition, double fontSize, D2D1::ColorF color)
{
#ifdef _MAPTOOL
    auto panelObj = parent->GetScene()->AddGameObject(parent);
    auto debugComp = panelObj->AddComponent<DebugObject>();
    auto panel = panelObj->AddComponent<D2DRectangle>();
    debugComp->onDebugEnabled = [panelObj]() {panelObj->SetSelfActive(true); };
    debugComp->onDebugDisabled = [panelObj]() {panelObj->SetSelfActive(false); };
    panel->width = fontSize * 10;
    panel->height = fontSize * 2;
    panel->color = D2D1::ColorF(1 - color.r, 1 - color.g, 1 - color.b, 1);
    panel->filled = true;
    panelObj->GetTransform()->position = relativePosition;
    auto textObj = parent->GetScene()->AddGameObject(panelObj);
    auto textComp = panelObj->AddGameObject()->AddComponent<D2DText>();
    textComp->color = color;
    textComp->fontSize = fontSize;
    textComp->width = panel->width;
    textComp->height = panel->height;
    debugComp->onUpdate = [=]() {textComp->text = stringFunctor(); };
#endif
}
void DebugObject::CreateArrow(GameObject* parent, Vector3d origin, Vector3d destination, double width, D2D1::ColorF color)
{
#ifdef _MAPTOOL
    const double lineLength = (destination - origin).Magnitude();
    GameObject* line;
    DebugObject* debugComp;
    D2DRectangle* image;
    GameObject* arrowLeftHead;
    GameObject* arrowRightHead;
    tie(line, debugComp, image, arrowLeftHead, arrowRightHead) = DebugObject::_CreateArrow(lineLength, width, color);
    debugComp->onDebugEnabled = [line]() {line->SetSelfActive(true); };
    debugComp->onDebugDisabled = [line]() {line->SetSelfActive(false); };
    line->SetParent(parent);
    line->GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(destination - origin).GetAngleDegree()));
    line->GetTransform()->SetWorldPosition(Vector3d::Lerp(origin, destination, 0.5));
#endif
}
void DebugObject::CreateArrow(GameObject* origin, GameObject* destination, double width, D2D1::ColorF color)
{
#ifdef _MAPTOOL
    GameObject* line;
    DebugObject* debugComp;
    D2DRectangle* image;
    GameObject* arrowLeftHead;
    GameObject* arrowRightHead;
    tie(line, debugComp, image, arrowLeftHead, arrowRightHead) = DebugObject::_CreateArrow(0, width, color);
    line->SetParent(origin);
    debugComp->onDebugEnabled = [line]() {line->SetSelfActive(true); };
    debugComp->onDebugDisabled = [line]() {line->SetSelfActive(false); };
    debugComp->onUpdate = [=]() {
        constexpr double arrowHeadAngle = 120;
        const double arrowWidth = width * 8.0 * sqrt(3) / 3;
        const double arrowHeight = width * 0.7;
        const double arrowDiameter = Vector2d(arrowWidth, arrowHeight).Magnitude();
        const double arrowInnerAngle = atan2(arrowHeight, arrowWidth);
        const double length = Vector2d(-origin->GetTransform()->GetWorldPosition() + destination->GetTransform()->GetWorldPosition()).Magnitude();
        line->GetTransform()->SetWorldPosition(Vector3d::Lerp(origin->GetTransform()->GetWorldPosition(), destination->GetTransform()->GetWorldPosition(), 0.5));
        line->GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(-origin->GetTransform()->GetWorldPosition() + destination->GetTransform()->GetWorldPosition()).GetAngleDegree()));
        image->width = length;
        arrowLeftHead->GetTransform()->position.x = arrowRightHead->GetTransform()->position.x = length * 0.5 + arrowDiameter * 0.5 * cos(arrowInnerAngle + arrowHeadAngle * YunutyMath::Deg2Rad);
    };
#endif
}
tuple<GameObject*, DebugObject*, D2DRectangle*, GameObject*, GameObject*> DebugObject::_CreateArrow(double lineLength, double width, D2D1::ColorF color)
{
    auto line = Scene::getCurrentScene()->AddGameObject();
    auto debugComp = line->AddComponent<DebugObject>();
    auto image = line->AddComponent<D2DRectangle>();
    image->width = lineLength;
    image->height = width;
    image->color = color;
    image->border = 0;
    image->filled = true;

    const double arrowHeadAngle = 120;
    const double arrowWidth = width * 8.0 * sqrt(3) / 3;
    const double arrowHeight = width * 0.7;
    const double arrowDiameter = Vector2d(arrowWidth, arrowHeight).Magnitude();
    const double arrowInnerAngle = atan2(arrowHeight, arrowWidth);

    auto leftArrowHead = Scene::getCurrentScene()->AddGameObject(line);
    auto laImage = leftArrowHead->AddComponent<D2DRectangle>();
    laImage->width = arrowWidth;
    laImage->height = arrowHeight;
    laImage->color = color;
    laImage->border = 0;
    laImage->filled = true;
    laImage->GetTransform()->rotation = Vector3d(0, 0, arrowHeadAngle);
    laImage->GetTransform()->position = Vector3d(lineLength * 0.5 + arrowDiameter * 0.5 * cos(arrowInnerAngle + arrowHeadAngle * YunutyMath::Deg2Rad), arrowDiameter * 0.5 * sin(arrowInnerAngle + arrowHeadAngle * YunutyMath::Deg2Rad), 0);

    auto rightArrowHead = Scene::getCurrentScene()->AddGameObject(line);
    auto raImage = rightArrowHead->AddComponent<D2DRectangle>();
    raImage->width = arrowWidth;
    raImage->height = arrowHeight;
    raImage->color = color;
    raImage->border = 0;
    raImage->filled = true;
    raImage->GetTransform()->rotation = Vector3d(0, 0, -arrowHeadAngle);
    raImage->GetTransform()->position = Vector3d(lineLength * 0.5 + arrowDiameter * 0.5 * cos(arrowInnerAngle + arrowHeadAngle * YunutyMath::Deg2Rad), -arrowDiameter * 0.5 * sin(arrowInnerAngle + arrowHeadAngle * YunutyMath::Deg2Rad), 0);

    return { line, debugComp, image ,leftArrowHead,rightArrowHead };
}
void DebugObject::_CreatePopUpCircle(GameObject* parent, Vector3d position, double radius, double duration, D2D1::ColorF color)
{
#ifdef _MAPTOOL;
    struct UpdateFunctor
    {
        Timer resizingTimer;
        Transform* const targetTransform = nullptr;
        GameObject* const targetGameObject = nullptr;
        UpdateFunctor(double duration, Transform* transform) :targetTransform(transform), targetGameObject(transform->GetGameObject())
        {
            resizingTimer.duration = duration;
            resizingTimer.Start();
            resizingTimer.onUpdate = [this](double progress) {
                targetTransform->scale = Vector3d::one * (1 - 4 * (0.5 - progress) * (0.5 - progress));
            };
            resizingTimer.onExpiration = [this]() {
                targetTransform->GetGameObject()->GetScene()->DestroyGameObject(targetTransform->GetGameObject());
            };
        }
        void operator()() {
            resizingTimer.Update();
        }
    };
    auto circleObj = Scene::getCurrentScene()->AddGameObject(parent);
    auto circleImage = circleObj->AddComponent<D2DCircle>();
    circleObj->GetTransform()->SetWorldPosition(position);
    circleImage->radius = radius;
    circleImage->color = color;
    circleImage->filled = true;
    auto debugComp = circleObj->AddComponent<DebugObject>();
    debugComp->onDebugEnabled = [circleObj]() {circleObj->SetSelfActive(true); };
    debugComp->onDebugDisabled = [circleObj]() {circleObj->SetSelfActive(false); };
    auto functor = new UpdateFunctor(duration, circleObj->GetTransform());
    debugComp->onUpdate = [functor]() {(*functor)(); };
    debugComp->onDestory = [functor]() {delete functor; };
#endif
}
void DebugObject::CreatePopUpCircle(GameObject* parent, double radius, double duration, D2D1::ColorF color)
{
    _CreatePopUpCircle(parent, parent->GetTransform()->GetWorldPosition(), radius, duration, color);
}
void DebugObject::CreatePopUpCircle(Vector3d position, double radius, double duration, D2D1::ColorF color)
{
    _CreatePopUpCircle(nullptr, position, radius, duration, color);
}
void DebugObject::Update()
{
#ifdef _MAPTOOL
    if (debugMode)
        onUpdate();
#endif
}
void DebugObject::OnDestroy()
{
#ifdef _MAPTOOL
    if (debugMode)
        onDestory();
#endif
}
