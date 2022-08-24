//=========================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <d3dx9.h>

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CRenderer;
class CObject;
class CPlayer;
class CTexture;
class CParticleManager;
class CBg;
class CImguiProperty;
class CParticleEdit;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CApplication
{
public: // �萔
	static const int SCREEN_WIDTH = 1280;	// �X�N���[���̕�
	static const int SCREEN_HEIGHT = 720;	// �X�N���[���̍���

public: // �V���O���g���̃C���X�^���X�擾
	static CApplication* GetInstance();

private:
	CApplication();
public:
	~CApplication();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CTexture* GetTextureClass() { return texture; }
	CParticleManager* GetParticleManager() { return paticleManager; }
	CParticleEdit* GetParticleEdit() { return particleEdit; }
	CImguiProperty* GetImguiProperty();

private:
	static CApplication* application;	// �A�v���P�[�V����
	CObject* object;					// �I�u�W�F�N�g
	CRenderer* renderer;				// �����_���\
	CTexture* texture;					// �e�N�X�`��
	CParticleManager* paticleManager;	// �p�[�e�B�N���}�l�W���[
	CParticleEdit* particleEdit;
	CBg* bg;							// �w�i
};
#endif // !_APPLICATION_H_
