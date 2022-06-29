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
// ��`
//--------------------------------------------------
const std::string CTexture::REL_PATH = "data/TEXTURE/";
const std::string CTexture::ABS_PATH = "data\\TEXTURE\\";

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
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

	std::string str;

	if (m_fileSave)
	{// �t�@�C���̃Z�[�u������
		size_t pos = -1;
		pos = m_fileName[index].find(REL_PATH.c_str());

		if (pos == -1)
		{// �z��̏ꏊ�ɉ摜���Ȃ�
			/*pos = m_fileName[index].rfind("\\", m_fileName[index].length());
			pos += 1;

			for (int i = (int)pos; i < m_fileName[index].length(); i++)
			{
				str += m_fileName[index][i];
			}*/

			assert(false);

			return;
		}

		pos += REL_PATH.length();

		for (int i = (int)pos; i < m_fileName[index].length(); i++)
		{
			str += m_fileName[index][i];
		}
	}
	else
	{// �t�@�C���̃Z�[�u�����ĂȂ�
		str = m_fileName[index];
	}

	//�N�G���[�̕ύX
	CreateDirectory(m_defaulttCurrent, NULL);
	SetCurrentDirectory(m_defaulttCurrent);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		str.c_str(),
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
	size_t pos = -1;

	pos = str.find(REL_PATH.c_str());

	if (pos == -1)
	{// ���΃p�X�p�̕����񂪂Ȃ�
		pos = str.find(ABS_PATH.c_str());

		if (pos == -1)
		{// �z��̏ꏊ�ɉ摜���Ȃ�
			m_fileName[m_numAll] = str;
			m_numAll++;
			m_fileSave = true;
			//assert(false);
			return;
		}

		pos += ABS_PATH.length();

		m_fileName[m_numAll] = REL_PATH;

		for (int i = (int)pos; i < str.length(); i++)
		{
			m_fileName[m_numAll] += str[i];
		}

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
// �ۑ�
//--------------------------------------------------
void CTexture::SavePath()
{
	/*for (int i = 0; i < m_numAll; i++)
	{
		LoadJsonTex(m_fileName[i].c_str());
	}

	OutputStatusTex();*/
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
