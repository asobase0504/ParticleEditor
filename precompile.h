#ifndef PRECOMPILE_H_
#define PRECOMPILE_H_

// 基本
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <tchar.h>

// メモリーリークのチェック
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// STL
#include <vector>
#include <string>
#include <map>

// DirectX9
#include <d3dx9.h>

// input
#define	 DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xinput.h>

// json
#include "nlohmann/json.hpp"

#endif // !PRECOMPILE_H_
