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
#include <imgui_internal.h>
#include <assert.h>

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
	ImGui::DestroyContext();

	// �E�C���h�E�̔j��
	::DestroyWindow(hWnd);

	// �E�C���h�E�N���X�̓o�^������
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

#endif // _DEBUG
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
	// �e�L�X�g�\��
	ImGui::Text("FPS  : %.2f", ImGui::GetIO().Framerate);
	ImGui::Separator();

	//�G�t�F�N�g�֌W
	if (ImGui::TreeNode("Effecttree1", "EffectSetting"))
	{
		//rot�v�Z�p
		static float s_fDeg = 0.0f;
		float rotX = imguiParticle.pos.x * cosf(s_fDeg) + imguiParticle.pos.x * sinf(s_fDeg);
		float rotY = imguiParticle.pos.y * sinf(s_fDeg) - imguiParticle.pos.y * cosf(s_fDeg);
		float fAngle = atan2f(rotX, rotY);
		imguiParticle.rot = D3DXVECTOR3(rotX, rotY, fAngle);

		imguiParticle.col.a = 1.0f;
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

			bSetEffect();
		}

		if (ImGui::Button("default"))
		{
			imguiParticle.pos.x = (float)SCREEN_WIDTH / 2;
			imguiParticle.pos.y = (float)SCREEN_HEIGHT / 2;
			imguiParticle.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			imguiParticle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			imguiParticle.col = D3DXCOLOR(0.5f,0.0f,1.0f,1.0f);
			imguiParticle.nLife = 60;
			imguiParticle.fScale = 50.0f;
			imguiParticle.fRadius = 4.5f;
			imguiParticle.fAngle = 20.5f;
			imguiParticle.fAttenuation = 0.98f;
		}

		//EffectData *Effect = GetStatus();
		ImGui::SliderFloat("PosX", &imguiParticle.pos.x, 0, (float)SCREEN_WIDTH);
		ImGui::SliderFloat("PosY", &imguiParticle.pos.y, 0, (float)SCREEN_HEIGHT);

		ImGui::InputFloat3("SettingEffectMove", imguiParticle.move, "%f");
		ImGui::SliderFloat("MoveX", &imguiParticle.move.x, -100.0f, 100.0f);
		ImGui::SliderFloat("MoveY", &imguiParticle.move.y, -100.0f, 100.0f);

		//�ڍ�
		if (ImGui::TreeNode("Effecttree2", "Details"))
		{
			ImGui::InputFloat3("SettingEffectRot", imguiParticle.rot, "%f");
			ImGui::SliderFloat("Rot", &s_fDeg, -D3DX_PI, D3DX_PI);

			if (ImGui::Checkbox("BackRot", &s_bRot))
			{
				imguiParticle.bBackrot = !imguiParticle.bBackrot;
			}

			//if (ImGui::Checkbox("TextureRot", &bTexRot))
			if (s_fDeg > D3DX_PI)
			{
				s_fDeg -= D3DX_PI * 2;
			}
			else if (s_fDeg < -D3DX_PI)
			{
				s_fDeg += D3DX_PI * 2;
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

		//�J���[�p���b�g
		ImGui::ColorEdit4("clear color", (float*)&imguiParticle.col); // Edit 3 floats representing a color
		GetColor();

		//�O���f�[�V����
		if (ImGui::TreeNode("Effecttree3", "Gradation"))
		{
			static float s_fCustR[10];
			static float s_fCustG[10];
			static float s_fCustB[10];
			static int s_nSpeed = 1;
			static int selecttype = 0;

			ImGui::RadioButton("RPlus GSubtract", &selecttype, 1);
			ImGui::RadioButton("GPlus BSubtract", &selecttype, 2);
			ImGui::RadioButton("BPlus RSubtract", &selecttype, 3);
			ImGui::RadioButton("Random", &selecttype, 4);

			if (selecttype == 4)
			{
				static float randColMax = 1.0f;
				static float randColMin = 0.0f;
				ImGui::InputFloat("RandomMin", &randColMin);
				ImGui::InputFloat("RandomMax", &randColMax);

				imguiParticle.colRandamMin = D3DXCOLOR(randColMin, randColMin, randColMin, 1.0f);
				imguiParticle.colRandamMax = D3DXCOLOR(randColMax, randColMax, randColMax, 1.0f);
				
			}

			ImGui::RadioButton("Custom", &selecttype, 5);

			if (selecttype == 5)
			{
				static int s_nSetTime = 0.0f;
				static float Color[10];

				static int nTypeNum = 0;
				const char *Items[] = { "Red", "Green", "Blue"};
				ImGui::Combo("ColorType", &nTypeNum, Items, IM_ARRAYSIZE(Items));

				ImGui::PlotLines("Custom Gradation", Color, IM_ARRAYSIZE(Color), 0, nullptr, 0.0f, 0.5f, ImVec2(0, 100));
				ImGui::SliderInt("SetTime", &s_nSetTime, 0, 10);
				ImGui::SliderInt("Speed", &s_nSpeed, 1, 30);

				if (nTypeNum == 0)
				{
					ImGui::SliderFloat("Red", &Color[s_nSetTime], 0.0f, 0.5f);

					if (ImGui::Button("Graph Assign"))
					{
						for (int i = 0; i < 10; i++)
						{
							s_fCustR[i] = Color[i];
						}
					}
				}

				if (nTypeNum == 1)
				{
					ImGui::SliderFloat("Green", &Color[s_nSetTime], 0.0f, 0.5f);

					if (ImGui::Button("Graph Assign"))
					{
						for (int i = 0; i < 10; i++)
						{
							s_fCustG[i] = Color[i];
						}
					}
				}

				if (nTypeNum == 2)
				{
					ImGui::SliderFloat("Blue", &Color[s_nSetTime], 0.0f, 0.5f);

					if (ImGui::Button("Graph Assign"))
					{
						for (int i = 0; i < 10; i++)
						{
							s_fCustB[i] = Color[i];
						}
					}
				}

				ImGui::SameLine();
				if (ImGui::Button("All Zero"))
				{
					for (int i = 0; i < 10; i++)
					{
						memset(&Color[i], 0, sizeof(Color[i]));
						memset(&s_fCustR[i], 0, sizeof(s_fCustR[i]));
						memset(&s_fCustG[i], 0, sizeof(s_fCustG[i]));
						memset(&s_fCustB[i], 0, sizeof(s_fCustB[i]));
					}
				}
			}

			ImGui::RadioButton("Gradation None", &selecttype, 0);

			//�F�ύX�iImGui�j
			D3DXCOLOR RandCol = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			static int s_nCounter;
			static int s_nTimer;
			static int s_nColNum;

			switch (selecttype)
			{
			case 1:
				imguiParticle.colTransition = D3DXCOLOR(0.0f, -0.01f, 0.0f, 0.0f);
				imguiParticle.col.r = 1.0f;
				break;

			case 2:
				imguiParticle.colTransition = D3DXCOLOR(0.0f, 0.0f, -0.01f, 0.0f);
				imguiParticle.col.g = 1.0f;
				break;

			case 3:
				imguiParticle.colTransition = D3DXCOLOR(-0.01f, 0.0f, 0.0f, 0.0f);
				imguiParticle.col.b = 1.0f;
				break;

			case 4:
				RandCol = ((float)imguiParticle.colRandamMin + (((float)rand() * (float)imguiParticle.colRandamMax - (float)imguiParticle.colRandamMin + 1.0f) / (1.0f + RAND_MAX)));
				imguiParticle.col = RandCol;

				imguiParticle.col.a = 1.0f;
				break;

			case 5:
				s_nCounter++;

				if ((s_nCounter % 3) == 0)
				{//��莞�Ԍo��
					s_nTimer++;

					if (s_nTimer >= 5)
					{
						imguiParticle.colTransition = D3DXCOLOR(s_fCustR[s_nColNum], s_fCustG[s_nColNum], s_fCustB[s_nColNum], 0.0f);
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

			case 0:
				break;
			default:
				break;
			}

			ImGui::SliderFloat("Alpha", &imguiParticle.colTransition.a, -1.0f, 0.0f);

			ImGui::TreePop();
		}

		//�c���[�����
		ImGui::TreePop();
	}

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
