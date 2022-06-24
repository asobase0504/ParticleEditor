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
		&m_pTexture[PARTICLETYPE_NORMAL]);

	// �f�[�^�̏�����
	memset(&m_data, 0, sizeof(m_data));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	m_pVtxBuff->Unlock();

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
		if (m_pTexture[i] != NULL)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	/* ���g�p���Ă���Ȃ火 */

	// �G�t�F�N�g�̈ړ�
	m_data.pos += m_data.move;

	// ����
	m_data.nLife--;							// �̗͂̌���
	m_data.move.y += m_data.fWeight;			// �d��
	m_data.move *= m_data.fAttenuation;			// �ړ��ʂ̐���
	m_data.fWeight += m_data.fWeightTransition;	// �d���̐���

	if (m_data.color.bColTransition)
	{// �F�̐���
		if (m_data.color.nEndTime >= m_data.color.nCntTransitionTime)
		{
			m_data.color.nCntTransitionTime++;
			m_data.color.col += m_data.color.colTransition;
		}
	}

	VERTEX_2D *pVtx = nullptr;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[1].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[2].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, m_data.fHeight, 0.0f);
	pVtx[3].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, m_data.fHeight, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_data.color.col;
	pVtx[1].col = m_data.color.col;
	pVtx[2].col = m_data.color.col;
	pVtx[3].col = m_data.color.col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

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

		if (g_aParticle[i]->m_data.nLife <= 0)
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

	switch (m_data.alphaBlend)
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
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[m_data.type]);

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

			g_aParticle[i]->m_idx = i;
			return g_aParticle[i];
		}
	}
}

//--------------------------------------------------
// �f�[�^�̏����ݒ�
//--------------------------------------------------
void CParticle::Set(const Particle& inParticle, const D3DXVECTOR3 & inPos)
{
	m_data = inParticle;

	m_data.fWidth = m_data.fScale;
	m_data.fHeight = m_data.fScale;
	m_data.pos = inPos;
	m_data.type = PARTICLETYPE_NORMAL;

	//m_data.fWidth = g_aParticle[]->m_data->fScale;
	//m_data.fHeight = g_aParticle->m_data->fScale;
	m_data.color.nCntTransitionTime = 0;
	m_data.bUse = true;

	// �����ʒu�̎Z�o
	m_data.pos.x += FloatRandam(m_data.maxPopPos.x, -m_data.minPopPos.x);
	m_data.pos.y += FloatRandam(m_data.maxPopPos.y, -m_data.minPopPos.y);
	m_data.pos.z += FloatRandam(m_data.maxPopPos.z, -m_data.minPopPos.z);

	// �F�̎Z�o
	if (m_data.color.bColRandom)
	{// �����_���J���[���g�p
		m_data.color.col.r = FloatRandam(m_data.color.colRandamMax.r, m_data.color.colRandamMin.r);
		m_data.color.col.g = FloatRandam(m_data.color.colRandamMax.g, m_data.color.colRandamMin.g);
		m_data.color.col.b = FloatRandam(m_data.color.colRandamMax.b, m_data.color.colRandamMin.b);

		if (m_data.color.bColTransition)
		{// �ړI�̐F�̐ݒ�
			if (m_data.color.bRandomTransitionTime)
			{
				m_data.color.nEndTime = rand() % m_data.nLife + 1;
			}

			m_data.color.destCol.r = FloatRandam(m_data.color.colRandamMax.r, m_data.color.colRandamMin.r);
			m_data.color.destCol.g = FloatRandam(m_data.color.colRandamMax.g, m_data.color.colRandamMin.g);
			m_data.color.destCol.b = FloatRandam(m_data.color.colRandamMax.b, m_data.color.colRandamMin.b);
		}
	}

	if (m_data.color.bColTransition)
	{// �g���f�B�V�I���J���[���g�p
		if (m_data.color.bRandomTransitionTime)
		{
			m_data.color.nEndTime = rand() % m_data.nLife + 1;
		}

		m_data.color.colTransition.r = (m_data.color.destCol.r - m_data.color.col.r) / m_data.color.nEndTime;
		m_data.color.colTransition.g = (m_data.color.destCol.g - m_data.color.col.g) / m_data.color.nEndTime;
		m_data.color.colTransition.b = (m_data.color.destCol.b - m_data.color.col.b) / m_data.color.nEndTime;
	}

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

					// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[1].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[2].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, m_data.fHeight, 0.0f);
	pVtx[3].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, m_data.fHeight, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_data.color.col;
	pVtx[1].col = m_data.color.col;
	pVtx[2].col = m_data.color.col;
	pVtx[3].col = m_data.color.col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	float ImAngle = GetAngle();
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (m_data.bBackrot)
	{
		// float fRad = (m_data.fAngle) * (D3DX_PI / 180);
		fGRad = (m_data.rot.z - g_fAngle);
	}
	else
	{
		fRad = (m_data.fAngle) * (D3DX_PI / 180);
		fGRad = (m_data.rot.z + g_fAngle);
	}

	// ����
	{
		/*
		g_fAngle += 30.0f * i;
		m_data.move.x = sinf(fGRad) * 1.3f;
		m_data.move.y = cosf(fGRad) * 1.3f;

		// ��
		g_fAngle += 0.7f;
		m_data.move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * m_data.fAttenuation;
		m_data.move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * m_data.fAttenuation;
		*/

		// ������������
		g_fAngle += ImAngle;
		m_data.move.x += (m_data.fRadius * sinf(fGRad)) * m_data.fAttenuation;
		m_data.move.y += (m_data.fRadius * cosf(fGRad)) * m_data.fAttenuation;
	}

	// ======================
	// ���K��
	// ======================
	if (m_data.fRadius > D3DX_PI)
	{
		m_data.fRadius -= D3DX_PI * 2;
	}
	else if (m_data.fRadius < -D3DX_PI)
	{
		m_data.fRadius += D3DX_PI * 2;
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
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, GetFileName(), &m_pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

//--------------------------------------------------
// ���̍폜
//--------------------------------------------------
void CParticle::Delete(const int data)
{
	// �f�[�^�̃��Z�b�g
	memset(&this->m_data, 0, sizeof(this->m_data));
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
