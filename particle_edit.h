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
#include "particle.h"
#include "particle_emitter.h"

//=========================================
// 前方宣言
//=========================================
class CParticleEmitter;

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
	CImguiProperty* GetImguiProperty() { return m_imguiProperty; }
private:
	CParticleEmitter* m_particleEditing;
	std::list<CParticleEmitter*> m_particleEmitter;	// エミッタ―情報
	CImguiProperty* m_imguiProperty;		// imgui
};

#endif // !_PARTICLE_FACTORY_H_
