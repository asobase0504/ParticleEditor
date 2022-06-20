// ����������������������������������������
// effect.cpp
// ����������������������������������������
#include "main.h"
#include "particle.h"
#include <time.h>
#include "imgui_property.h"
#include "utility.h"

// ==================================================
// �ÓI�����o�[�ϐ�
// ==================================================
CParticle::Particle CParticle::g_aParticle[] = {};

// --------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------
CParticle::CParticle()
{
}

// --------------------------------------------------
// �f�X�g���N�^
// --------------------------------------------------
CParticle::~CParticle()
{
}

// --------------------------------------------------
// ������
// --------------------------------------------------
HRESULT CParticle::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&pTexture[PARTICLETYPE_NORMAL]);

	// �e�N�X�`���̓ǂݍ���
	memset(g_aParticle, 0, sizeof(g_aParticle));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * maxNumber,	// �m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < maxNumber; nCnt++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		// ���_�f�[�^�̃|�C���^��4���W�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	return S_OK;
}

// --------------------------------------------------
// �I��
// --------------------------------------------------
void CParticle::Uninit()
{
	for (int i = 0; i < numType; i++)
	{
		// �e�N�X�`���̔j��
		if (pTexture[i] != NULL)
		{
			pTexture[i]->Release();
			pTexture[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (pVtxBuff != NULL)
	{
		pVtxBuff->Release();
		pVtxBuff = NULL;
	}
}

// --------------------------------------------------
// �X�V
// --------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	for (int i = 0; i < maxNumber; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (!pParticle->bUse)
		{// �G�t�F�N�g���g�p����Ă���Ȃ�
			continue;
		}

		/* ���g�p���Ă���Ȃ火 */

		// �G�t�F�N�g�̈ړ�
		pParticle->pos += pParticle->move;

		// ����
		pParticle->nLife--;									// �̗͂̌���
		pParticle->move.y += pParticle->fWeight;			// �d��
		pParticle->move *= pParticle->fAttenuation;			// �ړ��ʂ̐���
		pParticle->fWeight += pParticle->fWeightTransition;	// �d���̐���

		if (pParticle->color.bColTransition)
		{// �F�̐���
			if (pParticle->color.nEndTime >= pParticle->color.nCntTransitionTime)
			{
				pParticle->color.nCntTransitionTime++;
				pParticle->color.col += pParticle->color.colTransition;
			}
		}

		if (pParticle->nLife <= 0)
		{// �G�t�F�N�g�̎���
			Delete(i);
		}

		VERTEX_2D *pVtx = nullptr;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// ���_�f�[�^�̃|�C���^��4���W�߂�

		// ���_���W�̐ݒ�
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pParticle->color.col;
		pVtx[1].col = pParticle->color.col;
		pVtx[2].col = pParticle->color.col;
		pVtx[3].col = pParticle->color.col;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

// --------------------------------------------------
// �`��
// --------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �_�ɓ\��(true)�A�|���S���ɓ\��(false)
	// pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,true);

	// �J�����̈ʒu(true)�A�X�N���[���̈ʒu(false)
	// pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,false);

	// �T�C�Y��ݒ�
	// pDevice->SetRenderState(D3DRS_POINTSIZE,FloattoDword(1.0f));
	// pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloattoDword(0.0f));

	// �|�C���g�T�C�Y�̌v�Z
	// pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloattoDword(0.0f));
	// pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloattoDword(0.0f));
	// pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloattoDword(1.0f));

	for (int nCnt = 0; nCnt < maxNumber; nCnt++)
	{
		if (!g_aParticle[nCnt].bUse)
		{
			continue;
		}

		/* ���g�p���Ă���Ȃ火 */

		switch (g_aParticle[nCnt].alphaBlend)
		{
		case TYPE_NONE:		// ��Z
			break;

		case TYPE_ADD:		// ���Z
			// �A���t�@�u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case TYPE_SUB:			// ���Z
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		default:
			assert(false);
			break;
		}

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture[g_aParticle[nCnt].type]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		// pDevice->DrawPrimitive(D3DPT_POINTLIST,0, nCnt);

		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// �|�C���g�X�v���C�g����������
	// pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);

	// �e�N�X�`���������p���Ȃ�
	pDevice->SetTexture(0, NULL);
}

// --------------------------------------------------
// ����
// --------------------------------------------------
void CParticle::Create(Particle& inParticle)
{
	for (int i = 0; i < maxNumber; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (pParticle->bUse)
		{
			continue;
		}

		/* ���g�p����ĂȂ��Ȃ火 */

		// �f�[�^�̃��Z�b�g
		Delete(i);

		*pParticle = inParticle;
		pParticle->type = PARTICLETYPE_NORMAL;

		pParticle->fWidth = g_aParticle->fScale;
		pParticle->fHeight = g_aParticle->fScale;
		pParticle->color.nCntTransitionTime = 0;
		pParticle->bUse = true;

		// �����ʒu�̎Z�o
		pParticle->pos.x += FloatRandam(pParticle->maxPopPos.x, -pParticle->minPopPos.x);
		pParticle->pos.y += FloatRandam(pParticle->maxPopPos.y, -pParticle->minPopPos.y);
		pParticle->pos.z += FloatRandam(pParticle->maxPopPos.z, -pParticle->minPopPos.z);

		// �F�̎Z�o
		if (pParticle->color.bColRandom)
		{// �����_���J���[���g�p
			pParticle->color.col.r = FloatRandam(pParticle->color.colRandamMax.r, pParticle->color.colRandamMin.r);
			pParticle->color.col.g = FloatRandam(pParticle->color.colRandamMax.g, pParticle->color.colRandamMin.g);
			pParticle->color.col.b = FloatRandam(pParticle->color.colRandamMax.b, pParticle->color.colRandamMin.b);

			if (pParticle->color.bColTransition)
			{// �ړI�̐F�̐ݒ�
				if (pParticle->color.bRandomTransitionTime)
				{
					pParticle->color.nEndTime = rand() % pParticle->nLife + 1;
				}

				pParticle->color.destCol.r = FloatRandam(pParticle->color.colRandamMax.r, pParticle->color.colRandamMin.r);
				pParticle->color.destCol.g = FloatRandam(pParticle->color.colRandamMax.g, pParticle->color.colRandamMin.g);
				pParticle->color.destCol.b = FloatRandam(pParticle->color.colRandamMax.b, pParticle->color.colRandamMin.b);
			}
		}

		if (pParticle->color.bColTransition)
		{// �g���f�B�V�I���J���[���g�p
			if (pParticle->color.bRandomTransitionTime)
			{
				pParticle->color.nEndTime = rand() % pParticle->nLife + 1;
			}

			pParticle->color.colTransition.r = (pParticle->color.destCol.r - pParticle->color.col.r) / pParticle->color.nEndTime;
			pParticle->color.colTransition.g = (pParticle->color.destCol.g - pParticle->color.col.g) / pParticle->color.nEndTime;
			pParticle->color.colTransition.b = (pParticle->color.destCol.b - pParticle->color.col.b) / pParticle->color.nEndTime;
		}

		VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;	// ���_�f�[�^�̃|�C���^��4���W�߂�

						// ���_���W�̐ݒ�
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pParticle->color.col;
		pVtx[1].col = pParticle->color.col;
		pVtx[2].col = pParticle->color.col;
		pVtx[3].col = pParticle->color.col;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();

		float ImAngle = GetAngle();
		float fRad = 0.0f;
		float fGRad = 0.0f;

		if (pParticle->bBackrot)
		{
			// float fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (pParticle->rot.z - g_fAngle);
		}
		else
		{
			fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (pParticle->rot.z + g_fAngle);
		}

		// ����
		{
			/*
			g_fAngle += 30.0f * i;
			pParticle->move.x = sinf(fGRad) * 1.3f;
			pParticle->move.y = cosf(fGRad) * 1.3f;

			// ��
			g_fAngle += 0.7f;
			pParticle->move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * pParticle->fAttenuation;
			pParticle->move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * pParticle->fAttenuation;
			*/

			// ������������
			g_fAngle += ImAngle;
			pParticle->move.x += (pParticle->fRadius * sinf(fGRad)) * pParticle->fAttenuation;
			pParticle->move.y += (pParticle->fRadius * cosf(fGRad)) * pParticle->fAttenuation;
		}

		// ======================
		// ���K��
		// ======================
		if (pParticle->fRadius > D3DX_PI)
		{
			pParticle->fRadius -= D3DX_PI * 2;
		}
		else if (pParticle->fRadius < -D3DX_PI)
		{
			pParticle->fRadius += D3DX_PI * 2;
		}

		if (g_fAngle > D3DX_PI)
		{
			g_fAngle -= D3DX_PI * 2;
		}
		else if (g_fAngle < -D3DX_PI)
		{
			g_fAngle += D3DX_PI * 2;
		}

		break;
	}
}

// --------------------------------------------------
// �e�N�X�`���̓Ǎ���
// --------------------------------------------------
void CParticle::LoadTex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			GetFileName(),
			&pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

// --------------------------------------------------
// ���̍폜
// --------------------------------------------------
void CParticle::Delete(const int data)
{
	// �f�[�^�̃��Z�b�g
	memset(&g_aParticle[data], 0, sizeof(g_aParticle[data]));
}

// --------------------------------------------------
// ����S�č폜
// --------------------------------------------------
void CParticle::DeleteAll()
{
	for (int i = 0; i < maxNumber; i++)
	{
		Delete(i);
	}
}

// --------------------------------------------------
// �p�x�̏�����
// --------------------------------------------------
void CParticle::RemoveAngle(void)
{
	g_fAngle = 0;
}

// --------------------------------------------------
// float �� DWORD �ɕϊ�
// --------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}
