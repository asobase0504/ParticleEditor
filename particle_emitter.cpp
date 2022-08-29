//=========================================
// 
// �p�[�e�B�N���G�~�b�^
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "particle_emitter.h"
#include "utility.h"

//=========================================
// �ÓI�����o�[�ϐ�
//=========================================

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CParticleEmitter::CParticleEmitter() :
	m_info({}),
	m_particleInfo({}),
	m_needsDelete(false)
{

}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CParticleEmitter::~CParticleEmitter()
{

}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CParticleEmitter::Init()
{
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CParticleEmitter::Uninit()
{
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CParticleEmitter::Update()
{
	for (int i = 0; i < m_info.popNumber; i++)
	{
		PopParticle();
	}
}

//-----------------------------------------
// �ݒ�
//-----------------------------------------
void CParticleEmitter::SetPos(const D3DXVECTOR3 & inPos)
{
	m_pos = inPos;
}

//-----------------------------------------
// �p�[�e�B�N���̏o��
//-----------------------------------------
void CParticleEmitter::PopParticle(void)
{
	CParticle::Info popInfo = m_particleInfo;
	popInfo.nMaxLife = popInfo.nLife;
	popInfo.fWidth = popInfo.fScale;
	popInfo.fHeight = popInfo.fScale;
	popInfo.type = CParticle::PARTICLETYPE_NORMAL;

	D3DXVECTOR3 myPos = m_pos;
	// �����ʒu�̎Z�o
	myPos.x += FloatRandam(m_info.maxPopPos.x, -m_info.minPopPos.x);
	myPos.y += FloatRandam(m_info.maxPopPos.y, -m_info.minPopPos.y);
	myPos.z += FloatRandam(m_info.maxPopPos.z, -m_info.minPopPos.z);

	// �F�̎Z�o
	CParticle::Color& popColor = popInfo.color;
	popColor.nCntTransitionTime = 0;
	if (popColor.bColRandom)
	{// �����_���J���[���g�p
		popColor.colBigin.r = FloatRandam(popColor.colRandamMax.r, popColor.colRandamMin.r);
		popColor.colBigin.g = FloatRandam(popColor.colRandamMax.g, popColor.colRandamMin.g);
		popColor.colBigin.b = FloatRandam(popColor.colRandamMax.b, popColor.colRandamMin.b);

		if (popColor.bColTransition)
		{// �ړI�̐F�̐ݒ�
			if (popColor.bRandomTransitionTime)
			{
				popColor.nEndTime = rand() % popInfo.nLife + 1;
			}

			popColor.destCol.r = FloatRandam(popColor.colRandamMax.r, popColor.colRandamMin.r);
			popColor.destCol.g = FloatRandam(popColor.colRandamMax.g, popColor.colRandamMin.g);
			popColor.destCol.b = FloatRandam(popColor.colRandamMax.b, popColor.colRandamMin.b);
		}
	}

	if (popColor.bColTransition)
	{// �g���f�B�V�I���J���[���g�p
		if (popColor.bRandomTransitionTime)
		{
			popColor.nEndTime = rand() % popInfo.nLife + 1;
		}

		popColor.colTransition.r = (popColor.destCol.r - popColor.colBigin.r) / popColor.nEndTime;
		popColor.colTransition.g = (popColor.destCol.g - popColor.colBigin.g) / popColor.nEndTime;
		popColor.colTransition.b = (popColor.destCol.b - popColor.colBigin.b) / popColor.nEndTime;
	}

	static float ImAngle = 30.0f;
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (popInfo.bBackrot)
	{
		// float fRad = (popData.fAngle) * (D3DX_PI / 180);
		fGRad = (popInfo.rot.z - m_info.fAngle);
	}
	else
	{
		fRad = (popInfo.fAngle) * (D3DX_PI / 180);
		fGRad = (popInfo.rot.z + m_info.fAngle);
	}

	// ����
	{
		// ������������
		m_info.fAngle += ImAngle;
		popInfo.move.x += (popInfo.fRadius * sinf(fGRad)) * popInfo.fAttenuation;
		popInfo.move.y += (popInfo.fRadius * cosf(fGRad)) * popInfo.fAttenuation;
	}

	// ======================
	// ���K��
	// ======================
	if (popInfo.fRadius > D3DX_PI)
	{
		popInfo.fRadius -= D3DX_PI * 2;
	}
	else if (popInfo.fRadius < -D3DX_PI)
	{
		popInfo.fRadius += D3DX_PI * 2;
	}

	if (m_info.fAngle > D3DX_PI)
	{
		m_info.fAngle -= D3DX_PI * 2;
	}
	else if (m_info.fAngle < -D3DX_PI)
	{
		m_info.fAngle += D3DX_PI * 2;
	}

	CParticle::Create(popInfo, myPos);
}

//-----------------------------------------
// �ݒ�
//-----------------------------------------
void CParticleEmitter::SetParticle(const CParticle::Info& inParticle)
{
	m_particleInfo = inParticle;
}
