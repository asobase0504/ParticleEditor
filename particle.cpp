//����������������������������������������
//effect.cpp
//����������������������������������������
#include "main.h"
#include "particle.h"
#include <time.h>

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture[NUM_PARTICLE] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Particle g_aParticle[MAX_PARTICLE];
float g_fAngle = 0.0f;

//����������
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Flare.png",
		&s_pTexture[PARTICLETYPE_NORMAL]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bright.jpg",
		&s_pTexture[PARTICLETYPE_PLAYER]);

	// ������
	memset(g_aParticle, 0, sizeof(g_aParticle));

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
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
void UninitParticle(void)
{
	for (int i = 0; i < NUM_PARTICLE; i++)
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
void UpdateParticle(void)
{
	//(ImGui)
	D3DXVECTOR3 ImPos = GetPos();
	D3DXVECTOR3 ImMove = GetMove();
	D3DXVECTOR3 ImRot = GetRot();
	D3DXCOLOR ImColor = GetColor();
	int ImSelect = GetType();
	float ImRandMin = GetRandMin();
	float ImRandMax = GetRandMax();
	float ImAlpha = GetAlpha();
	float ImAttenuation = GetAttenuation();
	bool bEnable = bSetEffect();
	bool bBackRot = BackRot();
	bool bTexRot = TexRot();
	bool bTex = TexUse();

	float fRad = 0.0f;
	float fGRad = 0.0f;
	g_fAngle = 0;

	if (bEnable)
	{
		SetParticle(ImPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ImColor, 0, 50.0f, 50.0f, PARTICLETYPE_NORMAL);
	}

	if (bTex)
	{
		LoadTex();
	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (!pParticle->bUse)
		{//�G�t�F�N�g���g�p����Ă���Ȃ�
			continue;
		}

		/* ���g�p���Ă���Ȃ火 */

		static float s_fLength = sqrtf((pParticle->fWidth * pParticle->fWidth) + (pParticle->fHeight * pParticle->fHeight));	//�Ίp���̒������Z�o
		static float s_fAngle = atan2f(pParticle->fWidth, pParticle->fHeight);													//�Ίp���̊p�x���Z�o
		float fRandomR = 0.0f;
		float fRandomG = 0.0f;
		float fRandomB = 0.0f;

		//�G�t�F�N�g�̈ړ�
		pParticle->pos += pParticle->move;
		pParticle->pos += ImMove;

		if (bBackRot)
		{
			//float fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (ImRot.z - g_fAngle);
		}
		else
		{
			fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (ImRot.z + g_fAngle);
		}

		//�F�ύX�iImGui�j
		switch (ImSelect)
		{
		case 1:
			pParticle->colTransition = D3DXCOLOR(0.0f, -0.01f, 0.0f, 0.0f);
			pParticle->col.r = 1.0f;
			break;

		case 2:
			pParticle->colTransition = D3DXCOLOR(0.0f, 0.0f, -0.01f, 0.0f);
			pParticle->col.g = 1.0f;
			break;

		case 3:
			pParticle->colTransition = D3DXCOLOR(-0.01f, 0.0f, 0.0f, 0.0f);
			pParticle->col.b = 1.0f;
			break;

		case 4:
			fRandomR = (ImRandMin + ((rand() / (float)RAND_MAX) * (ImRandMax - ImRandMin)));
			pParticle->col.r = fRandomR;

			fRandomG = (ImRandMin + ((rand() / (float)RAND_MAX) * (ImRandMax - ImRandMin)));
			pParticle->col.g = fRandomG;

			fRandomB = (ImRandMin + ((rand() / (float)RAND_MAX) * (ImRandMax - ImRandMin)));
			pParticle->col.b = fRandomB;

			pParticle->col.a = 1.0f;
			break;

		case 0:
			break;

		default:
			break;
		}

		//�A���t�@�l�����p
		g_aParticle[i].col.a -= ImAlpha;

		//
		//�G�t�F�N�g�̎��
		//
		switch (pParticle->type)
		{
		case PARTICLETYPE_NORMAL:
			//*
			/*g_fAngle += 30.0f * i;
			pParticle->move.x = sinf(fGRad) * 1.3f;
			pParticle->move.y = cosf(fGRad) * 1.3f;*/
			break;

		case PARTICLETYPE_PLAYER:
			break;

		default:
			break;
		}

		// �����ȓ���
		{
			//��
			/*g_fAngle += 0.3f;
			pParticle->move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * 3.0f;
			pParticle->move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * 3.0f;*/

			//�ӂɂ�ӂɂ�`�`�`
			/*g_fAngle += 0.0996f;
			pParticle->move.x = 9 * powf(cosf(fGRad), 3.0f);
			pParticle->move.y = 9 * powf(sinf(fGRad), 3.0f);*/

			//������������
			g_fAngle += 20.5f;
			pParticle->move.x = (pParticle->fRadius * sinf(fGRad)) * ImAttenuation;
			pParticle->move.y = (pParticle->fRadius * cosf(fGRad)) * ImAttenuation;
		}

		// ����
		pParticle->nLife--;	// �̗͂̌���
		pParticle->move.y += pParticle->fWeight;			// �d��
		pParticle->col += pParticle->colTransition;			// �F�̐���
		pParticle->move += pParticle->moveTransition;		// �ړ��ʂ̐���
		pParticle->fWeight += pParticle->fWeightTransition;	// �d���̐���

		if (pParticle->nLife <= 0)
		{//�G�t�F�N�g�̎���
			DeleteParticle(i);
		}

		//======================
		//���K��
		//======================
		if (pParticle->fRadius > D3DX_PI)
		{
			pParticle->fRadius -= D3DX_PI * 2;
		}

		else if (pParticle->fRadius < -D3DX_PI)
		{
			pParticle->fRadius += D3DX_PI * 2;
		}

		VERTEX_2D *pVtx = nullptr;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		//���_�f�[�^�̃|�C���^��4���W�߂�

		//���_���W�̐ݒ�
		if (bTexRot)
		{
			pParticle->rot.z -= 0.05f;

			pVtx[0].pos.x = pParticle->pos.x + sinf(pParticle->rot.z + (D3DX_PI + s_fAngle)) * s_fLength;
			pVtx[0].pos.y = pParticle->pos.y + cosf(pParticle->rot.z + (D3DX_PI + s_fAngle)) * s_fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pParticle->pos.x + sinf(pParticle->rot.z + (D3DX_PI - s_fAngle)) * s_fLength;
			pVtx[1].pos.y = pParticle->pos.y + cosf(pParticle->rot.z + (D3DX_PI - s_fAngle)) * s_fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pParticle->pos.x + sinf(pParticle->rot.z - (0 + s_fAngle)) * s_fLength;
			pVtx[2].pos.y = pParticle->pos.y + cosf(pParticle->rot.z - (0 + s_fAngle)) * s_fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pParticle->pos.x + sinf(pParticle->rot.z - (0 - s_fAngle)) * s_fLength;
			pVtx[3].pos.y = pParticle->pos.y + cosf(pParticle->rot.z - (0 - s_fAngle)) * s_fLength;
			pVtx[3].pos.z = 0.0f;
		}

		else
		{
			pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
			pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
			pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
			pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;
		
		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}

	if (ImSelect == 4)
	{
		srand(time(nullptr));
	}
}

//�`�揈��
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (!g_aParticle[nCnt].bUse)
		{
			continue;
		}

		/* ���g�p���Ă���Ȃ火 */

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[g_aParticle[nCnt].type]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���������p���Ȃ�
	pDevice->SetTexture(0, NULL);
}

//�ݒ菈��
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type)
{
	int ImLife = GetLife();
	float ImRadius = GetRadius();

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (pParticle->bUse)
		{
			continue;
		}

		/* ���g�p����ĂȂ��Ȃ火 */

		// �f�[�^�̃��Z�b�g
		DeleteParticle(i);

		pParticle->pos = pos;
		pParticle->move = move;
		pParticle->col = col;
		pParticle->type = type;
		pParticle->fWidth = fWidth;
		pParticle->fHeight = fHeight;

		//(ImGui)
		pParticle->nLife = ImLife;
		pParticle->fRadius = ImRadius;
		//pParticle->nLife = nLife;

		pParticle->bUse = true;

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		//���_�f�[�^�̃|�C���^��4���W�߂�

		//���_���W�̐ݒ�
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

// �e�N�X�`���̓Ǎ���
void LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	for (int i = 0; i < 512; i++)
	{
		ImFile[i] = GetFileName(i);
	}

	if (ImTex)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			ImFile,
			&s_pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

//�폜����
void DeleteParticle(int nDelete)
{
	// �f�[�^�̃��Z�b�g
	memset(&g_aParticle[nDelete], 0, sizeof(g_aParticle[nDelete]));
}

void RemoveAngle(void)
{
	g_fAngle = 0;
}

Particle *GetParticle(void)
{
	return &g_aParticle[0];
}