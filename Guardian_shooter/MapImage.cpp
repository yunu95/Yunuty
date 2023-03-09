#include "Guardian_shooter.h"
#include "MapImage.h"

using namespace gs_map;
unordered_set<MapImage*> MapImage::mapImages;
D2DRectangle* MapImage::selectedRectangle = nullptr;
MapImage* MapImage::selectedImage = nullptr;
Vector3d MapImage::selectedDeltaPosition = Vector3d::zero;
void MapImage::Save(wstring mapRoot)
{
    wfstream file;
    const wstring filename = L"images.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);
    //if (file.fail())
    //	_wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

    for (auto each : mapImages)
    {
        file << (int)each->layerType << " ";
        file << each->sprite->GetGameObject()->GetChildIndex() << " ";
        file << each->sprite->GetSpriteFilePath() << " ";
        file << each->GetTransform()->scale.x << ' ';
        file << each->GetTransform()->scale.y << ' ';
        file << each->GetTransform()->rotation.Euler().z << ' ';
        file << each->scrollRate << ' ';
        file << each->scrollConstant << ' ';
        file << each->GetTransform()->position.y << ' ';
        file << "\n";   // 첫 줄 생략 구조 변경
    }
    file.close();
}
void MapImage::Load(wstring mapRoot)
{
    unordered_set<double> scrollConstantPool;
    unordered_set<double> yCoordPool;
    struct Info
    {
        int layerType;
        int childIndex;
        wstring spritePath;
        double scaleX;
        double scaleY;
        double eulerZ;
        double scrollRate;
        double scrollConstant;
        double positionY;
    };
    wfstream file;
    for (auto each : mapImages)
    {
        MapImage::DeleteMapImage(each);
    }
    const wstring filename = L"images.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    vector <Info> infos;
    wstring line;
    while (getline(file, line))
    {
        wstringstream sstream(line);
        Info info;
        sstream >> info.layerType;
        sstream >> info.childIndex;
        sstream >> info.spritePath;
        sstream >> info.scaleX;
        sstream >> info.scaleY;
        sstream >> info.eulerZ;
        sstream >> info.scrollRate;
        sstream >> info.scrollConstant;
        sstream >> info.positionY;
        infos.push_back(info);
    }
    sort(infos.begin(), infos.end(), [](Info& a, Info& b) {return a.childIndex < b.childIndex; });

    for (const auto& info : infos)
    {
        if (yCoordPool.find(info.positionY) != yCoordPool.end())
            if (scrollConstantPool.find(info.scrollConstant) != scrollConstantPool.end())
                continue;
        yCoordPool.insert(info.positionY);
        scrollConstantPool.insert(info.scrollConstant);
        auto img = CreateMapImage(Vector2d::zero, (LayerType)info.layerType);
        img->sprite->SetSpriteFilePath(info.spritePath);
        img->GetTransform()->scale = Vector2d(info.scaleX, info.scaleY);
        img->GetTransform()->rotation = Vector3d(0, 0, info.eulerZ);
        img->scrollRate = info.scrollRate;
        img->scrollConstant = info.scrollConstant;
        img->imageScroller->scrollRate = info.scrollRate;
        img->imageScroller->scrollConstant = info.scrollConstant;
        img->GetTransform()->position.y = info.positionY;
        img->ApplyScrollLogic();
    }
    file.close();
}
MapImage* MapImage::CreateMapImage(Vector2d location, LayerType layerType)
{
    GameObject* imageLayer;
    GameObject* editorLayer;
    switch (layerType)
    {
    case LayerType::BackCharacters:
        imageLayer = MapTool::GetInstance()->GetBackgroundObjectsLayer();
        editorLayer = MapTool::GetInstance()->GetEditorBackgroundLayer();
        break;
    case LayerType::ForeCharacters:
        imageLayer = MapTool::GetInstance()->GetForegroundObjectsLayer();
        editorLayer = MapTool::GetInstance()->GetEditorForegroundLayer();
        break;
    case LayerType::BehindBackground:
    default:
        imageLayer = MapTool::GetInstance()->GetBehindBackgroundLayer();
        editorLayer = MapTool::GetInstance()->GetEditorBackgroundLayer();
        break;
    }
    auto imageObj = imageLayer->AddGameObject();
    auto imageComp = imageObj->AddComponent<D2DSprite>();
    imageComp->SetSpriteFilePath(L"sprites/no_image.png");
    auto imageScrollerComp = imageObj->AddComponent<ImageScroller>();

    auto editorImageObj = editorLayer->AddGameObject();
    auto editorImageComp = editorImageObj->AddComponent<MapImage>();
    editorImageComp->sprite = imageComp;
    editorImageComp->imageScroller = imageScrollerComp;
    editorImageComp->layerType = layerType;
    imageObj->GetTransform()->SetWorldPosition(location);

    editorImageObj->GetTransform()->SetWorldPosition(location);
    BoxCollider2D* boxCollider;
#ifdef _MAPTOOL
    auto button = Button::AddDraggableButton(editorImageObj, imageComp->GetDrawRect().width, imageComp->GetDrawRect().height, &boxCollider);
    editorImageComp->boxCollider = boxCollider;
    button->onDrag = [=]()
    {
        button->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition() + MapImage::selectedDeltaPosition);
        imageObj->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition() + MapImage::selectedDeltaPosition);
        editorImageComp->SetScrollRate(editorImageComp->scrollRate);
    };
    button->onClick = [=]()
    {

        MapImage::selectedImage = editorImageComp;
        MapImage::selectedDeltaPosition = -D2DInput::getMouseWorldPosition() + imageObj->GetTransform()->GetWorldPosition();
        MapImage::selectedRectangle->GetGameObject()->SetSelfActive(true);
        MapImage::selectedRectangle->GetGameObject()->SetParent(editorImageObj);
        MapImage::selectedRectangle->GetTransform()->position = Vector3d::zero;
        MapImage::selectedRectangle->width = boxCollider->GetWidth();
        MapImage::selectedRectangle->height = boxCollider->GetHeight();
        MapImage::selectedRectangle->GetTransform()->SetWorldPosition(imageObj->GetTransform()->GetWorldPosition());
        MapImage::selectedRectangle->GetTransform()->SetWorldRotation(imageObj->GetTransform()->GetWorldRotation());
    };
#endif
    return editorImageComp;
}
MapImage* MapImage::CreateMapImage(Vector2d location, MapImage* original)
{
    auto editorImageComp = CreateMapImage(location, original->layerType);
    editorImageComp->sprite->SetSpriteFilePath(original->sprite->GetSpriteFilePath());
    editorImageComp->GetTransform()->scale = original->GetTransform()->scale;
    editorImageComp->GetTransform()->rotation = original->GetTransform()->rotation;
    editorImageComp->PropagateTransform();

    return editorImageComp;
}
void MapImage::DeleteMapImage(MapImage* image)
{
#ifdef _MAPTOOL
    MapImage::selectedRectangle->GetGameObject()->SetSelfActive(false);
    MapImage::selectedRectangle->GetGameObject()->SetParent(Scene::getCurrentScene());
#endif
    Scene::getCurrentScene()->DestroyGameObject(image->GetGameObject());
    Scene::getCurrentScene()->DestroyGameObject(image->sprite->GetGameObject());
    if (selectedImage == image)
        selectedImage = nullptr;
}
void MapImage::SetLayerType(LayerType layerType)
{
    this->layerType = layerType;
    switch (layerType)
    {
    case gs_map::MapImage::LayerType::ForeCharacters:
        sprite->GetGameObject()->SetParent(MapTool::GetInstance()->GetForegroundObjectsLayer());
        GetGameObject()->SetParent(MapTool::GetInstance()->GetEditorForegroundLayer());
        break;
    case gs_map::MapImage::LayerType::BackCharacters:
        sprite->GetGameObject()->SetParent(MapTool::GetInstance()->GetBackgroundObjectsLayer());
        GetGameObject()->SetParent(MapTool::GetInstance()->GetEditorBackgroundLayer());
        break;
    case gs_map::MapImage::LayerType::BehindBackground:
        sprite->GetGameObject()->SetParent(MapTool::GetInstance()->GetBehindBackgroundLayer());
        GetGameObject()->SetParent(MapTool::GetInstance()->GetEditorBackgroundLayer());
        break;
    default:
        break;
    }
}
void MapImage::SetScrollRate(double scrollRate)
{
    scrollConstant = GetTransform()->position.x - GSCamera::GetInstance()->GetTransform()->position.x * (1 - scrollRate);
    this->scrollRate = scrollRate;
    imageScroller->scrollConstant = scrollConstant;
    imageScroller->scrollRate = scrollRate;
}
void MapImage::PlaceTop()
{
    GetGameObject()->SetChildIndex(99999);
    sprite->GetGameObject()->SetChildIndex(99999);
}
void MapImage::PlaceBottom()
{
    GetGameObject()->SetChildIndex(0);
    sprite->GetGameObject()->SetChildIndex(0);
}
void MapImage::Update()
{
    if (scrollRate != 1)
    {
        ApplyScrollLogic();
    }
}
void MapImage::PropagateTransform()
{
    sprite->GetTransform()->position = GetTransform()->position;
    sprite->GetTransform()->rotation = GetTransform()->rotation;
    sprite->GetTransform()->scale = GetTransform()->scale;
#ifdef _MAPTOOL
    boxCollider->SetWidth(sprite->GetDrawRect().width);
    boxCollider->SetHeight(sprite->GetDrawRect().height);
#endif
}
void MapImage::ApplyScrollLogic()
{
    auto pos = GetTransform()->position;
    pos.x = scrollConstant + GSCamera::GetInstance()->GetTransform()->position.x * (1 - scrollRate);
    GetTransform()->position = pos;
    PropagateTransform();
}
