#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

class Button;
class Timer;

namespace UI
{
	class UIManager :
		public Component
	{
	public:
		enum class UIEnum
		{
			None,
			TitleUI,
			InGameUI,
			InGamePopUpUI,
			PlayerDeathUI,
			GameOverUI,
			GameClearUI,
			CreditUI
		};
		enum class FadeOption
		{
			FadeIn,
			FadeOut
		};
		enum class ScaleOption
		{
			ScaleUp,
			ScaleDown
		};
		static UIManager* GetInstance() { return instance; }
		UIManager() { if (!instance) instance = this; }
		~UIManager() { if (instance == this) instance = nullptr; }
		virtual void Update() override;
		void Initialize();
		void ResetUI(UIEnum targetUI);
		void ShowUI(UIEnum targetUI, GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject());
		void ShowUI(UIEnum targetUI, FadeOption fadeOption, double duration = 1, GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject());
		void ShowUI(UIEnum targetUI, ScaleOption scaleOption, double rate, double duration = 1, GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject());
		// void ShowUI(UIEnum targetUI, FadeOption fadeOption, ScaleOption scaleOption, double rate, double duration = 1, GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject());
		void HideUI(UIEnum targetUI);
		void HideUI(UIEnum targetUI, FadeOption fadeOption, double duration = 1);
		void HideUI(UIEnum targetUI, ScaleOption scaleOption, double rate, double duration = 1);
		// void HideUI(UIEnum targetUI, FadeOption fadeOption, ScaleOption scaleOption, double rate, double duration = 1 );
		bool readyToQuit=false;
	
	private:
		void CreateAllGS_UI();
		void InitializeAllGS_UI();

		class GS_UI
			: public Component
		{
		public:
			virtual void Update() {}
			virtual void CreateUI() {}
			virtual void Initialize() {}
			virtual void ResetThisUI() {}
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) {}
			virtual void HideThisUI() {}
			virtual void SetFadeIn(double duration);
			virtual void SetFadeOut(double duration);
			virtual void SetScaleUp(double duration, double rate);
			virtual void SetScaleDown(double duration, double rate);
			static void CreateAllUI();
			static void InitializeAllUI();
			static void ResetAllUI();
			static void HideAllUI();

			Timer uIFunctionTimer = Timer();
			double rate = double();
			static unordered_map<UIEnum, GS_UI*> GS_UIList;
			static unordered_map<GS_UI*, vector<D2DSprite*>> GS_UISpriteList;
			static unordered_map<GS_UI*, vector<D2DAnimatedSprite*>> GS_UIAnimatedSpriteList;
			static unordered_map<D2DSprite*, Rect> GS_UISpriteInitRectList;
			static unordered_map<D2DSprite*, Vector2d> GS_UISpriteInitPosList;
			static unordered_map<D2DAnimatedSprite*, Rect> GS_UIAnimatedSpriteInitRectList;
			static unordered_map<D2DAnimatedSprite*, Vector2d> GS_UIAnimatedSpriteInitPosList;
		};

		class Title
			: public GS_UI
		{
		public:
			Title();
			~Title();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static Title* GetInstance() { return instance; }
			static Title* instance;

			Timer timer = Timer();
			Timer timerHowToPlay = Timer();
			Timer glitchTimer = Timer();
			Button* CreateTitleButton(GameObject* parentPanel, const Vector2d& pos, const wstring& resourcePath);

			GameObject* background = nullptr;
			GameObject* creditButtonUI = nullptr;
			GameObject* playButtonUI = nullptr;
			GameObject* optionButtonUI = nullptr;
			GameObject* howToPlayButtonUI = nullptr;
			GameObject* quitButtonUI = nullptr;
			GameObject* artwork1UI = nullptr;
			GameObject* mouseInteractionIcon = nullptr;
			D2DSprite* playBtnSprite = nullptr;
			D2DSprite* optionBtnSprite = nullptr;
			D2DSprite* howToPlayBtnSprite = nullptr;
			D2DSprite* quitBtnSprite = nullptr;
			D2DSprite* howToPlaySprite = nullptr;
			D2DSprite* artworkSprite = nullptr;
		};

		class InGamePopUp
			: public GS_UI
		{
		public:
			InGamePopUp();
			~InGamePopUp();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static InGamePopUp* GetInstance() { return instance; }
			static InGamePopUp* instance;

			enum class Layer { None };
			Timer timer = Timer();
			Layer layer = Layer::None;
		};

		class InGame
			: public GS_UI
		{
		public:
			InGame();
			~InGame();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static InGame* GetInstance() { return instance; }
			static InGame* instance;

			Timer timerInGame = Timer();
			Timer comboTimer1 = Timer();
			Timer comboTimer10 = Timer();
			Timer comboTimer100 = Timer();
			Timer bossClearTimer = Timer();

			double beforeCombo = 0;
			double numberCreatePos = 50;
			bool dirNumDown = true;
			bool comboFadeInFinish = false;
			double floatingDistance = 138;
			double bossDelay = 5;

			GameObject* life1UI = nullptr;
			GameObject* life2UI = nullptr;
			GameObject* gauge = nullptr;
			GameObject* stack1 = nullptr;
			GameObject* stack2 = nullptr;
			GameObject* stack3 = nullptr;
			GameObject* hp1 = nullptr;
			GameObject* hp2 = nullptr;
			GameObject* hp3 = nullptr;
			GameObject* comboAll = nullptr;
			GameObject* comboWord = nullptr;
			GameObject* numPos1 = nullptr;
			GameObject* numPos10 = nullptr;
			GameObject* numPos100 = nullptr;
			GameObject* nowNumPos1 = nullptr;
			GameObject* nowNumPos10 = nullptr;
			GameObject* nowNumPos100 = nullptr;
			GameObject* blankNumPos1 = nullptr;
			GameObject* blankNumPos10 = nullptr;
			GameObject* blankNumPos100 = nullptr;
			vector<GameObject*> gauge_non = vector<GameObject*>(12);
			vector<GameObject*> countNumOrangePos1 = vector<GameObject*>(10);
			vector<GameObject*> countNumOrangePos10 = vector<GameObject*>(10);
			vector<GameObject*> countNumOrangePos100 = vector<GameObject*>(10);
			vector<GameObject*> countNumBluePos1 = vector<GameObject*>(10);
			vector<GameObject*> countNumBluePos10 = vector<GameObject*>(10);
			vector<GameObject*> countNumBluePos100 = vector<GameObject*>(10);
			unordered_set<D2DSprite*> comboSpriteList = unordered_set<D2DSprite*>();

			enum class TimerIndex
			{
				Pos1,
				Pos10,
				Pos100
			};
		private:
			void ChangeNumberEffect(GameObject* beforeNum, GameObject* nextNum, TimerIndex timerIndex);
		};

		class PlayerDeath
			: public GS_UI
		{
		public:
			PlayerDeath();
			~PlayerDeath();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static PlayerDeath* GetInstance() { return instance; }
			static PlayerDeath* instance;

			Timer timer = Timer();

			double interval = 1.5;
			GameObject* backgroundCover = nullptr;
			GameObject* upText = nullptr;
			GameObject* downText = nullptr;
			double downTextDefaultPos = -200;
			double range = 10;
			double rangeSpeed = 0.3;
			bool vibrationDirDown = false;
			vector<GameObject*> countNum = vector<GameObject*>(10);
		};

		class Credit
			: public GS_UI
		{
		public:
			Credit();
			~Credit();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static Credit* GetInstance() { return instance; }
			static Credit* instance;

			Timer timer = Timer();

			GameObject* background = nullptr;
			GameObject* credit = nullptr;
			GameObject* artwork = nullptr;
		};

		class GameOver
			: public GS_UI
		{
		public:
			GameOver();
			~GameOver();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static GameOver* GetInstance() { return instance; }
			static GameOver* instance;

			Timer timer = Timer();

			double nowText1 = true;
			double vibrationPeriod = 0.1;
			double playTime = 2;
			GameObject* background = nullptr;
			GameObject* gameover = nullptr;
			Rect beforeRect = Rect();
			Rect maxRect = Rect();
			double initialScale = 0.3;
			Vector2d indicatorVector = Vector2d();
		};

		class GameClear
			: public GS_UI
		{
		public:
			GameClear();
			~GameClear();
			virtual void Update() override;
			virtual void Initialize() override;
			virtual void ResetThisUI() override;
			virtual void ShowThisUI(GameObject* targetObj = D2DCamera::GetMainCamera()->GetGameObject()) override;
			virtual void HideThisUI() override;
			static GameClear* GetInstance() { return instance; }
			static GameClear* instance;

			Timer timer = Timer();

			double playTime = 3;
			double letterDropDistance = 100;
			double letterDropTime = 0.2;
			double interval = 0.05;
			double markDelay = 1;
			GameObject* background = nullptr;
			GameObject* gameClearUp = nullptr;
			GameObject* gameClearDown = nullptr;
			D2DSprite* mark = nullptr;
			D2DSprite* gameClearUpLetterList[7] = {};
			D2DSprite* gameClearDownLetterList[9] = {};

		private:
			Timer wordTimerUp[7] = {};
			Timer wordTimerDown[9] = {};

			void DropUpLetter(size_t index);
			void DropDownLetter(size_t index);
		};
		static UIManager* instance;
		static unordered_map<UIEnum, GameObject*> UIList;
	};
}

