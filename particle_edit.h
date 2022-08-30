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
#include "particle_manager.h"
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

	void DoNextEditingEmitter(int inIndex);

	// 外部
	void SaveEffect();

	// Getter
	CParticleEmitter* GetEmitter() { return m_emitterEditing; }
	CParticleManager::BundledData* GetBundledData() { return &m_editData; }
	CParticleImgui* GetImguiProperty() { return m_imgui; }

private:
	CParticleManager::BundledData m_editData;		// 編集データ
	CParticleManager::BundledData m_defaultData;	// 基本値
	int m_editDataIndex;							// 編集中情報体の番号
	CParticleEmitter* m_emitterEditing;				// 編集エミッタ―
	CParticleImgui* m_imgui;						// imgui
};

#endif // !_PARTICLE_FACTORY_H_
