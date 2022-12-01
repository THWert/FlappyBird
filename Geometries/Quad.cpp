#include "stdafx.h"
#include "Quad.h"

Quad::Quad()
{
	rect.left = rect.top = 0;
	rect.right = rect.bottom = 100;

	box = new BoundingBox(&rect);
}

Quad::Quad(long left, long top, long right, long bottom)
{
	rect.left   = left;
	rect.top    = top;
	rect.right  = right;
	rect.bottom = bottom;

	box = new BoundingBox(&rect);
}

Quad::~Quad()
{
	SAFE_DELETE(box);
}

void Quad::SetPosition(int x, int width)
{
	rect.left = x;
	rect.right = x + width;
}

void Quad::SetHeight(int y, int height)
{
	rect.top = y;
	rect.bottom = y + height; //아래로 갈수록 커지기 때문에 y를 더함
}

bool Quad::CheckOutSide()
{
	if (rect.right <= -100)
		return true;
	else
		return false;
}

void Quad::Update()
{
	Move();
}

void Quad::Render()
{
	if (bIntersected)
		FillRect(memDC, &rect, ResourceManager::Get()->GetBrush("Red"));
	else
		FillRect(memDC, &rect, ResourceManager::Get()->GetBrush("Blue"));
}

void Quad::Move()
{
	if (Keyboard::Get()->Press('W'))
	{
		rect.bottom -= 10;
		rect.top -= 10;
	}
	else if (Keyboard::Get()->Press('S'))
	{
		rect.bottom += 10;
		rect.top += 10;
	}
	if (Keyboard::Get()->Press('A'))
	{
		rect.left -= 10;
		rect.right -= 10;
	}
	else if (Keyboard::Get()->Press('D'))
	{
		rect.left += 10;
		rect.right += 10;
	}
}