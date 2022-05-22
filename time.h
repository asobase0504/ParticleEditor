//＝＝＝＝＝＝＝＝＝＝＝＝＝
//time.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _TIME_H_
#define _TIME_H_

#include"main.h"

#define	MAX_TIME	(3)

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
int GetTime(void);

#endif
