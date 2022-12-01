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

	float elapsedTime = 0.0f; //����ð�
	float duration = 1.0f;	  //���ӽð�
};