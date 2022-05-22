//=================================================
//	��������
//
// �}�b�v�G�f�B�^
// Auther�F Hamada Ryuuga
//
//=================================================
#include <stdio.h>
#include <d3d9.h>
#include <tchar.h>
#include <time.h>
#include "main.h"
#include "input.h"
#include "game.h"
// imgui�n��
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>
#include "resource1.h"
// imgui�ɕ`�悷����
#include "particle.h"
#include "effect.h"

//�}�N����`
#define MAX_NAME (7)
#define IMGUI_DEFINE_MATH_OPERATORS
#define CLASS_NAME	"WindowClass"
#define WINDOW_NAME	"effect"
#define IMGUI_DEFINE_MATH_OPERATORS

//�O���[�o���ϐ�
//LPDIRECT3D9			g_pD3D = NULL;
//LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;

//ImGui
//ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
//D3DXVECTOR3 setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//D3DXVECTOR3 setmove = D3DXVECTOR3(0.0f,0.0f,0.0f);
//D3DXVECTOR3 setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//static int selecttype = 0;
//static int nLife = 0;
//static float fRadius = 0;
//bool show_demo_window = true;
//bool show_another_window = false;
//bool useEffect = false;
//bool bMove = false;
//bool bBackRot = false;
//bool bEffectEnable = false;

//=================================================
// �ÓI�ϐ�
//=================================================
static LPDIRECT3D9 s_pD3D = NULL;				// Direct3d�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECT3DDEVICE9 s_pD3DDevice = NULL;	// Direct3d�f�o�C�X�ւ̂ۂ���
static int s_nCountFPS;							// FPS�̃J�E���^�[
static bool bPress = false;						// ���{���o�[�̃g���K�[�����̂��߂ɕK�v�ȕϐ�
static D3DPRESENT_PARAMETERS s_d3dpp = {};
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static bool show_demo_window = true;//��{�̌Ăяo��
static bool show_another_window = false;//������Ăяo��
static char Txet[8] = "";
static bool useEffect = false;
static bool bMove = false;
static bool bBackRot = false;
static bool bEffectEnable = false;
static int selecttype = 0;
static int nLife = 0;
static float fRadius = 0;
static D3DXVECTOR3 setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 setmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

bool ImGuiText(bool show_demo_window, bool show_another_window);
int Button(int nSize);
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//===================
//���C���֐�
//===================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hlnstacePrev, LPSTR ipCmdLine, int nCmdShow)
{
	HWND hWnd;	//Window�n���h�����ʎq
	MSG msg;	//���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),											// WNDCLASSEX�̃������T�C�Y
		CS_HREDRAW | CS_VREDRAW,									// �E�C���h�E�̃X�^�C��
		WindowProc,													// Window�v���V�[�W��  
		0,															// �[���ɂ���
		0,															// �[���ɂ���
		hInstance,													// �C���X�^���X�n���h��
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)),		// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),								// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),									// �N���C�A���g�̗̈�w�i�F
		MAKEINTRESOURCE(IDR_MENU1) ,								// ���j���[�o�[
		CLASS_NAME,													// Window�N���X�̖��O
		LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION))	// �t�@�C���A�C�R��
	};

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(
		0,							// �g��Windows�X�^�C��
		CLASS_NAME,					// Window�N���X�̖��O
		WINDOW_NAME,				// Window�̖��O
		WS_OVERLAPPEDWINDOW,		// Window�X�^�C��
		CW_USEDEFAULT,				// Window�̍���X���W
		CW_USEDEFAULT,				// Window����Y���W
		(rect.right - rect.left),	// Window��
		(rect.bottom - rect.top),	// Window����
		NULL,						// �eWindow�̃n���h��
		NULL,						// ���j���[�n���h���܂��͌qWindowID
		hInstance,					// �C���X�^���X�n���h��
		IDI_APPLICATION);			// Window�쐬�f�[�^

	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExedastTime;	// �Ō�X�V����
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS

	if (FAILED(Init(hInstance, hWnd, TRUE)))	// ������false�ɂ���Ƒ��ʂɂȂ�
	{// �����������s�����ꍇ
		return -1;
	}

	// Show the window
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontConfig config;
	config.MergeMode = true;

	// imgui�Ƀt�H���g��ǉ�
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(s_pD3DDevice);


	//����\�̐ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExedastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯂Ƃ����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5�b�o��
			  // FPS�v��
				s_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExedastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExedastTime = dwCurrentTime;	// �����J�n�̎���[���ݎ���]��ۑ�

				// imgui�̍X�V
				show_another_window = ImGuiText(show_demo_window, show_another_window);

				// �X�V
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	//�I������
	Uninit();

	::DestroyWindow(hWnd);
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

	//���@�\��߂�
	timeEndPeriod(1);
	// Window�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//========================
//�E�B���h�E�����Ă����
//========================
static void funcFileSave(HWND hWnd, bool nMap)
{
	static OPENFILENAME	ofn;
	static TCHAR		szPath[MAX_PATH];
	static TCHAR		szFile[MAX_PATH];

	if (szPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrInitialDir = szPath;	// �����t�H���_�ʒu
		ofn.lpstrFile = szFile;			// �I���t�@�C���i�[
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".txt");
		ofn.lpstrFilter = TEXT("txt�t�@�C��(*.txt)\0*.txt\0");
		ofn.lpstrTitle = TEXT("�e�L�X�g�t�@�C����ۑ����܂��B");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	if (GetSaveFileName(&ofn)) {
		MessageBox(hWnd, szFile, TEXT("�t�@�C������t���ĕۑ�"), MB_OK);
	}

	if (szFile[0] != '\0')
	{
		if (nMap)
		{
			//OutputMap(szFile);
		}
		if (!nMap)
		{
			//OutputEnemy(szFile);
		}
	}
	bPress = true;
}

//---------------------------------------
//�E�B���h�E�v���V�[�W��
//---------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�|�C���g�\����
	//	POINT    pt;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	int nID;//�Ԃ�l���i�[
	static HWND hWndEditlnput1;		//���̓E�B���h�E�n���h��(���ʎq)

	switch (uMsg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //	�L�[�̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE: //�G�X�P�[�v�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H", "�I���̃R�}���h", MB_YESNO | MB_TOPMOST);
			// ��������NULL�ɂ���ƃ��b�Z�[�WBOX�A�N�e�B�u�E�B���h�E�ɂȂ�Ȃ�
			// ��������hWnd�ɂ���Ƃ��̃E�B���h�E���e(�I�[�i�[)�ɂȂ�A
			// ���̃��b�Z�[�WBOX���������Ȃ�����ق��̏������ł��Ȃ�
			// ���̓E�B���h�E�̐���

			if (nID == IDYES)
			{// Window��j�󂷂�(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
		break;

		// case WM_RBUTTONDOWN:
		// pt.x = LOWORD(lParam);
		// pt.y = HIWORD(lParam);
		// //�N���C�A���g���W���X�N���[�����W�֕ϊ�
		// ClientToScreen(hWnd, &pt);
		// //�|�b�v�A�b�v���j���[��\��
		// TrackPopupMenu(GetSubMenu(GetMenu(hWnd), 0), TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		// break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		default:
			bPress = false;
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//---------------------------------------
//�f�o�C�X�擾
//---------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return s_pD3DDevice;
}

//---------------------------------------
// ������
//---------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE�F�E�C���h�E/FALSE:�t���X�N���[��
{
	// ���[�J���ϐ��錾
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3d�I�u�W�F�N�g�̐���
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �|���S���̏���������
	ZeroMemory(&d3dpp, sizeof(d3dpp));	// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`����
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ��i�f���M���̓����j
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ��ĂP�U��it���g��
	d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if ((FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))) &&
		(FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))) &&
		(FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))))
	{
		return E_FAIL;
	}

	//���W�_�[�X�e�[�g�̐ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h�ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �A���t�@�u�����h�ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �A���t�@�u�����h�ݒ�

	//�T���v���[�X�e�[�g�̐ݒ�
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �������̊g��
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �傫���̏k��
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�̐ݒ�
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �|���S���ƃe�N�X�`���̐F��������
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//���͏����̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�����̏�����
	srand((unsigned int)time(0));

	InitGame();	// �Q�[�����[�h�̏�����

	return S_OK;
}

//---------------------------------------
// �I��
//---------------------------------------
void Uninit(void)
{
	//�I������
	UninitInput();	// ����
	UninitGame();	// �Q�[�����[�h

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (s_pD3D != NULL)
	{
		s_pD3D->Release();
		s_pD3D = NULL;
	}
	if (s_pD3DDevice != NULL)
	{
		s_pD3DDevice->Release();
		s_pD3DDevice = NULL;
	}
}

//---------------------------------------
// �X�V
//---------------------------------------
void Update(void)
{
	//�X�V����
	UpdateInput();
	UpdateGame();
}

//---------------------------------------
// �`��
//---------------------------------------
void Draw(void)
{
	//��ʃN���A
	s_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{//���������Ƃ�

		DrawGame();

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		s_pD3DDevice->EndScene();	//�`��I��
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//---------------------------------------
// FPS�̎擾
//---------------------------------------
int GetFPS()
{
	return s_nCountFPS;
}

//---------------------------------------
//�f�t�H���g
//---------------------------------------
void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = s_pD3DDevice->Reset(&s_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

bool ImGuiText(bool show_demo_window, bool show_another_window)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGui�̃X�^�C���ύX
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImColor(0.0f, 0.0f, 0.1f, 0.4f);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(0.0f, 0.5f, 0.5f, 1.0f);

	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if (show_demo_window)
	{
		static float f = 0.0f;
		static float fSize = 0.0f;
		static int counter = 0;
		static int nSize = 0;
		static char Text[8];
		static bool bRot = false;
		Particle *pParticle = GetParticle();

		ImGui::Begin("Hello, world!");							// Create a window called "Hello, world!" and append into it.

		//FPS�\��
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("This is some useful text.");				// Display some text (you can use a format strings too
		ImGui::InputText("textbox 1", Text, sizeof(Text));
		//ImGui::Checkbox("Demo Window", &show_demo_window);	// Edit bools storing our window open/close state

		//�ʃE�B���h�E�𐶐�
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderInt("Size", &nSize, 0, 10);
		ImGui::SliderFloat("Size", &fSize, 0, 100.0f);
		ImGui::SliderFloat("float", &f, 0.0f, 100.0f);			// Edit 1 float using a slider from 0.0f to 1.0f

		nSize = Button(nSize);

		//�G�t�F�N�g�֌W
		if (ImGui::TreeNode("Effecttree1", "EffectSetting"))
		{
			//rot�v�Z�p
			static float fDeg = 0.0f;
			float fRad = fDeg;
			float rotX = setpos.x * cosf(fDeg) + setpos.x * sinf(fDeg);
			float rotY = setpos.y * sinf(fDeg) - setpos.y * cosf(fDeg);
			float fAngle = atan2f(rotX, rotY);
			setrot = D3DXVECTOR3(rotX, rotY, fAngle);

			if (ImGui::Checkbox("EffectEnable", &useEffect))
			{
				if (!bEffectEnable)
				{
					bEffectEnable = true;
				}

				else if (bEffectEnable)
				{
					bEffectEnable = false;
					setmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					nLife = 60;
					fRadius = 8.0f;
				}

				bSetEffect();
			}

			if (ImGui::Button("default"))
			{
				setpos.x = (float)SCREEN_WIDTH / 2;
				setpos.y = (float)SCREEN_HEIGHT / 2;
				setmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				nLife = 60;
				fRadius = 8.0f;
			}

			ImGui::InputFloat3("SettingEffectPos", setpos, "%f");
			ImGui::SliderFloat("PosX", &setpos.x, 0, (float)SCREEN_WIDTH);
			ImGui::SliderFloat("PosY", &setpos.y, 0, (float)SCREEN_HEIGHT);

			ImGui::InputFloat3("SettingEffectMove", setmove, "%f");
			ImGui::SliderFloat("MoveX", &setmove.x, -100.0f, 100.0f);
			ImGui::SliderFloat("MoveY", &setmove.y, -100.0f, 100.0f);

			//�ڍ�
			if (ImGui::TreeNode("Effecttree2", "Details"))
			{
				ImGui::InputFloat3("SettingEffectRot", setrot, "%f");
				ImGui::SliderFloat("Rot", &fDeg, -D3DX_PI, D3DX_PI);

				if (ImGui::Checkbox("BackRot", &bRot))
				{
					if (!bBackRot)
					{
						bBackRot = true;
					}

					else if (bBackRot)
					{
						bBackRot = false;
					}
				}

				//���K��
				if (fDeg > D3DX_PI)
				{
					fDeg -= D3DX_PI * 2;
				}

				else if (fDeg < -D3DX_PI)
				{
					fDeg += D3DX_PI * 2;
				}

				ImGui::SliderInt("Life", &nLife, 0, 500);
				ImGui::SliderFloat("Radius", &fRadius, 0.0f, 100.0f);

				//�������������Ȃ�����������p
				if (ImGui::Button("DataRemove"))
				{
					for (int i = 0; i < MAX_EFFECT; i++)
					{
						DeleteParticle(i);
					}
					RemoveAngle();
				}

				//�c���[�����
				ImGui::TreePop();
			}

			//�J���[�p���b�g
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			GetColor();

			if (ImGui::TreeNode("Effecttree3", "Gradation"))
			{
				ImGui::RadioButton("RPlus GSubtract", &selecttype, 1);
				ImGui::RadioButton("GPlus BSubtract", &selecttype, 2);
				ImGui::RadioButton("BPlus RSubtract", &selecttype, 3);
				ImGui::RadioButton("Gradation None", &selecttype, 0);

				//�c���[�����
				ImGui::TreePop();
			}

			//�c���[�����
			ImGui::TreePop();
		}

		static float v[] = { 0.390f, 0.575f, 0.565f, 1.000f };

		//ImGui::Bezier("������", v);       // draw
		//float y = ImGui::BezierValue(0.5f, v); // x delta in [0..1] range
		//{ static float v[] = { 0.680f, -0.55f, 0.265f, 1.550f }; ImGui::Bezier("easeInOutBack", v); }

		ImGui::SameLine();
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");

		if (ImGui::Button("Close Me"))
		{
			show_another_window = false;
		}

		ImGui::End();
	}

	////�������w�i�h���[
	return show_another_window;
}

int Button(int nSize)
{
	//�c���[�𐶐�
	if (ImGui::TreeNode("tree1", "Value"))
	{
		if (ImGui::Button("1++"))	// Buttons return true when clicked (most widgets return true when edited/activated)
		{
			nSize++;
		}

		if (ImGui::Button("1--"))	// Buttons return true when clicked (most widgets return true when edited/activated)
		{
			nSize--;
		}

		if (ImGui::Button("5++"))	// Buttons return true when clicked (most widgets return true when edited/activated)
		{
			nSize += 5;
		}

		if (ImGui::Button("5--"))	// Buttons return true when clicked (most widgets return true when edited/activated)
		{
			nSize -= 5;
		}

		if (ImGui::Button("remove"))	// Buttons return true when clicked (most widgets return true when edited/activated)
		{
			nSize = 0;
		}

		//�c���[�����
		ImGui::TreePop();
	}
	return nSize;
}

// �ʒu���Q�b�g
D3DXVECTOR3 GetPos(void)
{
	return D3DXVECTOR3(setpos.x, setpos.y, setpos.z);
}

// �ړ����Q�b�g
D3DXVECTOR3 GetMove(void)
{
	return D3DXVECTOR3(setmove.x, setmove.y, setmove.z);
}

// ��]���Q�b�g
D3DXVECTOR3 GetRot(void)
{
	return D3DXVECTOR3(setrot.x, setrot.y, setrot.z);
}

// �F���Q�b�g
D3DXCOLOR GetColor(void)
{
	return D3DXCOLOR(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
}

int GetType(void)
{
	return selecttype;
}

int GetLife(void)
{
	return nLife;
}

float GetRadius(void)
{
	return fRadius;
}

bool bSetEffect(void)
{
	return bEffectEnable;
}

bool BackRot(void)
{
	return bBackRot;
}