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
	class BoundingBox* box = nullptr; //�浹ü (�浹�� �Ǿ����� �ȵǾ����� �Ǵ�)
	bool bIntersected = false;

protected:
	RECT rect;
};