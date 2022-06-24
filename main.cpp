//=================================================
//
// �p�[�e�B�N���G�f�B�^
// Auther : Hamada Ryuuga
// Auther ; Tanaka Kouta
// Auther : Yuda Kaito
//
//=================================================
//=================================================
// include
//=================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "game.h"
#include "file.h"
#include "renderer.h"
// imgui�n��
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>
#include "resource1.h"
#include "imgui_property.h"

// ���C�u�����̓Ǎ���
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v

//�}�N����`
#define CLASS_NAME	"WindowClass"
#define WINDOW_NAME	"effect"

//=================================================
// �ÓI�ϐ�
//=================================================
static int s_nCountFPS;							// FPS�̃J�E���^�[
static bool bPress = false;						// ���{���o�[�̃g���K�[�����̂��߂ɕK�v�ȕϐ�
static D3DPRESENT_PARAMETERS s_d3dpp = {};

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ImGui
static float s_fAngle = 20.0f;
static char FileString[MAX_PATH * 256];
static bool bTexUse = false;
inline unsigned long FloattoDword(float fVal) { return *((unsigned long*)&fVal); }

char buffer1[MAX_PATH];
CRenderer* renderer;

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

	if (FAILED(Init(hInstance, hWnd, true)))	// ������false�ɂ���Ƒ��ʂɂȂ�
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
	InitImguiProperty(hWnd, renderer->GetDevice());
	//ImGui_ImplWin32_Init(hWnd);
	//ImGui_ImplDX9_Init(s_pD3DDevice);

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
				UpdateImguiProperty();
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
		ofn.lpstrDefExt = TEXT(".png");
		ofn.lpstrFilter = TEXT("png�t�@�C��(*.png)\0*.png\0");
		ofn.lpstrTitle = TEXT("�摜�t�@�C����ۑ����܂��B");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	if (GetSaveFileName(&ofn)) {
		MessageBox(hWnd, szFile, TEXT("�t�@�C������t���ĕۑ�"), MB_OK);
	}

	if (szFile[0] != '\0')
	{
		/*std::string Data = ;
		*/

		CopyFile((LPCTSTR)buffer1, // �����̃t�@�C���̖��O
			szFile, // �V�����t�@�C���̖��O
			false // �t�@�C�������݂���ꍇ�̓���
		);
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
	{
		return true;
	}

	int nID;//�Ԃ�l���i�[
	static HWND hWndEditlnput1;		//���̓E�B���h�E�n���h��(���ʎq)

									//char buffer1[MAX_PATH];
	int i, _size;

	switch (uMsg)
	{

	case WM_CREATE:
		//�t�@�C���h���b�v���󂯎���悤�ɐݒ肷��
		DragAcceptFiles(hWnd, true);
		break;

	case WM_DROPFILES:
		//�h���b�v���ꂽ�t�@�C�������擾����
		_size = DragQueryFile((HDROP)wParam, -1, buffer1, MAX_PATH);
		for (i = 0; i<_size; i++)
		{
			//�h���b�v���ꂽ�t�@�C�������擾����
			DragQueryFile((HDROP)wParam, i, buffer1, MAX_PATH);
			funcFileSave(hWnd, false);
		}

		MessageBox(hWnd, buffer1, "���", MB_OK);
		//�t�@�C�����̓����f�[�^���������
		DragFinish((HDROP)wParam);



		break;
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
	return renderer->GetDevice();
}

//---------------------------------------
// ������
//---------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)//TRUE�F�E�C���h�E/FALSE:�t���X�N���[��
{
	renderer = new CRenderer;

	renderer->Init(hWnd, bWindow);

	//���͏����̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�����̏�����
	srand((unsigned int)time(0));

	InitGame();	// �Q�[�����[�h�̏�����

	LoodJson(L"data/FILE/Effect.json");

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

	renderer->Uninit();
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
	renderer->Draw();
}

//---------------------------------------
//�f�B���N�g���\���p
//---------------------------------------
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir)
{
	OPENFILENAME ofn;

	fname[0] = TEXT('\0');

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = fname;
	ofn.nMaxFile = nsize;
	ofn.lpstrInitialDir = initDir;
	ofn.lpstrTitle = TEXT("�t�@�C���w��");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	bTexUse = true;

	return GetOpenFileName(&ofn);
}

//---------------------------------------
// FPS�̎擾
//---------------------------------------
int GetFPS()
{
	return s_nCountFPS;
}


float GetAngle(void)
{
	return s_fAngle;
}

//---------------------------------------
// �t�@�C���ˁ[��
//---------------------------------------
char GetFileName(int nNum)
{
	return FileString[nNum];
}

bool TexUse(void)
{
	return bTexUse;
}
