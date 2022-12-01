#pragma once

class ResourceManager : public SingletonBase<ResourceManager>
{
public:
	friend class SingletonBase<ResourceManager>;

	HBRUSH& GetBrush(string brushName);
	void AddBrush(string key, COLORREF color);

private:
	ResourceManager();
	~ResourceManager();
	void InitBrushes();

	unordered_map<string, HBRUSH> brushMap;
};