//��������������������������
//time.cpp
//��������������������������
#include "main.h"
#include"time.h"

static LPDIRECT3DTEXTURE9 s_pTextureTime = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffTime = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3 s_posTime; //�X�R�A�̈ʒu
static int s_nTime;

//===================
//����������
//===================
void InitTime(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	int nCntTime;
	s_nTime = 0;

	s_posTime = D3DXVECTOR3(550.0f, 50.0f, 0.0f);		//�ʒu�ݒ�

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&s_pTextureTime);

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTime,
		NULL);

	VERTEX_2D*pVtx;		//���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(s_posTime.x - 30.0f, s_posTime.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_posTime.x + 30.0f, s_posTime.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_posTime.x - 30.0f, s_posTime.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_posTime.x + 30.0f, s_posTime.y + 30.0f, 0.0f);

		s_posTime += D3DXVECTOR3(50.0f, 0.0f, 0.0f);

		if (nCntTime == 2)
		{
			s_posTime += D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		}

		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffTime->Unlock();

	SetTime(6000);
}

//===================
//�j������
//===================
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (s_pTextureTime != NULL)
	{
		s_pTextureTime->Release();
		s_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffTime != NULL)
	{
		s_pVtxBuffTime->Release();
		s_pVtxBuffTime = NULL;
	}

}

//===================
//�X�V����
//===================
void UpdateTime(void)
{
	if (s_nTime != 0)
	{
		AddTime(-1);
	}
}

//===================
//�`�揈��
//===================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureTime);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCntTime * 4,
			2);
	}
}

//===================
//�Z�b�g
//===================
void SetTime(int nTime)
{
	int nCntTime;
	int aPosTexU[MAX_TIME]; //�e���̐������i�[

	s_nTime = nTime;

	aPosTexU[0] = (s_nTime % 100000) / 10000;
	aPosTexU[1] = (s_nTime % 10000) / 1000;
	aPosTexU[2] = (s_nTime % 1000) / 100;

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffTime->Unlock();
}

//===================
//���Z
//===================
void AddTime(int nValue)
{
	int nCntTime;
	int aPosTexU[MAX_TIME]; //�e���̐������i�[

	s_nTime += nValue;

	aPosTexU[0] = (s_nTime % 100000) / 10000;
	aPosTexU[1] = (s_nTime % 10000) / 1000;
	aPosTexU[2] = (s_nTime % 1000) / 100;

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffTime->Unlock();
}

int GetTime(void)
{
	return s_nTime;
}