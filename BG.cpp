#include "input.h"
#include "BG.h"
#include "application.h"

//�}�N����`
#define NUM_BG (2)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9	g_pTextureBg = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBg = NULL;

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CBg::CBg()
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CBg::~CBg()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CBg::Init()
{
	CObject2D::Init();
	SetPos(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f));
	SetSize(D3DXVECTOR2((float)CApplication::SCREEN_WIDTH, (float)CApplication::SCREEN_HEIGHT));
	SetColor(D3DXCOLOR(0.2f, 0.2f, 0.22f, 1.0f));
	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CBg::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CBg::Update()
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CBg::Draw()
{
	CObject2D::Draw();
}
