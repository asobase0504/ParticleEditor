//**************************************************
// 
// particle_imgui.h
// Author  : katsuki mizuki
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _PARTICLE_IMGUI_H_	// ���̃}�N������`����ĂȂ�������
#define _PARTICLE_IMGUI_H_	// 2�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include "imgui_property.h"

//--------------------------------------------------
// �p�[�e�B�N���p��Imgui�\���N���X
//--------------------------------------------------
class CParticleImgui : public CImguiProperty
{
private:	// ��`

public:
	CParticleImgui();
	~CParticleImgui();

	HWND Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice) override;
	void Uninit(HWND hWnd, WNDCLASSEX wcex) override;
	bool Update() override;
	void Draw() override;
	bool bSetImguiParticle();
	void ParticleTemplate();

private:
	bool ParticleProperty();
private:
	bool s_bEffectEnable = true;
	const unsigned int gpu_id = 0;
	ImVec2 foo[10];
};

#endif // !_IMGUI_PROPERTY_H_
