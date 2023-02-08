#include "Button.h"


void Button::OnMouseOver()
{
	mouseOverCallable();
}

void Button::OnMouseExit()
{
	mouseExitCallable();
}

void Button::OnLeftClick()
{
	leftClickCallable();
}

void Button::OnMiddleClick()
{
	middleClickCallable();
}

void Button::OnRightClick()
{
	rightClickCallable();
}

void Button::SetMouseOverCallable(const function<void()>& callable)
{
	mouseOverCallable = callable;
}

void Button::SetMouseExitCallable(const function<void()>& callable)
{
	mouseExitCallable = callable;
}

void Button::SetLeftClickCallable(const function<void()>& callable)
{
	leftClickCallable = callable;
}

void Button::SetMiddleClickCallable(const function<void()>& callable)
{
	middleClickCallable = callable;
}

void Button::SetRightClickCallable(const function<void()>& callable)
{
	rightClickCallable = callable;
}
