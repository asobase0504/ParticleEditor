//����������������������������������������
//Main.h
//����������������������������������������

#ifndef _MAIN_H_		//��d�K�[�h�C���N���[�h
#define _MAIN_H_

#include <d3dx9.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir);
float GetAngle(void);
bool *TexUse(void);
char *GetBuffer(void);
HWND GetWnd(void);
void funcFileSave(HWND hWnd);
#endif
