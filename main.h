//����������������������������������������
//Main.h
//����������������������������������������

#ifndef _MAIN_H_		//��d�K�[�h�C���N���[�h
#define _MAIN_H_

#include <d3dx9.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir);
bool *TexUse(void);
char *GetBuffer(void);
HWND GetWnd(void);
void funcFileSave(HWND hWnd);
#endif
