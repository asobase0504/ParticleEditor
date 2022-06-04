//**************************************************
//
// Hackathon ( imgui_property.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "imgui_property.h"
#include "main.h"
#include "file.h"
#include "utility.h"
#include <imgui_internal.h>
#include <assert.h>
#include <implot.h>
#include <imgui_widget_flamegraph.h>

//==================================================
// �}�N����`
//==================================================
#define IMGUI_DEFINE_MATH_OPERATORS


//==================================================
// ��`
//==================================================
static const int	MAX_TEXT = 1024;		// �e�L�X�g�̍ő啶����
static const char*	WINDOW_NAME = "test";	// �E�C���h�E�̖��O (�L���v�V�����ɕ\��)

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
static char FileString[MAX_PATH * 256];	// �t�@�C����
static bool	s_window = false;	// �E�C���h�E���g�p���邩�ǂ���
static Particle imguiParticle;	// ImGui�ɕۑ�����Ă�p�[�e�B�N�����
static bool s_bEffectEnable = false;
static float s_fScale = 50.0f;


static void ProfilerValueGetter(float* startTimestamp, float* endTimestamp, ImU8* level, const char** caption, const void* data, int idx)
{
	auto entry = reinterpret_cast<const Profiler::Entry*>(data);
	auto& stage = entry->_stages[idx];
	if (startTimestamp)
	{
		std::chrono::duration<float, std::milli> fltStart = stage._start - entry->_frameStart;
		*startTimestamp = fltStart.count();
	}
	if (endTimestamp)
	{
		*endTimestamp = stage._end.time_since_epoch().count() / 1000000.0f;

		std::chrono::duration<float, std::milli> fltEnd = stage._end - entry->_frameStart;
		*endTimestamp = fltEnd.count();
	}
	if (level)
	{
		*level = stage._level;
	}
	if (caption)
	{
		*caption = stageNames[idx];
	}
}


//--------------------------------------------------
// ������
//--------------------------------------------------
void InitImguiProperty(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
#ifdef _DEBUG

	s_window = true;

	// �E�C���h�E�̕\��
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	// �����̐ݒ�
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// �X�^�C���̐ݒ�
	ImGui::StyleColorsDark();

	// �^�C�g���o�[�̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// �X���C�h�̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// �`�F�b�N�}�[�N�̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// �X�N���[���̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));

	// �v���b�g�t�H�[���̐ݒ�/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

#endif // _DEBUG
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitImguiProperty(HWND hWnd, WNDCLASSEX wcex)
{
#ifdef _DEBUG

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	// �E�C���h�E�̔j��
	::DestroyWindow(hWnd);

	// �E�C���h�E�N���X�̓o�^������
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

#endif // _DEBUG
}

//--------------------------------------------------
// �Ȑ�����T�C���J�[�u
//--------------------------------------------------
namespace ImGui
{
	template<int steps>
	void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) 
	{
		static float C[(steps + 1) * 4], *K = 0;
		if (!K) 
		{
			K = C;
			for (unsigned step = 0; step <= steps; ++step) 
			{
				float t = (float)step / (float)steps;
				C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
				C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t; // * P1
				C[step * 4 + 2] = 3 * (1 - t) * t*t;     // * P2
				C[step * 4 + 3] = t*t*t;               // * P3
			}
		}
		for (unsigned step = 0; step <= steps; ++step) 
		{
			ImVec2 point = {
				K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
				K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
			};
			results[step] = point;
		}
	}

	float BezierValue(float dt01, float P[4]) 
	{
		enum { STEPS = 512 };
		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
		ImVec2 results[STEPS + 1];
		bezier_table<STEPS>(Q, results);
		return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
	}

	int Bezier(const char *label, float P[4]) {
		// visuals
		enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
		enum { CURVE_WIDTH = 4 }; // main curved line width
		enum { LINE_WIDTH = 1 }; // handlers: small lines width
		enum { GRAB_RADIUS = 6 }; // handlers: circle radius
		enum { GRAB_BORDER = 2 }; // handlers: circle border width

		const ImGuiStyle& Style = GetStyle();
		const ImGuiIO& IO = GetIO();
		ImDrawList* DrawList = GetWindowDrawList();
		ImGuiWindow* Window = GetCurrentWindow();
		if (Window->SkipItems)
			return false;

		// header and spacing
		int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
		int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
		Dummy(ImVec2(0, 3));

		// prepare canvas
		const float avail = GetContentRegionAvail().x;
		const float dim = ImMin(avail, 128.f);

		/*width, height*/
		ImVec2 Canvas(dim, dim);

		ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
		ItemSize(bb);
		if (!ItemAdd(bb, NULL))
			return changed;

		const ImGuiID id = Window->GetID(label);
		// hovered |= 0 != IsItemHovered(ImRect(bb.Min, bb.Min + ImVec2(avail,dim)), id);

		RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

		// background grid
		for (int i = 0; i <= Canvas.x; i += (int)(Canvas.x * 0.25f)) {
			DrawList->AddLine(
				ImVec2(bb.Min.x + i, bb.Min.y),
				ImVec2(bb.Min.x + i, bb.Max.y),
				GetColorU32(ImGuiCol_TextDisabled));
		}
		for (int i = 0; i <= Canvas.y; i += (int)(Canvas.y * 0.25f)) {
			DrawList->AddLine(
				ImVec2(bb.Min.x, bb.Min.y + i),
				ImVec2(bb.Max.x, bb.Min.y + i),
				GetColorU32(ImGuiCol_TextDisabled));
		}

		// eval curve
		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
		ImVec2 results[SMOOTHNESS + 1];
		bezier_table<SMOOTHNESS>(Q, results);

		// control points: 2 lines and 2 circles
		{
			char buf[128];
			sprintf(buf, "0##%s", label);

			// handle grabbers
			for (int i = 0; i < 2; ++i)
			{
				ImVec2 pos = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
				SetCursorScreenPos(pos - ImVec2(GRAB_RADIUS, GRAB_RADIUS));
				InvisibleButton((buf[0]++, buf), ImVec2(2 * GRAB_RADIUS, 2 * GRAB_RADIUS));
				if (IsItemActive() || IsItemHovered())
				{
					SetTooltip("(%4.3f, %4.3f)", P[i * 2 + 0], P[i * 2 + 1]);
				}
				if (IsItemActive() && IsMouseDragging(0))
				{
					P[i * 2 + 0] += GetIO().MouseDelta.x / Canvas.x;
					P[i * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y;
					changed = true;
				}
			}

			if (hovered || changed) DrawList->PushClipRectFullScreen();

			// draw curve
			{
				ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
				for (int i = 0; i < SMOOTHNESS; ++i) {
					ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
					ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
					ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
					ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
					DrawList->AddLine(r, s, color, CURVE_WIDTH);
				}
			}

			// draw lines and grabbers
			float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
			ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
			ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
			ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
			ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
			DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
			DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
			DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
			DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
			DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
			DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));

			if (hovered || changed) DrawList->PopClipRect();

			// restore cursor pos
			SetCursorScreenPos(ImVec2(bb.Min.x, bb.Max.y + GRAB_RADIUS)); // :P
		}

		return changed;
	}
}
void ShowDemo_DragLines()
{
	ImGui::BulletText("�������Ɛ��������N���b�N���ăh���b�O���܂��B ");
	static double x1 = 0.2;
	static double x2 = 0.8;
	static double y1 = 0.25;
	static double y2 = 0.75;
	static double f = 0.1;
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2());
	ImGui::Begin("AAA",nullptr,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
	ImVec2 graphWindowSize = ImGui::GetContentRegionAvail(); // ImGui :: GetWindowSize();
	constexpr size_t GraphAmount = 15;
	ImVec2 oneGraphSize = ImVec2(-1,graphWindowSize.y / GraphAmount);
	if (oneGraphSize.y < 150.f)
	{
		oneGraphSize.y = 150.f;
	}
}
//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateImguiProperty(void)
{
#ifdef _DEBUG

	if (!s_window)
	{// �E�C���h�E���g�p���Ȃ�
		return;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	static int mode = 0;
	static int sliderInt = 0;
	static float sliderFloat = 0;
	static bool checkBox = true;
	static char text[MAX_TEXT] = "";

	static bool useEffect = false;
	static bool s_bRot = false;
	static bool s_bTexRot = false;
	static bool s_bUsesrand = false;

	// �E�C���h�E�̋N�����̏ꏊ
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiCond_Once);

	// �E�C���h�E�̖���
	ImGui::Begin(WINDOW_NAME, nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				sliderInt = 2;
			}
			else if (ImGui::MenuItem("Load"))
			{
				sliderInt = 5;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	if (ImGui::Button("DATA"))
	{
		imguiParticle = GetStatus();
	}
	if (ImGui::Button("OUT"))
	{
		OutputStatus();
	}

	//�O���t
	static float v[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	ImGui::Bezier("test22", v);       // draw

	float y = ImGui::BezierValue(0.5f, v); // x delta in [0..1] range

	//�O���t�̎l�p����łȂ��悤�ɂ�����
	{
		if (v[0] <= 0.0f)
		{
			v[0] = 0.0f;
		}

		if (v[0] >= 1.0f)
		{
			v[0] = 1.0f;
		}

		if (v[1] <= 0.0f)
		{
			v[1] = 0.0f;
		}

		if (v[1] >= 1.0f)
		{
			v[1] = 1.0f;
		}

		if (v[2] >= 1.0f)
		{
			v[2] = 1.0f;
		}

		if (v[2] <= 0.0f)
		{
			v[2] = 0.0f;
		}

		if (v[3] >= 1.0f)
		{
			v[3] = 1.0f;
		}

		if (v[3] <= 0.0f)
		{
			v[3] = 0.0f;
		}
	}

	// �e�L�X�g�\��
	ImGui::Text("FPS  : %.2f", ImGui::GetIO().Framerate);
	ImGui::Separator();


	//�G�t�F�N�g�֌W
	if (ImGui::TreeNode("Effecttree1", "EffectSetting"))
	{
		imguiParticle.color.col.a = 1.0f;
		imguiParticle.color.destCol.a = 1.0f;

		//imguiParticle.fScale = 50.0f;

		if (ImGui::Button("LOAD TEXTURE"))
		{
			GetFile(nullptr, FileString, sizeof(FileString), TEXT("C:\\"));
		}

		if (ImGui::Checkbox("EffectEnable", &useEffect))
		{
			if (!s_bEffectEnable)
			{
				s_bEffectEnable = true;
			}
			else if (s_bEffectEnable)
			{
				s_bEffectEnable = false;
				imguiParticle.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				imguiParticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				imguiParticle.nLife = 60;
				imguiParticle.fRadius = 0.5f;
			}
		}

		if (ImGui::Button("default"))
		{
			imguiParticle.pos.x = (float)SCREEN_WIDTH * 0.5f;
			imguiParticle.pos.y = (float)SCREEN_HEIGHT * 0.5f;
			imguiParticle.maxPopPos.x = 0.0f;
			imguiParticle.maxPopPos.y = 0.0f;
			imguiParticle.minPopPos.x = 0.0f;
			imguiParticle.minPopPos.y = 0.0f;
			imguiParticle.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			imguiParticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			imguiParticle.color.col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
			imguiParticle.color.destCol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			imguiParticle.color.colRandamMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			imguiParticle.color.colRandamMin = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			imguiParticle.color.colTransition = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			imguiParticle.nLife = 60;
			imguiParticle.fScale = 50.0f;
			imguiParticle.fRadius = 4.5f;
			imguiParticle.fAngle = 20.5f;
			imguiParticle.fAttenuation = 0.98f;
			imguiParticle.alphaBlend = (ALPHABLENDTYPE)0;
		}

		//EffectData *Effect = GetStatus();
		ImGui::SliderFloat("PosX", &imguiParticle.pos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("PosY", &imguiParticle.pos.y, 0, (float)SCREEN_HEIGHT);

		// �����͈͂̐ݒ�
		ImGui::SliderFloat("MaxPopPosX", &imguiParticle.maxPopPos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("MinPopPosX", &imguiParticle.minPopPos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("MaxPopPosY", &imguiParticle.maxPopPos.y, 0, (float)SCREEN_HEIGHT);
		ImGui::SliderFloat("MinPopPosY", &imguiParticle.minPopPos.y, 0, (float)SCREEN_HEIGHT);

		ImGui::InputFloat3("SettingEffectMove", imguiParticle.move, "%f");
		ImGui::SliderFloat("MoveX", &imguiParticle.move.x, -100.0f, 100.0f);
		ImGui::SliderFloat("MoveY", &imguiParticle.move.y, -100.0f, 100.0f);

		//�ڍ�
		if (ImGui::TreeNode("Effecttree2", "Details"))
		{
			//rot�v�Z�p
			static float s_fDeg = 0.0f;

			ImGui::InputFloat3("SettingEffectRot", imguiParticle.rot, "%f");
			ImGui::SliderFloat("Rot", &s_fDeg, -D3DX_PI, D3DX_PI);

			if (ImGui::Checkbox("BackRot", &s_bRot))
			{
				imguiParticle.bBackrot = !imguiParticle.bBackrot;
			}

			float rotX = imguiParticle.pos.x * cosf(s_fDeg) + imguiParticle.pos.x * sinf(s_fDeg);
			float rotY = imguiParticle.pos.y * sinf(s_fDeg) - imguiParticle.pos.y * cosf(s_fDeg);
			float fAngle = atan2f(rotX, rotY);
			imguiParticle.rot = D3DXVECTOR3(rotX, rotY, fAngle);
			
			if (imguiParticle.rot.z > D3DX_PI)
			{
				imguiParticle.rot.z -= D3DX_PI * 2;
			}
			else if (imguiParticle.rot.z < -D3DX_PI)
			{
				imguiParticle.rot.z += D3DX_PI * 2;
			}

			ImGui::SliderFloat("TextureScale", &imguiParticle.fScale, 0.0f, 100.0f);
			ImGui::SliderInt("Life", &imguiParticle.nLife, 0, 500);
			ImGui::SliderFloat("Radius", &imguiParticle.fRadius, 0.0f, 100.0f);
			ImGui::SliderAngle("Angle", &imguiParticle.fAngle, 0.0f, 2000.0f);
			ImGui::SliderFloat("Attenuation", &imguiParticle.fAttenuation, 0.0f, 1.0f);

			//�������������Ȃ�����������p
			if (ImGui::Button("DataRemove"))
			{
				DeleteParticleAll();
				RemoveAngle();
			}

			//�c���[�����
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Effecttree3", "Color"))
		{
			//�J���[�p���b�g
			ImGui::ColorEdit4("clear color", (float*)&imguiParticle.color.col);

			// �����_���J���[
			ImGui::Checkbox("ColorRandom", &imguiParticle.color.bColRandom);

			if (imguiParticle.color.bColRandom)
			{
				ImGui::ColorEdit4("clear RandamMax", (float*)&imguiParticle.color.colRandamMax);
				ImGui::ColorEdit4("clear RandamMin", (float*)&imguiParticle.color.colRandamMin);
			}

			// �J���[�g���f�B�V����
			ImGui::Checkbox("ColorTransition", &imguiParticle.color.bColTransition);

			if (imguiParticle.color.bColTransition)
			{// �ړI�̐F
				ImGui::ColorEdit4("clear destColor", (float*)&imguiParticle.color.destCol);

				ImGui::Checkbox("RandomTransitionTime", &imguiParticle.color.bRandomTransitionTime);

				if (!imguiParticle.color.bRandomTransitionTime)
				{
					ImGui::SliderInt("EndTime", &imguiParticle.color.nEndTime, 0, imguiParticle.nLife);
				}
			}

			//�c���[�����
			ImGui::TreePop();
		}

		//�O���f�[�V����
		if (ImGui::TreeNode("Effecttree4", "Gradation"))
		{
			static float s_fCustR[10];
			static float s_fCustG[10];
			static float s_fCustB[10];
			static int s_nSpeed = 1;
			static int selecttype = 0;

			ImGui::RadioButton("Custom", &selecttype, 1);

			if (selecttype == 1)
			{
				static int s_nSetTime = 0;
				static int nTypeNum = 0;
				const char *Items[] = { "Red", "Green", "Blue"};
				ImGui::Combo("ColorType", &nTypeNum, Items, IM_ARRAYSIZE(Items));

				//��
				if (nTypeNum == 0)
				{
					ImGui::PlotLines("Custom Gradation", s_fCustR, IM_ARRAYSIZE(s_fCustR), 0, nullptr, -0.5f, 0.5f, ImVec2(0, 100));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::SliderFloat("Red", &s_fCustR[s_nSetTime], -0.5f, 0.5f);
					ImGui::PopStyleColor();
				}

				//��
				if (nTypeNum == 1)
				{
					ImGui::PlotLines("Custom Gradation", s_fCustG, IM_ARRAYSIZE(s_fCustG), 0, nullptr, -0.5f, 0.5f, ImVec2(0, 100));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::SliderFloat("Green", &s_fCustG[s_nSetTime], -0.5f, 0.5f);
					ImGui::PopStyleColor();
				}

				//��
				if (nTypeNum == 2)
				{
					ImGui::PlotLines("Custom Gradation", s_fCustB, IM_ARRAYSIZE(s_fCustB), 0, nullptr, -0.5f, 0.5f, ImVec2(0, 100));

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.6f, 1.0f, 1.0f));
					ImGui::SliderFloat("Blue", &s_fCustB[s_nSetTime], -0.5f, 0.5f);
					ImGui::PopStyleColor();
				}

				ImGui::SliderInt("SetKey", &s_nSetTime, 0, 9);
				ImGui::SliderInt("Speed", &s_nSpeed, 1, 30);		//���l�������Ȃ�ƕω����x���������ɂȂ�

				/*�O���t�̑S�Ă̐F�̐��l���O�ɂ���*/
				if (ImGui::Button("All Zero"))
				{
					for (int i = 0; i < 10; i++)
					{
						memset(&s_fCustR[i], 0, sizeof(s_fCustR[i]));
						memset(&s_fCustG[i], 0, sizeof(s_fCustG[i]));
						memset(&s_fCustB[i], 0, sizeof(s_fCustB[i]));
					}
				}
			}

			ImGui::RadioButton("Gradation None", &selecttype, 0);

			static int s_nCounter;
			static int s_nTimer;
			static int s_nColNum;

			switch (selecttype)
			{
			case 1:
				s_nCounter++;

				//�[�����Z���
				if (s_nSpeed <= 0)
				{
					s_nSpeed = 1;
				}

				if ((s_nCounter % s_nSpeed) == 0)
				{//��莞�Ԍo��
					s_nTimer++;

					if (s_nTimer >= 5)
					{
						imguiParticle.color.colTransition = D3DXCOLOR(s_fCustR[s_nColNum], s_fCustG[s_nColNum], s_fCustB[s_nColNum], 0.0f);
						s_nColNum++;
						s_nTimer = 0;
					}
				}

				if (s_nCounter >= 60)
				{
					s_nCounter = 0;
				}

				if (s_nColNum >= 10)
				{
					s_nColNum = 0;
				}

				break;

			case 2:
				
				break;
			case 0:
				break;
			default:
				break;
			}

			ImGui::SliderFloat("Alpha", &imguiParticle.color.colTransition.a, -0.5f, 0.0f);

			ImGui::TreePop();
		}

		// ���u�����f�B���O�̎��
		if (ImGui::TreeNode("Effecttree5", "AlphaBlending"))
		{
			// �ϐ��錾
			int	nBlendingType = (int)imguiParticle.alphaBlend;		// ��ʕύX�p�̕ϐ�

			ImGui::RadioButton("AddBlend", &nBlendingType, 0);
			ImGui::RadioButton("SubBlend", &nBlendingType, 1);
			ImGui::RadioButton("BlendNone", &nBlendingType, 2);

			imguiParticle.alphaBlend = (ALPHABLENDTYPE)nBlendingType;

			//�c���[�����
			ImGui::TreePop();
		}

		//�c���[�����
		ImGui::TreePop();
	}



	ImPlot::ShowDemoWindow();

	ImGui::End();

#endif // _DEBUG
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawImguiProperty(void)
{
#ifdef _DEBUG

	if (!s_window)
	{// �E�C���h�E���g�p���Ȃ�
		return;
	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

#endif // _DEBUG
}

//--------------------------------------------------
// Imgui�̎擾
//--------------------------------------------------
Particle& GetImguiParticle(void)
{
	return imguiParticle;
}

//--------------------------------------------------
// �t�@�C�����̎擾
//--------------------------------------------------
char* GetFileName(void)
{
	return FileString;
}

//--------------------------------------------------
// �G�t�F�N�g�g�p�󋵂̎擾
//--------------------------------------------------
bool bSetImguiParticle(void)
{
	return s_bEffectEnable;
}

