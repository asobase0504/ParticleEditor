#include "bullet.h"
#include "effect.h"
#include "enemy.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�̃|�C���^
BULLET g_aBullet[MAX_BULLET];						//�e�̏��
float g_fBulletAngle;

//
//�e�̏���������
//
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		L"data\\TEXTURE\\bullet.png",
		&g_pTextureBullet);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBullet[nCntBullet].fBulletSize = 0.0f;
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,    //�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,            //���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D*pVtx;        //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
		pVtx[0].col = g_aBullet[nCntBullet].col;
		pVtx[1].col = g_aBullet[nCntBullet].col;
		pVtx[2].col = g_aBullet[nCntBullet].col;
		pVtx[3].col = g_aBullet[nCntBullet].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//
//�e�̏I������
//
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//
//�e�̍X�V����
//
void UpdateBullet(void)
{
	int nCntBullet = 0;
	g_fBulletAngle = 0;
	VERTEX_2D*pVtx;        //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
		 //�ʒu���X�V
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife == 0)
			{
				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ����
			}

			if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH)
			{
				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ����
			}

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBullet[nCntBullet].col;
			pVtx[1].col = g_aBullet[nCntBullet].col;
			pVtx[2].col = g_aBullet[nCntBullet].col;
			pVtx[3].col = g_aBullet[nCntBullet].col;
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//
//�e�̕`�揈��
//
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
		 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,
				2);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//
//�e�̐ݒ菈��
//
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	int nCntBullet;
	VERTEX_2D*pVtx;        //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].col = D3DXCOLOR(0.5f,1.0f, 0.4f,0.8f);
			g_aBullet[nCntBullet].fBulletSize = 20.0f;

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);

			pVtx[0].col = g_aBullet[nCntBullet].col;
			pVtx[1].col = g_aBullet[nCntBullet].col;
			pVtx[2].col = g_aBullet[nCntBullet].col;
			pVtx[3].col = g_aBullet[nCntBullet].col;

			g_aBullet[nCntBullet].nLife = 120;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//����������������������������������������
//�e�̔���
//����������������������������������������
bool CollisionBullet(D3DXVECTOR3 *ppos, float fRadius)
{
	bool bEnemy = false;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			float fDistance = ((ppos->x - g_aBullet[nCntBullet].pos.x) * (ppos->x - g_aBullet[nCntBullet].pos.x)) +
				((ppos->y - g_aBullet[nCntBullet].pos.y) * (ppos->y - g_aBullet[nCntBullet].pos.y));

			float fRad = (2.5f + fRadius) * (2.5f + fRadius);

			if (fDistance <= fRad)
			{
				g_aBullet[nCntBullet].bUse = false;
				bEnemy = true;
			}
		}
	}

	return bEnemy;
}

//����������������������������������������
//�e�̎擾
//����������������������������������������
BULLET *GetBullet(void)
{
	return &g_aBullet[0];
}