#include "stdafx.h"
#include "ResourceManager.h"

HBRUSH& ResourceManager::GetBrush(string brushName)
{
	if (brushMap.find(brushName) != brushMap.end()) //브러쉬 이름 찾아 넣고 없으면 흰색 반환
	{
		return brushMap.find(brushName)->second;
	}

	return GetBrush("White");
}

void ResourceManager::AddBrush(string key, COLORREF color)
{
	HBRUSH brush;
	brush = (HBRUSH)CreateSolidBrush(color);
	brushMap.insert(make_pair(key, brush));
}

ResourceManager::ResourceManager()
{
	InitBrushes();
}

ResourceManager::~ResourceManager()
{
	for (pair<string, HBRUSH> b : brushMap)
		DeleteObject(b.second);
}

void ResourceManager::InitBrushes()
{
	AddBrush("Red"  , RGB(255, 0  , 0  ));
	AddBrush("Green", RGB(0  , 255, 0  ));
	AddBrush("Blue",  RGB(0  , 0  , 255));
	AddBrush("White", RGB(255, 255, 255));
	AddBrush("Black", RGB(0  , 0  , 0  ));
}