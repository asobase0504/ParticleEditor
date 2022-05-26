//����������������������������������������
//Main.h
//����������������������������������������

#ifndef _MAIN_H_		//��d�K�[�h�C���N���[�h
#define _MAIN_H_

//�C���N���[�h�t�@�C��
#include <windows.h>
#include "d3dx9.h"		//�`�揈��
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>
#include <d3d9.h>
#include <tchar.h>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")

//�}�N����`
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_2D_TEX2		(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_TEX2)					// �}���`�e�N�X�`���p�̒��_�t�H�[�}�b�g[2D]

//���_���[2D]�̍\���̂��`
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
	D3DXVECTOR2 tex2;			// �e�N�X�`�����W(���x��2)
};

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
D3DXVECTOR3 GetPos(void);
D3DXVECTOR3 GetMove(void);
D3DXVECTOR3 GetRot(void);
D3DXCOLOR GetColor(void);
int GetType(void);
int GetLife(void);
float GetRandMin(void);
float GetRandMax(void);
float GetRadius(void);
float GetAlpha(void);
float GetAttenuation(void);
float GetAngle(void);
char GetFileName(int nNum);
bool bSetEffect(void);
bool BackRot(void);
bool TexRot(void);
bool BackTexRot(void);
bool TexUse(void);

#endif
