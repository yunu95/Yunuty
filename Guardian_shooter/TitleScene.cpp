#include "TitleScene.h"
#include "Cursor.h"
#include "Button.h"
#include "DebugObject.h"

TitleScene* TitleScene::instance = nullptr;

TitleScene::TitleScene()
{
	if (!instance)
		instance = this;
}
TitleScene::~TitleScene()
{
	if (instance == this)
		instance = nullptr;
}

void TitleScene::Initialize(Scene* gameScene, D2DCamera* gameCam)
{
	this->gameScene = gameScene;
	this->gameCam = gameCam;
	titleSceneUILayer = AddGameObject();
	creditsLayer = AddGameObject();
	optionLayer = AddGameObject();

	Cursor::CreateCursor();

	for (auto each : { creditsLayer, optionLayer })
		each->SetSelfActive(false);

	// 타이틀 화면 UI 구현
	{
		constexpr double x = -400;
		constexpr double dx = 150;
		constexpr double dy = -120;

		GameObject* backgroundUI = titleSceneUILayer->AddGameObject();
		GameObject* creditButtonUI = titleSceneUILayer->AddGameObject();
		GameObject* playButtonUI = titleSceneUILayer->AddGameObject();
		GameObject* optionButtonUI = titleSceneUILayer->AddGameObject();
		GameObject* exitButtonUI = titleSceneUILayer->AddGameObject();
		GameObject* artwork1UI = titleSceneUILayer->AddGameObject();
		GameObject* artwork2UI = titleSceneUILayer->AddGameObject();
		GameObject* credit = creditsLayer->AddGameObject();
		GameObject* mouseInteractionIcon = titleSceneUILayer->AddGameObject();

		D2DAnimatedSprite* animatedSprite = mouseInteractionIcon->AddComponent<D2DAnimatedSprite>();
		animatedSprite->LoadAnimationFromFile(L"sprites/player/torso/fire_forward");
		mouseInteractionIcon->SetSelfActive(false);

		D2DSprite* background = backgroundUI->AddComponent<D2DSprite>();
		background->SetSpriteFilePath(L"sprites/title/background.png");
		backgroundUI->GetTransform()->SetWorldPosition(Vector2d::zero);
		
		D2DSprite* creditSprite = credit->AddComponent<D2DSprite>();
		creditSprite->SetSpriteFilePath(L"sprites/title/credit.png");
		credit->GetTransform()->SetWorldPosition(Vector2d(0,-1900));
		
		Button* creditButton = CreateTitleButton(creditButtonUI, Vector2d(-400, 300), L"sprites/title/title.png");
		Button* playButton = CreateTitleButton(playButtonUI, Vector2d(x - dx * 0.5, dy * 0), L"sprites/title/play.png");
		Button* optionButton = CreateTitleButton(optionButtonUI, Vector2d(x - dx * 0.5, dy * 1), L"sprites/title/option.png");
		Button* exitButton = CreateTitleButton(exitButtonUI, Vector2d(x - dx * 0.5, dy * 2), L"sprites/title/exit.png");
		Button* artwork1 = CreateTitleButton(artwork1UI, Vector2d(300, 0), L"sprites/title/artwork1.png");
		Button* artwork2 = CreateTitleButton(artwork2UI, Vector2d(600, 0), L"sprites/title/artwork1.png");

		timer.duration = 3;
		creditButton->onMouseOver = [=]() { if (!timer.isActive) { timer.Start(); creditButton->selected = true; } };
		creditButton->onMouseExit = [=]() { timer.isActive = false; creditButton->selected = false; };
		creditButton->onUpdateWhileSelected = [=]() { timer.Update(); };
		timer.onUpdate = [=](double normT) { if(creditsLayer->GetSelfActive())credit->GetTransform()->SetWorldPosition(Vector2d::Lerp(credit->GetTransform()->GetWorldPosition(), Vector2d(0, 1900), normT)); };
		timer.onExpiration = [=]() 
		{
			layer = Layer::Credits; titleSceneUILayer->SetSelfActive(false); creditsLayer->SetSelfActive(true);
			timer.duration = 3;
			timer.isActive = true;
			timer.onExpiration = [=]()
			{
				timer.duration = 10;
				creditButton->selected = false;
				layer = Layer::None; titleSceneUILayer->SetSelfActive(true); creditsLayer->SetSelfActive(false);
			};
		};

		playButton->onMouseOver = [=]() { mouseInteractionIcon->SetSelfActive(true); mouseInteractionIcon->GetTransform()->SetWorldPosition(Vector2d(x - dx * 2, dy * 0)); };
		playButton->onMouseExit = [=]() { mouseInteractionIcon->SetSelfActive(false); };
		playButton->onClick = [=]()
		{ 
			for (auto each : { titleSceneUILayer, creditsLayer, optionLayer })
				each->SetSelfActive(false);
			Scene::LoadScene(TitleScene::GetInstance()->gameScene); gameCam->SetCameraMain();
		};

		optionButton->onMouseOver = [=]() { mouseInteractionIcon->SetSelfActive(true); mouseInteractionIcon->GetTransform()->SetWorldPosition(Vector2d(x - dx * 2, dy * 1)); };
		optionButton->onMouseExit = [=]() { mouseInteractionIcon->SetSelfActive(false); };
		optionButton->onSelect = [=]() {layer = Layer::Option; titleSceneUILayer->SetSelfActive(false); optionLayer->SetSelfActive(true); };

		exitButton->onMouseOver = [=]() { mouseInteractionIcon->SetSelfActive(true); mouseInteractionIcon->GetTransform()->SetWorldPosition(Vector2d(x - dx * 2, dy * 2)); };
		exitButton->onMouseExit = [=]() { mouseInteractionIcon->SetSelfActive(false); };
		exitButton->onClick = [=]() {  };	// 프로그램 종료 내용 필요
	}

	titleSceneUILayer->SetSelfActive(true);
}

Button* TitleScene::CreateTitleButton(GameObject* parentPanel, const Vector2d& pos, const wstring& resourcePath)
{
	parentPanel->GetTransform()->SetWorldPosition(pos);
	Button* button = parentPanel->GetScene()->AddGameObject(parentPanel)->AddComponent<Button>();
	
	D2DSprite* buttonSprite = button->GetGameObject()->AddComponent<D2DSprite>();
	buttonSprite->SetSpriteFilePath(resourcePath);

	auto spriteRect = buttonSprite->GetDrawRect();

	BoxCollider2D* buttonCollider = button->GetGameObject()->AddComponent<BoxCollider2D>();
	buttonCollider->SetWidth(spriteRect.width);
	buttonCollider->SetHeight(spriteRect.height);
	button->toggleButton = false;

	return button;
}