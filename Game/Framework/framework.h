// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
#include<mmsystem.h>
#pragma comment (lib, "winmm.lib")
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
//#include <cstddef>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//C++ STL
#include<string>
#include<vector>
#include<list>
#include<algorithm>

//デバッグ用
#include<consoleapi.h>
#include<iostream>
#include<io.h>
#include<fcntl.h>

#ifdef _DEBUG
#	define DEBUG_LOG(str, ...) printf(str, __VA_ARGS__)
#else
#	define DEBUG_LOG(str, ...)
#endif