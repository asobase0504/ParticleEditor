#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//�}�N����`
#define MAX_ENEMY	(256)						//�G�̍ő吔
#define NUM_ENEMY	(1)							//�G�̍ő吔
#define ENEMYFILE	("data\\FILE\\enemy00.txt")	//�t�@�C����

//�G�̏��
enum ENEMYSTATE
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
};

//�G�̎��
enum ENEMY_TYPE
{
	ENEMYTYPE_NORMAL = 0,
	ENEMYTYPE_MAX
};

//�G�\���̂̒�`
struct ENEMY
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 posSave;	//�ʒu�̏��̕ۑ�
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotDest;	//�ړI�̌���
	D3DXVECTOR3 vec;		//����
	D3DXCOLOR col;			//�F
	ENEMYSTATE state;		//�G�̏��
	ENEMY_TYPE type;		//�G�̎��
	int nHp;				//�̗�
	int nNow;				//���̎���
	int nTimeSave;			//���ԕۑ�
	float fWidth;			//��
	float fHeight;			//����
	float fAngle;			//�p�x
	float fAttenuation;		//����
	int nCounterState;		//��ԊǗ��J�E���^�[
	char aStrings[128];		//������
	bool bUse;				//�g�p���Ă��邩�ǂ���
};

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos,ENEMY_TYPE EnemyType, int nNum, int HP);
void LoadEnemy(void);
ENEMY *GetEnemy(void);

#endif