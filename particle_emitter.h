//=========================================
// 
// �p�[�e�B�N���G�~�b�^
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

//=========================================
// include
//=========================================
#include <d3dx9.h>
#include "particle.h"

//=========================================
// �O���錾
//=========================================
class CParticle;

//=========================================
// �p�[�e�B�N���G�~�b�^
//=========================================
class CParticleEmitter
{
public:	// �\����
	struct Emitter
	{
	};
public:	// �ÓI�����o�[�ϐ�
	static float m_fAngle;	// ���ˊp�x
public:
	CParticleEmitter();
	~CParticleEmitter();

	HRESULT Init();
	void Uninit();
	void Update();
	void SetPos(const D3DXVECTOR3& inPos);
	const D3DXVECTOR3& GetPos() { return pos; }
	void Pop(void);
	void SetParticle(CParticle::Particle* inParticle);
	CParticle::Particle* GetParticle() { return m_data; }
private:
	D3DXVECTOR3 pos;	// �o���ʒu
	Emitter emitterInfo;
	CParticle::Particle* m_data;	// ���̃G�~�b�^�[����o��p�[�e�B�N���̃f�[�^
};

#endif // !_PARTICLE_EMITTER_H_
