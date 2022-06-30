//**************************************************
// 
// Hackathon ( imugi_property.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _IMGUI_PROPERTY_H_	// ���̃}�N������`����ĂȂ�������
#define _IMGUI_PROPERTY_H_	// 2�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include "imgui.h"
#include "particle.h"

#include <d3d9.h>
#include <array>
#include <chrono>

//==================================================
// �v���g�^�C�v�錾
//==================================================
//--------------------------------------------------
// ������
//--------------------------------------------------
void InitImguiProperty(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitImguiProperty(HWND hWnd, WNDCLASSEX wcex);

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateImguiProperty(void);

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawImguiProperty(void);

//--------------------------------------------------
// ���͌��m
//--------------------------------------------------
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------
// �p�[�e�B�N���f�[�^�̎擾
//--------------------------------------------------
CParticle::Particle& GetImguiParticle(void);

//--------------------------------------------------
// �p�[�e�B�N���o���ʒu�̎擾
//--------------------------------------------------
D3DXVECTOR3& GetPopPos(void);

//--------------------------------------------------
// �t�@�C�����̎擾
//--------------------------------------------------
char* GetFileName(void);

//--------------------------------------------------
// �p�[�e�B�N���̎g�p�󋵎擾
//--------------------------------------------------
bool bSetImguiParticle(void);

void SetFileName(char*FileStringData);

class Profiler
{
public:

	enum Stage
	{
		SdlInput,
		Plot,
		NewFrame,
		DemoWindow,
		SampleWindow,
		AnotherWindow,
		ProfilerWindow,
		ImGuiRender,
		OpenGL,
		ImGuiRenderOpenGL,
		SwapWindow,
		Rendering,

		_StageCount,
	};

	struct Scope
	{
		ImU8 _level;
		std::chrono::system_clock::time_point _start;
		std::chrono::system_clock::time_point _end;
		bool _finalized = false;
	};

	struct Entry
	{
		std::chrono::system_clock::time_point _frameStart;
		std::chrono::system_clock::time_point _frameEnd;
		std::array<Scope, _StageCount> _stages;
	};

	void Frame()
	{
		auto& prevEntry = _entries[_currentEntry];
		_currentEntry = (_currentEntry + 1) % _bufferSize;
		prevEntry._frameEnd = _entries[_currentEntry]._frameStart = std::chrono::system_clock::now();
	}

	void Begin(Stage stage)
	{
		assert(_currentLevel < 255);
		auto& entry = _entries[_currentEntry]._stages[stage];
		entry._level = _currentLevel;
		_currentLevel++;
		entry._start = std::chrono::system_clock::now();
		entry._finalized = false;
	}

	void End(Stage stage)
	{
		assert(_currentLevel > 0);
		auto& entry = _entries[_currentEntry]._stages[stage];
		assert(!entry._finalized);
		_currentLevel--;
		assert(entry._level == _currentLevel);
		entry._end = std::chrono::system_clock::now();
		entry._finalized = true;
	}

	ImU8 GetCurrentEntryIndex() const
	{
		return (_currentEntry + _bufferSize - 1) % _bufferSize;
	}

	static const ImU8 _bufferSize = 100;
	std::array<Entry, _bufferSize> _entries;

private:
	ImU8 _currentEntry = _bufferSize - 1;
	ImU8 _currentLevel = 0;
};

static const std::array<const char*, Profiler::_StageCount> stageNames = {
	"SDL Input",
	"Plot",
	"New Frame",
	"Demo Window",
	"Sample Window",
	"Another Window",
	"Profiler Window",
	"ImGui::Render",
	"OpenGL",
	"ImGui_ImplOpenGL3_RenderDrawData",
	"SDL_GL_SwapWindow",
	"Rendering",
};


#endif // !_IMGUI_PROPERTY_H_
