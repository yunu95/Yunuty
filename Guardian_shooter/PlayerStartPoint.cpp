#include "Guardian_shooter.h"
#include "MapTool.h"
#include "PlayerStartPoint.h"

using namespace gs_map;
PlayerStartPoint* PlayerStartPoint::instance = nullptr;
void PlayerStartPoint::Save(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"playerStartPoint.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);
	//if (file.fail())
	//	_wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

    if (!instance)
    {
        file.close();
        return;
    }
    file << instance->GetTransform()->GetWorldPosition().x << ' ';
    file << instance->GetTransform()->GetWorldPosition().y << ' ';
    file.close();
}
void PlayerStartPoint::Load(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"playerStartPoint.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    string line;
    getline(file, line);
    stringstream sstream(line);
    double x;
    double y;
    sstream >> x;
    sstream >> y;
    PlayerStartPoint::CreatePlayerStartPoint(Vector2d(x, y));
    file.close();
}

PlayerStartPoint* PlayerStartPoint::CreatePlayerStartPoint(Vector2d location)
{
    if (instance)
    {
        instance->GetTransform()->SetWorldPosition(location);
        return instance;
    }
    auto startPointObj = MapTool::GetInstance()->GetEditorUnitsLayer()->AddGameObject();
    startPointObj->GetTransform()->SetWorldPosition(location);

    PlayerStartPoint* playertStartPoint = startPointObj->AddComponent<PlayerStartPoint>();
    DebugObject::CreateDebugRectImage(startPointObj, 100, 100, D2D1::ColorF::Green);

	auto imageObj = Scene::getCurrentScene()->AddGameObject(startPointObj);
    imageObj->AddComponent<D2DAnimatedSprite>()->LoadAnimationFromFile(L"sprites/player/torso/idle_forward");
#ifdef _MAPTOOL
    auto button = startPointObj->AddComponent<Button>();
#endif
    return playertStartPoint;
}

void PlayerStartPoint::Update()
{

}
void PlayerStartPoint::OnRestart()
{
    Player::CreatePlayer(GetTransform()->GetWorldPosition());
    this->GetGameObject()->SetSelfActive(false);
}
void PlayerStartPoint::OnEngagingMapEditMode()
{
    this->GetGameObject()->SetSelfActive(true);
}
void PlayerStartPoint::OnDisengagingMapEditMode()
{
    this->GetGameObject()->SetSelfActive(false);
}


ifstream PlayerStartPoint::fin = ifstream();
ofstream PlayerStartPoint::fout = ofstream();
