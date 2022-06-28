//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include "file.h"

#include <assert.h>

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() :
	m_numAll(0)
{
	memset(m_fileName, 0, sizeof(m_fileName));
	memset(m_pTexture, 0, sizeof(m_pTexture));
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
	m_fileName[m_numAll] = str;
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
std::string CTexture::GetPath(int index)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	return m_fileName[index];
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
