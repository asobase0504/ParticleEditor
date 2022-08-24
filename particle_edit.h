//=========================================
// 
// �p�[�e�B�N���G�f�B�^
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
// �O���錾
//=========================================
class CParticleEmitter;
class CParticleImgui;

//=========================================
// �p�[�e�B�N���G�f�B�^
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
	std::list<CParticleEmitter*> m_particleEmitter;	// �G�~�b�^�\���
	CParticleImgui* m_imgui;		// imgui
};

#endif // !_PARTICLE_FACTORY_H_
