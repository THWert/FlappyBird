#pragma once

#include <Windows.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>

using namespace std;

#include "Systems/Window.h"

#include "Utilities/SingletonBase.h"
#include "Utilities/Vector3.h"
#include "Utilities/ResourceManager.h"
#include "Utilities/BoundingBox.h"

#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/Timer.h"

#pragma comment(lib, "msimg32.lib")
//GDI 컴포넌트

#define SAFE_DELETE(p) { if(p) {delete(p); (p) = nullptr;} }
#define WinMaxWidth 1280 
#define WinMaxHeight 720

extern HDC memDC;
/*
 - Handle Device Context
   Device Context란?
   출력에 필요한 정보를 가지는 데이터 구조체, 좌표, 색, 굵기 등 출력에 필요한 모든 정보를 담고있다.

   결국 HDC란 DC의 정보를 저장하는 데이터 구조체의 위치를 알기 위해서 사용

   HDC는 포인터가 아님. 항상 실제 객체의 메모리 주소를 가리킬 뿐
   물리적으로 메모리 구조가 바뀌어도, DC의 실제위치를 찾아감
*/

typedef UINT uint;