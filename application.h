//=========================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "main.h"

//-----------------------------------------
// �O���錾
//-----------------------------------------
class CRenderer;
class CObject;
class CPlayer;
class CTexture;
class CParticle;
class CBg;

//-----------------------------------------
// �A�v���P�[�V�����N���X
//-----------------------------------------
class CApplication
{
public:
	static CApplication* GetInstance();
private:
	CApplication();
public:
	~CApplication();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	CRenderer* GetRenderer();
	CObject** GetMyObject();
	CTexture* GetTextureClass();
	CParticle* GetParticle();
private:
	static CApplication* application;
	CObject* object;
	CRenderer* renderer;
	CTexture* texture;
	CParticle* particle;
	CBg* bg;
};

#endif // !_APPLICATION_H_
