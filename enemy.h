#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//マクロ定義
#define MAX_ENEMY	(256)						//敵の最大数
#define NUM_ENEMY	(1)							//敵の最大数
#define ENEMYFILE	("data\\FILE\\enemy00.txt")	//ファイル名

//敵の状態
enum ENEMYSTATE
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
};

//敵の種類
enum ENEMY_TYPE
{
	ENEMYTYPE_NORMAL = 0,
	ENEMYTYPE_MAX
};

//敵構造体の定義
struct ENEMY
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 posSave;	//位置の情報の保存
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDest;	//目的の向き
	D3DXVECTOR3 vec;		//距離
	D3DXCOLOR col;			//色
	ENEMYSTATE state;		//敵の状態
	ENEMY_TYPE type;		//敵の種類
	int nHp;				//体力
	int nNow;				//今の時間
	int nTimeSave;			//時間保存
	float fWidth;			//幅
	float fHeight;			//高さ
	float fAngle;			//角度
	float fAttenuation;		//減衰
	int nCounterState;		//状態管理カウンター
	char aStrings[128];		//文字列
	bool bUse;				//使用しているかどうか
};

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos,ENEMY_TYPE EnemyType, int nNum, int HP);
void LoadEnemy(void);
ENEMY *GetEnemy(void);

#endif