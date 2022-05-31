//=================================================
//	おもちゃ
//
// マップエディタ
// Auther： Hamada Ryuuga
//
//=================================================
#include <stdio.h>
#include <d3d9.h>
#include <tchar.h>
#include <time.h>
#include "main.h"
#include "input.h"
#include "game.h"
#include "file.h"
// imgui系統
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>
#include "resource1.h"
#include "imgui_property.h"
// imguiに描画する情報
#include "particle.h"
#include "effect.h"

//マクロ定義
#define MAX_NAME (7)
#define IMGUI_DEFINE_MATH_OPERATORS
#define CLASS_NAME	"WindowClass"
#define WINDOW_NAME	"effect"
#define IMGUI_DEFINE_MATH_OPERATORS

//グローバル変数
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
// 静的変数
//=================================================
static LPDIRECT3D9 s_pD3D = NULL;				// Direct3dオブジェクトへのポインタ
static LPDIRECT3DDEVICE9 s_pD3DDevice = NULL;	// Direct3dデバイスへのぽいんた
static int s_nCountFPS;							// FPSのカウンター
static bool bPress = false;						// リボンバーのトリガー処理のために必要な変数
static D3DPRESENT_PARAMETERS s_d3dpp = {};

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ImGui
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
D3DXVECTOR3 setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 setmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
inline unsigned long FloattoDword(float fVal) { return *((unsigned long*)&fVal); }
static bool show_demo_window = true;
static bool show_another_window = false;
static float s_fRadius = 0;
static char Txet[8] = "";
static int s_selecttype = 0;
static int s_nLife = 0;
static int selecttype = 0;
static bool s_bBackRot = false;
static bool s_bTextureRot = false;
static bool s_bEffectEnable = false;
static int s_nItem = 0;
static float s_fAlpha = 0.0f;
static float s_fAttenuation = 4.0f;
static float s_fAngle = 20.0f;
static float s_fScale = 50.0f;
static float s_fRandMin = 0;
static float s_fRandMax = 0;
static char FileString[MAX_PATH * 256];
static bool useEffect = false;
static bool bMove = false;
static bool bTexUse = false;

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

	if (FAILED(Init(hInstance, hWnd, TRUE)))	// ここをfalseにすると大画面になる
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
	InitImguiProperty(hWnd, s_pD3DDevice);
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
				//show_another_window = ImGuiText(show_demo_window, show_another_window);

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
		ofn.lpstrDefExt = TEXT(".txt");
		ofn.lpstrFilter = TEXT("txtファイル(*.txt)\0*.txt\0");
		ofn.lpstrTitle = TEXT("テキストファイルを保存します。");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	if (GetSaveFileName(&ofn)) {
		MessageBox(hWnd, szFile, TEXT("ファイル名を付けて保存"), MB_OK);
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

	switch (uMsg)
	{

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
	return s_pD3DDevice;
}

//---------------------------------------
// 初期化
//---------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE：ウインドウ/FALSE:フルスクリーン
{
	// ローカル変数宣言
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3dオブジェクトの生成
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// ポリゴンの初期化処理
	ZeroMemory(&d3dpp, sizeof(d3dpp));	// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式数
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え（映像信号の同期）
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして１６ｂitを使う
	d3dpp.Windowed = bWindow;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	//Direct3Dデバイスの生成
	if ((FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))) &&
		(FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))) &&
		(FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))))
	{
		return E_FAIL;
	}

	//レジダーステートの設定
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンド設定
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンド設定
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// アルファブレンド設定

	// サンプラーステ―トの設定
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//テクスチャをリニア補完する
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//テクスチャをリニア補完する
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// サンプラーステ―トの設定
	s_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//テクスチャをリニア補完する
	s_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//テクスチャをリニア補完する
	s_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	s_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// 1枚目のテクスチャのテクスチャステージステートの設定
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 二枚目のテクスチャのテクスチャステージ設定
	s_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	s_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	s_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

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
	//画面クリア
	s_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{//成功したとき

		DrawGame();

		DrawImguiProperty();

		//ImGui::Render();
		//ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		s_pD3DDevice->EndScene();	//描画終了
	}
	//バックバッファとフロントバッファの入れ替え
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//ディレクトリ表示用
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


//移動量をゲット
D3DXVECTOR3 GetMove(void)
{
	return D3DXVECTOR3(setmove.x, setmove.y, setmove.z);
}

//向きをゲット
D3DXVECTOR3 GetRot(void)
{
	return D3DXVECTOR3(setrot.x, setrot.y, setrot.z);
}

//色をゲット
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
	return s_nLife;
}

float GetRandMin(void)
{
	return s_fRandMin;
}

float GetRandMax(void)
{
	return s_fRandMax;
}

float GetRadius(void)
{
	return s_fRadius;
}

float GetAngle(void)
{
	return s_fAngle;
}

float GetAlpha(void)
{
	return s_fAlpha;
}

float GetScale(void)
{
	return s_fScale;
}

float GetAttenuation(void)
{
	return s_fAttenuation;
}

char GetFileName(int nNum)
{
	return FileString[nNum];
}

bool bSetEffect(void)
{
	return s_bEffectEnable;
}

bool BackRot(void)
{
	return s_bBackRot;
}

bool TexRot(void)
{
	return s_bTextureRot;
}

bool TexUse(void)
{
	return bTexUse;
}