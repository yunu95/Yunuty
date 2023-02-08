#pragma once
#include "YunutyEngine.h"

class Button :
	public Component
{
public:
	void OnMouseOver();
	void OnMouseExit();
	void OnLeftClick();
	void OnMiddleClick();
	void OnRightClick();
	void SetMouseOverCallable(const function<void()>& callable);
	void SetMouseExitCallable(const function<void()>& callable);
	void SetLeftClickCallable(const function<void()>& callable);
	void SetMiddleClickCallable(const function<void()>& callable);
	void SetRightClickCallable(const function<void()>& callable);

	struct Comp
	{
		bool operator()(Button* const input1, Button* const input2)const
		{
			return input1->GetGameObject()->GetSceneIndex() > input2->GetGameObject()->GetSceneIndex();
		}
	};

private:
	function<void(void)> mouseOverCallable = []() {};
	function<void(void)> mouseExitCallable = []() {};
	function<void(void)> leftClickCallable = []() {};
	function<void(void)> middleClickCallable = []() {};
	function<void(void)> rightClickCallable = []() {};
};


