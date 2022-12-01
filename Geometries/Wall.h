#pragma once
#include "Quad.h"

class Wall
{
public:
	Wall(long x);
	~Wall();

	void Update();
	void Render();

	void RePosition(long x);
	long GetPosition() { return position; }
	bool CheckOutSide() { return lower->CheckOutSide(); }

	BoundingBox* GetUpperBox() { return upper->GetBox(); }
	BoundingBox* GetLowerBox() { return lower->GetBox(); }

private:
	Quad* upper = nullptr;
	Quad* lower = nullptr;

	int width = 100;
	int spacing = 150; //벽 사이의 간격

	int position = 0;
	int speed = 10;
};