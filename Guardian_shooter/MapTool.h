#pragma once
#include <fstream>
#include <vector>
#include "YunutyEngine.h"

class MapTool
	: public Component
{
public:
#if _DEBUG
	void Update() override
	{
		if ((!mapFlag) && D2DInput::isKeyPushed(KeyCode::M))
		{
			mapFlag = true;
			initCamPos = cam->GetGameObject()->GetTransform()->position;
			bDebugGroup->SetSelfActive(true);
			buttonList[3]->GetGameObject()->SetSelfActive(false);
		}
		else if (mapFlag && D2DInput::isKeyPushed(KeyCode::M))
		{
			mapFlag = false;
			cam->GetGameObject()->GetTransform()->position = initCamPos;
			bDebugGroup->SetSelfActive(false);
		}

		if (mapFlag)
		{
			if (D2DInput::isKeyDown(KeyCode::UpArrow))
			{
				cam->GetGameObject()->GetTransform()->position.y += 10;
			}

			if (D2DInput::isKeyDown(KeyCode::DownArrow))
			{
				cam->GetGameObject()->GetTransform()->position.y -= 10;
			}

			if (D2DInput::isKeyDown(KeyCode::LeftArrow))
			{
				cam->GetGameObject()->GetTransform()->position.x -= 10;
			}

			if (D2DInput::isKeyDown(KeyCode::RightArrow))
			{
				cam->GetGameObject()->GetTransform()->position.x += 10;
			}

			for (size_t i = 0; i < buttonList.size(); ++i)
			{
				buttonList[i]->GetGameObject()->GetTransform()->SetWorldPosition(Vector2d(cam->GetGameObject()->GetTransform()->GetWorldPosition().x + 800, cam->GetGameObject()->GetTransform()->GetWorldPosition().y + 450 - buttonList[i]->GetGameObject()->GetComponent<D2DRectangle>()->height * 1.2 * i));
			}
		}


		/// ��ư �̺�Ʈ�� ���� �ʿ�
		if (D2DInput::isKeyDown(KeyCode::M))
		{
			if (D2DInput::isKeyPushed(KeyCode::MouseLeftClick))
			{
				AddTerrainVectorByClick(D2DInput::getMouseWorldPosition());
			}
		}

		if (D2DInput::isKeyPushed(KeyCode::S))
		{
			SaveVectorListToFile("Map.txt");
		}

		if (D2DInput::isKeyPushed(KeyCode::B))
		{
			BuildTerrain();
		}
	}
#endif
	enum class Label
	{
		TERRAIN,
		PLAYER,
		MONSTER
	};

	static MapTool* GetInstance()
	{
		return m_pInstance;
	}

	void Initialize()
	{
		LoadVectorListFromFile("Map.txt");
		BuildTerrain();
		BuildPlayer();
		BuildMonster();

#if _DEBUG
		cam = (*(this->GetGameObject()->GetScene()->GetChildren().begin()))->GetComponent<D2DCamera>();
		initCamPos = cam->GetGameObject()->GetTransform()->position;
		bDebugGroup = this->GetGameObject()->GetScene()->AddGameObject();
		tDebugGroup = this->GetGameObject()->GetScene()->AddGameObject();
		pDebugGroup = this->GetGameObject()->GetScene()->AddGameObject();
		mDebugGroup = this->GetGameObject()->GetScene()->AddGameObject();
		Button* button1 = this->GetGameObject()->GetScene()->AddGameObject()->AddComponent<Button>();
		CreateButton(button1, { 0,0 }, L"Button 1");
		button1->SetMouseOverCallable([=]() {button1->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::Gray; });
		button1->SetMouseExitCallable([=]() {button1->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::White; });
		Button* button2 = this->GetGameObject()->GetScene()->AddGameObject()->AddComponent<Button>();
		CreateButton(button2, { 0,0 }, L"Button 2");
		button2->SetMouseOverCallable([=]() {button2->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::Gray; });
		button2->SetMouseExitCallable([=]() {button2->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::White; });
		Button* button3 = this->GetGameObject()->GetScene()->AddGameObject()->AddComponent<Button>();
		CreateButton(button3, { 0,0 }, L"Button 3");
		button3->SetMouseOverCallable([=]() {button3->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::Gray; });
		button3->SetMouseExitCallable([=]() {button3->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::White; });
		Button* button4 = this->GetGameObject()->GetScene()->AddGameObject()->AddComponent<Button>();
		CreateButton(button4, { 0,0 }, L"Button 4");
		button4->SetMouseOverCallable([=]() {button4->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::Gray; });
		button4->SetMouseExitCallable([=]() {button4->GetGameObject()->GetComponent<D2DRectangle>()->color = D2D1::ColorF::White; });
		bDebugGroup->SetSelfActive(false);
#endif
	}

private:
	vector<Vector2d> LoadVectorListFromFile(const string& filePath)
	{
		fin.open(filePath);
		if (fin.fail())
		{
			return std::vector<YunutyEngine::Vector2d>();
		}

		size_t label;

		tVectorList.resize(0);
		pVectorList.resize(0);
		mVectorList.resize(0);

		vector<Vector2d> tempVector;
		tempVector.reserve(listCapa);
		Vector2d temp;

		for (size_t i = 0; ; ++i)
		{
			fin >> label;
			if (fin.eof())
			{
				break;
			}
			else if (label == (size_t)Label::TERRAIN)
			{
				fin >> temp.x;
				fin >> temp.y;
				tVectorList.emplace_back(temp);
				tempVector.emplace_back(temp);
			}
			else if (label == (size_t)Label::PLAYER)
			{
				fin >> temp.x;
				fin >> temp.y;
				pVectorList.emplace_back(temp);
				tempVector.emplace_back(temp);
			}
			else if (label == (size_t)Label::MONSTER)
			{
				fin >> temp.x;
				fin >> temp.y;
				mVectorList.emplace_back(temp);
				tempVector.emplace_back(temp);
			}
		}
		fin.close();

		return tempVector;
	}

	void InsertTerrainList(const vector<Vector2d>& terrainList)
	{
		tVectorList = terrainList;
	}

	void InsertPlayerList(const vector<Vector2d>& playerList)
	{
		pVectorList = playerList;
	}

	void InsertMonsterList(const vector<Vector2d>& monsterList)
	{
		mVectorList = monsterList;
	}

	vector<Vector2d> GetTerrainList()
	{
		return tVectorList;
	}

	vector<Vector2d> GetPlayerList()
	{
		return pVectorList;
	}

	vector<Vector2d> GetMonsterList()
	{
		return mVectorList;
	}

	bool SaveVectorListToFile(const string& filePath)
	{
		fout.open(filePath);
		if (fout.fail())
		{
			return false;
		}
		
		size_t tSize = tVectorList.size();
		size_t pSize = pVectorList.size();
		size_t mSize = mVectorList.size();

		for (size_t i = 0; i < tSize; ++i)
		{
			fout << (size_t)Label::TERRAIN << " " << tVectorList[i].x << " " << tVectorList[i].y << "\n";
		}
		for (size_t i = 0; i < pSize; ++i)
		{
			fout << (size_t)Label::PLAYER << " " << pVectorList[i].x << " " << pVectorList[i].y << "\n";
		}
		for (size_t i = 0; i < mSize; ++i)
		{
			fout << (size_t)Label::MONSTER << " " << mVectorList[i].x << " " << mVectorList[i].y << "\n";
		}
		fout.close();

		return true;
	}

#if _DEBUG
	void AddTerrainVectorByClick(const Vector2d& pos)
	{
		tVectorList.emplace_back(pos);
		GameObject* circle = this->GetGameObject()->GetScene()->AddGameObject();
		circle->AddComponent<D2DCircle>();
		circle->GetComponent<D2DCircle>()->radius = 10;
		circle->GetComponent<D2DCircle>()->GetTransform()->SetWorldPosition(pos);
		circle->GetComponent<D2DCircle>()->color = D2D1::ColorF::Red;
	}

	/// <summary>
	/// ���ۼ�
	/// </summary>
	void AddPlayerVectorByClick(const Vector2d& pos)
	{
		pVectorList.emplace_back(pos);
	}

	/// <summary>
	/// ���ۼ�
	/// </summary>
	void AddMonsterVectorByClick(const Vector2d& pos)
	{
		mVectorList.emplace_back(pos);
	}
#endif

	/// <summary>
	/// Collider �߰�, �ð�ȭ ��� �߰�
	/// </summary>
	void BuildTerrain()
	{
		if (tVectorList.size() == 0)
		{
			return;
		}
		for (size_t i = 0; i < tVectorList.size() - 1; ++i)
		{
			SetTerrainBox(tVectorList[i], tVectorList[i + 1]);
		}
	}

	void SetTerrainBox(const Vector2d& pos1, const Vector2d& pos2)
	{
		GameObject* box = this->GetGameObject()->GetScene()->AddGameObject();
		box->AddComponent<D2DRectangle>();

		Vector2d temp = pos2 - pos1;

		box->GetComponent<D2DRectangle>()->width = temp.Magnitude();
		box->GetComponent<D2DRectangle>()->height = 100;
		box->GetTransform()->SetWorldPosition(Vector2d((pos1.x + pos2.x) / 2, (pos1.y + pos2.y) / 2) + Quaternion(0, 0, 0, -1) * temp.Normalized() * 50);
		box->GetTransform()->SetWorldRotation(Vector3d(0,0 ,temp.GetAngleDegree()));
	}

	void BuildPlayer()
	{
		// �÷��̾� ���� �Լ��� �޾Ƽ� Vector2d �̿��� �ش� ��ġ�� �����ϱ�
	}

	void BuildMonster()
	{
		// ���� ���� �Լ��� �޾Ƽ� Vector2d �̿��� �ش� ��ġ�� �����ϱ�
	}

	void CreateButton(Button* button, const Vector2d& pos, const wstring& string, const double& width = 200, const double& height = 80, const double& fontsize = 20, D2D1::ColorF::Enum textcolor = D2D1::ColorF::Black, D2D1::ColorF::Enum backgroundcolor = D2D1::ColorF::White)
	{
		D2DRectangle* rect = button->GetGameObject()->AddComponent<D2DRectangle>();
		rect->color = backgroundcolor;
		rect->width = width;
		rect->height = height;
		
		BoxCollider2D* collider = button->GetGameObject()->AddComponent<BoxCollider2D>();
		collider->SetWidth(width);
		collider->SetHeight(height);

		D2DText* text = button->GetGameObject()->AddComponent<D2DText>();
		text->text = string;
		text->color = textcolor;
		text->fontSize = fontsize;
		text->width = width;
		text->height = height;

		button->GetGameObject()->SetParent(bDebugGroup);
		buttonList.push_back(button);
	}

public:
	MapTool()
		: Component(), fout(), fin(), listCapa(1024), tVectorList(), pVectorList(), mVectorList() 
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = this;
		}
		tVectorList.reserve(listCapa);
		pVectorList.reserve(listCapa);
		mVectorList.reserve(listCapa);
	}
	~MapTool()
	{

	}

private:
	ofstream fout;
	ifstream fin;
	size_t listCapa;
	vector<Vector2d> tVectorList;
	vector<Vector2d> pVectorList;
	vector<Vector2d> mVectorList;
	static MapTool* m_pInstance;

#if _DEBUG
	bool mapFlag = false;							// �� ���� ������� Ȯ���ϴ� �÷���
	D2DCamera* cam = nullptr;						// �� ���� ��ġ�� ���� ī�޶� ������
	Vector2d initCamPos = Vector2d{};				// �ʱ� ī�޶� ��ġ
	GameObject* bDebugGroup = nullptr;				// ��ư ������ ��� ��� �ִ� ���� ������Ʈ
	GameObject* tDebugGroup = nullptr;				// ���� ���� ��� �ð�ȭ ������ ��� ��� �ִ� ���� ������Ʈ
	GameObject* pDebugGroup = nullptr;				// �÷��̾� ���� ��� �ð�ȭ ������ ��� ��� �ִ� ���� ������Ʈ
	GameObject* mDebugGroup = nullptr;				// ���� ���� ��� �ð�ȭ ������ ��� ��� �ִ� ���� ������Ʈ
	vector<Button*> buttonList = vector<Button*>();	// ��ư ����Ʈ
#endif
};

MapTool* MapTool::m_pInstance = nullptr;

