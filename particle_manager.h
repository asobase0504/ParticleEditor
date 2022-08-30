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
#include "file.h"

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

	void ChangeEmitterInfo(CParticleEmitter* inEmitter, const int& idx);

	// �O���Ǎ���
	void LoadBundledData(const wchar_t* cUrl);

	// Setter
	int SetBundledData(const BundledData&inData);

	// Getter
	std::vector<BundledData>& GetBundledData() { return m_bundledData; }	// ���̑S�̂̎擾
	std::list<CParticleEmitter*> GetEmitter() { return m_particleEmitter; }	// �G�~�b�^�\�S�̂̎擾

private:
	void ReleaseEmitter();
private:
	std::vector<BundledData> m_bundledData;	// ����
	std::list<CParticleEmitter*> m_particleEmitter;	// �G�~�b�^�\���
};

#endif // !_PARTICLE_FACTORY_H_
