#pragma once

class Quad
{
public:
	Quad();
	Quad(long left, long top, long right, long bottom);
	virtual ~Quad();

	void SetPosition(int x, int width);
	void SetHeight(int y, int height);
	bool CheckOutSide();

	void Update();
	void Render();

	void Move();
	void SetIntersected(bool bIntersected) { this->bIntersected = bIntersected; }

	BoundingBox* GetBox() { return box; }

private:
	class BoundingBox* box = nullptr; //충돌체 (충돌이 되었는지 안되었는지 판단)
	bool bIntersected = false;

protected:
	RECT rect;
};