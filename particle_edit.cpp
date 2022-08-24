//=============================================================================
// 
// パーティクルエディタ
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include "renderer.h"
#include "particle_manager.h"
#include "particle_edit.h"
#include "particle_imgui.h"
#include "input.h"
#include "file.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CParticleEdit::CParticleEdit()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CParticleEdit::~CParticleEdit()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CParticleEdit::Init(HWND hWnd)
{

	m_imgui = new CParticleImgui;
	m_imgui->Init(hWnd, CApplication::GetInstance()->GetRenderer()->GetDevice());

	m_particleEditing = CApplication::GetInstance()->GetParticleManager()->Create(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	m_defaultData = CApplication::GetInstance()->GetParticleManager()->GetBundledData()[0];
	m_particleEmitter.push_back(m_particleEditing);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CParticleEdit::Uninit()
{
	// imguiの解放
	if (m_imgui != nullptr)
	{
		//imguiProperty->Uninit();

		delete m_imgui;
		m_imgui = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CParticleEdit::Update()
{
	m_imgui->Update();	// imguiの更新

	if (GetKeyboardTrigger(DIK_J))
	{
		CApplication::GetInstance()->GetParticleManager()->SetBundledData(m_defaultData);
		m_particleEditing = CApplication::GetInstance()->GetParticleManager()->Create(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f), 0);
		m_particleEmitter.push_back(m_particleEditing);
	}
	if (GetKeyboardTrigger(DIK_K))
	{
		DoNextEditingEmitter(1);
	}
	if (GetKeyboardTrigger(DIK_U))
	{
		m_particleEditing->SetNeedsDelete(true);
		DoNextEditingEmitter(1);
	}
}

//-----------------------------------------------------------------------------
// 編集するエミッタ―を進める
//-----------------------------------------------------------------------------
void CParticleEdit::DoNextEditingEmitter(int inIndex)
{
	for (auto it = m_particleEmitter.begin(); it != m_particleEmitter.end(); it++)
	{
		if (*it != m_particleEditing)
		{
			continue;
		}

		/* ↓listに編集中のエミッタ―と同じデータがあった場合↓ */

		if (it == m_particleEmitter.begin() && inIndex < 0)
		{ // listの最初の要素が同じデータだった場合
			it = m_particleEmitter.end();
		}

		// 指定した数、イテレータをずらす
		std::advance(it, inIndex);

		if (it == m_particleEmitter.end() && inIndex > 0)
		{ // ずらした場所がendだった場合
			it = m_particleEmitter.begin();
		}

		// 編集するイテレータを更新する
		m_particleEditing = *it;

		return;
	}
}

//-----------------------------------------------------------------------------
// 編集してるエミッタ―をセーブする
//-----------------------------------------------------------------------------
void CParticleEdit::SaveEffect()
{
	nlohmann::json saveEffect;//リストの生成

	CParticleEmitter* loadData = m_particleEditing;

	CParticle::Info& particleInfo = *loadData->GetParticle();
	CParticleEmitter::Info& emitterInfo = *loadData->GetEmitterInfo();

	saveEffect["POSMAX"] = { { "X", emitterInfo.maxPopPos.x } ,{ "Y", emitterInfo.maxPopPos.y } ,{ "Z", emitterInfo.maxPopPos.z } };
	saveEffect["POSMIN"] = { { "X", emitterInfo.minPopPos.x } ,{ "Y", emitterInfo.minPopPos.y } ,{ "Z", emitterInfo.minPopPos.z } };
	saveEffect["MOVE"] = { { "X", particleInfo.move.x } ,{ "Y", particleInfo.move.y } ,{ "Z", particleInfo.move.z } };
	saveEffect["MOVETRANSITION"] = { { "X", particleInfo.moveTransition.x } ,{ "Y", particleInfo.moveTransition.y } ,{ "Z", particleInfo.moveTransition.z } };
	saveEffect["ROT"] = { { "X", particleInfo.rot.x } ,{ "Y", particleInfo.rot.y },{ "Z", particleInfo.rot.z } };

	saveEffect["COL"] = { { "R", particleInfo.color.colBigin.r },{ "G" ,particleInfo.color.colBigin.g } ,{ "B", particleInfo.color.colBigin.b } ,{ "A", particleInfo.color.colBigin.a } };
	saveEffect["COLRANDAMMAX"] = { { "R", particleInfo.color.colRandamMax.r },{ "G" ,particleInfo.color.colRandamMax.g } ,{ "B", particleInfo.color.colRandamMax.b } ,{ "A", particleInfo.color.colRandamMax.a } };
	saveEffect["COLRANDAMMIN"] = { { "R", particleInfo.color.colRandamMin.r },{ "G" ,particleInfo.color.colRandamMin.g } ,{ "B", particleInfo.color.colRandamMin.b } ,{ "A", particleInfo.color.colRandamMin.a } };
	saveEffect["COLTRANSITION"] = { { "R", particleInfo.color.colTransition.r },{ "G" ,particleInfo.color.colTransition.g } ,{ "B", particleInfo.color.colTransition.b } ,{ "A", particleInfo.color.colTransition.a } };
	saveEffect["DESTCOL"] = { { "R", particleInfo.color.destCol.r },{ "G" ,particleInfo.color.destCol.g } ,{ "B", particleInfo.color.destCol.b } ,{ "A", particleInfo.color.destCol.a } };
	saveEffect["ENDTIME"] = particleInfo.color.nEndTime;
	saveEffect["CNTTRANSITIONTIME"] = particleInfo.color.nCntTransitionTime;
	saveEffect["BCOLTRANSITION"] = particleInfo.color.bColTransition;
	saveEffect["COLRANDOM"] = particleInfo.color.bColRandom;
	saveEffect["RANDOMTRANSITIONTIME"] = particleInfo.color.bRandomTransitionTime;

	saveEffect["TYPE"] = particleInfo.type;
	saveEffect["SCALE_TRANSITION"] = { { "X", particleInfo.scaleTransition.x } ,{ "Y", particleInfo.scaleTransition.y } ,{ "Z", particleInfo.scaleTransition.y } };
	saveEffect["WIDTH"] = particleInfo.fWidth;
	saveEffect["HEIGHT"] = particleInfo.fHeight;
	saveEffect["ANGLE"] = particleInfo.fAngle;
	saveEffect["ATTENUATION"] = particleInfo.fAttenuation;
	saveEffect["RADIUS"] = particleInfo.fRadius;
	saveEffect["WEIGHT"] = particleInfo.fWeight;
	saveEffect["WEIGHTTRANSITION"] = particleInfo.fWeightTransition;
	saveEffect["LIFE"] = particleInfo.nLife;
	saveEffect["BACKROT"] = particleInfo.bBackrot;
	saveEffect["SCALE"] = particleInfo.fScale;

	SaveJson(saveEffect, "data/FILE/DataEffectOutput.json");
}
