//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "texture.h"
#include "application.h"
#include "renderer.h"
#include "file.h"

#include <assert.h>

//--------------------------------------------------
// 定義
//--------------------------------------------------
const std::string CTexture::REL_PATH = "data/TEXTURE/";
const std::string CTexture::ABS_PATH = "data\\TEXTURE\\";

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() :
	m_numAll(0),
	m_fileSave(false)
{
	memset(m_fileName, 0, sizeof(m_fileName));
	memset(m_pTexture, 0, sizeof(m_pTexture));
	
	GetCurrentDirectory(sizeof(m_defaulttCurrent),m_defaulttCurrent);
	//MessageBox(NULL, m_defaulttCurrent, NULL, MB_OK);
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		// 読み込み
		Load(i);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(int index)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	if (m_pTexture[index] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	//クエリーの変更
	CreateDirectory(m_defaulttCurrent, NULL);
	SetCurrentDirectory(m_defaulttCurrent);

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		m_fileName[index].c_str(),
		&m_pTexture[index]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::UnloadAll()
{
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		// 解放
		Unload(i);
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Unload(int index)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	if (m_pTexture[index] != nullptr)
	{// テクスチャの解放
		m_pTexture[index]->Release();
		m_pTexture[index] = nullptr;
	}
}

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
void CTexture::SetPath(std::string str)
{
	size_t pos = -1;

	for (int i = 0; i < m_numAll; i++)
	{
		if (m_fileName[i] == str)
		{// 同じパス
			return;
		}
	}

	pos = str.find(REL_PATH.c_str());

	if (pos == -1)
	{// 相対パス用の文字列がない
		pos = str.find(ABS_PATH.c_str());

		if (pos == -1)
		{// 想定の場所にファイルがない
			assert(false);
			return;
		}

		pos += ABS_PATH.length();

		std::string copy;
		copy = REL_PATH;

		for (int i = (int)pos; i < str.length(); i++)
		{
			copy += str[i];
		}

		for (int i = 0; i < m_numAll; i++)
		{
			if (m_fileName[i] == copy)
			{// 同じパス
				return;
			}
		}

		m_fileName[m_numAll] = copy;

		CParticle::SetIdxTex(m_numAll);

		m_numAll++;
		m_fileSave = true;

		return;
	}
	
	for (int i = (int)pos; i < str.length(); i++)
	{
		m_fileName[m_numAll] += str[i];
	}

	m_numAll++;
}

//--------------------------------------------------
// 保存
//--------------------------------------------------
void CTexture::SavePath()
{
	for (int i = 0; i < m_numAll; i++)
	{
		LoadJsonTex(m_fileName[i].c_str());
	}

	OutputStatusTex();
}


//--------------------------------------------------
// パスの取得
//--------------------------------------------------
std::string CTexture::GetPath(int index, bool path)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	if (path)
	{// パスあり
		return m_fileName[index];
	}

	std::string str;
	size_t pos = -1;
	pos = m_fileName[index].find(REL_PATH.c_str());

	if (pos == -1)
	{// 想定の場所にファイルがない
		assert(false);
		return m_fileName[index];
	}

	pos += REL_PATH.length();

	for (int i = (int)pos; i < m_fileName[index].length(); i++)
	{
		str += m_fileName[index][i];
	}

	return str;
}

//--------------------------------------------------
// 総数の取得
//--------------------------------------------------
int CTexture::GetNumAll()
{
	return m_numAll;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int index)
{
	if (index == NONE_TEXTURE)
	{// テクスチャを使用しない
		return nullptr;
	}

	// 読み込み
	Load(index);

	return m_pTexture[index];
}
