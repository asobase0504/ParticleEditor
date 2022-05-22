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
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�F
	PARTICLETYPE type;	//�G�t�F�N�g�̎��
	float fWidth;		//��
	float fHeight;		//����
	float fAngle;		//�p�x
	float fRadius;		//���a
	int nLife;			//����
	bool bUse;			//�g�p���Ă��邩
};

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type);
void DeleteParticle(int nDelete);
void RemoveAngle(void);
Particle *GetParticle(void);

#endif