//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>
#include <string>

//==================================================
// ��`
//==================================================
class CTexture
{
	/* ��` */
public:
	static const int NONE_TEXTURE = -1;	// �e�N�X�`�����g�p���Ȃ�

private:
	static const int MAX_TEXTURE = 256;	// �e�N�X�`���̍ő吔
	static const std::string REL_PATH;	// ���΃p�X
	static const std::string ABS_PATH;	// ��΃p�X

	/* �����o�֐� */
public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public:
	void LoadAll();			// �S�Ă̓ǂݍ���
	void UnloadAll();		// �S�Ă̔j��
	void Load(int index);	// �w��̓ǂݍ���
	void Unload(int index);	// �w��̔j��
	void SetPath(std::string str);	// �p�X�̐ݒ�
	void SavePath();				// �p�X�̕ۑ�
	std::string GetPath(int index);	// �p�X�̎擾
	int GetNumAll();				// �����̎擾
	LPDIRECT3DTEXTURE9 GetTexture(int index);	// ���̎擾
	std::string GetPath();

	/* �����o�ϐ� */
private:
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	// �e�N�X�`���̏��
	std::string m_fileName[MAX_TEXTURE];		// �t�@�C���p�X
	char m_defaulttCurrent[256];
	int m_numAll;								// ����
	bool m_fileSave;							// �t�@�C���̃Z�[�u���������ǂ���
};

#endif // !_TEXTURE_H_
