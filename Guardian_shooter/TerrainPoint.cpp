#include "Guardian_shooter.h"
#include "MapTool.h"
#include "Button.h"
#include "Cursor.h"
#include "TerrainPoint.h"

using namespace gs_map;

unordered_set<TerrainPoint*> TerrainPoint::terrainPoints = unordered_set<TerrainPoint*>();
TerrainPoint* TerrainPoint::extendStartPoint = nullptr;


void TerrainPoint::Save(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"terrainPoints.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);
    //if (file.fail())
    //	_wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
//#endif

    vector<TerrainPoint*> heads;
    for (auto each : terrainPoints)
    {
        if (!each->previous)
            heads.push_back(each);
    }

    int groupIndex = 0;
    for (auto each : heads)
    {
        auto node = each;
        ++groupIndex;
        do
        {
            file << groupIndex << " ";
            file << (node->penetrable ? 1 : 0) << ' ';
            file << node->GetTransform()->GetWorldPosition().x << ' ';
            file << node->GetTransform()->GetWorldPosition().y << ' ';
            file << "\n";   // 첫 줄 생략 구조 변경
        } while (node = node->next);
    }
    file.close();
}
void TerrainPoint::Load(wstring mapRoot)
{
    fstream file;
    // 현존하는 모든 TerrainPoint가 포함된 게임 오브젝트들을 삭제
    for (auto each : terrainPoints)
    {
        TerrainPoint::DeleteTerrainPoint(each);
    }
    const wstring filename = L"terrainPoints.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    vector < tuple<int, int, double, double >> infos;
    string line;
    while (getline(file, line))
    {
        stringstream sstream(line);
        int groupIndex;
        int penetrable;
        double x;
        double y;
        sstream >> groupIndex;
        sstream >> penetrable;
        sstream >> x;
        sstream >> y;
        infos.push_back(tie(groupIndex, penetrable, x, y));
        /*file << groupIndex << ' ';
        file << (node->penetrable ? 1 : 0) << ' ';
        file << node->GetTransform()->GetWorldPosition().x << ' ';
        file << node->GetTransform()->GetWorldPosition().y << ' ';*/
        //file << "\n";
    }
    int lastGroupIndex = -1;
    for (const auto& each : infos)
    {
        int groupIndex;
        int penetrable;
        double x;
        double y;
        tie(groupIndex, penetrable, x, y) = each;
        CreateTerrainPoint(Vector2d(x, y), groupIndex != lastGroupIndex ? nullptr : TerrainPoint::extendStartPoint, penetrable);
        lastGroupIndex = groupIndex;
    }
    file.close();
}
TerrainPoint* TerrainPoint::CreateTerrainPoint(Vector2d location, TerrainPoint* previous, bool penetrable)
{
    //GameObject* tempGameObj = Scene::getCurrentScene()->AddGameObject();
    GameObject* tempGameObj = MapTool::GetInstance()->GetGroundLayer()->AddGameObject();
    tempGameObj->GetTransform()->SetWorldPosition(location);

    TerrainPoint* tempTerrainPoint = tempGameObj->AddComponent<TerrainPoint>();
    tempTerrainPoint->penetrable = penetrable;
    tempTerrainPoint->previous = previous;
    if (tempTerrainPoint->previous)
    {
        tempTerrainPoint->next = previous->next;
        tempTerrainPoint->previous->next = tempTerrainPoint;
        CreateGround(previous, tempTerrainPoint);
        if (tempTerrainPoint->next)
        {
            tempTerrainPoint->next->previous = tempTerrainPoint;
            tempTerrainPoint->rightGround = tempTerrainPoint->next->leftGround;
            AdjustGrounds(tempTerrainPoint);
        }
    }

    DebugObject::CreateDebugCircleImage(tempGameObj, 10, D2D1::ColorF::Red, 1, true);
    tempGameObj->AddComponent<CircleCollider2D>()->SetRadius(10);
#ifdef _MAPTOOL
    auto dragBtn = Button::AddDraggableButton(tempGameObj, 10);
    dragBtn->onClick = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Terrain)
            return;
        TerrainPoint::extendStartPoint = tempTerrainPoint;
    };
    dragBtn->onDrag = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Terrain)
            return;
        dragBtn->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
        AdjustGrounds(tempTerrainPoint);
    };
#endif
    return extendStartPoint = tempTerrainPoint;
}
void TerrainPoint::DeleteTerrainPoint(TerrainPoint* point)
{
    Scene::getCurrentScene()->DestroyGameObject(point->GetGameObject());
    auto ground = CreateGround(point->previous, point->next);
    if (point->previous)
    {
        point->previous->next = point->next;
        point->previous->rightGround = ground;
        Scene::getCurrentScene()->DestroyGameObject(point->leftGround->GetGameObject());
        AdjustGrounds(point->previous);
    }
    if (point->next)
    {
        point->next->previous = point->previous;
        point->next->leftGround = ground;
        Scene::getCurrentScene()->DestroyGameObject(point->rightGround->GetGameObject());
        AdjustGrounds(point->next);
    }
    if (TerrainPoint::extendStartPoint == point)
        TerrainPoint::extendStartPoint = nullptr;
}


Ground* TerrainPoint::CreateGround(TerrainPoint* tp1, TerrainPoint* tp2)
{
    if (!tp1 || !tp2)
        return nullptr;
    Vector2d v1 = tp1->GetTransform()->position;
    Vector2d v2 = tp2->GetTransform()->position;
    Vector2d dirVector = v2 - v1;
    Ground* tempGround = Ground::CreateGround((v1 + dirVector.Normalized() * dirVector.Magnitude() / 2) + Quaternion(Vector3d(0, 0, 90)) * dirVector.Normalized() * -Ground::groundHeight*0.5, dirVector.GetAngleRadian(), dirVector.Magnitude(), tp2->penetrable);	// Ground Default Height 20, so 20 / 2 == 10
    tp1->rightGround = tempGround;
    tp2->leftGround = tempGround;
    return tempGround;
}
void TerrainPoint::AdjustGrounds(TerrainPoint* terrainPoint)
{
    if (terrainPoint->previous)
    {
        Vector2d p1 = terrainPoint->previous->GetTransform()->GetWorldPosition();
        Vector2d p2 = terrainPoint->GetTransform()->GetWorldPosition();
        Vector2d dirVector = p2 - p1;
        terrainPoint->leftGround->GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(-p1 + p2).GetAngleDegree()));
        terrainPoint->leftGround->GetTransform()->SetWorldPosition((p1 + p2) * 0.5 + Quaternion(Vector3d(0, 0, 90)) * dirVector.Normalized() * -Ground::groundHeight*0.5);
        terrainPoint->leftGround->groundCollider->SetWidth(Vector2d(p2 - p1).Magnitude());
    }
    if (terrainPoint->next)
    {
        Vector2d p1 = terrainPoint->GetTransform()->GetWorldPosition();
        Vector2d p2 = terrainPoint->next->GetTransform()->GetWorldPosition();
        Vector2d dirVector = p2 - p1;
        terrainPoint->rightGround->GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(-p1 + p2).GetAngleDegree()));
        terrainPoint->rightGround->GetTransform()->SetWorldPosition((p1 + p2) * 0.5 + Quaternion(Vector3d(0, 0, 90)) * dirVector.Normalized() * -Ground::groundHeight*0.5);
        terrainPoint->rightGround->groundCollider->SetWidth(Vector2d(p2 - p1).Magnitude());
    }
}