#pragma once
#include "YunutyEngine.h"
#include "Button.h" 

class Cursor :
	public Component
{
protected:
	void Update() override
	{
		GetTransform()->SetWorldPosition(Input::getMouseWorldPosition());

		if (buttonList.empty())
		{
			return;
		}

		if (D2DInput::isKeyPushed(KeyCode::MouseLeftClick))
		{
			(*buttonList.begin())->OnLeftClick();
		}

		if (D2DInput::isKeyPushed(KeyCode::MouseMiddleClick))
		{
			(*buttonList.begin())->OnMiddleClick();
		}

		if (D2DInput::isKeyPushed(KeyCode::MouseRightClick))
		{
			(*buttonList.begin())->OnRightClick();
		}
	}

	void OnCollisionEnter2D(const Collision2D& collision) override
	{
		Button* button = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
		if (button == nullptr)
		{
			return;
		}
		buttonList.insert(button);
		button->OnMouseOver();
	}

	void OnCollisionExit2D(const Collision2D& collision) override
	{
		Button* button = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
		if (button == nullptr)
		{
			return;
		}
		// set은 log(n)의 시간복잡도로 탐색과 제거를 시행할 수 있다.
		buttonList.erase(button);
		button->OnMouseExit();
	}

	static Cursor* GetInstance()
	{
		return m_pInstance;
	}

public:
	Cursor()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = this;
		}
	}

	~Cursor()
	{

	}

private:
	set<Button*, Button::Comp> buttonList;
	static Cursor* m_pInstance;
};

Cursor* Cursor::m_pInstance = nullptr;
