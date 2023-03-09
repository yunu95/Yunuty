#include "Guardian_shooter.h"
#include "EnemyWave.h"

using namespace gs_map;
EnemyWave* EnemyWave::selectedWave = nullptr;
unordered_set<EnemyWave*> EnemyWave::waves;
EnemyWave* EnemyWave::CreateEnemyWave(Vector3d position)
{
    auto obj = MapTool::GetInstance()->GetEnemyLayer()->AddGameObject();
    obj->GetTransform()->SetWorldPosition(position);

    auto enemyWave = obj->AddComponent<EnemyWave>();
    DebugObject::CreateDebugRectImage(obj, 30, 30, D2D1::ColorF::Red, 1, true);
    DebugObject::CreateDebugRectImage(obj, 1920, 1080);
    DebugObject::CreateDebugText(obj, [enemyWave] {return to_wstring(enemyWave->waveTimer.elapsed) + L"seconds"; }, Vector2d(0, 400));
    enemyWave->eventCollider = enemyWave->GetGameObject()->AddComponent<BoxCollider2D>();
    enemyWave->eventCollider->SetHeight(1080);
    enemyWave->eventCollider->SetWidth(1920);

#ifdef _MAPTOOL
    DebugObject::CreateDebugText(obj, [=]() {
        wstringstream wsstream;
        wsstream << enemyWave->passRequirementsLeft.size();
        return wsstream.str();
        }, Vector3d(0, 400, 0));
    enemyWave->uiButton = Button::CreateToggleButton(MapTool::GetInstance()->GetEditorUnitsLayer(), enemyWave->GetButtonPosition(), L"wave");
    enemyWave->uiButton->deselectOnESC = true;
    enemyWave->uiButton->onSelect = [enemyWave]()
    {
        EnemyWave::selectedWave = enemyWave;
    };
    enemyWave->uiButton->onDrag = [=]()
    {
        enemyWave->Relocate(D2DInput::getMouseWorldPosition() - Vector3d(0, 700, 0));
    };
    enemyWave->uiButton->onSelect = [enemyWave]()
    {
        EnemyWave::selectedWave = enemyWave;
    };
    enemyWave->uiButton->onDeselect = [=]()
    {
        if (EnemyWave::selectedWave == enemyWave)
            EnemyWave::selectedWave = nullptr;
    };
#endif
    enemyWave->Relocate(position);
    return enemyWave;
}
void gs_map::EnemyWave::DeleteWave(EnemyWave* wave)
{
    if (EnemyWave::selectedWave == wave)
        EnemyWave::selectedWave = nullptr;
    Scene::getCurrentScene()->DestroyGameObject(wave->GetGameObject());
#if _MAPTOOL
    Scene::getCurrentScene()->DestroyGameObject(wave->uiButton->GetGameObject());
#endif
    for (auto each : wave->enemyMarkers)
    {
        each.second->enemyWave = nullptr;
        EnemyMarker::DeleteEnemyMarker(each.second);
    }
}
void gs_map::EnemyWave::Save(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"enemyWaves.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);

    for (auto eachWave : waves)
    {
        file << eachWave->GetTransform()->position.x << " ";
        file << eachWave->GetTransform()->position.y << " ";
        file << (int)eachWave->enemyMarkers.size() << " ";
        file << (int)eachWave->passRequirements.size() << " ";
        for (auto eachMarker : eachWave->enemyMarkers)
        {
            file << eachMarker.first << " ";
            file << eachMarker.second->markerIndex << " ";
        }
        for (auto eachMarker : eachWave->passRequirements)
        {
            file << eachMarker->markerIndex << " ";
        }
        file << "\n";   // 첫 줄 생략 구조 변경
    }
    file.close();
}
void gs_map::EnemyWave::Load(wstring mapRoot)
{
    fstream file;
    for (auto each : waves)
    {
        EnemyWave::DeleteWave(each);
    }
    const wstring filename = L"enemyWaves.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    string line;
    while (getline(file, line))
    {
        stringstream sstream(line);
        double x;
        double y;
        int markerSize;
        int requirementsSize;
        sstream >> x;
        sstream >> y;
        sstream >> markerSize;
        sstream >> requirementsSize;

        auto wave = EnemyWave::CreateEnemyWave(Vector3d(x, y, 0));
        queue<double> offsets;
        queue<int> indexQueue;
        queue<int> requirementsIndexQueue;
        for (int i = 0; i < markerSize; i++)
        {
            double offset;
            int index;
            sstream >> offset;
            sstream >> index;
            offsets.push(offset);
            indexQueue.push(index);
        }
        for (int i = 0; i < requirementsSize; i++)
        {
            int index;
            sstream >> index;
            requirementsIndexQueue.push(index);
        }
        while (!indexQueue.empty())
        {
            auto index = indexQueue.front();
            auto offset = offsets.front();
            indexQueue.pop();
            offsets.pop();
            wave->waveTimer.elapsed = offset;
            wave->InsertMarker(EnemyMarker::enemyMarkers[index]);
        }
        while (!requirementsIndexQueue.empty())
        {
            auto index = requirementsIndexQueue.front();
            requirementsIndexQueue.pop();
            wave->ToggleMarkerEssential(EnemyMarker::enemyMarkers[index]);
        }

        //infos.push_back(tie(enemyType, x, y, index));
    }
    file.close();
}
void gs_map::EnemyWave::Relocate(Vector3d position)
{
    //GetTransform()->SetWorldPosition(CameraRail::GetNearestPos(position));
    GetTransform()->SetWorldPosition(position);
#ifdef _MAPTOOL
    uiButton->GetTransform()->SetWorldPosition(GetButtonPosition());
#endif
}
void gs_map::EnemyWave::InsertMarker(EnemyMarker* marker)
{
    marker->enemyWave = this;
    marker->summonOnTouch = false;
    enemyMarkers.push_back(make_pair(waveTimer.elapsed, marker));
#ifdef _MAPTOOL
    markersArrow[marker] = uiButton->GetGameObject()->AddGameObject();
#endif
    DebugObject::CreateArrow(markersArrow[marker], marker->GetGameObject(), 2, D2D1::ColorF::DarkCyan);
}
void gs_map::EnemyWave::ToggleMarkerEssential(EnemyMarker* marker)
{
    if (marker->enemyWave != this)
        return;

    if (passRequirements.find(marker) == passRequirements.end())
        // 추가해야됨
    {
        passRequirements.insert(marker);
#ifdef _MAPTOOL
        requirementsArrows[marker] = uiButton->GetGameObject()->AddGameObject();
#endif
        DebugObject::CreateArrow(requirementsArrows[marker], marker->GetGameObject(), 4, D2D1::ColorF::DarkRed);
    }
    // 없애야됨
    else
    {
        passRequirements.erase(marker);
        Scene::getCurrentScene()->DestroyGameObject(requirementsArrows[marker]);
        requirementsArrows.erase(marker);
    }
}
void gs_map::EnemyWave::HandleDeathEvent(Threat* threat)
{
    if (passRequirementsLeft.find(threat->birthMother) != passRequirementsLeft.end())
        passRequirementsLeft.erase(threat->birthMother);
    if (passRequirementsLeft.empty()&&!waveEnded)
    {
        waveEnded = true;
        GSCamera::GetInstance()->UnLockPosition();
    }
}
void gs_map::EnemyWave::HandleMarkerDeletion(EnemyMarker* deletedMarker)
{
    auto markerItr = find_if(enemyMarkers.begin(), enemyMarkers.end(), [=](pair<double, EnemyMarker*> marker) {return marker.second == deletedMarker; });
    if (passRequirements.find(deletedMarker) != passRequirements.end())
        ToggleMarkerEssential(deletedMarker);

    Scene::getCurrentScene()->DestroyGameObject(markersArrow[markerItr->second]);
    markersArrow.erase(markerItr->second);

    enemyMarkers.erase(markerItr);
    //remove_if(enemyMarkers.begin(), enemyMarkers.end(), [=](const pair<double, EnemyMarker*>& marker) {return marker.second == deletedMarker; });
    //remove_if(enemyMarkers.begin(), enemyMarkers.end(), markerItr);
}
EnemyWave::EnemyWave()
{
    waves.insert(this);
}
EnemyWave::~EnemyWave()
{
    waves.erase(this);
    for (auto each : enemyMarkers)
        each.second->enemyWave = nullptr;
}
void gs_map::EnemyWave::Start()
{
    waveTimer.onUpdate = [&](double progress)
    {
        if (!MapTool::GetInstance()->isEditingMap())
        {
            while (!enemyMarkersQueue.empty() && enemyMarkersQueue.top().first <= waveTimer.elapsed)
            {
                auto top = enemyMarkersQueue.top().second;
                enemyMarkersQueue.pop();
                top->Invoke();
            }
        }
    };
}
void gs_map::EnemyWave::Update()
{
    waveTimer.Update();
    if (isInMapEditor() && EnemyWave::selectedWave == this)
    {
        int timeRateIndex = -1;
        constexpr double timeEditRate[10] = { 1,2,4,8,16 ,-1,-2,-4,-8,-16 };
        if (D2DInput::isKeyDown(KeyCode::NUM_1))
            timeRateIndex = 0;
        if (D2DInput::isKeyDown(KeyCode::NUM_2))
            timeRateIndex = 1;
        if (D2DInput::isKeyDown(KeyCode::NUM_3))
            timeRateIndex = 2;
        if (D2DInput::isKeyDown(KeyCode::NUM_4))
            timeRateIndex = 3;
        if (D2DInput::isKeyDown(KeyCode::NUM_5))
            timeRateIndex = 4;
        if (D2DInput::isKeyDown(KeyCode::NUMPAD_1))
            timeRateIndex = 5;
        if (D2DInput::isKeyDown(KeyCode::NUMPAD_2))
            timeRateIndex = 6;
        if (D2DInput::isKeyDown(KeyCode::NUMPAD_3))
            timeRateIndex = 7;
        if (D2DInput::isKeyDown(KeyCode::NUMPAD_4))
            timeRateIndex = 8;
        if (D2DInput::isKeyDown(KeyCode::NUMPAD_5))
            timeRateIndex = 9;

        if (timeRateIndex != -1)
        {
            waveTimer.elapsed += Time::GetDeltaTimeUnscaled() * timeEditRate[timeRateIndex];
            if (waveTimer.elapsed < 0)
                waveTimer.elapsed = 0;
            else if (waveTimer.elapsed > 1200)
                waveTimer.elapsed = 1200;

            for (auto each : enemyMarkers)
            {
                constexpr double fadingTime = 5;
                each.second->SetOpacity(each.first <= waveTimer.elapsed ? (fadingTime - (waveTimer.elapsed - each.first)) / fadingTime : 0);
            }
        }
    }
}
void gs_map::EnemyWave::OnCollisionEnter2D(const Collision2D& collision)
{
    auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
    if (player && !invokedBefore)
        Invoke();
}
void gs_map::EnemyWave::OnRestart()
{
    waveTimer.isActive = false;
    waveEnded = false;
    invokedBefore = false;
    passRequirementsLeft = passRequirements;
    GSCamera::GetInstance()->UnLockPosition();
}
void gs_map::EnemyWave::Invoke()
{
    invokedBefore = true;
    double lastSummonTime = 0;
    for (auto each : enemyMarkers)
    {
        each.second->alreadySummoned = false;
        each.second->summonOnTouch = false;
        if (each.first > lastSummonTime)
            lastSummonTime = each.first;
    }
    if (lastSummonTime >= waveTimer.duration)
        waveTimer.duration = lastSummonTime;
    struct SortingOrder
    {
        bool operator()(pair<double, EnemyMarker*> a, pair<double, EnemyMarker*> b)
        {
            return a.first > b.first;
        }
    };
    waveTimer.Start();
    enemyMarkersQueue = priority_queue<pair<double, EnemyMarker*>, vector<pair<double, EnemyMarker*>>, greater<pair<double, EnemyMarker*>>>(enemyMarkers.begin(), enemyMarkers.end());
    if (!passRequirementsLeft.empty())
        GSCamera::GetInstance()->LockPosition(GetTransform()->GetWorldPosition());
}
void gs_map::EnemyWave::OnEngagingMapEditMode()
{
    for (auto each : enemyMarkers)
    {
        constexpr double fadingTime = 5;
        each.second->SetOpacity(each.first < waveTimer.elapsed ? (fadingTime - (waveTimer.elapsed - each.first)) / fadingTime : 0);
    }
}
Vector3d gs_map::EnemyWave::GetButtonPosition()
{
    return GetTransform()->GetWorldPosition() + Vector3d(0, 700, 0);
}
