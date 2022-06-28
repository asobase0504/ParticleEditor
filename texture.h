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
private:
	static const int MAX_TEXTURE = 256;	// テクスチャの最大数

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
	LPDIRECT3DTEXTURE9 GetTexture(int index);	// 情報の取得

	/* メンバ変数 */
private:
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	// テクスチャの情報
	std::string m_fileName[MAX_TEXTURE];		// ファイルパス
	int m_numAll;								// 総数
};

#endif // !_TEXTURE_H_
