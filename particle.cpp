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
CParticle* CParticle::g_aParticle[] = {};
float CParticle::g_fAngle = 0.0f;

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CParticle::CParticle()
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CParticle::~CParticle()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CParticle::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

												// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&pTexture[PARTICLETYPE_NORMAL]);

	// �f�[�^�̏�����
	memset(&data, 0, sizeof(data));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

								// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

					// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();



	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
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

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool *bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	/* ���g�p���Ă���Ȃ火 */

	// �G�t�F�N�g�̈ړ�
	data.pos += data.move;

	// ����
	data.nLife--;							// �̗͂̌���
	data.move.y += data.fWeight;			// �d��
	data.move *= data.fAttenuation;			// �ړ��ʂ̐���
	data.fWeight += data.fWeightTransition;	// �d���̐���

	if (data.color.bColTransition)
	{// �F�̐���
		if (data.color.nEndTime >= data.color.nCntTransitionTime)
		{
			data.color.nCntTransitionTime++;
			data.color.col += data.color.colTransition;
		}
	}
	//pParticle->color.col.a -= 1.0f / pParticle->nMaxLife;

	VERTEX_2D *pVtx = nullptr;		// ���_���ւ̃|�C���^

									// ���_�o�b�t�@�����b�N
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = data.pos + D3DXVECTOR3(-data.fWidth, -data.fHeight, 0.0f);
	pVtx[1].pos = data.pos + D3DXVECTOR3(data.fWidth, -data.fHeight, 0.0f);
	pVtx[2].pos = data.pos + D3DXVECTOR3(-data.fWidth, data.fHeight, 0.0f);
	pVtx[3].pos = data.pos + D3DXVECTOR3(data.fWidth, data.fHeight, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = data.color.col;
	pVtx[1].col = data.color.col;
	pVtx[2].col = data.color.col;
	pVtx[3].col = data.color.col;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

}

//--------------------------------------------------
// �S�Ă��X�V
//--------------------------------------------------
void CParticle::AllUpdate()
{
	for (int i = 0; i < maxNumber; i++)
	{
		if (g_aParticle[i] == nullptr)
		{
			continue;
		}

		g_aParticle[i]->Update();

		if (g_aParticle[i]->data.nLife <= 0)
		{// �G�t�F�N�g�̎���
			g_aParticle[i]->Uninit();
			delete g_aParticle[i];
			g_aParticle[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	switch (data.alphaBlend)
	{
	case TYPE_NONE:	// ��Z
		break;

	case TYPE_ADD:	// ���Z
					// �A���t�@�u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case TYPE_SUB:	// ���Z
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
	pDevice->SetTexture(0, pTexture[data.type]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���������p���Ȃ�
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �S�Ă̕`��
//--------------------------------------------------
void CParticle::AllDraw()
{
	for (int i = 0; i < maxNumber; i++)
	{
		if (g_aParticle[i] == nullptr)
		{
			continue;
		}

		g_aParticle[i]->Draw();
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CParticle* CParticle::Create(const Particle& inParticle, const D3DXVECTOR3& inPos)
{
	for (int i = 0; i < maxNumber; i++)
	{
		if (g_aParticle[i] == nullptr)
		{
			g_aParticle[i] = new CParticle;
			g_aParticle[i]->Init();
			g_aParticle[i]->Set(inParticle, inPos);

			g_aParticle[i]->idx = i;
			return g_aParticle[i];
		}
	}
}

//--------------------------------------------------
// �f�[�^�̏����ݒ�
//--------------------------------------------------
void CParticle::Set(const Particle& inParticle, const D3DXVECTOR3 & inPos)
{
	data = inParticle;

	data.fWidth = data.fScale;
	data.fHeight = data.fScale;
	data.pos = inPos;
	data.type = PARTICLETYPE_NORMAL;

	//data.fWidth = g_aParticle[]->data->fScale;
	//data.fHeight = g_aParticle->data->fScale;
	data.color.nCntTransitionTime = 0;
	data.bUse = true;

	// �����ʒu�̎Z�o
	data.pos.x += FloatRandam(data.maxPopPos.x, -data.minPopPos.x);
	data.pos.y += FloatRandam(data.maxPopPos.y, -data.minPopPos.y);
	data.pos.z += FloatRandam(data.maxPopPos.z, -data.minPopPos.z);

	// �F�̎Z�o
	if (data.color.bColRandom)
	{// �����_���J���[���g�p
		data.color.col.r = FloatRandam(data.color.colRandamMax.r, data.color.colRandamMin.r);
		data.color.col.g = FloatRandam(data.color.colRandamMax.g, data.color.colRandamMin.g);
		data.color.col.b = FloatRandam(data.color.colRandamMax.b, data.color.colRandamMin.b);

		if (data.color.bColTransition)
		{// �ړI�̐F�̐ݒ�
			if (data.color.bRandomTransitionTime)
			{
				data.color.nEndTime = rand() % data.nLife + 1;
			}

			data.color.destCol.r = FloatRandam(data.color.colRandamMax.r, data.color.colRandamMin.r);
			data.color.destCol.g = FloatRandam(data.color.colRandamMax.g, data.color.colRandamMin.g);
			data.color.destCol.b = FloatRandam(data.color.colRandamMax.b, data.color.colRandamMin.b);
		}
	}

	if (data.color.bColTransition)
	{// �g���f�B�V�I���J���[���g�p
		if (data.color.bRandomTransitionTime)
		{
			data.color.nEndTime = rand() % data.nLife + 1;
		}

		data.color.colTransition.r = (data.color.destCol.r - data.color.col.r) / data.color.nEndTime;
		data.color.colTransition.g = (data.color.destCol.g - data.color.col.g) / data.color.nEndTime;
		data.color.colTransition.b = (data.color.destCol.b - data.color.col.b) / data.color.nEndTime;
	}

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

					// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = data.pos + D3DXVECTOR3(-data.fWidth, -data.fHeight, 0.0f);
	pVtx[1].pos = data.pos + D3DXVECTOR3(data.fWidth, -data.fHeight, 0.0f);
	pVtx[2].pos = data.pos + D3DXVECTOR3(-data.fWidth, data.fHeight, 0.0f);
	pVtx[3].pos = data.pos + D3DXVECTOR3(data.fWidth, data.fHeight, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = data.color.col;
	pVtx[1].col = data.color.col;
	pVtx[2].col = data.color.col;
	pVtx[3].col = data.color.col;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	float ImAngle = GetAngle();
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (data.bBackrot)
	{
		// float fRad = (data.fAngle) * (D3DX_PI / 180);
		fGRad = (data.rot.z - g_fAngle);
	}
	else
	{
		fRad = (data.fAngle) * (D3DX_PI / 180);
		fGRad = (data.rot.z + g_fAngle);
	}

	// ����
	{
		/*
		g_fAngle += 30.0f * i;
		data.move.x = sinf(fGRad) * 1.3f;
		data.move.y = cosf(fGRad) * 1.3f;

		// ��
		g_fAngle += 0.7f;
		data.move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * data.fAttenuation;
		data.move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * data.fAttenuation;
		*/

		// ������������
		g_fAngle += ImAngle;
		data.move.x += (data.fRadius * sinf(fGRad)) * data.fAttenuation;
		data.move.y += (data.fRadius * cosf(fGRad)) * data.fAttenuation;
	}

	// ======================
	// ���K��
	// ======================
	if (data.fRadius > D3DX_PI)
	{
		data.fRadius -= D3DX_PI * 2;
	}
	else if (data.fRadius < -D3DX_PI)
	{
		data.fRadius += D3DX_PI * 2;
	}

	if (g_fAngle > D3DX_PI)
	{
		g_fAngle -= D3DX_PI * 2;
	}
	else if (g_fAngle < -D3DX_PI)
	{
		g_fAngle += D3DX_PI * 2;
	}
}

//--------------------------------------------------
// �e�N�X�`���̓Ǎ���
//--------------------------------------------------
void CParticle::LoadTex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool *ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, GetFileName(), &pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

//--------------------------------------------------
// ���̍폜
//--------------------------------------------------
void CParticle::Delete(const int data)
{
	// �f�[�^�̃��Z�b�g
	memset(&this->data, 0, sizeof(this->data));
}

//--------------------------------------------------
// ����S�č폜
//--------------------------------------------------
void CParticle::DeleteAll()
{
	for (int i = 0; i < maxNumber; i++)
	{
		Delete(i);
	}
}

//--------------------------------------------------
// �p�x�̏�����
//--------------------------------------------------
void CParticle::RemoveAngle(void)
{
	g_fAngle = 0;
}

//--------------------------------------------------
// float �� DWORD �ɕϊ�
//--------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}
