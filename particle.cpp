// ����������������������������������������
// effect.cpp
// ����������������������������������������
#include "main.h"
#include "particle.h"
#include <time.h>
#include "application.h"
#include "imgui_property.h"
#include "utility.h"
#include "application.h"
#include "renderer.h"

//==================================================
// �ÓI�����o�[�ϐ�
//==================================================
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
	CObject2D::Init();

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CParticle::Uninit()
{
	CObject2D::Uninit();
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
	D3DXVECTOR3 myPos = GetPos();
	myPos += m_data.move;

	// ����
	m_data.nLife--;							// �̗͂̌���
	m_data.move.y += m_data.fWeight;			// �d��
	m_data.move *= m_data.fAttenuation;			// �ړ��ʂ̐���
	m_data.fWeight += m_data.fWeightTransition;	// �d���̐���

	D3DXCOLOR myColor = GetColor();
	if (m_data.color.bColTransition)
	{// �F�̐���
		if (m_data.color.nEndTime >= m_data.color.nCntTransitionTime)
		{
			m_data.color.nCntTransitionTime++;
			myColor += m_data.color.colTransition;
		}
	}
	myColor.a -= 1.0f / m_data.nMaxLife;

	SetPos(myPos);
	SetColor(myColor);
	SetSize(D3DXVECTOR2(m_data.fWidth, m_data.fHeight));

	if (m_data.nLife <= 0)
	{// �G�t�F�N�g�̎���
		Release();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

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

	CObject2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���������p���Ȃ�
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CParticle* CParticle::Create(const Particle& inParticle, const D3DXVECTOR3& inPos, const D3DXCOLOR& color)
{
	CParticle* particle = nullptr;
	if (particle == nullptr)
	{
		particle = new CParticle;
		particle->Init();
		particle->Set(inParticle, inPos, color);
		return particle;
	}
	return nullptr;
}

//--------------------------------------------------
// �f�[�^�̏����ݒ�
//--------------------------------------------------
void CParticle::Set(const Particle& inParticle, const D3DXVECTOR3& inPos, const D3DXCOLOR& color)
{
	m_data = inParticle;

	m_data.nMaxLife = m_data.nLife;
	m_data.fWidth = m_data.fScale;
	m_data.fHeight = m_data.fScale;
	m_data.type = PARTICLETYPE_NORMAL;

	//m_data.fWidth = g_aParticle[]->m_data->fScale;
	//m_data.fHeight = g_aParticle->m_data->fScale;
	m_data.color.nCntTransitionTime = 0;
	m_data.bUse = true;

	D3DXVECTOR3 myPos = inPos;
	// �����ʒu�̎Z�o
	myPos.x += FloatRandam(m_data.maxPopPos.x, -m_data.minPopPos.x);
	myPos.y += FloatRandam(m_data.maxPopPos.y, -m_data.minPopPos.y);
	myPos.z += FloatRandam(m_data.maxPopPos.z, -m_data.minPopPos.z);

	// �F�̎Z�o
	D3DXCOLOR myColor = color;
	if (m_data.color.bColRandom)
	{// �����_���J���[���g�p
		myColor.r = FloatRandam(m_data.color.colRandamMax.r, m_data.color.colRandamMin.r);
		myColor.g = FloatRandam(m_data.color.colRandamMax.g, m_data.color.colRandamMin.g);
		myColor.b = FloatRandam(m_data.color.colRandamMax.b, m_data.color.colRandamMin.b);

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

		m_data.color.colTransition.r = (m_data.color.destCol.r - myColor.r) / m_data.color.nEndTime;
		m_data.color.colTransition.g = (m_data.color.destCol.g - myColor.g) / m_data.color.nEndTime;
		m_data.color.colTransition.b = (m_data.color.destCol.b - myColor.b) / m_data.color.nEndTime;
	}

	SetPos(myPos);
	SetSize(D3DXVECTOR2(m_data.fWidth, m_data.fHeight));
	SetTexture(1);
	SetColor(myColor);

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
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool *ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// �e�N�X�`���̓ǂݍ���
		//D3DXCreateTextureFromFile(pDevice, , &m_pTexture[PARTICLETYPE_NORMAL]);

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
