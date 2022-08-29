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
#include <vector>

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
	//D3DXVECTOR3をvector<float>に変更する
	auto Vector3ToVectorFloat = [](D3DXVECTOR3 inVector)->std::vector<float>
	{
		return{ inVector.x, inVector.y, inVector.z };
	};

	nlohmann::json saveEffect;//リストの生成
	CParticle::Info& particleInfo = *m_particleEditing->GetParticle();
	CParticleEmitter::Info& emitterInfo = *m_particleEditing->GetEmitterInfo();

	saveEffect["ver.01"];
	saveEffect["POSMAX"] = Vector3ToVectorFloat(emitterInfo.maxPopPos);
	saveEffect["POSMIN"] = Vector3ToVectorFloat(emitterInfo.minPopPos);
	saveEffect["MOVE"] = Vector3ToVectorFloat(particleInfo.move);
	saveEffect["MOVE_TRANSITION"] = Vector3ToVectorFloat(particleInfo.moveTransition);
	saveEffect["ROT"] = Vector3ToVectorFloat(particleInfo.rot);

	{ // 色の設定

		//D3DXCOLORをvector<float>に変更する
		auto ColorToVectorFloat = [](D3DXCOLOR inColor)->std::vector<float>
		{
			return{ inColor.r, inColor.g, inColor.b, inColor.a };
		};

		CParticle::Color saveColor = particleInfo.color;
		D3DXCOLOR transition = saveColor.colTransition;
		D3DXCOLOR dest = saveColor.destCol;

		saveEffect["COLOR"]["BIGIN"] = ColorToVectorFloat(saveColor.colBigin);
		saveEffect["COLOR"]["DEST"] = ColorToVectorFloat(saveColor.destCol);
		saveEffect["COLOR"]["ENDTIME"] = saveColor.nEndTime;

		// カラーにランダム性を持たせる場合
		if (particleInfo.color.bColRandom)
		{
			saveEffect["COLOR"]["RANDAM"]["MAX"] = ColorToVectorFloat(saveColor.colRandamMax);
			saveEffect["COLOR"]["RANDAM"]["MIN"] = ColorToVectorFloat(saveColor.colRandamMin);
		}
		if (particleInfo.color.bColTransition)
		{
			saveEffect["COLOR"]["TRANSITION"]["VALUE"] = ColorToVectorFloat(saveColor.colTransition);
			saveEffect["COLOR"]["TRANSITION"]["TIME"] = saveColor.nCntTransitionTime;
		}
		if (saveColor.bRandomTransitionTime)
		{
			saveEffect["COLOR"]["RANDOM_TRANSITION_TIME"] = saveColor.bRandomTransitionTime;
		}
	}

	{ // 大きさの設定
		saveEffect["SCALE"]["BASIC_VALUE"] = particleInfo.fScale;
		saveEffect["SCALE"]["TRANSITION"] = Vector3ToVectorFloat(particleInfo.scaleTransition);
		saveEffect["SCALE"]["PLUS_WIDTH"] = particleInfo.fWidth;
		saveEffect["SCALE"]["PLUS_HEIGHT"] = particleInfo.fHeight;
	}

	saveEffect["TYPE"] = particleInfo.type;
	saveEffect["RADIUS"] = particleInfo.fRadius;
	saveEffect["ANGLE"] = particleInfo.fAngle;
	saveEffect["ATTENUATION"] = particleInfo.fAttenuation;
	saveEffect["WEIGHT"] = particleInfo.fWeight;
	saveEffect["WEIGHT_TRANSITION"] = particleInfo.fWeightTransition;
	saveEffect["LIFE"] = particleInfo.nLife;
	saveEffect["BACKROT"] = particleInfo.bBackrot;

	SaveJson(saveEffect, "data/FILE/DataEffectOutput2.json");
}
