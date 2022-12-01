//Wall.cpp

#include "stdafx.h"
#include "Wall.h"

Wall::Wall(long x)
	:position(x)
{
	upper = new Quad();
	lower = new Quad();

	upper->SetPosition(x, width);
	lower->SetPosition(x, width);

	int upperHeight = rand() % (WinMaxHeight / 4) + (WinMaxHeight / 4); //½ò¸² ¹æÁö¸¦ À§ÇÑ ³ª´°¼À
	upper->SetHeight(0, upperHeight);
	lower->SetHeight(upperHeight + spacing, WinMaxHeight - (upperHeight + spacing));
}

Wall::~Wall()
{
	SAFE_DELETE(upper);
	SAFE_DELETE(lower);
}

void Wall::Update()
{
	position -= speed;
	upper->SetPosition(position, width);
	lower->SetPosition(position, width);
}

void Wall::Render()
{
	upper->Render();
	lower->Render();
}

void Wall::RePosition(long x)
{
	upper->SetPosition(x, width);
	lower->SetPosition(x, width);

	int upperHeight = rand() % (WinMaxHeight / 4) + (WinMaxHeight / 4);
	upper->SetHeight(0, upperHeight);
	lower->SetHeight(upperHeight + spacing, WinMaxHeight - (upperHeight + spacing));
	position = x;
}