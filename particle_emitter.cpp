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

//==================================================
// �ÓI�����o�[�ϐ�
//==================================================
float CParticleEmitter::m_fAngle = 0.0f;

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CParticleEmitter::CParticleEmitter() :
	emitterInfo({}),
	m_data(nullptr)
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
	Pop();
}

//-----------------------------------------
// �ݒ�
//-----------------------------------------
void CParticleEmitter::SetPos(const D3DXVECTOR3 & inPos)
{
	pos = inPos;
}

//-----------------------------------------
// �o��
//-----------------------------------------
void CParticleEmitter::Pop(void)
{
	CParticle::Particle popData = *m_data;
	popData.nMaxLife = popData.nLife;
	popData.fWidth = popData.fScale;
	popData.fHeight = popData.fScale;
	popData.type = CParticle::PARTICLETYPE_NORMAL;

	popData.color.nCntTransitionTime = 0;
	popData.bUse = true;

	D3DXVECTOR3 myPos = pos;
	// �����ʒu�̎Z�o
	myPos.x += FloatRandam(popData.maxPopPos.x, -popData.minPopPos.x);
	myPos.y += FloatRandam(popData.maxPopPos.y, -popData.minPopPos.y);
	myPos.z += FloatRandam(popData.maxPopPos.z, -popData.minPopPos.z);

	// �F�̎Z�o
	if (popData.color.bColRandom)
	{// �����_���J���[���g�p
		popData.color.colBigin.r = FloatRandam(popData.color.colRandamMax.r, popData.color.colRandamMin.r);
		popData.color.colBigin.g = FloatRandam(popData.color.colRandamMax.g, popData.color.colRandamMin.g);
		popData.color.colBigin.b = FloatRandam(popData.color.colRandamMax.b, popData.color.colRandamMin.b);

		if (popData.color.bColTransition)
		{// �ړI�̐F�̐ݒ�
			if (popData.color.bRandomTransitionTime)
			{
				popData.color.nEndTime = rand() % popData.nLife + 1;
			}

			popData.color.destCol.r = FloatRandam(popData.color.colRandamMax.r, popData.color.colRandamMin.r);
			popData.color.destCol.g = FloatRandam(popData.color.colRandamMax.g, popData.color.colRandamMin.g);
			popData.color.destCol.b = FloatRandam(popData.color.colRandamMax.b, popData.color.colRandamMin.b);
		}
	}

	if (popData.color.bColTransition)
	{// �g���f�B�V�I���J���[���g�p
		if (popData.color.bRandomTransitionTime)
		{
			popData.color.nEndTime = rand() % popData.nLife + 1;
		}

		popData.color.colTransition.r = (popData.color.destCol.r - popData.color.colBigin.r) / popData.color.nEndTime;
		popData.color.colTransition.g = (popData.color.destCol.g - popData.color.colBigin.g) / popData.color.nEndTime;
		popData.color.colTransition.b = (popData.color.destCol.b - popData.color.colBigin.b) / popData.color.nEndTime;
	}

	static float ImAngle = 10.0f;
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (popData.bBackrot)
	{
		// float fRad = (popData.fAngle) * (D3DX_PI / 180);
		fGRad = (popData.rot.z - m_fAngle);
	}
	else
	{
		fRad = (popData.fAngle) * (D3DX_PI / 180);
		fGRad = (popData.rot.z + m_fAngle);
	}

	// ����
	{
		/*
		m_fAngle += 30.0f * i;
		popData.move.x = sinf(fGRad) * 1.3f;
		popData.move.y = cosf(fGRad) * 1.3f;

		// ��
		m_fAngle += 0.7f;
		popData.move.x = sinf((D3DX_PI / 180) * 17 * m_fAngle) * popData.fAttenuation;
		popData.move.y = sinf((D3DX_PI / 180) * 8 * m_fAngle) * popData.fAttenuation;
		*/

		// ������������
		m_fAngle += ImAngle;
		popData.move.x += (popData.fRadius * sinf(fGRad)) * popData.fAttenuation;
		popData.move.y += (popData.fRadius * cosf(fGRad)) * popData.fAttenuation;
	}

	// ======================
	// ���K��
	// ======================
	if (popData.fRadius > D3DX_PI)
	{
		popData.fRadius -= D3DX_PI * 2;
	}
	else if (popData.fRadius < -D3DX_PI)
	{
		popData.fRadius += D3DX_PI * 2;
	}

	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2;
	}
	else if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2;
	}

	CParticle::Create(popData, myPos);
}

void CParticleEmitter::SetParticle(CParticle::Particle* inParticle)
{
	m_data = inParticle;
}
