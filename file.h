//============================
//
// �t�@�C���ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_			// ���̃}�N����`������ĂȂ�������
#define _FILE_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//=====================================
// �C���N���[�h
//=====================================
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "nlohmann/json.hpp"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <sstream>

#include "main.h"

//�\����
struct EffectData
{
	//
	D3DXVECTOR3 nPopPos;
	D3DXVECTOR3 ROT;
	D3DXVECTOR2 DIVISION;
	D3DXCOLOR COL;
	int Effecttype;
	int Alphablend_TYPE;
	int	nLife;					// ����
	int TEXTURE;
	int TEXTURE2;
	int TIME;
	int NUMPARTICLE;
	int MULTITEX;
	float   Radius;
	float   Speed;
	float   Weight;
	float	fRadius;				// ���a
	float	fSpeed;					// �ړ����x
	float	fWeight;				// �d��
	float	fAttenuationWeight;		// �d���̌�����
	bool	bMultiTex;				// �}���`�e�N�X�`���̎g�p��
	bool	bUse;					// �g�p��
};

//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�

EffectData *GetStatus(void);//�Q�b�g
void OutputStatus();
void LoodJson(const wchar_t* cUrl);

#endif
