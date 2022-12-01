#include "stdafx.h"
#include "Program.h"

#include "Geometries/Quad.h"
#include "Geometries/JumpingQuad.h"
#include "Geometries/Wall.h"

Program::Program()
{
	ResourceManager::Create();
	jq = new JumpingQuad(50, WinMaxHeight / 2 - 50, 100, WinMaxHeight / 2);
	CreateWall();
}

Program::~Program()
{
	ResourceManager::Delete();
	SAFE_DELETE(jq);

	for (Wall* wall : walls)
		SAFE_DELETE(wall);
}

void Program::Update()
{
	jq->Update();
	if (Keyboard::Get()->Press(VK_SPACE))
		jq->Jump();

	for (Wall* wall : walls)
	{
		wall->Update();
		if (wall->CheckOutSide()) //화면 왼쪽 끝까지 가게 되면 다시 오른쪽으로 돌려냄
		{
			wall->RePosition(walls.back()->GetPosition() + 500);
			rotate(walls.begin(), walls.begin() + 1, walls.end());
		}
	}

	if (BoundingBox::AABB(jq->GetBox(), walls.front()->GetUpperBox())
		|| BoundingBox::AABB(jq->GetBox(), walls.front()->GetLowerBox()))
		jq->SetIntersected(true);
	else
		jq->SetIntersected(false);
}

void Program::Render()
{
	jq->Render();
	for (Wall* wall : walls)
		wall->Render();
}

void Program::CreateWall()
{
	for (int i = 0; i < 6; i++)
		walls.push_back(new Wall(WinMaxHeight + i * 500));
}