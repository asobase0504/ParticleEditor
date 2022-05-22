#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//�}�N����`
#define MAX_BULLET	(512)	//�e�̍ő吔

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�e�̐F
	float fBulletSize;	//�e�̑傫��
	int nLife;			//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
}BULLET;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);
bool CollisionBullet(D3DXVECTOR3 *ppos, float fRadius);
BULLET *GetBullet(void);

#endif

