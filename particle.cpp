//=============================================================================
//
// effect.cpp
// Author : Tanaka Kouta
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// iclude
//-----------------------------------------------------------------------------
#include "particle.h"
#include "main.h"
#include "application.h"
#include "utility.h"
#include "renderer.h"

#include <assert.h>

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CParticle::CParticle() :
	m_data({})
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CParticle::~CParticle()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CParticle::Init()
{
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CParticle::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool *bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	/* ���g�p���Ă���Ȃ火 */

	// ����
	m_data.nLife--;									// �̗͂̌���
	m_data.move.y += m_data.fWeight;				// �d��
	m_data.move *= m_data.fAttenuation;				// �ړ��ʂ̐���
	m_data.fWeight += m_data.fWeightTransition;		// �d���̐���
	m_data.fWidth += m_data.scaleTransition.x;		// ���̐���
	m_data.fHeight += m_data.scaleTransition.y;		// �����̐���

	ColorTransition();	// �F�̑J��

	AddPos(m_data.move);	// �ʒu�̉��Z
	AddRotY(m_data.rot.z);	// �p�x�̉��Z
	SetSize(D3DXVECTOR2(m_data.fWidth, m_data.fHeight));

	if (m_data.nLife <= 0)
	{// �G�t�F�N�g�̎���
		Release();
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	switch (m_data.alphaBlend)
	{
	case TYPE_NONE:	// ��Z
		break;

	case TYPE_ADD:	// ���Z
		// ���u�����f�B���O�����Z�����ɐݒ�
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

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CParticle* CParticle::Create(const Info& inParticle, const D3DXVECTOR3& inPos)
{
	CParticle* particle = nullptr;

	if (particle != nullptr)
	{
		return particle;
	}

	particle = new CParticle;
	particle->Init();
	particle->m_data = inParticle;
	particle->SetPos(inPos);
	particle->SetSize(D3DXVECTOR2(particle->m_data.fWidth, particle->m_data.fHeight));
	particle->SetTexture(particle->m_data.nIdxTex);
	particle->SetColor(particle->m_data.color.colBigin);
	return particle;
}

//-----------------------------------------------------------------------------
// �e�N�X�`���̓Ǎ���
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �F�̑J��
//-----------------------------------------------------------------------------
void CParticle::ColorTransition()
{
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

	SetColor(myColor);
}
