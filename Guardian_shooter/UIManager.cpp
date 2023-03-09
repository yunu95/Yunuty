#include "Guardian_shooter.h"
#include "UIManager.h"
#include "Button.h"
#include "Cursor.h"
#include "Player.h"
#include "Boss.h"
#include "MapTool.h"

using namespace UI;

UIManager* UIManager::instance = nullptr;
unordered_map<UIManager::UIEnum, GameObject*> UIManager::UIList = unordered_map<UIManager::UIEnum, GameObject*>();

void UIManager::Update()
{
    Player* player = Player::GetInstance();
    if (!player)
        return;
    if (player->lifeCount == -1)
    {
        player->lifeCount = -2;
        ResetUI(UIEnum::PlayerDeathUI);
        ShowUI(UIEnum::PlayerDeathUI, FadeOption::FadeIn);
    }
}

void UIManager::Initialize()
{
    CreateAllGS_UI();
    InitializeAllGS_UI();
    GS_UI::HideAllUI();
}

void UIManager::ResetUI(UIEnum targetUI)
{
    GS_UI::GS_UIList[targetUI]->ResetThisUI();
}

void UIManager::ShowUI(UIEnum targetUI, GameObject* targetObj)
{
    GS_UI::GS_UIList[targetUI]->ShowThisUI(targetObj);
}

void UIManager::ShowUI(UIEnum targetUI, FadeOption fadeOption, double duration, GameObject* targetObj)
{
    GS_UI::GS_UIList[targetUI]->ShowThisUI(targetObj);
    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.Start();

    if (fadeOption == FadeOption::FadeIn)
    {
        GS_UI::GS_UIList[targetUI]->SetFadeIn(duration);
    }
    else if (fadeOption == FadeOption::FadeOut)
    {
        GS_UI::GS_UIList[targetUI]->SetFadeOut(duration);
    }

    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.onExpiration = [=]() {};
}

void UIManager::ShowUI(UIEnum targetUI, ScaleOption scaleOption, double rate, double duration, GameObject* targetObj)
{
    GS_UI::GS_UIList[targetUI]->ShowThisUI(targetObj);
    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.Start();

    if (scaleOption == ScaleOption::ScaleUp)
    {
        GS_UI::GS_UIList[targetUI]->SetScaleUp(duration, rate);
    }
    else if (scaleOption == ScaleOption::ScaleDown)
    {
        GS_UI::GS_UIList[targetUI]->SetScaleDown(duration, rate);
    }

    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.onExpiration = [=]() {};
}

//void UIManager::ShowUI(UIEnum targetUI, FadeOption fadeOption, ScaleOption scaleOption, double rate, double duration, GameObject* targetObj)
//{
//	GS_UI::GS_UIList[targetUI]->ShowThisUI(targetObj);
//	GS_UI::GS_UIList[targetUI]->uIFunctionTimer.Start();
//
//	if (fadeOption == FadeOption::FadeIn)
//	{
//		GS_UI::GS_UIList[targetUI]->SetFadeIn(duration);
//	}
//	else if (fadeOption == FadeOption::FadeOut)
//	{
//		GS_UI::GS_UIList[targetUI]->SetFadeOut(duration);
//	}
//
//	if (scaleOption == ScaleOption::ScaleUp)
//	{
//		GS_UI::GS_UIList[targetUI]->SetScaleUp(duration, rate);
//	}
//	else if (scaleOption == ScaleOption::ScaleDown)
//	{
//		GS_UI::GS_UIList[targetUI]->SetScaleDown(duration, rate);
//	}
//
//	GS_UI::GS_UIList[targetUI]->uIFunctionTimer.onExpiration = [=]() {};
//}

void UIManager::HideUI(UIEnum targetUI)
{
    GS_UI::GS_UIList[targetUI]->HideThisUI();
}

void UIManager::HideUI(UIEnum targetUI, FadeOption fadeOption, double duration)
{
    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.Start();

    if (fadeOption == FadeOption::FadeIn)
    {
        GS_UI::GS_UIList[targetUI]->SetFadeIn(duration);
    }
    else if (fadeOption == FadeOption::FadeOut)
    {
        GS_UI::GS_UIList[targetUI]->SetFadeOut(duration);
    }

    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.onExpiration = [=]() { GS_UI::GS_UIList[targetUI]->HideThisUI(); };
}

void UIManager::HideUI(UIEnum targetUI, ScaleOption scaleOption, double rate, double duration)
{
    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.Start();

    if (scaleOption == ScaleOption::ScaleUp)
    {
        GS_UI::GS_UIList[targetUI]->SetScaleUp(duration, rate);
    }
    else if (scaleOption == ScaleOption::ScaleDown)
    {
        GS_UI::GS_UIList[targetUI]->SetScaleDown(duration, rate);
    }

    GS_UI::GS_UIList[targetUI]->uIFunctionTimer.onExpiration = [=]() { GS_UI::GS_UIList[targetUI]->HideThisUI(); };
}

//void UIManager::HideUI(UIEnum targetUI, FadeOption fadeOption, ScaleOption scaleOption, double rate, double duration)
//{
//	GS_UI::GS_UIList[targetUI]->uIFunctionTimer.Start();
//
//	if (fadeOption == FadeOption::FadeIn)
//	{
//		GS_UI::GS_UIList[targetUI]->SetFadeIn(duration);
//	}
//	else if (fadeOption == FadeOption::FadeOut)
//	{
//		GS_UI::GS_UIList[targetUI]->SetFadeOut(duration);
//	}
//
//	if (scaleOption == ScaleOption::ScaleUp)
//	{
//		GS_UI::GS_UIList[targetUI]->SetScaleUp(duration, rate);
//	}
//	else if (scaleOption == ScaleOption::ScaleDown)
//	{
//		GS_UI::GS_UIList[targetUI]->SetScaleDown(duration, rate);
//	}
//
//	GS_UI::GS_UIList[targetUI]->uIFunctionTimer.onExpiration = [=]() { GS_UI::GS_UIList[targetUI]->HideThisUI(); };
//}

void UIManager::CreateAllGS_UI()
{
    GS_UI::CreateAllUI();
}

void UIManager::InitializeAllGS_UI()
{
    GS_UI::InitializeAllUI();
}



/// GS_UI 영역

unordered_map<UIManager::UIEnum, UIManager::GS_UI*> UIManager::GS_UI::GS_UIList = unordered_map<UIManager::UIEnum, UIManager::GS_UI*>();
unordered_map<UIManager::GS_UI*, vector<D2DSprite*>> UIManager::GS_UI::GS_UISpriteList = unordered_map<UIManager::GS_UI*, vector<D2DSprite*>>();
unordered_map<UIManager::GS_UI*, vector<D2DAnimatedSprite*>> UIManager::GS_UI::GS_UIAnimatedSpriteList = unordered_map<UIManager::GS_UI*, vector<D2DAnimatedSprite*>>();
unordered_map<D2DSprite*, Rect> UIManager::GS_UI::GS_UISpriteInitRectList = unordered_map<D2DSprite*, Rect>();
unordered_map<D2DSprite*, Vector2d> UIManager::GS_UI::GS_UISpriteInitPosList = unordered_map<D2DSprite*, Vector2d>();
unordered_map<D2DAnimatedSprite*, Rect> UIManager::GS_UI::GS_UIAnimatedSpriteInitRectList = unordered_map<D2DAnimatedSprite*, Rect>();
unordered_map<D2DAnimatedSprite*, Vector2d> UIManager::GS_UI::GS_UIAnimatedSpriteInitPosList = unordered_map<D2DAnimatedSprite*, Vector2d>();


void UIManager::GS_UI::SetFadeIn(double duration)
{
    uIFunctionTimer.duration = duration;
    uIFunctionTimer.onUpdate = [=](double normT)
    {
        for (auto each : GS_UISpriteList[this])
        {
            each->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
        }

        for (auto each : GS_UIAnimatedSpriteList[this])
        {
            each->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
        }
    };

    for (auto each : GS_UISpriteList[this])
    {
        each->color.a = 0;
    }

    for (auto each : GS_UIAnimatedSpriteList[this])
    {
        each->color.a = 0;
    }
}

void UIManager::GS_UI::SetFadeOut(double duration)
{
    uIFunctionTimer.duration = duration;
    uIFunctionTimer.onUpdate = [=](double normT)
    {
        for (auto each : GS_UISpriteList[this])
        {
            each->color.a = (float)(Vector2d::Lerp(Vector2d(0, 1.0f), Vector2d(0, 0), normT)).y;
        }

        for (auto each : GS_UIAnimatedSpriteList[this])
        {
            each->color.a = (float)(Vector2d::Lerp(Vector2d(0, 1.0f), Vector2d(0, 0), normT)).y;
        }
    };
}

void UIManager::GS_UI::SetScaleUp(double duration, double rate)
{
    // rate 가 1 이하여야 함
    // 예외 처리 로직은 없음
    uIFunctionTimer.duration = duration;
    this->rate = rate;
    uIFunctionTimer.onUpdate = [=](double normT)
    {
        for (auto each : GS_UISpriteList[this])
        {
            each->GetGameObject()->GetTransform()->position = Vector2d::Lerp(GS_UISpriteInitPosList[each] * rate, GS_UISpriteInitPosList[each], normT);
            Vector2d indicatorVector = Vector2d::Lerp(Vector2d(GS_UISpriteInitRectList[each].width * rate, GS_UISpriteInitRectList[each].height * rate), Vector2d(GS_UISpriteInitRectList[each].width, GS_UISpriteInitRectList[each].height), normT);
            each->SetDrawRect(Rect(indicatorVector.x, indicatorVector.y));
        }

        for (auto each : GS_UIAnimatedSpriteList[this])
        {
            each->GetGameObject()->GetTransform()->position = Vector2d::Lerp(GS_UIAnimatedSpriteInitPosList[each] * rate, GS_UIAnimatedSpriteInitPosList[each], normT);
            Vector2d indicatorVector = Vector2d::Lerp(Vector2d(GS_UIAnimatedSpriteInitRectList[each].width * rate, GS_UIAnimatedSpriteInitRectList[each].height * rate), Vector2d(GS_UIAnimatedSpriteInitRectList[each].width, GS_UIAnimatedSpriteInitRectList[each].height), normT);
            each->SetWidth(indicatorVector.x);
            each->SetHeight(indicatorVector.y);
        }
    };

    for (auto each : GS_UISpriteList[this])
    {
        each->color.a = 1.0f;
    }

    for (auto each : GS_UIAnimatedSpriteList[this])
    {
        each->color.a = 1.0f;
    }
}

void UIManager::GS_UI::SetScaleDown(double duration, double rate)
{
    // rate 가 1 이상이어야 함
    // 예외 처리 로직은 없음
    uIFunctionTimer.duration = duration;
    this->rate = rate;
    uIFunctionTimer.onUpdate = [=](double normT)
    {
        for (auto each : GS_UISpriteList[this])
        {
            each->GetGameObject()->GetTransform()->position = Vector2d::Lerp(GS_UISpriteInitPosList[each] * rate, GS_UISpriteInitPosList[each], normT);
            Vector2d indicatorVector = Vector2d::Lerp(Vector2d(GS_UISpriteInitRectList[each].width * rate, GS_UISpriteInitRectList[each].height * rate), Vector2d(GS_UISpriteInitRectList[each].width, GS_UISpriteInitRectList[each].height), normT);
            each->SetDrawRect(Rect(indicatorVector.x, indicatorVector.y));
        }

        for (auto each : GS_UIAnimatedSpriteList[this])
        {
            each->GetGameObject()->GetTransform()->position = Vector2d::Lerp(GS_UIAnimatedSpriteInitPosList[each] * rate, GS_UIAnimatedSpriteInitPosList[each], normT);
            Vector2d indicatorVector = Vector2d::Lerp(Vector2d(GS_UIAnimatedSpriteInitRectList[each].width * rate, GS_UIAnimatedSpriteInitRectList[each].height * rate), Vector2d(GS_UIAnimatedSpriteInitRectList[each].width, GS_UIAnimatedSpriteInitRectList[each].height), normT);
            each->SetWidth(indicatorVector.x);
            each->SetHeight(indicatorVector.y);
        }
    };

    for (auto each : GS_UISpriteList[this])
    {
        each->color.a = 1.0f;
    }

    for (auto each : GS_UIAnimatedSpriteList[this])
    {
        each->color.a = 1.0f;
    }
}

void UIManager::GS_UI::CreateAllUI()
{
    GameObject* UIManagerObj = UIManager::GetInstance()->GetGameObject();
    GameObject* titleObj = UIManagerObj->AddGameObject();
    GameObject* inGamePopUpObj = UIManagerObj->AddGameObject();
    GameObject* inGameObj = UIManagerObj->AddGameObject();
    GameObject* playerDeathObj = UIManagerObj->AddGameObject();
    GameObject* creditObj = UIManagerObj->AddGameObject();
    GameObject* gameOverObj = UIManagerObj->AddGameObject();
    GameObject* gameCleaObj = UIManagerObj->AddGameObject();

    titleObj->AddComponent<Title>();
    inGamePopUpObj->AddComponent<InGamePopUp>();
    inGameObj->AddComponent<InGame>();
    playerDeathObj->AddComponent<PlayerDeath>();
    creditObj->AddComponent<Credit>();
    gameOverObj->AddComponent<GameOver>();
    gameCleaObj->AddComponent<GameClear>();

    UIList[UIEnum::TitleUI] = titleObj;
    UIList[UIEnum::InGamePopUpUI] = inGamePopUpObj;
    UIList[UIEnum::InGameUI] = inGameObj;
    UIList[UIEnum::PlayerDeathUI] = playerDeathObj;
    UIList[UIEnum::CreditUI] = creditObj;
    UIList[UIEnum::GameOverUI] = gameOverObj;
    UIList[UIEnum::GameClearUI] = gameCleaObj;
}

void UIManager::GS_UI::InitializeAllUI()
{
    for (auto each : GS_UIList)
    {
        each.second->Initialize();
    }
}

void UIManager::GS_UI::ResetAllUI()
{
    for (auto each : GS_UIList)
    {
        each.second->ResetThisUI();
    }
}

void UIManager::GS_UI::HideAllUI()
{
    for (auto each : GS_UIList)
    {
        each.second->HideThisUI();
    }
}



/// Title 영역

UIManager::Title* UIManager::Title::instance = nullptr;

UIManager::Title::Title()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::TitleUI] = this;
}

UIManager::Title::~Title()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::TitleUI);
    UIList.erase(UIEnum::TitleUI);
}

void UIManager::Title::Update()
{
    uIFunctionTimer.Update();
    timer.Update();
    timerHowToPlay.Update();
    glitchTimer.Update();

    if (D2DInput::isKeyPushed(KeyCode::ESC))
    {
        howToPlaySprite->color.a = 0;
        timerHowToPlay.isActive = false;
        creditButtonUI->SetSelfActive(true);
        playButtonUI->SetSelfActive(true);
        optionButtonUI->SetSelfActive(true);
        howToPlayButtonUI->SetSelfActive(true);
        quitButtonUI->SetSelfActive(true);
        artwork1UI->SetSelfActive(true);
    }
}

void UIManager::Title::Initialize()
{
    GameObject* titleObj = GetGameObject();

    Player::autoShot = true;

    constexpr double x = -270;
    constexpr double y = -40;
    constexpr double dx = 140;
    constexpr double dy = -120;

    background = titleObj->AddGameObject();
    creditButtonUI = titleObj->AddGameObject();
    playButtonUI = titleObj->AddGameObject();
    optionButtonUI = titleObj->AddGameObject();
    howToPlayButtonUI = titleObj->AddGameObject();
    quitButtonUI = titleObj->AddGameObject();
    artwork1UI = titleObj->AddGameObject();
    mouseInteractionIcon = titleObj->AddGameObject();
    howToPlaySprite = titleObj->AddGameObject()->AddComponent<D2DSprite>();
    howToPlaySprite->SetSpriteFilePath(L"sprites/UI/title/howtoplay.png");
    howToPlaySprite->color.a = 0;

    D2DAnimatedSprite* animatedSprite = mouseInteractionIcon->AddComponent<D2DAnimatedSprite>();
    animatedSprite->LoadAnimationFromFile(L"sprites/player/torso/fire_forward");
    mouseInteractionIcon->SetSelfActive(false);

    D2DSprite* backgroundSprite = background->AddComponent<D2DSprite>();
    backgroundSprite->SetSpriteFilePath(L"sprites/UI/title/background.png");

    (GS_UISpriteList[this]).push_back(backgroundSprite);
    GS_UISpriteInitRectList[backgroundSprite] = Rect(backgroundSprite->GetDrawRect());
    GS_UISpriteInitPosList[backgroundSprite] = Vector2d(0, 0);

    Button* creditButton = CreateTitleButton(creditButtonUI, Vector2d(-350, 190), L"sprites/UI/title/title.png");
    Button* playButton = CreateTitleButton(playButtonUI, Vector2d(x - dx * 0.5, y + dy * 0), L"sprites/UI/title/play_non.png");
    Button* optionButton = CreateTitleButton(optionButtonUI, Vector2d(x - dx * 0.5 + 320, y + dy * 0), L"sprites/UI/title/autoclicker_on.png");
    Button* howToPlayButton = CreateTitleButton(howToPlayButtonUI, Vector2d(x - dx * 0.5, y + dy * 1), L"sprites/UI/title/howtoplay_non.png");
    Button* quitButton = CreateTitleButton(quitButtonUI, Vector2d(x - dx * 0.5, y + dy * 2), L"sprites/UI/title/quit_non.png");
    Button* artwork = CreateTitleButton(artwork1UI, Vector2d(500, 0), L"sprites/UI/title/artwork1.png");

    playBtnSprite = playButton->GetGameObject()->GetComponent<D2DSprite>();
    optionBtnSprite = optionButton->GetGameObject()->GetComponent<D2DSprite>();
    howToPlayBtnSprite = howToPlayButton->GetGameObject()->GetComponent<D2DSprite>();
    quitBtnSprite = quitButton->GetGameObject()->GetComponent<D2DSprite>();
    artworkSprite = artwork->GetGameObject()->GetComponent<D2DSprite>();

    artworkSprite->color.a = 0;

    timer.duration = 3;
    creditButton->onMouseOver = [=]()
    {
        if (!timer.isActive) timer.Start();
    };
    creditButton->onMouseExit = [=]() { timer.isActive = false; };
    timer.onExpiration = [=]()
    {
        MapTool::GetInstance()->DisableMap();
        UIManager::GetInstance()->HideUI(UIEnum::TitleUI, FadeOption::FadeOut);
        GS_UIList[UIEnum::CreditUI]->ResetThisUI();
        UIManager::GetInstance()->ShowUI(UIEnum::CreditUI, FadeOption::FadeIn);
        SoundSystem::StopMusic();
        SoundSystem::PlayMusic("sounds/ui/Credit_ost.mp3");
    };

    playButton->onMouseOver = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_select.wav");
        playBtnSprite->SetSpriteFilePath(L"sprites/UI/title/play_hover.png");
        mouseInteractionIcon->SetSelfActive(true);
        mouseInteractionIcon->GetTransform()->position = Vector2d(x - dx * 2, y + dy * 0);
    };
    playButton->onMouseExit = [=]()
    {
        playBtnSprite->SetSpriteFilePath(L"sprites/UI/title/play_non.png");
        mouseInteractionIcon->SetSelfActive(false);
    };
    playButton->onClick = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_selectM.mp3");
        MapTool::GetInstance()->EnableMap();
        UIManager::GetInstance()->HideUI(UIEnum::TitleUI);
        MapTool::GetInstance()->EnableMap();
        MapTool::GetInstance()->LoadMap(MapTool::GetInstance()->GetLastEditedMapName());
        MapTool::GetInstance()->SetMapToolActive(false);
        MapTool::GetInstance()->Restart();
        UIManager::GetInstance()->ShowUI(UIEnum::InGameUI);
#ifndef _MAPTOOL
#endif
    };	// 게임 플레이 로직

    optionButton->onMouseOver = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_select.wav");
        optionBtnSprite->color.a = 0.5f;
    };
    optionButton->onMouseExit = [=]()
    {
        optionBtnSprite->color.a = 1.0f;
    };
    optionButton->onClick = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_selectM.mp3");
        if (Player::autoShot)
        {
            Player::autoShot = false;
            optionBtnSprite->SetSpriteFilePath(L"sprites/UI/title/autoclicker_off.png");
        }
        else
        {
            Player::autoShot = true;
            optionBtnSprite->SetSpriteFilePath(L"sprites/UI/title/autoclicker_on.png");
        }
        optionBtnSprite->color.a = 1.0f;
    };


    timerHowToPlay.duration = 0.5;
    timerHowToPlay.onUpdate = [=](double normT)
    {
        howToPlaySprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
        creditButtonUI->SetSelfActive(false);
        playButtonUI->SetSelfActive(false);
        optionButtonUI->SetSelfActive(false);
        howToPlayButtonUI->SetSelfActive(false);
        quitButtonUI->SetSelfActive(false);
        artwork1UI->SetSelfActive(false);
    };

    howToPlayButton->onMouseOver = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_select.wav");
        howToPlayBtnSprite->SetSpriteFilePath(L"sprites/UI/title/howtoplay_hover.png");
        mouseInteractionIcon->SetSelfActive(true);
        mouseInteractionIcon->GetTransform()->position = Vector2d(x - dx * 2, y + dy * 1);
    };
    howToPlayButton->onMouseExit = [=]()
    {
        howToPlayBtnSprite->SetSpriteFilePath(L"sprites/UI/title/howtoplay_non.png");
        mouseInteractionIcon->SetSelfActive(false);
    };
    howToPlayButton->onClick = [=]()
    {
		SoundSystem::PlaySoundfile("sounds/ui/UI_selectM.mp3");
		if (!timerHowToPlay.isActive)
			timerHowToPlay.Start();
    };

    quitButton->onMouseOver = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_select.wav");
        quitBtnSprite->SetSpriteFilePath(L"sprites/UI/title/quit_hover.png");
        mouseInteractionIcon->SetSelfActive(true);
        mouseInteractionIcon->GetTransform()->position = Vector2d(x - dx * 2, y + dy * 2);
    };
    quitButton->onMouseExit = [=]()
    {
        quitBtnSprite->SetSpriteFilePath(L"sprites/UI/title/quit_non.png");
        mouseInteractionIcon->SetSelfActive(false);
    };
    quitButton->onClick = [=]()
    {
        SoundSystem::PlaySoundfile("sounds/ui/UI_selectM.mp3");
        UIManager::GetInstance()->readyToQuit = true;
    };	// 프로그램 종료 내용 필요

    glitchTimer.duration = 1;
    artwork->onClick = [=]()
    {
        if (!glitchTimer.isActive)
        {
            SoundSystem::PlaySoundfile("sounds/ui/electrical_short.wav");
            glitchTimer.Start();
        }
    };
    glitchTimer.onUpdate = [=](double normT)
    {
        switch (((int)(normT * 10)) % 3)
        {
            case 0:
                artworkSprite->SetSpriteFilePath(L"sprites/UI/title/artwork2.png");
                artworkSprite->color.a = 0.5f;
                artworkSprite->GetTransform()->position = Vector2d(520, 0);
                break;
            case 1:
                artworkSprite->SetSpriteFilePath(L"sprites/UI/title/artwork1.png");
                artworkSprite->color.a = 0.5f;
                artworkSprite->GetTransform()->position = Vector2d(480, 0);
                break;
            case 2:
                artworkSprite->SetSpriteFilePath(L"sprites/UI/title/artwork0.png");
                artworkSprite->color.a = 0.5f;
				artworkSprite->GetTransform()->position = Vector2d(500, 0);
                break;
            default:
                break;
        }
    };
    glitchTimer.onExpiration = [=]()
    {
        artworkSprite->SetSpriteFilePath(L"sprites/UI/title/artwork1.png");
        artworkSprite->color.a = 0;
        artworkSprite->GetTransform()->position = Vector2d(500, 0);
    };
}

void UIManager::Title::ResetThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    timerHowToPlay.isActive = false;
    glitchTimer.isActive = false;
    playButtonUI->GetComponent<Button>()->OnMouseExit();
    optionButtonUI->GetComponent<Button>()->OnMouseExit();
    howToPlayButtonUI->GetComponent<Button>()->OnMouseExit();
    quitButtonUI->GetComponent<Button>()->OnMouseExit();
    UIManager::GetInstance()->ResetUI(UIEnum::GameClearUI);
    for (auto each : GS_UISpriteList[this])
    {
        each->color.a = 1.0f;
    }

    howToPlaySprite->color.a = 0;
    artworkSprite->color.a = 0;
}

void UIManager::Title::ShowThisUI(GameObject* targetObj)
{
    SoundSystem::StopMusic();
    SoundSystem::PlayMusic("sounds/ui/Main_ost.wav");
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(1);
    GetGameObject()->SetSelfActive(true);
    ProjectilePool::GetInstance()->RetriveAll();
}

void UIManager::Title::HideThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    timerHowToPlay.isActive = false;
    glitchTimer.isActive = false;
    GetGameObject()->SetSelfActive(false);
}

Button* UIManager::Title::CreateTitleButton(GameObject* parentPanel, const Vector2d& pos, const wstring& resourcePath)
{
    Button* button = parentPanel->AddComponent<Button>();

    D2DSprite* buttonSprite = button->GetGameObject()->AddComponent<D2DSprite>();
    buttonSprite->SetSpriteFilePath(resourcePath);

    auto spriteRect = buttonSprite->GetDrawRect();

    BoxCollider2D* buttonCollider = button->GetGameObject()->AddComponent<BoxCollider2D>();
    buttonCollider->SetWidth(spriteRect.width);
    buttonCollider->SetHeight(spriteRect.height);
    button->toggleButton = false;

    button->GetTransform()->position = pos;

    (GS_UISpriteList[this]).push_back(buttonSprite);
    GS_UISpriteInitRectList[buttonSprite] = Rect(buttonSprite->GetDrawRect());
    GS_UISpriteInitPosList[buttonSprite] = pos;

    return button;
}



/// InGamePopUp 영역

UIManager::InGamePopUp* UIManager::InGamePopUp::instance = nullptr;

UIManager::InGamePopUp::InGamePopUp()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::InGamePopUpUI] = this;
}

UIManager::InGamePopUp::~InGamePopUp()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::InGamePopUpUI);
    UIList.erase(UIEnum::InGamePopUpUI);
}

void UIManager::InGamePopUp::Update()
{
    uIFunctionTimer.Update();
}

void UIManager::InGamePopUp::Initialize()
{
    GameObject* inGamePopUpObj = GetGameObject();

}

void UIManager::InGamePopUp::ResetThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
}

void UIManager::InGamePopUp::ShowThisUI(GameObject* targetObj)
{
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(12);
    GetGameObject()->SetSelfActive(true);
}

void UIManager::InGamePopUp::HideThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    GetGameObject()->SetSelfActive(false);
}



/// InGame 영역

UIManager::InGame* UIManager::InGame::instance = nullptr;

UIManager::InGame::InGame()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::InGameUI] = this;
}

UIManager::InGame::~InGame()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::InGameUI);
    UIList.erase(UIEnum::InGameUI);
}

void UIManager::InGame::Update()
{
	uIFunctionTimer.Update();
	timerInGame.Update();
	comboTimer1.Update();
	comboTimer10.Update();
	comboTimer100.Update();
	bossClearTimer.Update();

    Player* player = Player::GetInstance();
    Boss* boss = Boss::GetInstance();

    if (boss && !bossClearTimer.isActive && boss->bossState == BossState::DEATH)
    {
        bossClearTimer.Start();
    }

    switch (player->lifeCount)
    {
    case 0:
        life1UI->SetSelfActive(false);
        life2UI->SetSelfActive(false);
        break;
    case 1:
        life1UI->SetSelfActive(true);
        life2UI->SetSelfActive(false);
        break;
    case 2:
        life1UI->SetSelfActive(true);
        life2UI->SetSelfActive(true);
        break;
    default:
        break;
    }
    
    switch (player->hp)
    {
    case 0:
        hp1->SetSelfActive(false);
        hp2->SetSelfActive(false);
        hp3->SetSelfActive(false);
        break;
    case 1:
        hp1->SetSelfActive(true);
        hp2->SetSelfActive(false);
        hp3->SetSelfActive(false);
        break;
    case 2:
        hp1->SetSelfActive(true);
        hp2->SetSelfActive(true);
        hp3->SetSelfActive(false);
        break;
    case 3:
        hp1->SetSelfActive(true);
        hp2->SetSelfActive(true);
        hp3->SetSelfActive(true);
        break;
    default:
        break;
    }

    if (player->lifeCount == -1 || player->lifeCount == -2)
    {
        hp1->SetSelfActive(false);
        hp2->SetSelfActive(false);
        hp3->SetSelfActive(false);
    }

    switch (player->skillCount)
    {
    case 0:
        stack1->SetSelfActive(false);
        stack2->SetSelfActive(false);
        stack3->SetSelfActive(false);
        break;
    case 1:
        stack1->SetSelfActive(true);
        stack2->SetSelfActive(false);
        stack3->SetSelfActive(false);
        break;
    case 2:
        stack1->SetSelfActive(true);
        stack2->SetSelfActive(true);
        stack3->SetSelfActive(false);
        break;
    case 3:
        stack1->SetSelfActive(true);
        stack2->SetSelfActive(true);
        stack3->SetSelfActive(true);
        break;
    default:
        break;
    }

    switch (player->gauge / 10)
    {
    case 0:
        for (size_t i = 0; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 1:
        for (size_t i = 0; i < 1; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 1; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 2:
        for (size_t i = 0; i < 2; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 2; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 3:
        for (size_t i = 0; i < 3; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 3; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 4:
        for (size_t i = 0; i < 4; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 4; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 5:
        for (size_t i = 0; i < 5; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 5; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 6:
        for (size_t i = 0; i < 6; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 6; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 7:
        for (size_t i = 0; i < 7; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 7; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 8:
        for (size_t i = 0; i < 8; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 8; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 9:
        for (size_t i = 0; i < 9; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 9; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 10:
        for (size_t i = 0; i < 10; ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        for (size_t i = 10; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(true);
        }
        break;
    case 11:
        if (player->gauge)
        {
			for (size_t i = 0; i < gauge_non.size(); ++i)
			{
				gauge_non[i]->SetSelfActive(false);
			}
        }
        else
        {
			for (size_t i = 0; i < 11; ++i)
			{
				gauge_non[i]->SetSelfActive(false);
			}
			for (size_t i = 11; i < gauge_non.size(); ++i)
			{
				gauge_non[i]->SetSelfActive(true);
			}
        }
        break;
    case 12:
        for (size_t i = 0; i < gauge_non.size(); ++i)
        {
            gauge_non[i]->SetSelfActive(false);
        }
        break;
    default:
        break;
    }

	if (player->combo == 0)
	{
		for (auto each : comboSpriteList)
		{
			each->GetTransform()->position = Vector2d(0, -2160);
		}

        comboWord->GetTransform()->position = Vector2d(0, 0);
		blankNumPos1->GetTransform()->position = Vector2d(0, 0);
		blankNumPos10->GetTransform()->position = Vector2d(0, 0);
		blankNumPos100->GetTransform()->position = Vector2d(0, 0);

        nowNumPos1 = blankNumPos1;
        nowNumPos10 = blankNumPos10;
        nowNumPos100 = blankNumPos100;

		comboAll->SetSelfActive(false);
		comboFadeInFinish = false;
	}
	else
	{
		/*if ((!comboFadeInFinish) && (!timerInGame.isActive))
		{
            comboFadeInFinish = true;
			timerInGame.Start();
			comboAll->SetSelfActive(true);
			comboAll->GetTransform()->position = player->GetPlayerPos();
			for (auto each : comboSpriteList)
			{
				each->color.a = 0;
			}
		}*/

        comboAll->SetSelfActive(true);



        if (player->combo / 10 == 0)
        {
            comboAll->GetTransform()->SetWorldPosition(player->GetPlayerPos() + Vector2d(55, floatingDistance));
            switch (player->combo)
            {
            case 0:
                //ChangeNumberEffect(countNumOrangePos1[9], countNumOrangePos1[0], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[0];
                break;
            case 1:
                if (nowNumPos1 == blankNumPos1)
                {
                    //ChangeNumberEffect(nowNumPos1, countNumOrangePos1[1], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[1];
                }
                else
                {
                    //ChangeNumberEffect(countNumOrangePos1[0], countNumOrangePos1[1], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[1];
                }
                break;
            case 2:
                //ChangeNumberEffect(countNumOrangePos1[1], countNumOrangePos1[2], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[2];
                break;
            case 3:
                //ChangeNumberEffect(countNumOrangePos1[2], countNumOrangePos1[3], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[3];
                break;
            case 4:
                //ChangeNumberEffect(countNumOrangePos1[3], countNumOrangePos1[4], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[4];
                break;
            case 5:
                //ChangeNumberEffect(countNumOrangePos1[4], countNumOrangePos1[5], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[5];
                break;
            case 6:
                //ChangeNumberEffect(countNumOrangePos1[5], countNumOrangePos1[6], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[6];
                break;
            case 7:
                //ChangeNumberEffect(countNumOrangePos1[6], countNumOrangePos1[7], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[7];
                break;
            case 8:
                //ChangeNumberEffect(countNumOrangePos1[7], countNumOrangePos1[8], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[8];
                break;
            case 9:
                //ChangeNumberEffect(countNumOrangePos1[8], countNumOrangePos1[9], TimerIndex::Pos1);
                nowNumPos1 = countNumOrangePos1[9];
                break;
            default:
                break;
            }
        }
        else if (player->combo / 100 == 0)
        {
            comboAll->GetTransform()->SetWorldPosition(player->GetPlayerPos() + Vector2d(80, floatingDistance));
            switch (player->combo / 10)
            {
            case 0:
                //ChangeNumberEffect(countNumBluePos10[9], countNumBluePos10[0], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[0];
                break;
            case 1:
                if (nowNumPos10 == blankNumPos10)
                {
                    //ChangeNumberEffect(nowNumPos10, countNumOrangePos10[1], TimerIndex::Pos10);
                    nowNumPos10 = countNumOrangePos10[1];
                }
                else
                {
                    //ChangeNumberEffect(countNumOrangePos10[0], countNumOrangePos10[1], TimerIndex::Pos10);
                    nowNumPos10 = countNumOrangePos10[1];
                }
                break;
            case 2:
                //ChangeNumberEffect(countNumOrangePos10[1], countNumOrangePos10[2], TimerIndex::Pos10);
                nowNumPos10 = countNumOrangePos10[2];
                break;
            case 3:
                //ChangeNumberEffect(countNumOrangePos10[2], countNumOrangePos10[3], TimerIndex::Pos10);
                nowNumPos10 = countNumOrangePos10[3];
                break;
            case 4:
                //ChangeNumberEffect(countNumOrangePos10[3], countNumOrangePos10[4], TimerIndex::Pos10);
                nowNumPos10 = countNumOrangePos10[4];
                break;
            case 5:
                //ChangeNumberEffect(countNumOrangePos10[4], countNumBluePos10[5], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[5];
                break;
            case 6:
                //ChangeNumberEffect(countNumBluePos10[5], countNumBluePos10[6], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[6];
                break;
            case 7:
                //ChangeNumberEffect(countNumBluePos10[6], countNumBluePos10[7], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[7];
                break;
            case 8:
                //ChangeNumberEffect(countNumBluePos10[7], countNumBluePos10[8], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[8];
                break;
            case 9:
                //ChangeNumberEffect(countNumBluePos10[8], countNumBluePos10[9], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[9];
                break;
            default:
                break;
            }

            if (player->combo >= 50)
            {
                switch (player->combo % 10)
                {
                case 0:
                    if (nowNumPos1 == countNumOrangePos1[9])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[9], countNumBluePos1[0], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[0];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[9], countNumBluePos1[0], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[0];
                    }
                    break;
                case 1:
                    if (nowNumPos1 == countNumOrangePos1[0])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[0], countNumBluePos1[1], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[1];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[0], countNumBluePos1[1], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[1];
                    }
                    break;
                case 2:
                    if (nowNumPos1 == countNumOrangePos1[1])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[1], countNumBluePos1[2], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[2];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[1], countNumBluePos1[2], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[2];
                    }
                    break;
                case 3:
                    if (nowNumPos1 == countNumOrangePos1[2])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[2], countNumBluePos1[3], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[3];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[2], countNumBluePos1[3], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[3];
                    }
                    break;
                case 4:
                    if (nowNumPos1 == countNumOrangePos1[3])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[3], countNumBluePos1[4], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[4];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[3], countNumBluePos1[4], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[4];
                    }
                    break;
                case 5:
                    if (nowNumPos1 == countNumOrangePos1[4])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[4], countNumBluePos1[5], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[5];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[4], countNumBluePos1[5], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[5];
                    }
                    break;
                case 6:
                    if (nowNumPos1 == countNumOrangePos1[5])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[5], countNumBluePos1[6], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[6];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[5], countNumBluePos1[6], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[6];
                    }
                    break;
                case 7:
                    if (nowNumPos1 == countNumOrangePos1[6])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[6], countNumBluePos1[7], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[7];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[6], countNumBluePos1[7], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[7];
                    }
                    break;
                case 8:
                    if (nowNumPos1 == countNumOrangePos1[7])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[7], countNumBluePos1[8], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[8];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumBluePos1[7], countNumBluePos1[8], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[8];
                    }
                    break;
                case 9:
                    if (nowNumPos1 == countNumOrangePos1[8])
                    {
                        //ChangeNumberEffect(countNumOrangePos1[8], countNumBluePos1[9], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[9];
                    }
                    else
                    {
                        //ChangeNumberEffect(countNumOrangePos1[8], countNumBluePos1[9], TimerIndex::Pos1);
                        nowNumPos1 = countNumBluePos1[9];
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (player->combo % 10)
                {
                case 0:
                    //ChangeNumberEffect(countNumOrangePos1[9], countNumOrangePos1[0], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[0];
                    break;
                case 1:
                    //ChangeNumberEffect(countNumOrangePos1[0], countNumOrangePos1[1], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[1];
                    break;
                case 2:
                    //ChangeNumberEffect(countNumOrangePos1[1], countNumOrangePos1[2], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[2];
                    break;
                case 3:
                    //ChangeNumberEffect(countNumOrangePos1[2], countNumOrangePos1[3], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[3];
                    break;
                case 4:
                    //ChangeNumberEffect(countNumOrangePos1[3], countNumOrangePos1[4], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[4];
                    break;
                case 5:
                    //ChangeNumberEffect(countNumOrangePos1[4], countNumOrangePos1[5], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[5];
                    break;
                case 6:
                    //ChangeNumberEffect(countNumOrangePos1[5], countNumOrangePos1[6], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[6];
                    break;
                case 7:
                    //ChangeNumberEffect(countNumOrangePos1[6], countNumOrangePos1[7], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[7];
                    break;
                case 8:
                    //ChangeNumberEffect(countNumOrangePos1[7], countNumOrangePos1[8], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[8];
                    break;
                case 9:
                    //ChangeNumberEffect(countNumOrangePos1[8], countNumOrangePos1[9], TimerIndex::Pos1);
                    nowNumPos1 = countNumOrangePos1[9];
                    break;
                default:
                    break;
                }
            }
        }
        else if (player->combo / 1000 == 0)
        {
            comboAll->GetTransform()->SetWorldPosition(player->GetPlayerPos() + Vector2d(105, floatingDistance));
            switch (player->combo / 100)
            {
            case 0:
                //ChangeNumberEffect(countNumBluePos100[9], countNumBluePos100[0], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[0];
                break;
            case 1:
                if (nowNumPos100 == blankNumPos100)
                {
                    //ChangeNumberEffect(nowNumPos100, countNumBluePos100[1], TimerIndex::Pos100);
                    nowNumPos100 = countNumBluePos100[1];
                }
                else
                {
                    //ChangeNumberEffect(countNumBluePos100[0], countNumBluePos100[1], TimerIndex::Pos100);
                    nowNumPos100 = countNumBluePos100[1];
                }
                break;
            case 2:
                //ChangeNumberEffect(countNumBluePos100[1], countNumBluePos100[2], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[2];
                break;
            case 3:
                //ChangeNumberEffect(countNumBluePos100[2], countNumBluePos100[3], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[3];
                break;
            case 4:
                //ChangeNumberEffect(countNumBluePos100[3], countNumBluePos100[4], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[4];
                break;
            case 5:
                //ChangeNumberEffect(countNumBluePos100[4], countNumBluePos100[5], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[5];
                break;
            case 6:
                //ChangeNumberEffect(countNumBluePos100[5], countNumBluePos100[6], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[6];
                break;
            case 7:
                //ChangeNumberEffect(countNumBluePos100[6], countNumBluePos100[7], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[7];
                break;
            case 8:
                //ChangeNumberEffect(countNumBluePos100[7], countNumBluePos100[8], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[8];
                break;
            case 9:
                //ChangeNumberEffect(countNumBluePos100[8], countNumBluePos100[9], TimerIndex::Pos100);
                nowNumPos100 = countNumBluePos100[9];
                break;
            default:
                break;
            }

            switch ((player->combo % 100)/10)
            {
            case 0:
                //ChangeNumberEffect(countNumBluePos10[9], countNumBluePos10[0], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[0];
                break;
            case 1:
                //ChangeNumberEffect(countNumBluePos10[0], countNumBluePos10[1], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[1];
                break;
            case 2:
                //ChangeNumberEffect(countNumBluePos10[1], countNumBluePos10[2], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[2];
                break;
            case 3:
                //ChangeNumberEffect(countNumBluePos10[2], countNumBluePos10[3], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[3];
                break;
            case 4:
                //ChangeNumberEffect(countNumBluePos10[3], countNumBluePos10[4], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[4];
                break;
            case 5:
                //ChangeNumberEffect(countNumBluePos10[4], countNumBluePos10[5], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[5];
                break;
            case 6:
                //ChangeNumberEffect(countNumBluePos10[5], countNumBluePos10[6], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[6];
                break;
            case 7:
                //ChangeNumberEffect(countNumBluePos10[6], countNumBluePos10[7], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[7];
                break;
            case 8:
                //ChangeNumberEffect(countNumBluePos10[7], countNumBluePos10[8], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[8];
                break;
            case 9:
                //ChangeNumberEffect(countNumBluePos10[8], countNumBluePos10[9], TimerIndex::Pos10);
                nowNumPos10 = countNumBluePos10[9];
                break;
            default:
                break;
            }

            switch (player->combo % 10)
            {
            case 0:
                //ChangeNumberEffect(countNumBluePos1[9], countNumBluePos1[0], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[0];
                break;
            case 1:
                //ChangeNumberEffect(countNumBluePos1[0], countNumBluePos1[1], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[1];
                break;
            case 2:
                //ChangeNumberEffect(countNumBluePos1[1], countNumBluePos1[2], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[2];
                break;
            case 3:
                //ChangeNumberEffect(countNumBluePos1[2], countNumBluePos1[3], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[3];
                break;
            case 4:
                //ChangeNumberEffect(countNumBluePos1[3], countNumBluePos1[4], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[4];
                break;
            case 5:
                //ChangeNumberEffect(countNumBluePos1[4], countNumBluePos1[5], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[5];
                break;
            case 6:
                //ChangeNumberEffect(countNumBluePos1[5], countNumBluePos1[6], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[6];
                break;
            case 7:
                //ChangeNumberEffect(countNumBluePos1[6], countNumBluePos1[7], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[7];
                break;
            case 8:
                //ChangeNumberEffect(countNumBluePos1[7], countNumBluePos1[8], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[8];
                break;
            case 9:
                //ChangeNumberEffect(countNumBluePos1[8], countNumBluePos1[9], TimerIndex::Pos1);
                nowNumPos1 = countNumBluePos1[9];
                break;
            default:
                break;
            }
        }

        for (auto each : comboSpriteList)
        {
            each->GetTransform()->position = Vector2d(0, -2160);
        }

        comboWord->GetTransform()->position = Vector2d(0, 0);
        nowNumPos1->GetTransform()->position = Vector2d(0, 0);
        nowNumPos10->GetTransform()->position = Vector2d(0, 0);
        nowNumPos100->GetTransform()->position = Vector2d(0, 0);
    }
}

void UIManager::InGame::Initialize()
{
    GameObject* inGameObj = GetGameObject();

    life1UI = inGameObj->AddGameObject();
    life2UI = inGameObj->AddGameObject();
    gauge = inGameObj->AddGameObject();
    stack1 = inGameObj->AddGameObject();
    stack2 = inGameObj->AddGameObject();
    stack3 = inGameObj->AddGameObject();
    hp1 = inGameObj->AddGameObject();
    hp2 = inGameObj->AddGameObject();
    hp3 = inGameObj->AddGameObject();
    comboAll = inGameObj->AddGameObject();

    /// Combo 관련
    {
        comboWord = comboAll->AddGameObject();
        D2DSprite* comboWordSprite = comboWord->AddComponent<D2DSprite>();
        comboWordSprite->SetSpriteFilePath(L"sprites/UI/inGame/combo/combo.png");
        comboWordSprite->GetTransform()->position = Vector2d(0, 0);

        (GS_UISpriteList[this]).push_back(comboWordSprite);
        GS_UISpriteInitRectList[comboWordSprite] = Rect(comboWordSprite->GetDrawRect());
        GS_UISpriteInitPosList[comboWordSprite] = Vector2d(0, 0);
        comboSpriteList.insert(comboWordSprite);

        numPos1 = comboAll->AddGameObject();
        numPos1->GetTransform()->position = Vector2d(-116, 0);

        numPos10 = comboAll->AddGameObject();
        numPos10->GetTransform()->position = Vector2d(-174, 0);

        numPos100 = comboAll->AddGameObject();
        numPos100->GetTransform()->position = Vector2d(-232, 0);

        blankNumPos1 = numPos1->AddGameObject();
        D2DSprite* blankNumPos1Sprite = blankNumPos1->AddComponent<D2DSprite>();
        blankNumPos1Sprite->SetSpriteFilePath(L"sprites/UI/inGame/combo/blank.png");

        (GS_UISpriteList[this]).push_back(blankNumPos1Sprite);
        GS_UISpriteInitRectList[blankNumPos1Sprite] = Rect(blankNumPos1Sprite->GetDrawRect());
        GS_UISpriteInitPosList[blankNumPos1Sprite] = Vector2d(0, 0);
        comboSpriteList.insert(blankNumPos1Sprite);

        blankNumPos10 = numPos10->AddGameObject();
        D2DSprite* blankNumPos10Sprite = blankNumPos10->AddComponent<D2DSprite>();
        blankNumPos10Sprite->SetSpriteFilePath(L"sprites/UI/inGame/combo/blank.png");

        (GS_UISpriteList[this]).push_back(blankNumPos10Sprite);
        GS_UISpriteInitRectList[blankNumPos10Sprite] = Rect(blankNumPos10Sprite->GetDrawRect());
        GS_UISpriteInitPosList[blankNumPos10Sprite] = Vector2d(0, 0);
        comboSpriteList.insert(blankNumPos10Sprite);

        blankNumPos100 = numPos100->AddGameObject();
        D2DSprite* blankNumPos100Sprite = blankNumPos100->AddComponent<D2DSprite>();
        blankNumPos100Sprite->SetSpriteFilePath(L"sprites/UI/inGame/combo/blank.png");

        (GS_UISpriteList[this]).push_back(blankNumPos100Sprite);
        GS_UISpriteInitRectList[blankNumPos100Sprite] = Rect(blankNumPos100Sprite->GetDrawRect());
        GS_UISpriteInitPosList[blankNumPos100Sprite] = Vector2d(0, 0);
        comboSpriteList.insert(blankNumPos100Sprite);

        for (size_t i = 0; i < 10; ++i)
        {
            D2DSprite* spriteContainer = nullptr;
            countNumOrangePos1[i] = numPos1->AddGameObject();
            countNumOrangePos1[i]->GetTransform()->position = Vector2d(0, -2160);
            spriteContainer = countNumOrangePos1[i]->AddComponent<D2DSprite>();
            (GS_UISpriteList[this]).push_back(spriteContainer);
            GS_UISpriteInitPosList[spriteContainer] = Vector2d(0, -2160);
            comboSpriteList.insert(spriteContainer);

            countNumOrangePos10[i] = numPos10->AddGameObject();
            countNumOrangePos10[i]->GetTransform()->position = Vector2d(0, -2160);
            spriteContainer = countNumOrangePos10[i]->AddComponent<D2DSprite>();
            (GS_UISpriteList[this]).push_back(spriteContainer);
            GS_UISpriteInitPosList[spriteContainer] = Vector2d(0, -2160);
            comboSpriteList.insert(spriteContainer);

            countNumOrangePos100[i] = numPos100->AddGameObject();
            countNumOrangePos100[i]->GetTransform()->position = Vector2d(0, -2160);
            spriteContainer = countNumOrangePos100[i]->AddComponent<D2DSprite>();
            (GS_UISpriteList[this]).push_back(spriteContainer);
            GS_UISpriteInitPosList[spriteContainer] = Vector2d(0, -2160);
            comboSpriteList.insert(spriteContainer);

            countNumBluePos1[i] = numPos1->AddGameObject();
            countNumBluePos1[i]->GetTransform()->position = Vector2d(0, -2160);
            spriteContainer = countNumBluePos1[i]->AddComponent<D2DSprite>();
            (GS_UISpriteList[this]).push_back(spriteContainer);
            GS_UISpriteInitPosList[spriteContainer] = Vector2d(0, -2160);
            comboSpriteList.insert(spriteContainer);

            countNumBluePos10[i] = numPos10->AddGameObject();
            countNumBluePos10[i]->GetTransform()->position = Vector2d(0, -2160);
            spriteContainer = countNumBluePos10[i]->AddComponent<D2DSprite>();
            (GS_UISpriteList[this]).push_back(spriteContainer);
            GS_UISpriteInitPosList[spriteContainer] = Vector2d(0, -2160);
            comboSpriteList.insert(spriteContainer);

            countNumBluePos100[i] = numPos100->AddGameObject();
            countNumBluePos100[i]->GetTransform()->position = Vector2d(0, -2160);
            spriteContainer = countNumBluePos100[i]->AddComponent<D2DSprite>();
            (GS_UISpriteList[this]).push_back(spriteContainer);
            GS_UISpriteInitPosList[spriteContainer] = Vector2d(0, -2160);
            comboSpriteList.insert(spriteContainer);
        }

        D2DSprite* o_numPos1_0 = countNumOrangePos1[0]->GetComponent<D2DSprite>();
        o_numPos1_0->SetSpriteFilePath(L"sprites/UI/inGame/combo/0_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_0] = Rect(o_numPos1_0->GetDrawRect());

        D2DSprite* b_numPos1_0 = countNumBluePos1[0]->GetComponent<D2DSprite>();
        b_numPos1_0->SetSpriteFilePath(L"sprites/UI/inGame/combo/0_blue.png");
        GS_UISpriteInitRectList[b_numPos1_0] = Rect(b_numPos1_0->GetDrawRect());

        D2DSprite* o_numPos1_1 = countNumOrangePos1[1]->GetComponent<D2DSprite>();
        o_numPos1_1->SetSpriteFilePath(L"sprites/UI/inGame/combo/1_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_1] = Rect(o_numPos1_1->GetDrawRect());

        D2DSprite* b_numPos1_1 = countNumBluePos1[1]->GetComponent<D2DSprite>();
        b_numPos1_1->SetSpriteFilePath(L"sprites/UI/inGame/combo/1_blue.png");
        GS_UISpriteInitRectList[b_numPos1_1] = Rect(b_numPos1_1->GetDrawRect());

        D2DSprite* o_numPos1_2 = countNumOrangePos1[2]->GetComponent<D2DSprite>();
        o_numPos1_2->SetSpriteFilePath(L"sprites/UI/inGame/combo/2_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_2] = Rect(o_numPos1_2->GetDrawRect());

        D2DSprite* b_numPos1_2 = countNumBluePos1[2]->GetComponent<D2DSprite>();
        b_numPos1_2->SetSpriteFilePath(L"sprites/UI/inGame/combo/2_blue.png");
        GS_UISpriteInitRectList[b_numPos1_2] = Rect(b_numPos1_2->GetDrawRect());

        D2DSprite* o_numPos1_3 = countNumOrangePos1[3]->GetComponent<D2DSprite>();
        o_numPos1_3->SetSpriteFilePath(L"sprites/UI/inGame/combo/3_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_3] = Rect(o_numPos1_3->GetDrawRect());

        D2DSprite* b_numPos1_3 = countNumBluePos1[3]->GetComponent<D2DSprite>();
        b_numPos1_3->SetSpriteFilePath(L"sprites/UI/inGame/combo/3_blue.png");
        GS_UISpriteInitRectList[b_numPos1_3] = Rect(b_numPos1_3->GetDrawRect());

        D2DSprite* o_numPos1_4 = countNumOrangePos1[4]->GetComponent<D2DSprite>();
        o_numPos1_4->SetSpriteFilePath(L"sprites/UI/inGame/combo/4_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_4] = Rect(o_numPos1_4->GetDrawRect());

        D2DSprite* b_numPos1_4 = countNumBluePos1[4]->GetComponent<D2DSprite>();
        b_numPos1_4->SetSpriteFilePath(L"sprites/UI/inGame/combo/4_blue.png");
        GS_UISpriteInitRectList[b_numPos1_4] = Rect(b_numPos1_4->GetDrawRect());

        D2DSprite* o_numPos1_5 = countNumOrangePos1[5]->GetComponent<D2DSprite>();
        o_numPos1_5->SetSpriteFilePath(L"sprites/UI/inGame/combo/5_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_5] = Rect(o_numPos1_5->GetDrawRect());

        D2DSprite* b_numPos1_5 = countNumBluePos1[5]->GetComponent<D2DSprite>();
        b_numPos1_5->SetSpriteFilePath(L"sprites/UI/inGame/combo/5_blue.png");
        GS_UISpriteInitRectList[b_numPos1_5] = Rect(b_numPos1_5->GetDrawRect());

        D2DSprite* o_numPos1_6 = countNumOrangePos1[6]->GetComponent<D2DSprite>();
        o_numPos1_6->SetSpriteFilePath(L"sprites/UI/inGame/combo/6_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_6] = Rect(o_numPos1_6->GetDrawRect());

        D2DSprite* b_numPos1_6 = countNumBluePos1[6]->GetComponent<D2DSprite>();
        b_numPos1_6->SetSpriteFilePath(L"sprites/UI/inGame/combo/6_blue.png");
        GS_UISpriteInitRectList[b_numPos1_6] = Rect(b_numPos1_6->GetDrawRect());

        D2DSprite* o_numPos1_7 = countNumOrangePos1[7]->GetComponent<D2DSprite>();
        o_numPos1_7->SetSpriteFilePath(L"sprites/UI/inGame/combo/7_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_7] = Rect(o_numPos1_7->GetDrawRect());

        D2DSprite* b_numPos1_7 = countNumBluePos1[7]->GetComponent<D2DSprite>();
        b_numPos1_7->SetSpriteFilePath(L"sprites/UI/inGame/combo/7_blue.png");
        GS_UISpriteInitRectList[b_numPos1_7] = Rect(b_numPos1_7->GetDrawRect());

        D2DSprite* o_numPos1_8 = countNumOrangePos1[8]->GetComponent<D2DSprite>();
        o_numPos1_8->SetSpriteFilePath(L"sprites/UI/inGame/combo/8_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_8] = Rect(o_numPos1_8->GetDrawRect());

        D2DSprite* b_numPos1_8 = countNumBluePos1[8]->GetComponent<D2DSprite>();
        b_numPos1_8->SetSpriteFilePath(L"sprites/UI/inGame/combo/8_blue.png");
        GS_UISpriteInitRectList[b_numPos1_8] = Rect(b_numPos1_8->GetDrawRect());

        D2DSprite* o_numPos1_9 = countNumOrangePos1[9]->GetComponent<D2DSprite>();
        o_numPos1_9->SetSpriteFilePath(L"sprites/UI/inGame/combo/9_yellow.png");
        GS_UISpriteInitRectList[o_numPos1_9] = Rect(o_numPos1_9->GetDrawRect());

        D2DSprite* b_numPos1_9 = countNumBluePos1[9]->GetComponent<D2DSprite>();
        b_numPos1_9->SetSpriteFilePath(L"sprites/UI/inGame/combo/9_blue.png");
        GS_UISpriteInitRectList[b_numPos1_9] = Rect(b_numPos1_9->GetDrawRect());

        D2DSprite* o_numPos10_0 = countNumOrangePos10[0]->GetComponent<D2DSprite>();
        o_numPos10_0->SetSpriteFilePath(L"sprites/UI/inGame/combo/0_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_0] = Rect(o_numPos10_0->GetDrawRect());

        D2DSprite* b_numPos10_0 = countNumBluePos10[0]->GetComponent<D2DSprite>();
        b_numPos10_0->SetSpriteFilePath(L"sprites/UI/inGame/combo/0_blue.png");
        GS_UISpriteInitRectList[b_numPos10_0] = Rect(b_numPos10_0->GetDrawRect());

        D2DSprite* o_numPos10_1 = countNumOrangePos10[1]->GetComponent<D2DSprite>();
        o_numPos10_1->SetSpriteFilePath(L"sprites/UI/inGame/combo/1_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_1] = Rect(o_numPos10_1->GetDrawRect());

        D2DSprite* b_numPos10_1 = countNumBluePos10[1]->GetComponent<D2DSprite>();
        b_numPos10_1->SetSpriteFilePath(L"sprites/UI/inGame/combo/1_blue.png");
        GS_UISpriteInitRectList[b_numPos10_1] = Rect(b_numPos10_1->GetDrawRect());

        D2DSprite* o_numPos10_2 = countNumOrangePos10[2]->GetComponent<D2DSprite>();
        o_numPos10_2->SetSpriteFilePath(L"sprites/UI/inGame/combo/2_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_2] = Rect(o_numPos10_2->GetDrawRect());

        D2DSprite* b_numPos10_2 = countNumBluePos10[2]->GetComponent<D2DSprite>();
        b_numPos10_2->SetSpriteFilePath(L"sprites/UI/inGame/combo/2_blue.png");
        GS_UISpriteInitRectList[b_numPos10_2] = Rect(b_numPos10_2->GetDrawRect());

        D2DSprite* o_numPos10_3 = countNumOrangePos10[3]->GetComponent<D2DSprite>();
        o_numPos10_3->SetSpriteFilePath(L"sprites/UI/inGame/combo/3_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_3] = Rect(o_numPos10_3->GetDrawRect());

        D2DSprite* b_numPos10_3 = countNumBluePos10[3]->GetComponent<D2DSprite>();
        b_numPos10_3->SetSpriteFilePath(L"sprites/UI/inGame/combo/3_blue.png");
        GS_UISpriteInitRectList[b_numPos10_3] = Rect(b_numPos10_3->GetDrawRect());

        D2DSprite* o_numPos10_4 = countNumOrangePos10[4]->GetComponent<D2DSprite>();
        o_numPos10_4->SetSpriteFilePath(L"sprites/UI/inGame/combo/4_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_4] = Rect(o_numPos10_4->GetDrawRect());

        D2DSprite* b_numPos10_4 = countNumBluePos10[4]->GetComponent<D2DSprite>();
        b_numPos10_4->SetSpriteFilePath(L"sprites/UI/inGame/combo/4_blue.png");
        GS_UISpriteInitRectList[b_numPos10_4] = Rect(b_numPos10_4->GetDrawRect());

        D2DSprite* o_numPos10_5 = countNumOrangePos10[5]->GetComponent<D2DSprite>();
        o_numPos10_5->SetSpriteFilePath(L"sprites/UI/inGame/combo/5_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_5] = Rect(o_numPos10_5->GetDrawRect());

        D2DSprite* b_numPos10_5 = countNumBluePos10[5]->GetComponent<D2DSprite>();
        b_numPos10_5->SetSpriteFilePath(L"sprites/UI/inGame/combo/5_blue.png");
        GS_UISpriteInitRectList[b_numPos10_5] = Rect(b_numPos10_5->GetDrawRect());

        D2DSprite* o_numPos10_6 = countNumOrangePos10[6]->GetComponent<D2DSprite>();
        o_numPos10_6->SetSpriteFilePath(L"sprites/UI/inGame/combo/6_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_6] = Rect(o_numPos10_6->GetDrawRect());

        D2DSprite* b_numPos10_6 = countNumBluePos10[6]->GetComponent<D2DSprite>();
        b_numPos10_6->SetSpriteFilePath(L"sprites/UI/inGame/combo/6_blue.png");
        GS_UISpriteInitRectList[b_numPos10_6] = Rect(b_numPos10_6->GetDrawRect());

        D2DSprite* o_numPos10_7 = countNumOrangePos10[7]->GetComponent<D2DSprite>();
        o_numPos10_7->SetSpriteFilePath(L"sprites/UI/inGame/combo/7_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_7] = Rect(o_numPos10_7->GetDrawRect());

        D2DSprite* b_numPos10_7 = countNumBluePos10[7]->GetComponent<D2DSprite>();
        b_numPos10_7->SetSpriteFilePath(L"sprites/UI/inGame/combo/7_blue.png");
        GS_UISpriteInitRectList[b_numPos10_7] = Rect(b_numPos10_7->GetDrawRect());

        D2DSprite* o_numPos10_8 = countNumOrangePos10[8]->GetComponent<D2DSprite>();
        o_numPos10_8->SetSpriteFilePath(L"sprites/UI/inGame/combo/8_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_8] = Rect(o_numPos10_8->GetDrawRect());

        D2DSprite* b_numPos10_8 = countNumBluePos10[8]->GetComponent<D2DSprite>();
        b_numPos10_8->SetSpriteFilePath(L"sprites/UI/inGame/combo/8_blue.png");
        GS_UISpriteInitRectList[b_numPos10_8] = Rect(b_numPos10_8->GetDrawRect());

        D2DSprite* o_numPos10_9 = countNumOrangePos10[9]->GetComponent<D2DSprite>();
        o_numPos10_9->SetSpriteFilePath(L"sprites/UI/inGame/combo/9_yellow.png");
        GS_UISpriteInitRectList[o_numPos10_9] = Rect(o_numPos10_9->GetDrawRect());

        D2DSprite* b_numPos10_9 = countNumBluePos10[9]->GetComponent<D2DSprite>();
        b_numPos10_9->SetSpriteFilePath(L"sprites/UI/inGame/combo/9_blue.png");
        GS_UISpriteInitRectList[b_numPos10_9] = Rect(b_numPos10_9->GetDrawRect());

        D2DSprite* o_numPos100_0 = countNumOrangePos100[0]->GetComponent<D2DSprite>();
        o_numPos100_0->SetSpriteFilePath(L"sprites/UI/inGame/combo/0_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_0] = Rect(o_numPos100_0->GetDrawRect());

        D2DSprite* b_numPos100_0 = countNumBluePos100[0]->GetComponent<D2DSprite>();
        b_numPos100_0->SetSpriteFilePath(L"sprites/UI/inGame/combo/0_blue.png");
        GS_UISpriteInitRectList[b_numPos100_0] = Rect(b_numPos100_0->GetDrawRect());

        D2DSprite* o_numPos100_1 = countNumOrangePos100[1]->GetComponent<D2DSprite>();
        o_numPos100_1->SetSpriteFilePath(L"sprites/UI/inGame/combo/1_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_1] = Rect(o_numPos100_1->GetDrawRect());

        D2DSprite* b_numPos100_1 = countNumBluePos100[1]->GetComponent<D2DSprite>();
        b_numPos100_1->SetSpriteFilePath(L"sprites/UI/inGame/combo/1_blue.png");
        GS_UISpriteInitRectList[b_numPos100_1] = Rect(b_numPos100_1->GetDrawRect());

        D2DSprite* o_numPos100_2 = countNumOrangePos100[2]->GetComponent<D2DSprite>();
        o_numPos100_2->SetSpriteFilePath(L"sprites/UI/inGame/combo/2_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_2] = Rect(o_numPos100_2->GetDrawRect());

        D2DSprite* b_numPos100_2 = countNumBluePos100[2]->GetComponent<D2DSprite>();
        b_numPos100_2->SetSpriteFilePath(L"sprites/UI/inGame/combo/2_blue.png");
        GS_UISpriteInitRectList[b_numPos100_2] = Rect(b_numPos100_2->GetDrawRect());

        D2DSprite* o_numPos100_3 = countNumOrangePos100[3]->GetComponent<D2DSprite>();
        o_numPos100_3->SetSpriteFilePath(L"sprites/UI/inGame/combo/3_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_3] = Rect(o_numPos100_3->GetDrawRect());

        D2DSprite* b_numPos100_3 = countNumBluePos100[3]->GetComponent<D2DSprite>();
        b_numPos100_3->SetSpriteFilePath(L"sprites/UI/inGame/combo/3_blue.png");
        GS_UISpriteInitRectList[b_numPos100_3] = Rect(b_numPos100_3->GetDrawRect());

        D2DSprite* o_numPos100_4 = countNumOrangePos100[4]->GetComponent<D2DSprite>();
        o_numPos100_4->SetSpriteFilePath(L"sprites/UI/inGame/combo/4_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_4] = Rect(o_numPos100_4->GetDrawRect());

        D2DSprite* b_numPos100_4 = countNumBluePos100[4]->GetComponent<D2DSprite>();
        b_numPos100_4->SetSpriteFilePath(L"sprites/UI/inGame/combo/4_blue.png");
        GS_UISpriteInitRectList[b_numPos100_4] = Rect(b_numPos100_4->GetDrawRect());

        D2DSprite* o_numPos100_5 = countNumOrangePos100[5]->GetComponent<D2DSprite>();
        o_numPos100_5->SetSpriteFilePath(L"sprites/UI/inGame/combo/5_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_5] = Rect(o_numPos100_5->GetDrawRect());

        D2DSprite* b_numPos100_5 = countNumBluePos100[5]->GetComponent<D2DSprite>();
        b_numPos100_5->SetSpriteFilePath(L"sprites/UI/inGame/combo/5_blue.png");
        GS_UISpriteInitRectList[b_numPos100_5] = Rect(b_numPos100_5->GetDrawRect());

        D2DSprite* o_numPos100_6 = countNumOrangePos100[6]->GetComponent<D2DSprite>();
        o_numPos100_6->SetSpriteFilePath(L"sprites/UI/inGame/combo/6_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_6] = Rect(o_numPos100_6->GetDrawRect());

        D2DSprite* b_numPos100_6 = countNumBluePos100[6]->GetComponent<D2DSprite>();
        b_numPos100_6->SetSpriteFilePath(L"sprites/UI/inGame/combo/6_blue.png");
        GS_UISpriteInitRectList[b_numPos100_6] = Rect(b_numPos100_6->GetDrawRect());

        D2DSprite* o_numPos100_7 = countNumOrangePos100[7]->GetComponent<D2DSprite>();
        o_numPos100_7->SetSpriteFilePath(L"sprites/UI/inGame/combo/7_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_7] = Rect(o_numPos100_7->GetDrawRect());

        D2DSprite* b_numPos100_7 = countNumBluePos100[7]->GetComponent<D2DSprite>();
        b_numPos100_7->SetSpriteFilePath(L"sprites/UI/inGame/combo/7_blue.png");
        GS_UISpriteInitRectList[b_numPos100_7] = Rect(b_numPos100_7->GetDrawRect());

        D2DSprite* o_numPos100_8 = countNumOrangePos100[8]->GetComponent<D2DSprite>();
        o_numPos100_8->SetSpriteFilePath(L"sprites/UI/inGame/combo/8_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_8] = Rect(o_numPos100_8->GetDrawRect());

        D2DSprite* b_numPos100_8 = countNumBluePos100[8]->GetComponent<D2DSprite>();
        b_numPos100_8->SetSpriteFilePath(L"sprites/UI/inGame/combo/8_blue.png");
        GS_UISpriteInitRectList[b_numPos100_8] = Rect(b_numPos100_8->GetDrawRect());

        D2DSprite* o_numPos100_9 = countNumOrangePos100[9]->GetComponent<D2DSprite>();
        o_numPos100_9->SetSpriteFilePath(L"sprites/UI/inGame/combo/9_yellow.png");
        GS_UISpriteInitRectList[o_numPos100_9] = Rect(o_numPos100_9->GetDrawRect());

		D2DSprite* b_numPos100_9 = countNumBluePos100[9]->GetComponent<D2DSprite>();
		b_numPos100_9->SetSpriteFilePath(L"sprites/UI/inGame/combo/9_blue.png");
		GS_UISpriteInitRectList[b_numPos100_9] = Rect(b_numPos100_9->GetDrawRect());
	}
	
	//timerInGame.duration = 1;
	//timerInGame.onUpdate = [this](double normT)
	//{
	//	for (auto each : comboSpriteList)
	//	{
	//		each->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
	//	}
	//	comboAll->GetTransform()->position = Vector2d::Lerp(Player::GetInstance()->GetPlayerPos(), Player::GetInstance()->GetPlayerPos() + Vector2d(0, floatingDistance), normT);
	//};

    bossClearTimer.duration = bossDelay;
    bossClearTimer.onExpiration = [=]()
    {
        UIManager::GetInstance()->ResetUI(UIEnum::GameClearUI);
        UIManager::GetInstance()->ShowUI(UIEnum::GameClearUI);
    };

    comboTimer1.duration = 0.5;
    comboTimer10.duration = 0.5;
    comboTimer100.duration = 0.5;


    /// 게이지 중심
    double gauge_x = -600;
    double gauge_y = 385;

    /// life 관련
    {
        double x = -199;
        double y = 89;
        double dx = 86;

        D2DSprite* life1 = life1UI->AddComponent<D2DSprite>();
        life1->SetSpriteFilePath(L"sprites/UI/inGame/life/life.png");
        life1UI->GetTransform()->position = Vector2d(gauge_x + x + dx * 0, gauge_y + y);

        (GS_UISpriteList[this]).push_back(life1);
        GS_UISpriteInitRectList[life1] = Rect(life1->GetDrawRect());
        GS_UISpriteInitPosList[life1] = life1UI->GetTransform()->position;

        D2DSprite* life2 = life2UI->AddComponent<D2DSprite>();
        life2->SetSpriteFilePath(L"sprites/UI/inGame/life/life.png");
        life2UI->GetTransform()->position = Vector2d(gauge_x + x + dx * 1, gauge_y + y);

        (GS_UISpriteList[this]).push_back(life2);
        GS_UISpriteInitRectList[life2] = Rect(life2->GetDrawRect());
        GS_UISpriteInitPosList[life2] = life2UI->GetTransform()->position;
    }

    /// 게이지 관련
    {
        double start_x = -214;
        double dx = 38.5;

        D2DSprite* gaugeCharge = gauge->AddGameObject()->AddComponent<D2DSprite>();
        gaugeCharge->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_charge.png");

        (GS_UISpriteList[this]).push_back(gaugeCharge);
        GS_UISpriteInitRectList[gaugeCharge] = Rect(gaugeCharge->GetDrawRect());
        GS_UISpriteInitPosList[gaugeCharge] = Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon1Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon1 = gaugeNon1Obj->AddComponent<D2DSprite>();
        gaugeNon1->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non_0.png");
        gaugeNon1Obj->GetTransform()->position = Vector2d(start_x + dx * 0, 0);
        gauge_non[0] = gaugeNon1Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon1);
        GS_UISpriteInitRectList[gaugeNon1] = Rect(gaugeNon1->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon1] = gaugeNon1Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon2Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon2 = gaugeNon2Obj->AddComponent<D2DSprite>();
        gaugeNon2->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon2Obj->GetTransform()->position = Vector2d(start_x + dx * 1, 0);
        gauge_non[1] = gaugeNon2Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon2);
        GS_UISpriteInitRectList[gaugeNon2] = Rect(gaugeNon2->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon2] = gaugeNon2Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon3Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon3 = gaugeNon3Obj->AddComponent<D2DSprite>();
        gaugeNon3->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon3Obj->GetTransform()->position = Vector2d(start_x + dx * 2, 0);
        gauge_non[2] = gaugeNon3Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon3);
        GS_UISpriteInitRectList[gaugeNon3] = Rect(gaugeNon3->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon3] = gaugeNon3Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon4Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon4 = gaugeNon4Obj->AddComponent<D2DSprite>();
        gaugeNon4->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon4Obj->GetTransform()->position = Vector2d(start_x + dx * 3, 0);
        gauge_non[3] = gaugeNon4Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon4);
        GS_UISpriteInitRectList[gaugeNon4] = Rect(gaugeNon4->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon4] = gaugeNon4Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon5Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon5 = gaugeNon5Obj->AddComponent<D2DSprite>();
        gaugeNon5->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon5Obj->GetTransform()->position = Vector2d(start_x + dx * 4, 0);
        gauge_non[4] = gaugeNon5Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon5);
        GS_UISpriteInitRectList[gaugeNon5] = Rect(gaugeNon5->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon5] = gaugeNon5Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon6Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon6 = gaugeNon6Obj->AddComponent<D2DSprite>();
        gaugeNon6->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon6Obj->GetTransform()->position = Vector2d(start_x + dx * 5, 0);
        gauge_non[5] = gaugeNon6Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon6);
        GS_UISpriteInitRectList[gaugeNon6] = Rect(gaugeNon6->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon6] = gaugeNon6Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon7Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon7 = gaugeNon7Obj->AddComponent<D2DSprite>();
        gaugeNon7->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon7Obj->GetTransform()->position = Vector2d(start_x + dx * 6, 0);
        gauge_non[6] = gaugeNon7Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon7);
        GS_UISpriteInitRectList[gaugeNon7] = Rect(gaugeNon7->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon7] = gaugeNon7Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon8Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon8 = gaugeNon8Obj->AddComponent<D2DSprite>();
        gaugeNon8->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon8Obj->GetTransform()->position = Vector2d(start_x + dx * 7, 0);
        gauge_non[7] = gaugeNon8Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon8);
        GS_UISpriteInitRectList[gaugeNon8] = Rect(gaugeNon8->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon8] = gaugeNon8Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon9Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon9 = gaugeNon9Obj->AddComponent<D2DSprite>();
        gaugeNon9->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon9Obj->GetTransform()->position = Vector2d(start_x + dx * 8, 0);
        gauge_non[8] = gaugeNon9Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon9);
        GS_UISpriteInitRectList[gaugeNon9] = Rect(gaugeNon9->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon9] = gaugeNon9Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon10Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon10 = gaugeNon10Obj->AddComponent<D2DSprite>();
        gaugeNon10->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon10Obj->GetTransform()->position = Vector2d(start_x + dx * 9, 0);
        gauge_non[9] = gaugeNon10Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon10);
        GS_UISpriteInitRectList[gaugeNon10] = Rect(gaugeNon10->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon10] = gaugeNon10Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon11Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon11 = gaugeNon11Obj->AddComponent<D2DSprite>();
        gaugeNon11->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon11Obj->GetTransform()->position = Vector2d(start_x + dx * 10, 0);
        gauge_non[10] = gaugeNon11Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon11);
        GS_UISpriteInitRectList[gaugeNon11] = Rect(gaugeNon11->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon11] = gaugeNon11Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        GameObject* gaugeNon12Obj = gauge->AddGameObject();
        D2DSprite* gaugeNon12 = gaugeNon12Obj->AddComponent<D2DSprite>();
        gaugeNon12->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_non.png");
        gaugeNon12Obj->GetTransform()->position = Vector2d(start_x + dx * 11, 0);
        gauge_non[11] = gaugeNon12Obj;

        (GS_UISpriteList[this]).push_back(gaugeNon12);
        GS_UISpriteInitRectList[gaugeNon12] = Rect(gaugeNon12->GetDrawRect());
        GS_UISpriteInitPosList[gaugeNon12] = gaugeNon12Obj->GetTransform()->position + Vector2d(gauge_x, gauge_y);

        ///

        D2DSprite* gaugeCover = gauge->AddGameObject()->AddComponent<D2DSprite>();
        gaugeCover->SetSpriteFilePath(L"sprites/UI/inGame/gauge/gauge_cover.png");

        (GS_UISpriteList[this]).push_back(gaugeCover);
        GS_UISpriteInitRectList[gaugeCover] = Rect(gaugeCover->GetDrawRect());
        GS_UISpriteInitPosList[gaugeCover] = Vector2d(gauge_x, gauge_y);

        gauge->GetTransform()->position = Vector2d(gauge_x, gauge_y);
    }

    /// 스택 관련
    {
        double x = 354;
        double dx = 52;

        D2DSprite* stack1Sprite = stack1->AddComponent<D2DSprite>();
        stack1Sprite->SetSpriteFilePath(L"sprites/UI/inGame/gauge/stack.png");
        stack1->GetTransform()->position = Vector2d(gauge_x + x + dx * 0, gauge_y);

        (GS_UISpriteList[this]).push_back(stack1Sprite);
        GS_UISpriteInitRectList[stack1Sprite] = Rect(stack1Sprite->GetDrawRect());
        GS_UISpriteInitPosList[stack1Sprite] = stack1->GetTransform()->position;

        D2DSprite* stack2Sprite = stack2->AddComponent<D2DSprite>();
        stack2Sprite->SetSpriteFilePath(L"sprites/UI/inGame/gauge/stack.png");
        stack2->GetTransform()->position = Vector2d(gauge_x + x + dx * 1, gauge_y);

        (GS_UISpriteList[this]).push_back(stack2Sprite);
        GS_UISpriteInitRectList[stack2Sprite] = Rect(stack2Sprite->GetDrawRect());
        GS_UISpriteInitPosList[stack2Sprite] = stack2->GetTransform()->position;

        D2DSprite* stack3Sprite = stack3->AddComponent<D2DSprite>();
        stack3Sprite->SetSpriteFilePath(L"sprites/UI/inGame/gauge/stack.png");
        stack3->GetTransform()->position = Vector2d(gauge_x + x + dx * 2, gauge_y);

        (GS_UISpriteList[this]).push_back(stack3Sprite);
        GS_UISpriteInitRectList[stack3Sprite] = Rect(stack3Sprite->GetDrawRect());
        GS_UISpriteInitPosList[stack3Sprite] = stack3->GetTransform()->position;
    }

    /// hp 관련
    {
        double x = -238;
        double y = -90;
        double dx = 92;

        D2DSprite* hp1Sprite = hp1->AddComponent<D2DSprite>();
        hp1Sprite->SetSpriteFilePath(L"sprites/UI/inGame/hp/hp.png");
        hp1->GetTransform()->position = Vector2d(gauge_x + x + dx * 0, gauge_y + y);

        (GS_UISpriteList[this]).push_back(hp1Sprite);
        GS_UISpriteInitRectList[hp1Sprite] = Rect(hp1Sprite->GetDrawRect());
        GS_UISpriteInitPosList[hp1Sprite] = hp1->GetTransform()->position;

        D2DSprite* hp2Sprite = hp2->AddComponent<D2DSprite>();
        hp2Sprite->SetSpriteFilePath(L"sprites/UI/inGame/hp/hp.png");
        hp2->GetTransform()->position = Vector2d(gauge_x + x + dx * 1, gauge_y + y);

        (GS_UISpriteList[this]).push_back(hp2Sprite);
        GS_UISpriteInitRectList[hp2Sprite] = Rect(hp2Sprite->GetDrawRect());
        GS_UISpriteInitPosList[hp2Sprite] = hp2->GetTransform()->position;

        D2DSprite* hp3Sprite = hp3->AddComponent<D2DSprite>();
        hp3Sprite->SetSpriteFilePath(L"sprites/UI/inGame/hp/hp.png");
        hp3->GetTransform()->position = Vector2d(gauge_x + x + dx * 2, gauge_y + y);

        (GS_UISpriteList[this]).push_back(hp3Sprite);
        GS_UISpriteInitRectList[hp3Sprite] = Rect(hp3Sprite->GetDrawRect());
        GS_UISpriteInitPosList[hp3Sprite] = hp3->GetTransform()->position;
    }

    ResetThisUI();
}

void UIManager::InGame::ResetThisUI()
{
	uIFunctionTimer.isActive = false;
	timerInGame.isActive = false;
	comboTimer1.isActive = false;
	comboTimer10.isActive = false;
	comboTimer100.isActive = false;
	bossClearTimer.isActive = false;
	dirNumDown = true;
	comboFadeInFinish = false;
	life1UI->SetSelfActive(true);
	life2UI->SetSelfActive(true);
	stack1->SetSelfActive(false);
	stack2->SetSelfActive(false);
	stack3->SetSelfActive(false);
	hp1->SetSelfActive(true);
	hp2->SetSelfActive(true);
	hp3->SetSelfActive(true);
	for (auto each : gauge_non)
	{
		each->SetSelfActive(true);
	}
	comboAll->SetSelfActive(false);

    for (auto each : GS_UISpriteList[this])
    {
        each->color.a = 1.0f;
    }

    //for (auto each : comboSpriteList)
    //{
    //    each->GetGameObject()->SetSelfActive(true);
    //    each->GetTransform()->position = Vector2d(0, -2160);
    //    each->color.a = 0;
    //}

    nowNumPos1 = blankNumPos1;
    nowNumPos10 = blankNumPos10;
    nowNumPos100 = blankNumPos100;
}

void UIManager::InGame::ShowThisUI(GameObject* targetObj)
{
    SoundSystem::StopMusic();
    SoundSystem::PlayMusic("sounds/ui/Ingame.wav");
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(11);
    GetGameObject()->SetSelfActive(true);
}

void UIManager::InGame::HideThisUI()
{
	uIFunctionTimer.isActive = false;
	timerInGame.isActive = false;
	comboTimer1.isActive = false;
	comboTimer10.isActive = false;
	comboTimer100.isActive = false;
	bossClearTimer.isActive = false;
	GetGameObject()->SetSelfActive(false);
}

void UIManager::InGame::ChangeNumberEffect(GameObject* beforeNum, GameObject* nextNum, TimerIndex timerIndex)
{
    /* D2DSprite* beforeNumSprite = beforeNum->GetComponent<D2DSprite>();
    D2DSprite* nextNumSprite = nextNum->GetComponent<D2DSprite>();

    nextNumSprite->color.a = 0;
    nextNum->GetTransform()->position = Vector2d(0, numberCreatePos);

    switch (timerIndex)
    {
    case  TimerIndex::Pos1:
        comboTimer1.Start();
        comboTimer1.onUpdate = [=](double normT)
        {
            beforeNumSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 1.0f), Vector2d(0, 0), normT)).y;
            nextNumSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
            if (dirNumDown && (nextNum->GetTransform()->position.y < -20))
            {
                dirNumDown = false;
                nextNum->GetTransform()->position.y += 2;
            }
            else if (!dirNumDown && (nextNum->GetTransform()->position.y == 0))
            {

            }
            else
            {
                nextNum->GetTransform()->position.y -= 2;
            }
            beforeNum->GetTransform()->position.y -= 2;
        };

			comboTimer1.onExpiration = [=]()
			{
				beforeNum->GetTransform()->position = Vector2d(0, -2160);
				nextNum->GetTransform()->position = Vector2d(0, 0);
				beforeNumSprite->color.a = 0;
				nextNumSprite->color.a = 1.0f;
			};
			break;
		case  TimerIndex::Pos10:
			comboTimer10.Start();
			comboTimer10.onUpdate = [=](double normT)
			{
				beforeNumSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 1.0f), Vector2d(0, 0), normT)).y;
				nextNumSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
				if (dirNumDown && (nextNum->GetTransform()->position.y < -20))
				{
					dirNumDown = false;
					nextNum->GetTransform()->position.y += 2;
				}
				else if (!dirNumDown && (nextNum->GetTransform()->position.y == 0))
				{

            }
            else
            {
                nextNum->GetTransform()->position.y -= 2;
            }
            beforeNum->GetTransform()->position.y -= 2;
        };

			comboTimer10.onExpiration = [=]()
			{
				beforeNum->GetTransform()->position = Vector2d(0, -2160);
				nextNum->GetTransform()->position = Vector2d(0, 0);
				beforeNumSprite->color.a = 0;
				nextNumSprite->color.a = 1.0f;
			};
			break;
		case  TimerIndex::Pos100:
			comboTimer100.Start();
			comboTimer100.onUpdate = [=](double normT)
			{
				beforeNumSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 1.0f), Vector2d(0, 0), normT)).y;
				nextNumSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
				if (dirNumDown && (nextNum->GetTransform()->position.y < -20))
				{
					dirNumDown = false;
					nextNum->GetTransform()->position.y += 2;
				}
				else if (!dirNumDown && (nextNum->GetTransform()->position.y == 0))
				{

            }
            else
            {
                nextNum->GetTransform()->position.y -= 2;
            }
            beforeNum->GetTransform()->position.y -= 2;
        };

			comboTimer100.onExpiration = [=]()
			{
				beforeNum->GetTransform()->position = Vector2d(0, -2160);
				nextNum->GetTransform()->position = Vector2d(0, 0);
				beforeNumSprite->color.a = 0;
				nextNumSprite->color.a = 1.0f;
			};
			break;
		default:
			break;
	}*/
}

/// PlayerDeath 영역

UIManager::PlayerDeath* UIManager::PlayerDeath::instance = nullptr;

UIManager::PlayerDeath::PlayerDeath()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::PlayerDeathUI] = this;
}

UIManager::PlayerDeath::~PlayerDeath()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::PlayerDeathUI);
    UIList.erase(UIEnum::PlayerDeathUI);
}

void UIManager::PlayerDeath::Update()
{
    uIFunctionTimer.Update();
    timer.Update();

    if (timer.isActive)
    {
        for (size_t i = 0; i < 9; ++i)
        {
            if (timer.elapsed > interval * (i + 1))
            {
                countNum[9 - i]->SetSelfActive(false);
                countNum[8 - i]->SetSelfActive(true);
            }
        }

        if (downText->GetTransform()->position.y > downTextDefaultPos + range)
        {
            vibrationDirDown = true;
        }
        else if (downText->GetTransform()->position.y < downTextDefaultPos - range)
        {
            vibrationDirDown = false;
        }

        if (vibrationDirDown)
        {
            downText->GetTransform()->position.y -= rangeSpeed;
        }
        else
        {
            downText->GetTransform()->position.y += rangeSpeed;
        }

        if (D2DInput::isKeyPushed(KeyCode::Z) || D2DInput::isKeyPushed(KeyCode::X) || D2DInput::isKeyPushed(KeyCode::C) || D2DInput::isKeyPushed(KeyCode::V))
        {
            timer.elapsed += interval;
        }

        if (D2DInput::isKeyPushed(KeyCode::R))
        {
            ResetThisUI();
            HideThisUI();
            Player* player = Player::GetInstance();
            if (player)
            {
                player->Revive();
            }
        }
    }
}

void UIManager::PlayerDeath::Initialize()
{
    GameObject* playerDeathObj = GetGameObject();
    vector<D2DSprite*> countNumSprites(10);

    backgroundCover = playerDeathObj->AddGameObject();
    upText = playerDeathObj->AddGameObject();
    downText = playerDeathObj->AddGameObject();
    for (size_t i = 0; i < 10; ++i)
    {
        countNum[i] = playerDeathObj->AddGameObject();
        countNumSprites[i] = countNum[i]->AddComponent<D2DSprite>();
    }

    timer.duration = interval * 10;
    timer.onExpiration = [=]()
    {
        uIFunctionTimer.isActive = false;
        timer.isActive = false;
        upText->SetSelfActive(false);
        downText->SetSelfActive(false);
        countNum[0]->SetSelfActive(false);
        GS_UIList[UIEnum::GameOverUI]->ResetThisUI();
        UIManager::GetInstance()->ShowUI(UIEnum::GameOverUI, FadeOption::FadeIn);
    };

    D2DSprite* backgroundCoverSprite = backgroundCover->AddComponent<D2DSprite>();
    backgroundCoverSprite->SetSpriteFilePath(L"sprites/UI/playerDeath/backgroundCover.png");

    (GS_UISpriteList[this]).push_back(backgroundCoverSprite);
    GS_UISpriteInitRectList[backgroundCoverSprite] = Rect(backgroundCoverSprite->GetDrawRect());
    GS_UISpriteInitPosList[backgroundCoverSprite] = Vector2d(0, 0);

    D2DSprite* upTextSprite = upText->AddComponent<D2DSprite>();
    upTextSprite->SetSpriteFilePath(L"sprites/UI/playerDeath/upText.png");
    upText->GetTransform()->position = Vector2d(0, 222);

    (GS_UISpriteList[this]).push_back(upTextSprite);
    GS_UISpriteInitRectList[upTextSprite] = Rect(upTextSprite->GetDrawRect());
    GS_UISpriteInitPosList[upTextSprite] = upText->GetTransform()->position;

    D2DSprite* downTextSprite = downText->AddComponent<D2DSprite>();
    downTextSprite->SetSpriteFilePath(L"sprites/UI/playerDeath/downText.png");
    downText->GetTransform()->position = Vector2d(0, downTextDefaultPos);

    (GS_UISpriteList[this]).push_back(downTextSprite);
    GS_UISpriteInitRectList[downTextSprite] = Rect(downTextSprite->GetDrawRect());
    GS_UISpriteInitPosList[downTextSprite] = downText->GetTransform()->position;

    countNumSprites[0]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count0.png");
    countNumSprites[1]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count1.png");
    countNumSprites[2]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count2.png");
    countNumSprites[3]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count3.png");
    countNumSprites[4]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count4.png");
    countNumSprites[5]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count5.png");
    countNumSprites[6]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count6.png");
    countNumSprites[7]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count7.png");
    countNumSprites[8]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count8.png");
    countNumSprites[9]->SetSpriteFilePath(L"sprites/UI/playerDeath/countNum/count9.png");

    for (size_t i = 0; i < 10; ++i)
    {
        (GS_UISpriteList[this]).push_back(countNumSprites[i]);
        GS_UISpriteInitRectList[countNumSprites[i]] = Rect(countNumSprites[i]->GetDrawRect());
        GS_UISpriteInitPosList[countNumSprites[i]] = Vector2d(0, 0);
    }

    ResetThisUI();
}

void UIManager::PlayerDeath::ResetThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    downText->GetTransform()->position = Vector2d(0, downTextDefaultPos);
    for (size_t i = 0; i < 10; ++i)
    {
        countNum[i]->SetSelfActive(false);
    }
    countNum[9]->SetSelfActive(true);
    upText->SetSelfActive(true);
    downText->SetSelfActive(true);

    for (auto each : GS_UISpriteList[this])
    {
        each->color.a = 1.0f;
    }
}

void UIManager::PlayerDeath::ShowThisUI(GameObject* targetObj)
{
    timer.Start();
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(13);
    GetGameObject()->SetSelfActive(true);
}

void UIManager::PlayerDeath::HideThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    GetGameObject()->SetSelfActive(false);
}



/// Credit 영역

UIManager::Credit* UIManager::Credit::instance = nullptr;

UIManager::Credit::Credit()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::CreditUI] = this;
}

UIManager::Credit::~Credit()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::CreditUI);
    UIList.erase(UIEnum::CreditUI);
}

void UIManager::Credit::Update()
{
    uIFunctionTimer.Update();
    timer.Update();

    if (D2DInput::isKeyPushed(KeyCode::ESC))
    {
        UIManager* uIManagerInstance = UIManager::GetInstance();
        uIManagerInstance->HideUI(UIEnum::CreditUI, FadeOption::FadeOut);
        uIManagerInstance->ResetUI(UIEnum::TitleUI);
        uIManagerInstance->ShowUI(UIEnum::TitleUI, ScaleOption::ScaleDown, 1.8);
    }
}

void UIManager::Credit::Initialize()
{
    GameObject* creditObj = GetGameObject();

    background = creditObj->AddGameObject();
    credit = creditObj->AddGameObject();
    artwork = creditObj->AddGameObject();

    D2DSprite* backgroundSprite = background->AddComponent<D2DSprite>();
    backgroundSprite->SetSpriteFilePath(L"sprites/UI/credit/background.png");

    (GS_UISpriteList[this]).push_back(backgroundSprite);
    GS_UISpriteInitRectList[backgroundSprite] = Rect(backgroundSprite->GetDrawRect());
    GS_UISpriteInitPosList[backgroundSprite] = Vector2d(0, 0);

    D2DSprite* creditSprite = credit->AddComponent<D2DSprite>();
    creditSprite->SetSpriteFilePath(L"sprites/UI/credit/credit.png");
    credit->GetTransform()->position = Vector2d(-322, -1240);

    (GS_UISpriteList[this]).push_back(creditSprite);
    GS_UISpriteInitRectList[creditSprite] = Rect(creditSprite->GetDrawRect());
    GS_UISpriteInitPosList[creditSprite] = credit->GetTransform()->position;

    D2DAnimatedSprite* down = artwork->AddGameObject()->AddComponent<D2DAnimatedSprite>();
    down->LoadAnimationFromFile(L"sprites/UI/credit/artwork_down");
    down->SetWidth(700);
    down->SetHeight(700);

    (GS_UIAnimatedSpriteList[this]).push_back(down);
    GS_UIAnimatedSpriteInitRectList[down] = Rect(down->GetWidth(), down->GetHeight());
    GS_UIAnimatedSpriteInitPosList[down] = Vector2d(0, 0);

    D2DAnimatedSprite* up = artwork->AddGameObject()->AddComponent<D2DAnimatedSprite>();
    up->LoadAnimationFromFile(L"sprites/UI/credit/artwork_up");
    up->SetWidth(700);
    up->SetHeight(700);

    (GS_UIAnimatedSpriteList[this]).push_back(up);
    GS_UIAnimatedSpriteInitRectList[up] = Rect(up->GetWidth(), up->GetHeight());
    GS_UIAnimatedSpriteInitPosList[up] = Vector2d(0, 0);

    artwork->GetTransform()->position = Vector2d(340, 100);

    timer.duration = 15;
    timer.onUpdate = [=](double normT) { credit->GetTransform()->position = Vector2d::Lerp(Vector2d(-322, -1240), Vector2d(-322, 1240), normT); };
    timer.onExpiration = [=]()
    {
        UIManager::GetInstance()->HideUI(UIEnum::CreditUI, FadeOption::FadeOut);
        UIManager::GetInstance()->ResetUI(UIEnum::TitleUI);
        UIManager::GetInstance()->ShowUI(UIEnum::TitleUI);
    };
}

void UIManager::Credit::ResetThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    credit->GetTransform()->position = Vector2d(-322, -1240);
}

void UIManager::Credit::ShowThisUI(GameObject* targetObj)
{
    timer.Start();
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(16);
    GetGameObject()->SetSelfActive(true);
}

void UIManager::Credit::HideThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    GetGameObject()->SetSelfActive(false);
}



/// GameOver 영역

UIManager::GameOver* UIManager::GameOver::instance = nullptr;

UIManager::GameOver::GameOver()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::GameOverUI] = this;
}

UIManager::GameOver::~GameOver()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::GameOverUI);
    UIList.erase(UIEnum::GameOverUI);
}

void UIManager::GameOver::Update()
{
    uIFunctionTimer.Update();
    timer.Update();
}

void UIManager::GameOver::Initialize()
{
    GameObject* gameOverObj = GetGameObject();

    background = gameOverObj->AddGameObject();
    gameover = gameOverObj->AddGameObject();

    D2DSprite* backgroundSprite = background->AddComponent<D2DSprite>();
    backgroundSprite->SetSpriteFilePath(L"sprites/UI/gameover/backgroundCover.png");

    (GS_UISpriteList[this]).push_back(backgroundSprite);
    GS_UISpriteInitRectList[backgroundSprite] = Rect(backgroundSprite->GetDrawRect());
    GS_UISpriteInitPosList[backgroundSprite] = Vector2d(0, 0);

    D2DSprite* gameoverSprite = gameover->AddComponent<D2DSprite>();
    gameoverSprite->SetSpriteFilePath(L"sprites/UI/gameover/gameover_text01.png");
    maxRect = gameoverSprite->GetDrawRect();

    (GS_UISpriteList[this]).push_back(gameoverSprite);
    GS_UISpriteInitRectList[gameoverSprite] = Rect(gameoverSprite->GetDrawRect());
    GS_UISpriteInitPosList[gameoverSprite] = Vector2d(0, 0);

    timer.duration = playTime;
    timer.onUpdate = [=](double normT)
    {
        Vector2d indicatorVector = Vector2d::Lerp(Vector2d(maxRect.width * initialScale, maxRect.height * initialScale), Vector2d(maxRect.width, maxRect.height), normT);
        gameoverSprite->SetDrawRect(Rect(indicatorVector.x, indicatorVector.y));
        gameoverSprite->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
        beforeRect = gameoverSprite->GetDrawRect();
        if ((int)(timer.elapsed / vibrationPeriod) % 2)
        {
            gameoverSprite->SetSpriteFilePath(L"sprites/UI/gameover/gameover_text01.png");
            gameoverSprite->SetDrawRect(beforeRect);
        }
        else
        {
            gameoverSprite->SetSpriteFilePath(L"sprites/UI/gameover/gameover_text02.png");
            gameoverSprite->SetDrawRect(beforeRect);
        }
       
    };
    timer.onExpiration = [=]()
    {
        UIManager* uIManagerInstance = UIManager::GetInstance();
        MapTool::GetInstance()->DisableMap();
        uIManagerInstance->ResetUI(UIManager::UIEnum::InGameUI);
        uIManagerInstance->HideUI(UIManager::UIEnum::InGameUI);
        uIManagerInstance->HideUI(UIManager::UIEnum::PlayerDeathUI);
        uIManagerInstance->HideUI(UIEnum::GameOverUI, FadeOption::FadeOut);
        uIManagerInstance->ResetUI(UIEnum::TitleUI);
        uIManagerInstance->ShowUI(UIEnum::TitleUI, FadeOption::FadeIn);
    };

    ResetThisUI();
}

void UIManager::GameOver::ResetThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    nowText1 = true;
    beforeRect = maxRect;
    D2DSprite* gameoverSprite = gameover->GetComponent<D2DSprite>();
    gameoverSprite->SetDrawRect(Rect(maxRect.width * initialScale, maxRect.height * initialScale));
    gameoverSprite->SetSpriteFilePath(L"sprites/UI/gameover/gameover_text01.png");
    gameoverSprite->color.a = 0;
}

void UIManager::GameOver::ShowThisUI(GameObject* targetObj)
{
    timer.Start();
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(14);
    GetGameObject()->SetSelfActive(true);
}

void UIManager::GameOver::HideThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    GetGameObject()->SetSelfActive(false);
}



/// GameClear 영역

UIManager::GameClear* UIManager::GameClear::instance = nullptr;

UIManager::GameClear::GameClear()
{
    if (!instance)
        instance = this;
    GS_UIList[UIEnum::GameClearUI] = this;
}

UIManager::GameClear::~GameClear()
{
    if (instance == this)
        instance = nullptr;
    GS_UIList.erase(UIEnum::GameClearUI);
    UIList.erase(UIEnum::GameClearUI);
}

void UIManager::GameClear::Update()
{
    uIFunctionTimer.Update();
    timer.Update();
    for (size_t i = 0; i < 7; ++i)
    {
        wordTimerUp[i].Update();
    }
    for (size_t i = 0; i < 9; ++i)
    {
        wordTimerDown[i].Update();
    }
}

void UIManager::GameClear::Initialize()
{
    GameObject* gameClearObj = GetGameObject();

    background = gameClearObj->AddGameObject();
    gameClearUp = gameClearObj->AddGameObject();
    gameClearDown = gameClearObj->AddGameObject();

    double x_up = -526;
    double y_up = 140;
    double dx_up = 170;
    double x_down = -636;
    double y_down = -42;
    double dx_down = 170;

    gameClearUp->GetTransform()->position = Vector2d(0, y_up);
    gameClearDown->GetTransform()->position = Vector2d(0, y_down);

    D2DSprite* backgroundSprite = background->AddComponent<D2DSprite>();
    backgroundSprite->SetSpriteFilePath(L"sprites/UI/gameclear/backgroundCover.png");

    (GS_UISpriteList[this]).push_back(backgroundSprite);
    GS_UISpriteInitRectList[backgroundSprite] = Rect(backgroundSprite->GetDrawRect());
    GS_UISpriteInitPosList[backgroundSprite] = Vector2d(0, 0);

    for (size_t i = 0; i < 7; ++i)
    {
        gameClearUpLetterList[i] = gameClearUp->AddGameObject()->AddComponent<D2DSprite>();
        gameClearUpLetterList[i]->GetGameObject()->GetTransform()->position = Vector2d(x_up + dx_up * i, 0);
        (GS_UISpriteList[this]).push_back(gameClearUpLetterList[i]);
        GS_UISpriteInitPosList[gameClearUpLetterList[i]] = Vector2d(x_up + dx_up * i, 0);
    }

    for (size_t i = 0; i < 8; ++i)
    {
        gameClearDownLetterList[i] = gameClearDown->AddGameObject()->AddComponent<D2DSprite>();
        gameClearDownLetterList[i]->GetGameObject()->GetTransform()->position = Vector2d(x_down + dx_down * i, 0);
        (GS_UISpriteList[this]).push_back(gameClearDownLetterList[i]);
        GS_UISpriteInitPosList[gameClearDownLetterList[i]] = Vector2d(x_down + dx_down * i, 0);
    }

    mark = gameClearDown->AddGameObject()->AddComponent<D2DSprite>();
    mark->SetSpriteFilePath(L"sprites/UI/gameclear/mark.png");
    mark->GetTransform()->position = Vector2d(x_down + dx_down * 7 + 150, 0);
    (GS_UISpriteList[this]).push_back(mark);
    GS_UISpriteInitRectList[mark] = Rect(mark->GetDrawRect());
    GS_UISpriteInitPosList[mark] = Vector2d(x_down + dx_down * 7 + 150, 0);

    gameClearDownLetterList[8] = mark;

    gameClearUpLetterList[0]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_01.png");
    gameClearUpLetterList[1]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_02.png");
    gameClearUpLetterList[2]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_03.png");
    gameClearUpLetterList[3]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_04.png");
    gameClearUpLetterList[4]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_05.png");
    gameClearUpLetterList[5]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_06.png");
    gameClearUpLetterList[6]->SetSpriteFilePath(L"sprites/UI/gameclear/mission_07.png");

    gameClearDownLetterList[0]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_01.png");
    gameClearDownLetterList[1]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_02.png");
    gameClearDownLetterList[2]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_03.png");
    gameClearDownLetterList[3]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_04.png");
    gameClearDownLetterList[4]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_05.png");
    gameClearDownLetterList[5]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_06.png");
    gameClearDownLetterList[6]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_07.png");
    gameClearDownLetterList[7]->SetSpriteFilePath(L"sprites/UI/gameclear/complete_08.png");

    for (auto each : gameClearUpLetterList)
    {
        GS_UISpriteInitRectList[each] = Rect(each->GetDrawRect());
    }

    for (auto each : gameClearDownLetterList)
    {
        GS_UISpriteInitRectList[each] = Rect(each->GetDrawRect());
    }


    timer.duration = playTime;
    timer.onUpdate = [=](double normT)
    {
        double deltaTime = Time::GetDeltaTimeUnscaled();
        if (timer.elapsed >= interval * 15 + markDelay && timer.elapsed < interval * 15 + markDelay + deltaTime)
        {
            DropDownLetter(8);
        }
        else if (timer.elapsed >= interval * 15 && timer.elapsed < interval * 15 + deltaTime)
        {
            DropDownLetter(7);
        }
		else if (timer.elapsed >= interval * 14 && timer.elapsed < interval * 14 + deltaTime)
		{
			DropUpLetter(6);
		}
		else if (timer.elapsed >= interval * 13 && timer.elapsed < interval * 13 + deltaTime)
		{
			DropDownLetter(6);
		}
		else if (timer.elapsed >= interval * 12 && timer.elapsed < interval * 12 + deltaTime)
		{
            DropUpLetter(5);
		}
		else if (timer.elapsed >= interval * 11 && timer.elapsed < interval * 11 + deltaTime)
		{
			DropDownLetter(5);
		}
		else if (timer.elapsed >= interval * 10 && timer.elapsed < interval * 10 + deltaTime)
		{
            DropUpLetter(4);
		}
		else if (timer.elapsed >= interval * 9 && timer.elapsed < interval * 9 + deltaTime)
		{
			DropDownLetter(4);
		}
		else if (timer.elapsed >= interval * 8 && timer.elapsed < interval * 8 + deltaTime)
		{
            DropUpLetter(3);
		}
		else if (timer.elapsed >= interval * 7 && timer.elapsed < interval * 7 + deltaTime)
		{
			DropDownLetter(3);
		}
		else if (timer.elapsed >= interval * 6 && timer.elapsed < interval * 6 + deltaTime)
		{
            DropUpLetter(2);
		}
		else if (timer.elapsed >= interval * 5 && timer.elapsed < interval * 5 + deltaTime)
		{
			DropDownLetter(2);
		}
		else if (timer.elapsed >= interval * 4 && timer.elapsed < interval * 4 + deltaTime)
		{
            DropUpLetter(1);
		}
		else if (timer.elapsed >= interval * 3 && timer.elapsed < interval * 3 + deltaTime)
		{
			DropDownLetter(1);
		}
		else if (timer.elapsed >= interval * 2 && timer.elapsed < interval * 2 + deltaTime)
		{
            DropUpLetter(0);
		}
		else if (timer.elapsed >= interval * 1 && timer.elapsed < interval * 1 + deltaTime)
		{
			DropDownLetter(0);
		}
    };

    timer.onExpiration = [=]()
    {
        MapTool::GetInstance()->DisableMap();
        UIManager* uIManagerInstance = UIManager::GetInstance();
        uIManagerInstance->ResetUI(UIManager::UIEnum::InGameUI);
        uIManagerInstance->HideUI(UIManager::UIEnum::InGameUI);
        uIManagerInstance->HideUI(UIManager::UIEnum::GameClearUI, FadeOption::FadeOut);
        GS_UIList[UIManager::UIEnum::CreditUI]->ResetThisUI();
        uIManagerInstance->ShowUI(UIEnum::CreditUI, FadeOption::FadeIn);
        SoundSystem::StopMusic();
        SoundSystem::PlayMusic("sounds/ui/Ending_credit.mp3");
    };

    ResetThisUI();
}

void UIManager::GameClear::ResetThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    mark->color.a = 0;
    for (auto each : gameClearUpLetterList)
    {
        each->color.a = 0;
    }
    for (auto each : gameClearDownLetterList)
    {
        each->color.a = 0;
    }
	for (size_t i = 0; i < 7; ++i)
	{
		wordTimerUp[i].isActive = false;
	}
	for (size_t i = 0; i < 9; ++i)
	{
		wordTimerDown[i].isActive = false;
	}
    background->GetComponent<D2DSprite>()->color.a = 1.0f;
}

void UIManager::GameClear::ShowThisUI(GameObject* targetObj)
{
    timer.Start();
    this->GetGameObject()->SetParent(targetObj);
    GetGameObject()->SetChildIndex(15);
    GetGameObject()->SetSelfActive(true);
}

void UIManager::GameClear::HideThisUI()
{
    uIFunctionTimer.isActive = false;
    timer.isActive = false;
    for (size_t i = 0; i < 7; ++i)
    {
        wordTimerUp[i].isActive = false;
    }
    for (size_t i = 0; i < 9; ++i)
    {
        wordTimerDown[i].isActive = false;
    }
    GetGameObject()->SetSelfActive(false);
}

void UIManager::GameClear::DropUpLetter(size_t index)
{
    wordTimerUp[index].duration = letterDropTime;
    wordTimerUp[index].Start();
    wordTimerUp[index].onUpdate = [=](double normT)
    {
        gameClearUpLetterList[index]->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
        gameClearUpLetterList[index]->GetTransform()->position = Vector2d::Lerp(GS_UISpriteInitPosList[gameClearUpLetterList[index]] + Vector2d(0, letterDropDistance), GS_UISpriteInitPosList[gameClearUpLetterList[index]], normT);
    };
}

void UIManager::GameClear::DropDownLetter(size_t index)
{
    if (index == 8)
    {
        wordTimerDown[index].duration = letterDropTime/2;
        wordTimerDown[index].Start();
        wordTimerDown[index].onUpdate = [=](double normT)
        {
            mark->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
            mark->GetTransform()->position = Vector2d::Lerp(GS_UISpriteInitPosList[mark] + Vector2d(0, -letterDropDistance), GS_UISpriteInitPosList[mark], normT);
        };
        return;
    }

    wordTimerDown[index].duration = letterDropTime;
    wordTimerDown[index].Start();
    wordTimerDown[index].onUpdate = [=](double normT)
    {
        gameClearDownLetterList[index]->color.a = (float)(Vector2d::Lerp(Vector2d(0, 0), Vector2d(0, 1.0f), normT)).y;
        gameClearDownLetterList[index]->GetTransform()->position = Vector2d::Lerp(GS_UISpriteInitPosList[gameClearDownLetterList[index]] + Vector2d(0, -letterDropDistance), GS_UISpriteInitPosList[gameClearDownLetterList[index]], normT);
    };
}
