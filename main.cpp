//=================================================
//
// パーティクルエディタ
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
// imgui系統
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>
#include "resource1.h"
#include "imgui_property.h"

// ライブラリの読込み
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要

//マクロ定義
#define CLASS_NAME	"WindowClass"
#define WINDOW_NAME	"effect"

//=================================================
// 静的変数
//=================================================
static int s_nCountFPS;							// FPSのカウンター
static bool bPress = false;						// リボンバーのトリガー処理のために必要な変数
static D3DPRESENT_PARAMETERS s_d3dpp = {};

//プロトタイプ宣言
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
//メイン関数
//===================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hlnstacePrev, LPSTR ipCmdLine, int nCmdShow)
{
	HWND hWnd;	//Windowハンドル識別子
	MSG msg;	//メッセージを格納する変数
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),											// WNDCLASSEXのメモリサイズ
		CS_HREDRAW | CS_VREDRAW,									// ウインドウのスタイル
		WindowProc,													// Windowプロシージャ  
		0,															// ゼロにする
		0,															// ゼロにする
		hInstance,													// インスタンスハンドル
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)),		// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),								// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),									// クライアントの領域背景色
		MAKEINTRESOURCE(IDR_MENU1) ,								// メニューバー
		CLASS_NAME,													// Windowクラスの名前
		LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION))	// ファイルアイコン
	};

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(
		0,							// 拡張Windowsスタイル
		CLASS_NAME,					// Windowクラスの名前
		WINDOW_NAME,				// Windowの名前
		WS_OVERLAPPEDWINDOW,		// Windowスタイル
		CW_USEDEFAULT,				// Windowの左上X座標
		CW_USEDEFAULT,				// Window左上Y座標
		(rect.right - rect.left),	// Window幅
		(rect.bottom - rect.top),	// Window高さ
		NULL,						// 親Windowのハンドル
		NULL,						// メニューハンドルまたは個子WindowID
		hInstance,					// インスタンスハンドル
		IDI_APPLICATION);			// Window作成データ

	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExedastTime;	// 最後更新時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後のFPS

	if (FAILED(Init(hInstance, hWnd, true)))	// ここをfalseにすると大画面になる
	{// 初期化が失敗した場合
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

	// imguiにフォントを追加
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	InitImguiProperty(hWnd, renderer->GetDevice());
	//ImGui_ImplWin32_Init(hWnd);
	//ImGui_ImplDX9_Init(s_pD3DDevice);

	//分解能の設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExedastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受けとったらメッセージループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過
			  // FPS計測
				s_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
		

			if ((dwCurrentTime - dwExedastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExedastTime = dwCurrentTime;	// 処理開始の時刻[現在時刻]を保存

				// imguiの更新
				UpdateImguiProperty();
				// 更新
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	//終了処理
	Uninit();

	::DestroyWindow(hWnd);
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

	//分機能を戻す
	timeEndPeriod(1);

	// Windowクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//========================
//ウィンドウだしてやるやつ
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
		ofn.lpstrInitialDir = szPath;	// 初期フォルダ位置
		ofn.lpstrFile = szFile;			// 選択ファイル格納
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".png");
		ofn.lpstrFilter = TEXT("pngファイル(*.png)\0*.png\0");
		ofn.lpstrTitle = TEXT("画像ファイルを保存します。");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	if (GetSaveFileName(&ofn)) {
		MessageBox(hWnd, szFile, TEXT("ファイル名を付けて保存"), MB_OK);
	}

	if (szFile[0] != '\0')
	{
		/*std::string Data = ;
			*/
		
		 CopyFile((LPCTSTR)buffer1, // 既存のファイルの名前
			szFile, // 新しいファイルの名前
			false // ファイルが存在する場合の動作
		);
	}
	bPress = true;
}

//---------------------------------------
//ウィンドウプロシージャ
//---------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ポイント構造体
	//	POINT    pt;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	int nID;//返り値を格納
	static HWND hWndEditlnput1;		//入力ウィンドウハンドル(識別子)

	//char buffer1[MAX_PATH];
	int i, _size;

	switch (uMsg)
	{

	case WM_CREATE:
		//ファイルドロップを受け取れるように設定する
		DragAcceptFiles(hWnd, true);
		break;

	case WM_DROPFILES:
		//ドロップされたファイル数を取得する
		_size = DragQueryFile((HDROP)wParam, -1, buffer1, MAX_PATH);
		for (i = 0; i<_size; i++)
		{
			//ドロップされたファイル名を取得する
			DragQueryFile((HDROP)wParam, i, buffer1, MAX_PATH);
			funcFileSave(hWnd, false);
		}
	
		MessageBox(hWnd, buffer1, "情報", MB_OK);
		//ファイル情報の内部データを解放する
		DragFinish((HDROP)wParam);

		

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //	キーのメッセージ

		switch (wParam)
		{
		case VK_ESCAPE: //エスケープが押された

			nID = MessageBox(hWnd, "終了しますか？","終わりのコマンド", MB_YESNO | MB_TOPMOST);
			// 第一引数をNULLにするとメッセージBOXアクティブウィンドウにならない
			// 第一引数をhWndにするとこのウィンドウが親(オーナー)になり、
			// このメッセージBOXを処理しない限りほかの処理ができない
			// 入力ウィンドウの生成

			if (nID == IDYES)
			{// Windowを破壊する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
		break;

		// case WM_RBUTTONDOWN:
		// pt.x = LOWORD(lParam);
		// pt.y = HIWORD(lParam);
		// //クライアント座標をスクリーン座標へ変換
		// ClientToScreen(hWnd, &pt);
		// //ポップアップメニューを表示
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
//デバイス取得
//---------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return renderer->GetDevice();
}

//---------------------------------------
// 初期化
//---------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)//TRUE：ウインドウ/FALSE:フルスクリーン
{
	renderer = new CRenderer;

	renderer->Init(hWnd, bWindow);

	//入力処理の初期化処理
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//乱数の初期化
	srand((unsigned int)time(0));

	InitGame();	// ゲームモードの初期化

	LoodJson(L"data/FILE/Effect.json");

	return S_OK;
}

//---------------------------------------
// 終了
//---------------------------------------
void Uninit(void)
{
	//終了処理
	UninitInput();	// 入力
	UninitGame();	// ゲームモード

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	renderer->Uninit();
}

//---------------------------------------
// 更新
//---------------------------------------
void Update(void)
{
	//更新処理
	UpdateInput();
	UpdateGame();
}

//---------------------------------------
// 描画
//---------------------------------------
void Draw(void)
{
	renderer->Draw();
}

//---------------------------------------
//ディレクトリ表示用
//---------------------------------------
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir)
{
	OPENFILENAME ofn;

	fname[0] = TEXT('\0');

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = TEXT("全てのファイル(*.*)\0*.*\0");
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = fname;
	ofn.nMaxFile = nsize;
	ofn.lpstrInitialDir = initDir;
	ofn.lpstrTitle = TEXT("ファイル指定");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	bTexUse = true;

	return GetOpenFileName(&ofn);
}

//---------------------------------------
// FPSの取得
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
// ファイルねーむ
//---------------------------------------
char GetFileName(int nNum)
{
	return FileString[nNum];
}

bool TexUse(void)
{
	return bTexUse;
}

