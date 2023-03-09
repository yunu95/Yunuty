#include "Guardian_shooter.h"
#include "ItemMarker.h"

using namespace gs_map;

ItemMarker* ItemMarker::selectedItemMarker = nullptr;
vector<ItemMarker*> ItemMarker::itemMarkers;

ItemMarker::ItemMarker()
{
    this->markerIndex = itemMarkers.size(); itemMarkers.push_back(this);
}
ItemMarker::~ItemMarker()
{
    size_t size = itemMarkers.size();
    if (size == 0)
    {
        return;
    }
    for (int i = this->markerIndex; i < size - 1; i++)
    {
        itemMarkers[i] = itemMarkers[i + 1];
        itemMarkers[i]->markerIndex--;
    }
    itemMarkers.resize(size - 1);
}

void ItemMarker::Save(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"itemMarkers.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);
    //if (file.fail())
    //	_wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

    for (int i = 0; i < itemMarkers.size(); i++)
    {
        auto each = itemMarkers[i];
        file << (int)each->itemType << " ";
        file << each->GetTransform()->GetWorldPosition().x << ' ';
        file << each->GetTransform()->GetWorldPosition().y << ' ';
        file << i << ' ';
        file << "\n";   // 첫 줄 생략 구조 변경
    }
    file.close();
}

void ItemMarker::Load(wstring mapRoot)
{
    fstream file;
    for (auto each : itemMarkers)
    {
        ItemMarker::DeleteItemMarker(each);
    }
    const wstring filename = L"itemMarkers.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    vector < tuple<int, double, double, int >> infos;
    string line;
    while (getline(file, line))
    {
        stringstream sstream(line);
        int itemType;
        double x;
        double y;
        int index;
        sstream >> itemType;
        sstream >> x;
        sstream >> y;
        sstream >> index;
        infos.push_back(tie(itemType, x, y, index));
    }

    for (const auto& each : infos)
    {
        int itemType;
        double x;
        double y;
        int index;
        tie(itemType, x, y, index) = each;
        CreateItemMarker(Vector2d(x, y), (ItemType)itemType)->markerIndex = index;
    }
    file.close();
}

ItemMarker* ItemMarker::CreateItemMarker(Vector2d location, ItemType itemType)
{
    auto obj = MapTool::GetInstance()->GetItemLayer()->AddGameObject();
    obj->GetTransform()->SetWorldPosition(location);
    auto itemMarker = obj->AddComponent<ItemMarker>();
    auto imageObj = Scene::getCurrentScene()->AddGameObject(obj);
    itemMarker->markerSprite = imageObj->AddComponent<D2DAnimatedSprite>();
    obj->AddComponent<CircleCollider2D>()->SetRadius(1);
    DebugObject::CreateDebugRectImage(imageObj, 100, 100, D2D1::ColorF::Pink);
    switch (itemType)
    {
    case ItemType::HealPack:
        itemMarker->markerSprite->LoadAnimationFromFile(L"sprites/Item/healpack");
        itemMarker->itemType = itemType;
        break;
    case ItemType::AttackSpeedUp:
        itemMarker->markerSprite->LoadAnimationFromFile(L"sprites/Item/forcepack");
        itemMarker->itemType = itemType;
        break;
    default:
        break;
    }

#ifdef _MAPTOOL
    auto dragBtn = Button::AddDraggableButton(obj, 50);
    dragBtn->onClick = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Item)
            return;
        ItemMarker::selectedItemMarker = itemMarker;
    };
    dragBtn->onDrag = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Item)
            return;
        dragBtn->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
    };
#endif

    return itemMarker;
}

void ItemMarker::Update()
{

}

void ItemMarker::OnCollisionEnter2D(const Collision2D& collision)
{
    if (!MapTool::GetInstance()->isEditingMap() && !alreadySummoned)
    {
        Invoke();
    }
}

void ItemMarker::OnCollisionStay2D(const Collision2D& collision)
{
    // 시작부터 카메라 위치에 Marker가 있는 경우
    // 굳이 해당 함수를 작성하지 않고 카메라 밖에서 아이템을 배치하거나,
    // 시작 화면 내부에 위치할 Item 만 하드코딩으로 배치하는 등의 방법을 사용하는 것이 좋아 보임
    if (!MapTool::GetInstance()->isEditingMap() && !alreadySummoned)
    {
        Invoke();
    }
}

// opacity가 0이하면 완전히 사라진다.
void ItemMarker::SetOpacity(double opacity)
{
    if (opacity <= 0)
    {
        GetGameObject()->SetSelfActive(false);
    }
    else
    {
        if (!GetGameObject()->GetSelfActive())
            DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition(), 70, 0.4, D2D1::ColorF::Red);
        GetGameObject()->SetSelfActive(true);
        if (markerSprite)
            markerSprite->color.a = opacity;
    }
}

void ItemMarker::Invoke()
{
    alreadySummoned = true;
    switch (itemType)
    {
    case ItemType::HealPack:
        Item::CreateItem(GetTransform()->GetWorldPosition(), itemType);
        break;
    case ItemType::AttackSpeedUp:
        Item::CreateItem(GetTransform()->GetWorldPosition(), itemType);
        break;
    default:
        break;
    }
    DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition());
}
void ItemMarker::OnRestart()
{
    alreadySummoned = false;
    markerSprite->GetGameObject()->SetSelfActive(false);
}

void ItemMarker::OnEngagingMapEditMode()
{
    markerSprite->GetGameObject()->SetSelfActive(true);
}
void ItemMarker::OnDisengagingMapEditMode()
{
    markerSprite->GetGameObject()->SetSelfActive(false);
}

void ItemMarker::DeleteItemMarker(ItemMarker* marker)
{
    Scene::getCurrentScene()->DestroyGameObject(marker->GetGameObject());
    if (selectedItemMarker == marker)
        selectedItemMarker = nullptr;
}

ifstream ItemMarker::fin = ifstream();
ofstream ItemMarker::fout = ofstream();


