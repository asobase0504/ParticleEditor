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

	CParticleManager* manager = CApplication::GetInstance()->GetParticleManager();

	m_emitterEditing = manager->Create(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	m_defaultData = manager->GetBundledData()[0];
	m_editData = manager->GetBundledData()[0];

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
		//m_imgui->Uninit();

		delete m_imgui;
		m_imgui = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CParticleEdit::Update()
{
	CParticleManager* manager = CApplication::GetInstance()->GetParticleManager();
	bool existsChange = m_imgui->Update();	// imguiの更新

	// 追加
	if (GetKeyboardTrigger(DIK_J))
	{
		m_editDataIndex = manager->SetBundledData(m_defaultData);

		D3DXVECTOR3 posCreate = D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f);
		m_emitterEditing = manager->Create(posCreate, m_editDataIndex);
	}
	// 移動
	if (GetKeyboardTrigger(DIK_K))
	{
		DoNextEditingEmitter(1);
	}
	// 削除
	if (GetKeyboardTrigger(DIK_U))
	{
		m_emitterEditing->SetNeedsDelete(true);
		DoNextEditingEmitter(1);
	}

	// 編集中のエミッタ―に編集したデータを代入する。
	if (existsChange)
	{
		m_emitterEditing->SetEmitter(m_editData.emitterData);
		m_emitterEditing->SetParticle(m_editData.particleData);
		manager->ChangeBundledData(m_emitterEditing->GetBundledIndex(), m_editData);
	}
}

//-----------------------------------------------------------------------------
// 編集するエミッタ―を進める
//-----------------------------------------------------------------------------
void CParticleEdit::DoNextEditingEmitter(int inIndex)
{
	std::list<CParticleEmitter*> emitterList = CApplication::GetInstance()->GetParticleManager()->GetEmitter();
	for (auto it = emitterList.begin(); it != emitterList.end(); it++)
	{
		if (*it != m_emitterEditing)
		{
			continue;
		}

		/* ↓listに編集中のエミッタ―と同じデータがあった場合↓ */

		if (it == emitterList.begin() && inIndex < 0)
		{ // listの最初の要素が同じデータだった場合
			it = emitterList.end();
		}

		// 指定した数、イテレータをずらす
		std::advance(it, inIndex);

		if (it == emitterList.end() && inIndex > 0)
		{ // ずらした場所がendだった場合
			it = emitterList.begin();
		}

		// 編集するイテレータを更新する
		m_emitterEditing = *it;
		m_editData = CApplication::GetInstance()->GetParticleManager()->GetBundledData()[m_emitterEditing->GetBundledIndex()];

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
	CParticle::Info& particleInfo = m_editData.particleData;
	CParticleEmitter::Info& emitterInfo = m_editData.emitterData;

	saveEffect["ver.01"];
	saveEffect["POSMAX"] = Vector3ToVectorFloat(emitterInfo.maxPopPos);
	saveEffect["POP"] = emitterInfo.popNumber;
	saveEffect["POSMIN"] = Vector3ToVectorFloat(emitterInfo.minPopPos);
	saveEffect["ANGLE"] = emitterInfo.fAngle;
	saveEffect["ADD_ANGLE"] = emitterInfo.fAddAngle;
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

	saveEffect["RADIUS"] = particleInfo.fRadius;
	saveEffect["ATTENUATION"] = particleInfo.fAttenuation;
	saveEffect["WEIGHT"] = particleInfo.fWeight;
	saveEffect["WEIGHT_TRANSITION"] = particleInfo.fWeightTransition;
	saveEffect["LIFE"] = particleInfo.nLife;
	saveEffect["BACKROT"] = particleInfo.bBackrot;

	SaveJson(saveEffect, "data/FILE/DataEffectOutput2.json");
}
