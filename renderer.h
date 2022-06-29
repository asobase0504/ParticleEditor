//=========================================
// 
// �����_�[�X�e�[�g�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3dx9.h>

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 GetDevice() { return pD3DDevice; }
private:
	LPDIRECT3D9 pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 pD3DDevice;	// Device�I�u�W�F�N�g

#ifdef _DEBUG
	// �t�H���g
	LPD3DXFONT pFont;
#endif // _DEBUG
};

#endif // !_RENDERER_H_
