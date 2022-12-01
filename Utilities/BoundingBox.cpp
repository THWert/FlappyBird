#include "stdafx.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox(RECT* rect)
	:rect(rect) {}

bool BoundingBox::AABB(BoundingBox* a, BoundingBox* b)
{
	if (a == nullptr || b == nullptr) return false;

	RECT owner = *a->rect;
	RECT other = *b->rect;

	if (owner.right  >= other.left
	 && owner.left   <= other.right
	 && owner.top    <= other.bottom
	 && owner.bottom >= other.top)
		return true;
	else
		return false;
}