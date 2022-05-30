//����������������������������������������
//particle.h
//����������������������������������������
#ifndef _PARTICLE_H_		//���̃}�N������`����ĂȂ�������
#define _PARTICLE_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//�}�N����`
#define MAX_PARTICLE	(4096)	//�G�t�F�N�g�̍ő吔
#define NUM_PARTICLE	(10)		//�G�t�F�N�g�̎��

//�G�t�F�N�g�̗񋓌^
enum PARTICLETYPE
{
	PARTICLETYPE_NONE = 0,
	PARTICLETYPE_NORMAL,
	PARTICLETYPE_PLAYER,
	PARTICLETYPE_MAX
};

//�G�t�F�N�g�̍\���̂̒�`
struct Particle
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 moveTransition;	// �ړ��ʂ̐��ڗ�
	D3DXVECTOR3 rot;			//����
	D3DXCOLOR col;				// �F
	D3DXCOLOR colRandamMax;		// �����_���ȐF�͈̔�(�ő�)
	D3DXCOLOR colRandamMin;		// �����_���ȐF�͈̔�(�ŏ�)
	D3DXCOLOR colTransition;	// �F�̐��ڗ�
	PARTICLETYPE type;			// �G�t�F�N�g�̎��
	float fWidth;				// ��
	float fHeight;				// ����
	float fAngle;				// �p�x
	float fAttenuation;			// �����l
	float fRadius;				// ���a
	float fWeight;				// �d��
	float fWeightTransition;	// �d���̐��ڗ�
	int nLife;					// ����
	bool bBackrot;
	bool bUse;					// �g�p���Ă��邩
};

struct Texture
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	float size;
};
#define FVF_VERTEX_TEST	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE)

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type);
void SetParticleImgui(Particle& inParticle);
void LoadTex(void);
void DeleteParticle(int nDelete);
void DeleteParticleAll();
void RemoveAngle(void);
Particle *GetParticle(void);
DWORD DwordtoFloat(float f);
#endif