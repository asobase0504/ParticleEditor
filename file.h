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
#include <d3dx9.h>
#include "particle.h"

//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�

CParticle::Particle GetStatus(void);//�Q�b�g
void OutputStatus();
void LoadJson(const wchar_t* cUrl);
void LoadJsonTex(const char* cUrl);
void OutputStatusTex();
void SetTex();
#endif
