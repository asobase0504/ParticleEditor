#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"

//�}�N����`
#define MAX_ENEMYBULLET		(512)	//�e�̍ő吔

//�e�\���̂̒�`
struct ENEMYBULLET
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotDest;	//�ړI�̌���
	D3DXVECTOR3 vec;
	D3DXCOLOR col;			//�e�̐F
	float fBulletSize;		//�e�̑傫��
	float fAngle;			//�p�x
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
};

//�v���g�^�C�v�錾
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);
bool CollisionEnemyBullet(D3DXVECTOR3 *ppos, float fRadius);
ENEMYBULLET *GetEnemyBullet(void);

#endif

