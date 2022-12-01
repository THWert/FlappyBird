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
//GDI ������Ʈ

#define SAFE_DELETE(p) { if(p) {delete(p); (p) = nullptr;} }
#define WinMaxWidth 1280 
#define WinMaxHeight 720

extern HDC memDC;
/*
 - Handle Device Context
   Device Context��?
   ��¿� �ʿ��� ������ ������ ������ ����ü, ��ǥ, ��, ���� �� ��¿� �ʿ��� ��� ������ ����ִ�.

   �ᱹ HDC�� DC�� ������ �����ϴ� ������ ����ü�� ��ġ�� �˱� ���ؼ� ���

   HDC�� �����Ͱ� �ƴ�. �׻� ���� ��ü�� �޸� �ּҸ� ����ų ��
   ���������� �޸� ������ �ٲ�, DC�� ������ġ�� ã�ư�
*/

typedef UINT uint;