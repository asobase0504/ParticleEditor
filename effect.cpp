//����������������������������������������
//effect.cpp
//����������������������������������������
#include "main.h"
#include "effect.h"

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture[NUM_EFFECT] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];

//����������
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&s_pTexture[EFFECTTYPE_NORMAL]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bright.jpg",
		&s_pTexture[EFFECTTYPE_PLAYER]);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		g_aEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[i].fWidth = 0.0f;
		g_aEffect[i].fHeight = 0.0f;
		g_aEffect[i].fRadius = 0.0f;
		g_aEffect[i].nLife = 0;
		g_aEffect[i].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
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

		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//�I������
void UninitEffect(void)
{
	for (int i = 0; i < NUM_EFFECT; i++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//�X�V����
void UpdateEffect(void)
{
	VERTEX_2D *pVtx = nullptr;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse)
		{//�G�t�F�N�g���g�p����Ă���Ȃ�
		 //�G�t�F�N�g�̈ړ�
			g_aEffect[i].pos += g_aEffect[i].move;

			//�F�ύX
			g_aEffect[i].col.r -= 0.01f;
			g_aEffect[i].col.b++;

			g_aEffect[i].nLife--;

			if (g_aEffect[i].nLife == 0)
			{//�G�t�F�N�g�̎���
				DeleteEffect(i);
			}

			//======================
			//���K��
			//======================
			if (g_aEffect[i].fRadius > D3DX_PI)
			{
				g_aEffect[i].fRadius -= D3DX_PI * 2;
			}

			else if (g_aEffect[i].fRadius < -D3DX_PI)
			{
				g_aEffect[i].fRadius += D3DX_PI * 2;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[1].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[2].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);
			pVtx[3].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[i].col;
			pVtx[1].col = g_aEffect[i].col;
			pVtx[2].col = g_aEffect[i].col;
			pVtx[3].col = g_aEffect[i].col;
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//�`�揈��
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse)
		{//�G�t�F�N�g���g�p����Ă���
		 //���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture[g_aEffect[nCnt].type]);

		 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);						//�`�悷��v���~�e�B�u��
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���������p���Ȃ�
	pDevice->SetTexture(0, NULL);
}

//�ݒ菈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, EFFECTTYPE type)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (!g_aEffect[i].bUse)
		{//�G�t�F�N�g���g�p����Ă���
			g_aEffect[i].pos = pos;
			g_aEffect[i].move = move;
			g_aEffect[i].col = col;
			g_aEffect[i].type = type;
			g_aEffect[i].fWidth = fWidth;
			g_aEffect[i].fHeight = fHeight;
			g_aEffect[i].nLife = nLife;

			//���_���W�̐ݒ�
			pVtx[0].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[1].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[2].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);
			pVtx[3].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aEffect[i].bUse = true;

			break;
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���W�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//�폜����
void DeleteEffect(int nDelete)
{
	g_aEffect[nDelete].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aEffect[nDelete].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aEffect[nDelete].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aEffect[nDelete].type = EFFECTTYPE_NONE;
	g_aEffect[nDelete].fWidth = 0.0f;
	g_aEffect[nDelete].fHeight = 0.0f;
	g_aEffect[nDelete].fRadius = 0.0f;
	g_aEffect[nDelete].nLife = 0;
	g_aEffect[nDelete].bUse = false;
}