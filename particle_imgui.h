//**************************************************
// 
// particle_imgui.h
// Author  : katsuki mizuki
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _PARTICLE_IMGUI_H_	// このマクロが定義されてなかったら
#define _PARTICLE_IMGUI_H_	// 2重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "imgui_property.h"

//--------------------------------------------------
// パーティクル用のImgui表示クラス
//--------------------------------------------------
class CParticleImgui : public CImguiProperty
{
private:	// 定義

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
