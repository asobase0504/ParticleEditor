//**************************************************
// 
// imugi_property.h
// Author  : katsuki mizuki
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _IMGUI_PROPERTY_H_	// このマクロが定義されてなかったら
#define _IMGUI_PROPERTY_H_	// 2重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "imgui.h"
#include <d3dx9.h>

//--------------------------------------------------
// 入力検知
//--------------------------------------------------
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------
// imgui表示クラス
//--------------------------------------------------
class CImguiProperty
{
private:	// 定義
	static const char* FontPath;			// 使用するフォント
	static const float FontSize;			// フォントサイズ
	static const ImVec4 TitleBarColor;		// タイトルバーの色
	static const ImVec4 SliderBarColor;		// スライダーバーの色
	static const ImVec4 CheckMarkColor;		// チェックマークの色
	static const ImVec4 ScrollBarColor;		// スクロールバーの色
	static const ImVec4 HeaderColor;		// ヘッダーの基本色
	static const ImVec4 HeaderHoveredColor;	// ヘッダーにマウスカーソルを合わせた時の色
	static const ImVec4 HeaderActiveColor;	// ヘッダークリック時の色

	///static const int	MAX_TEXT;		// テキストの最大文字数
	static const char*	WINDOW_NAME;	// ウインドウの名前 (キャプションに表示)
public:
	CImguiProperty();
	~CImguiProperty();

	HWND Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(HWND hWnd, WNDCLASSEX wcex);
	void Update();
	void Draw();
	char* GetFileName();
	void SetFileName(char*FileStringData);
	bool bSetImguiParticle();
	void ParticleTemplate();
	void ColorPalette4(const char* label, float col[4]);
	void ColorPalette(float color[4], float backup_color[4], ImGuiColorEditFlags flags);
	ImVec4 ColorToImVec4(const D3DXCOLOR& color);
	D3DXCOLOR ImVec4ToColor(const ImVec4& vec);
private:
	void ParticleProperty();
private:
	char FileString[MAX_PATH * 256];	// ファイル名
	bool s_window = false;	// ウインドウを使用するかどうか
	bool s_bEffectEnable = true;
	const unsigned int gpu_id = 0;
	ImVec2 foo[10];
};

/* 使ってなさそう */
//#include <array>
//#include <chrono>

//class Profiler
//{
//public:
//
//	enum Stage
//	{
//		SdlInput,
//		Plot,
//		NewFrame,
//		DemoWindow,
//		SampleWindow,
//		AnotherWindow,
//		ProfilerWindow,
//		ImGuiRender,
//		OpenGL,
//		ImGuiRenderOpenGL,
//		SwapWindow,
//		Rendering,
//
//		_StageCount,
//	};
//
//	struct Scope
//	{
//		ImU8 _level;
//		std::chrono::system_clock::time_point _start;
//		std::chrono::system_clock::time_point _end;
//		bool _finalized = false;
//	};
//
//	struct Entry
//	{
//		std::chrono::system_clock::time_point _frameStart;
//		std::chrono::system_clock::time_point _frameEnd;
//		std::array<Scope, _StageCount> _stages;
//	};
//
//	void Frame()
//	{
//		auto& prevEntry = _entries[_currentEntry];
//		_currentEntry = (_currentEntry + 1) % _bufferSize;
//		prevEntry._frameEnd = _entries[_currentEntry]._frameStart = std::chrono::system_clock::now();
//	}
//
//	void Begin(Stage stage)
//	{
//		assert(_currentLevel < 255);
//		auto& entry = _entries[_currentEntry]._stages[stage];
//		entry._level = _currentLevel;
//		_currentLevel++;
//		entry._start = std::chrono::system_clock::now();
//		entry._finalized = false;
//	}
//
//	void End(Stage stage)
//	{
//		assert(_currentLevel > 0);
//		auto& entry = _entries[_currentEntry]._stages[stage];
//		assert(!entry._finalized);
//		_currentLevel--;
//		assert(entry._level == _currentLevel);
//		entry._end = std::chrono::system_clock::now();
//		entry._finalized = true;
//	}
//
//	ImU8 GetCurrentEntryIndex() const
//	{
//		return (_currentEntry + _bufferSize - 1) % _bufferSize;
//	}
//
//	static const ImU8 _bufferSize = 100;
//	std::array<Entry, _bufferSize> _entries;
//
//private:
//	ImU8 _currentEntry = _bufferSize - 1;
//	ImU8 _currentLevel = 0;
//};

//static const std::array<const char*, Profiler::_StageCount> stageNames = {
//	"SDL Input",
//	"Plot",
//	"New Frame",
//	"Demo Window",
//	"Sample Window",
//	"Another Window",
//	"Profiler Window",
//	"ImGui::Render",
//	"OpenGL",
//	"ImGui_ImplOpenGL3_RenderDrawData",
//	"SDL_GL_SwapWindow",
//	"Rendering",
//};

#endif // !_IMGUI_PROPERTY_H_
