#pragma once

/*
	싱글턴 패턴
	생성자가 여러차례 호출되어도 실제로 생성된 객체는 하나
	만약, 계속 생성자로 호출되면 최초에 생성된 객체만 리턴
*/

template<typename T>

class SingletonBase
{
public:
	static void Create()
	{
		if (instance == nullptr)
			instance = new T();
	}

	static void Delete()
	{
		delete instance;
		instance = nullptr;
	}

	static T* Get() { return instance; }

protected:
	static T* instance;
};

template<typename T> T* SingletonBase<T>::instance = nullptr;