#include "main.h"
#include "input.h"
#include "BG.h"

//�}�N����`
#define NUM_BG (2)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9	g_pTextureBg = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBg = NULL;

//����������������������������������������
//BG�̏���������
//����������������������������������������
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	//���n
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aquaBG.jpg",
		&g_pTextureBg);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
	//	g_aBg.nCounterAnim = 0;		//�J�E���^�[��������������
	//	g_aBg.nPatternAnim = 0;		//�A�j���[�V�����p�^�[��No.
	//}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,    //�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	VERTEX_2D*pVtx;				 //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	pVtx += 4;

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBg->Unlock();
}

//����������������������������������������
//BG�̏I������
//����������������������������������������
void UninitBg(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//����������������������������������������
//BG�̍X�V����
//����������������������������������������
void UpdateBg(void)
{
	//�e�N�X�`�����W�̍X�V
	VERTEX_2D*pVtx;

	//g_aBg.nCounterAnim++;		//�J�E���^�[�����Z

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
	//	if ((g_aBg.nCounterAnim % 60) == 0)
	//	{//��莞�Ԍo��
	//	 //�p�^�[��No.���X�V
	//		g_aBg.nPatternAnim = (g_aBg.nPatternAnim + 1) % 2;
	//		g_aBg.nPatternAnim++;

	//		if (g_aBg.nPatternAnim == 8)
	//		{
	//			g_aBg.nPatternAnim = 0;
	//		}
	//	}
	//	pVtx += 4;
	//}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBg->Unlock();
}

//����������������������������������������
//BG�̕`�揈��
//����������������������������������������
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;        //�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg);

	//�|���S���̕`��
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntBG * 4,
			2);						//�v���~�e�B�u(�|���S��)��
	}
}
