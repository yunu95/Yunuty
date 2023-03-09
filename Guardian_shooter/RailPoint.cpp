#include "Guardian_shooter.h"
#include "MapTool.h"
#include "Button.h"
#include "Cursor.h"
#include "DebugObject.h"
#include "RailPoint.h"
#define _DEBUG

using namespace gs_map;

unordered_set<RailPoint*> RailPoint::railPoints = unordered_set<RailPoint*>();
RailPoint* RailPoint::extendStartPoint = nullptr;
RailPoint* RailPoint::firstPoint = nullptr;

void RailPoint::Save(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"railPoints.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);

    //if (file.fail())
    //	_wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

    vector<RailPoint*> heads;
    for (auto each : railPoints)
    {
        if (!each->previous)
            heads.push_back(each);
    }

    int groupIndex = 0;
    for (const auto& each : heads)
    {
        auto node = each;
        ++groupIndex;
        do
        {
            file << groupIndex << ' ';
            file << node->GetTransform()->GetWorldPosition().x << ' ';
            file << node->GetTransform()->GetWorldPosition().y << ' ';
            file << '\n';
        } while (node = node->next);
    }
    file.close();
}

void RailPoint::Load(wstring mapRoot)
{
    fstream file;
    // 현존하는 모든 RailPoint가 포함된 게임 오브젝트들을 삭제
    for (auto each : railPoints)
    {
        RailPoint::DeleteRailPoint(each);
    }
    railPoints.clear();
    const wstring filename = L"railPoints.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    vector < std::tuple<int, double, double>> infos;
    string line;
    while (getline(file, line))
    {
        stringstream sstream(line);
        int groupIndex;
        double x;
        double y;
        sstream >> groupIndex;
        sstream >> x;
        sstream >> y;
        infos.push_back(tie(groupIndex, x, y));
    }
    int lastGroupIndex = -1;
    for (auto each : infos)
    {
        int groupIndex;
        double x;
        double y;
        tie(groupIndex, x, y) = each;
        CreateRailPoint(Vector2d(x, y), groupIndex != lastGroupIndex ? nullptr : RailPoint::extendStartPoint);
        lastGroupIndex = groupIndex;
    }
    file.close();
}

RailPoint* RailPoint::CreateRailPoint(Vector2d location, RailPoint* previous)
{
    GameObject* railPointObj = MapTool::GetInstance()->GetRailLayer()->AddGameObject();
    railPointObj->GetTransform()->SetWorldPosition(location);

    RailPoint* railPoint = railPointObj->AddComponent<RailPoint>();
    railPoint->previous = previous;
    if (railPoint->previous)
    {
        railPoint->next = previous->next;
        railPoint->previous->next = railPoint;
        CameraRail* newRail = CreateCameraRail(previous, railPoint);
        if (railPoint->previous->beforeRail)
        {
            railPoint->previous->beforeRail->nextRail = newRail;
            newRail->beforeRail = railPoint->previous->beforeRail;
        }
        if (railPoint->next)
        {
            railPoint->next->previous = railPoint;
            railPoint->nextRail = railPoint->next->beforeRail;
            newRail->nextRail = railPoint->nextRail;
            railPoint->nextRail->beforeRail = newRail;
            AdjustRails(railPoint);
        }
    }

    if (railPoints.size() == 1)
    {
        firstPoint = railPoint;
    }

    DebugObject::CreateDebugCircleImage(railPointObj, 10, D2D1::ColorF::Blue, 1, true);
    railPointObj->AddComponent<CircleCollider2D>()->SetRadius(10);
#ifdef _MAPTOOL
    auto dragBtn = Button::AddDraggableButton(railPointObj, 10);
    dragBtn->onClick = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Rail)
            return;
        RailPoint::extendStartPoint = railPoint;
    };
    dragBtn->onDrag = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Rail)
            return;
        dragBtn->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
        AdjustRails(railPoint);
    };
#endif
    return  extendStartPoint = railPoint;
}

void RailPoint::DeleteRailPoint(RailPoint* point)
{
    if (point == firstPoint)
    {
        firstPoint = point->next;
        if (!(firstPoint == nullptr))
            firstPoint->beforeRail = nullptr;
    }
    Scene::getCurrentScene()->DestroyGameObject(point->GetGameObject());
    if (point->beforeRail)
        Scene::getCurrentScene()->DestroyGameObject(point->beforeRail->GetGameObject());
    if (point->nextRail)
        Scene::getCurrentScene()->DestroyGameObject(point->nextRail->GetGameObject());
    auto cameraRail = CreateCameraRail(point->previous, point->next);
    if (point->previous)
    {
        point->previous->next = point->next;
        point->previous->nextRail = cameraRail;
        if (point->previous->beforeRail)
        {
            point->previous->beforeRail->nextRail = cameraRail;
        }
        Scene::getCurrentScene()->DestroyGameObject(point->beforeRail->GetGameObject());
        AdjustRails(point->previous);
    }
    if (point->next)
    {
        point->next->previous = point->previous;
        point->next->beforeRail = cameraRail;
        if (cameraRail)
            cameraRail->nextRail = point->next->nextRail;
        point->next->beforeRail = cameraRail;
        Scene::getCurrentScene()->DestroyGameObject(point->nextRail->GetGameObject());
        AdjustRails(point->next);
    }
    if (RailPoint::extendStartPoint == point)
        RailPoint::extendStartPoint = nullptr;
}

CameraRail* RailPoint::CreateCameraRail(RailPoint* rp1, RailPoint* rp2)
{
    if (!rp1 || !rp2)
        return nullptr;
    Vector2d v1 = rp1->GetTransform()->position;
    Vector2d v2 = rp2->GetTransform()->position;
    CameraRail* cameraRail = CameraRail::CreateCameraRail(v1, v2);
    rp1->nextRail = cameraRail;
    rp2->beforeRail = cameraRail;

    if (rp1 == firstPoint)
    {
        CameraRail::firstRail = cameraRail;
        CameraRail::nowRail = cameraRail;
    }

    return cameraRail;
}

void RailPoint::AdjustRails(RailPoint* railPoint)
{
    if (railPoint->previous)
    {
        Vector2d p1 = railPoint->previous->GetTransform()->GetWorldPosition();
        Vector2d p2 = railPoint->GetTransform()->GetWorldPosition();
        railPoint->beforeRail->GetTransform()->SetWorldPosition((p1 + p2) * 0.5);
        railPoint->beforeRail->railVector = p2 - p1;
#ifdef _DEBUG
        railPoint->beforeRail->startArrow->GetTransform()->SetWorldPosition(p1);
        railPoint->beforeRail->endArrow->GetTransform()->SetWorldPosition(p2);
#endif
    }
    if (railPoint->next)
    {
        Vector2d p1 = railPoint->GetTransform()->GetWorldPosition();
        Vector2d p2 = railPoint->next->GetTransform()->GetWorldPosition();
        railPoint->nextRail->GetTransform()->SetWorldPosition((p1 + p2) * 0.5);
        railPoint->nextRail->startVector = p1;
        railPoint->nextRail->railVector = p2 - p1;
#ifdef _DEBUG
        railPoint->nextRail->startArrow->GetTransform()->SetWorldPosition(p1);
        railPoint->nextRail->endArrow->GetTransform()->SetWorldPosition(p2);
#endif
    }
}
