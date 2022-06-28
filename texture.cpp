//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include "file.h"

#include <assert.h>

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CTexture::CTexture() :
	m_numAll(0)
{
	memset(m_fileName, 0, sizeof(m_fileName));
	memset(m_pTexture, 0, sizeof(m_pTexture));
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// �S�Ă̓ǂݍ���
//--------------------------------------------------
void CTexture::LoadAll()
{
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		// �ǂݍ���
		Load(i);
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CTexture::Load(int index)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	if (m_pTexture[index] != nullptr)
	{// �e�N�X�`���̓ǂݍ��݂�����Ă���
		return;
	}

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		m_fileName[index].c_str(),
		&m_pTexture[index]);
}

//--------------------------------------------------
// �S�Ẳ��
//--------------------------------------------------
void CTexture::UnloadAll()
{
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		// ���
		Unload(i);
	}
}

//--------------------------------------------------
// ���
//--------------------------------------------------
void CTexture::Unload(int index)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	if (m_pTexture[index] != nullptr)
	{// �e�N�X�`���̉��
		m_pTexture[index]->Release();
		m_pTexture[index] = nullptr;
	}
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ�
//--------------------------------------------------
void CTexture::SetPath(std::string str)
{
	m_fileName[m_numAll] = str;
	m_numAll++;
}

//--------------------------------------------------
// �ۑ�
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
// �p�X�̎擾
//--------------------------------------------------
std::string CTexture::GetPath(int index)
{
	assert(index > NONE_TEXTURE && index < MAX_TEXTURE);

	return m_fileName[index];
}

//--------------------------------------------------
// �����̎擾
//--------------------------------------------------
int CTexture::GetNumAll()
{
	return m_numAll;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int index)
{
	if (index == NONE_TEXTURE)
	{// �e�N�X�`�����g�p���Ȃ�
		return nullptr;
	}

	// �ǂݍ���
	Load(index);

	return m_pTexture[index];
}
