#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

class Cursor;
class Button;

class TitleScene :
    public Scene
{
public:
	enum class Layer { None, Credits, Option };
	TitleScene();
	~TitleScene();
	void Initialize(Scene* gameScene, D2DCamera* gameCam);
	bool isTitleScene() { return titleSceneUILayer->GetActive(); }
	Layer GetFirstLayer() { return layer; };
	Scene* GetGameScene() { return gameScene; };
	D2DCamera* GetGameD2DCamera() { return gameCam; };
	static TitleScene* GetInstance() { return instance; }
private:
	Button* CreateTitleButton(GameObject* parentPanel, const Vector2d& pos, const wstring& resourcePath);
	Timer timer = Timer();
	Layer layer = Layer::None;
	GameObject* titleSceneUILayer = nullptr;
	GameObject* creditsLayer = nullptr;
	GameObject* optionLayer = nullptr;
	Scene* gameScene = nullptr;
	D2DCamera* gameCam = nullptr;
	static TitleScene* instance;
};

