//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include <string>

//==================================================
// 定義
//==================================================
class CTexture
{
	/* 定義 */
public:
	static const int NONE_TEXTURE = -1;	// テクスチャを使用しない

private:
	static const int MAX_TEXTURE = 256;	// テクスチャの最大数
	static const std::string REL_PATH;	// 相対パス
	static const std::string ABS_PATH;	// 絶対パス

	/* メンバ関数 */
public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public:
	void LoadAll();			// 全ての読み込み
	void UnloadAll();		// 全ての破棄
	void Load(int index);	// 指定の読み込み
	void Unload(int index);	// 指定の破棄
	void SetPath(std::string str);	// パスの設定
	void SavePath();				// パスの保存
	std::string GetPath(int index);	// パスの取得
	int GetNumAll();				// 総数の取得
	LPDIRECT3DTEXTURE9 GetTexture(int index);	// 情報の取得
	std::string GetPath();

	/* メンバ変数 */
private:
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	// テクスチャの情報
	std::string m_fileName[MAX_TEXTURE];		// ファイルパス
	char m_defaulttCurrent[256];
	int m_numAll;								// 総数
	bool m_fileSave;							// ファイルのセーブをしたかどうか
};

#endif // !_TEXTURE_H_
