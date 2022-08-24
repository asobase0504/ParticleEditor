//=========================================
// 
// �p�[�e�B�N���G�~�b�^
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

//=========================================
// include
//=========================================
#include <d3dx9.h>
#include <list>
#include <vector>
#include "particle.h"
#include "particle_emitter.h"

//=========================================
// �O���錾
//=========================================
class CParticleEmitter;

//=========================================
// �p�[�e�B�N���t�@�N�g���[
//=========================================
class CParticleManager
{
public:	// �\����
	struct BundledData
	{
		CParticle::Info particleData;
		CParticleEmitter::Info emitterData;
	};
public:
	CParticleManager();
	~CParticleManager();

	HRESULT Init();
	void Uninit();
	void Update();
	CParticleEmitter* Create(const D3DXVECTOR3& pos, const int& idx);
	void ChangeBundledData(const int idx, const BundledData&inData);

	// Setter
	void SetBundledData(const BundledData&inData);

	// Getter
	std::vector<BundledData>& GetBundledData() { return m_bundledData; }
	std::list<CParticleEmitter*> GetEmitter() { return m_particleEmitter; }

private:
	void ReleaseEmitter();
private:
	std::vector<BundledData> m_bundledData;	// ����
	std::list<CParticleEmitter*> m_particleEmitter;	// �G�~�b�^�\���
};

#endif // !_PARTICLE_FACTORY_H_
