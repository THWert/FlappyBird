#pragma once

class BoundingBox
{
public:
	BoundingBox(RECT* rect);
	~BoundingBox() = default;

	static bool AABB(BoundingBox* a, BoundingBox* b);

private:
	RECT* rect;
};