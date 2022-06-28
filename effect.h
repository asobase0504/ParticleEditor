//����������������������������������������
//effect.h
//����������������������������������������
#ifndef _EFFECT_H_		//���̃}�N������`����ĂȂ�������
#define _EFFECT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//�}�N����`
#define MAX_EFFECT	(4096)	//�G�t�F�N�g�̍ő吔
#define NUM_EFFECT	(10)	//�G�t�F�N�g�̎��

//�G�t�F�N�g�̗񋓌^
enum EFFECTTYPE
{
	EFFECTTYPE_NONE = 0,
	EFFECTTYPE_NORMAL,
	EFFECTTYPE_PLAYER,
	EFFECTTYPE_MAX
};

//�G�t�F�N�g�̍\���̂̒�`
struct Effect
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;				// �F
	EFFECTTYPE type;			// �G�t�F�N�g�̎��
	float fWidth;				// ��
	float fHeight;				// ����
	float fRadius;				// ���a
	int nLife;					// ����
	bool bUse;					// �g�p���Ă��邩
};

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, EFFECTTYPE type);
void DeleteEffect(int nDelete);

#endif