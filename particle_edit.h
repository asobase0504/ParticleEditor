//=========================================
// 
// パーティクルエディタ
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_EDIT_H_
#define _PARTICLE_EDIT_H_

//=========================================
// include
//=========================================
#include <d3dx9.h>
#include <list>
#include <vector>
#include "particle_emitter.h"

//=========================================
// 前方宣言
//=========================================
class CParticleEmitter;
class CParticleImgui;

//=========================================
// パーティクルエディタ
//=========================================
class CParticleEdit
{
public:
	CParticleEdit();
	~CParticleEdit();

	HRESULT Init(HWND hWnd);
	void Uninit();
	void Update();

	CParticleEmitter* GetEmitter() { return m_particleEditing; }
	CParticleImgui* GetImguiProperty() { return m_imgui; }
private:
	CParticleEmitter* m_particleEditing;
	std::list<CParticleEmitter*> m_particleEmitter;	// エミッタ―情報
	CParticleImgui* m_imgui;		// imgui
};

#endif // !_PARTICLE_FACTORY_H_
