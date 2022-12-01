#pragma once

class Program
{
public:
	Program();
	~Program();

	void Update();
	void Render();

	void CreateWall();

private:
	class JumpingQuad* jq = nullptr;
	vector<class Wall*> walls;

	float elapsedTime = 0.0f; //경과시간
	float duration = 1.0f;	  //지속시간
};