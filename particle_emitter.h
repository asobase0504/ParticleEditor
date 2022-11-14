//=============================================================================
// 
// �p�[�e�B�N���G�~�b�^
// Author YudaKaito
// 
//=============================================================================
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <d3dx9.h>
#include "particle.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CParticle;

//-----------------------------------------------------------------------------
// �p�[�e�B�N���G�~�b�^
//-----------------------------------------------------------------------------
class CParticleEmitter
{
public:	// �\����
	struct Info
	{
		D3DXVECTOR3 maxPopPos;		// �����ʒu(�ő�)
		D3DXVECTOR3 minPopPos;		// �����ʒu(�ŏ�)
		float fAngle;				// ���ˊp�x
		float fAddAngle;			// ���ˊp�x�𐶐����ɕω��������
		int popNumber;				// ��x�̏o����
	};
public:	// �ÓI�����o�[�ϐ�
public:
	CParticleEmitter();
	~CParticleEmitter();

	HRESULT Init();
	void Uninit();
	void Update();

	// Setter
	void SetPos(const D3DXVECTOR3& inPos);
	void SetBundledDataIndex(const int inIndex) { m_bundledDataIndex = inIndex; }
	void SetParticle(const CParticle::Info& inParticle);
	void SetEmitter(const Info& inEmitter) { m_info = inEmitter; }
	void SetNeedsDelete(const bool inNeedsDelete) { m_needsDelete = inNeedsDelete; }

	// Getter
	const D3DXVECTOR3& GetPos() { return m_pos; }
	bool GetNeedsDelete() { return m_needsDelete; }
	const int GetBundledIndex() { return m_bundledDataIndex; }

private:
	void PopParticle(void);

private:
	D3DXVECTOR3 m_pos;	// �o���ʒu
	Info m_info;		// �G�~�b�^�[���Ǘ�������ꗗ
	CParticle::Info m_particleInfo;	// ���̃G�~�b�^�[����o��p�[�e�B�N���̃f�[�^
	int m_bundledDataIndex;
	bool m_needsDelete;	// �f���[�g����K�v�����邩
};

#endif // !_PARTICLE_EMITTER_H_
