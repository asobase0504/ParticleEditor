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
#include "particle_manager.h"
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

	void DoNextEditingEmitter(int inIndex);

	// �O��
	void SaveEffect();

	// Getter
	CParticleEmitter* GetEmitter() { return m_emitterEditing; }
	CParticleManager::BundledData* GetBundledData() { return &m_editData; }
	CParticleImgui* GetImguiProperty() { return m_imgui; }

private:
	CParticleManager::BundledData m_editData;		// �ҏW�f�[�^
	CParticleManager::BundledData m_defaultData;	// ��{�l
	int m_editDataIndex;							// �ҏW�����̂̔ԍ�
	CParticleEmitter* m_emitterEditing;				// �ҏW�G�~�b�^�\
	CParticleImgui* m_imgui;						// imgui
};

#endif // !_PARTICLE_FACTORY_H_
