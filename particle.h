//����������������������������������������
//particle.h
// Author : Tanaka Kouta
//����������������������������������������
#ifndef _PARTICLE_H_		//���̃}�N������`����ĂȂ�������
#define _PARTICLE_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include <d3dx9.h>
#include "object2d.h"

//-------------------------------------------------
// �p�[�e�B�N���N���X
// Author : Tanaka Kouta
// Author : Yuda Kaito
// �T�v : �p�[�e�B�N���̏�����S������N���X
//-------------------------------------------------
class CParticle : public CObject2D
{
public:	// �萔
public:	// �ÓI�ϐ�
public:	// �񋓌^
public: // �\����

	//-------------------------------------------------
	// �e�N�X�`�����
	// Author : 
	// �T�v : �e�N�X�`���Ɋւ���f�[�^�\����
	//-------------------------------------------------
	struct ParticleTex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float size;
	};

	//-------------------------------------------------
	// �G�t�F�N�g�̃J���[�\����
	// Author : �������l
	// �T�v : �G�t�F�N�g�̐F�֌W�̍\����
	//-------------------------------------------------
	struct Color	// ���V�K�ǉ��\����(�t�@�C���ɒǉ�������Ă�������)
	{
		D3DXCOLOR colBigin;			// �n�܂�̐F
		D3DXCOLOR colRandamMax;		// �����_���ȐF�͈̔�(�ő�)
		D3DXCOLOR colRandamMin;		// �����_���ȐF�͈̔�(�ŏ�)
		D3DXCOLOR colTransition;	// �F�̑J�ڗ�
		D3DXCOLOR destCol;			// �ړI�̐F
		int nEndTime;				// �J�ڊ����܂ł̎���
		int nCntTransitionTime;		// �J�ڃJ�E���g
		bool bColTransition;		// �J�ڂ��s����
		bool bColRandom;			// �����_���ŐF��ύX���邩
		bool bRandomTransitionTime;	// �J�ڊ������Ԃ̗���
	};

	//-------------------------------------------------
	// �G�t�F�N�g�̃��u�����h�̎�ʂ̗񋓌^
	// Author : �������l
	// �T�v : ��Z�A���Z�A���Z
	//-------------------------------------------------
	enum ALPHABLENDTYPE
	{
		TYPE_ADD = 0,		// ���Z
		TYPE_SUB,			// ���Z
		TYPE_NONE,			// ��Z
		MAX_TYPE,			// ��ʂ̍ő吔
	};

	//-------------------------------------------------
	//�G�t�F�N�g�̍\���̂̒�`
	// Author : Tanaka Kouta
	// Author : Yuda Kaito
	// Author : Hamada Ryuga
	// Author : �������l
	//-------------------------------------------------
	struct Info
	{
		D3DXVECTOR3 move;				// �ړ���
		D3DXVECTOR3 moveTransition;		// �ړ��ʂ̐��ڗ�
		D3DXVECTOR3 rot;				// ����
		Color color;					// �J���[
		ALPHABLENDTYPE alphaBlend;		// ���u�����f�B���O�̎��
		D3DXVECTOR3 scaleTransition;	// �T�C�Y�̐��ڗ�			���V�K�ǉ��ϐ�(�t�@�C���ɒǉ�������Ă�������)
		float fWidth;					// ��
		float fHeight;					// ����
		float fAngle;					// �p�x
		float fAttenuation;				// �����l
		float fRadius;					// ���a
		float fWeight;					// �d��
		float fWeightTransition;		// �d���̐��ڗ�
		float fScale;					// �S�̓I�ȑ傫��
		int nLife;						// ����
		int nMaxLife;					// �ő����
		int nMoveTime;					//							�����g�p�ϐ�
		int nIdxTex;					// �e�N�X�`���̔ԍ�
		bool bUseWeight;				// �d�����g�p���邩�ǂ���
		bool bBackrot;					// �t��]�����邩�ǂ���
		bool bTexturerot;				// �e�N�X�`����]�����邩�ǂ���
	};

public:
	CParticle();
	~CParticle();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CParticle* Create(const Info& inParticle, const D3DXVECTOR3& inPos);
	void LoadTex();

private:	// �v���C�x�[�g�֐�
	void ColorTransition();

private:	// �����o�[�ϐ�
	Info m_data;
	int m_idx;
};
#endif
