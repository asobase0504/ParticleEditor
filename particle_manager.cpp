//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "particle_manager.h"
#include "file.h"

#include <assert.h>

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CParticleManager::CParticleManager() :
	m_bundledData({}),
	m_particleEmitter({})
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CParticleManager::~CParticleManager()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CParticleManager::Init()
{
	LoadBundledData(L"data/FILE/DataEffectOutput2.json");
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CParticleManager::Uninit()
{
	for (CParticleEmitter* emitter : m_particleEmitter)
	{
		if (emitter != nullptr)
		{
			emitter->Uninit();
			delete emitter;
			emitter = nullptr;
		}
	}
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CParticleManager::Update()
{
	for (CParticleEmitter* i : m_particleEmitter)
	{
		if (i == nullptr)
		{
			continue;
		}

		i->Update();
	}
	ReleaseEmitter();
}

//-----------------------------------------
// 生成
//-----------------------------------------
CParticleEmitter* CParticleManager::Create(const D3DXVECTOR3& pos, const int& index)
{
	CParticleEmitter* emitter = new CParticleEmitter();

	emitter->Init();		// 初期化
	emitter->SetPos(pos);	// 位置の更新

	// 指定された番号内にデータが入っているか否か
	if (0 > index || m_bundledData.size() <= index)
	{
		assert(false);
		return 0;
	}

	emitter->SetParticle(m_bundledData.at(index).particleData);	// 指定されてたパーティクルデータの挿入
	emitter->SetEmitter(m_bundledData.at(index).emitterData);	// 指定されてたエミッタ―データの挿入

	m_particleEmitter.push_back(emitter);

	return emitter;
}

//-----------------------------------------
// 指定したエミッタ―を削除する
//-----------------------------------------
void CParticleManager::ReleaseEmitter()
{
	m_particleEmitter.remove_if([](CParticleEmitter* inEmitter)
	{
		bool needsDelete = inEmitter->GetNeedsDelete();

		if (needsDelete)
		{
			inEmitter->Uninit();
			delete inEmitter;
			inEmitter = nullptr;
		}

		return needsDelete;
	});
}

//-----------------------------------------
// 設定
//-----------------------------------------
void CParticleManager::SetBundledData(const BundledData& inData)
{
	m_bundledData.push_back(inData);
}

//-----------------------------------------
// チェンジ機能
//-----------------------------------------
void CParticleManager::ChangeBundledData(const int idx, const BundledData& inData)
{
	m_bundledData.at(idx) = inData;
}

//-----------------------------------------
// データのロード
//-----------------------------------------
void CParticleManager::LoadBundledData(const wchar_t* cUrl)
{
	nlohmann::json inData = LoadJson(cUrl);

	CParticleManager::BundledData loadData = {};
	CParticle::Info& particleInfo = loadData.particleData;
	CParticleEmitter::Info& emitterInfo = loadData.emitterData;

	//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
	//StringToWString(UTF8toSjis(j["name"]));
	//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));

	if (inData.count("ver.01") != 0)
	{ // 新しいバージョン

	  // ["COLOR"]["TRANSITION"]内の情報テーブル
		DeepTable color_transition = {
			{
				{ Type::Int,	"TIME" , &particleInfo.color.nCntTransitionTime },
				{ Type::Color,	"VALUE" , &particleInfo.color.colTransition },
			}
			,&particleInfo.color.bColTransition
		};

		// ["COLOR"]["RANDUM"]内の情報テーブル
		DeepTable color_randum = {
			{
				{ Type::Color,	"MAX" , &particleInfo.color.colRandamMax },
				{ Type::Color,	"MIN" , &particleInfo.color.colRandamMin },
			}
			,&particleInfo.color.bColRandom
		};

		// ["COLOR"]内の情報テーブル
		DeepTable color = {
			{
				{ Type::Color, "BIGIN" , &particleInfo.color.colBigin },
				{ Type::Color, "DEST" , &particleInfo.color.destCol },
				{ Type::Int, "ENDTIME" , &particleInfo.move },
				{ Type::Bool, "RANDOM_TRANSITION_TIME" , &particleInfo.color.bRandomTransitionTime },
				{ Type::UseChack, "RANDAM" , &color_randum },
				{ Type::UseChack, "TRANSITION" , &color_transition },
			}
			,&particleInfo.hasColor
		};

		// ["SCALE"]内の情報テーブル
		DeepTable scale = {
			{
			{ Type::Float, "BASIC_VALUE",&particleInfo.fScale },
			{ Type::Float, "PLUS_HEIGHT",&particleInfo.fHeight },
			{ Type::Float, "PLUS_WIDTH",&particleInfo.fWidth },
			{ Type::Vector3, "TRANSITION",&particleInfo.scaleTransition },
			},nullptr
		};

		// 読み込むjsonファイル全体の情報テーブル
		DeepTable table = {
			{
				{ Type::Vector3, "POSMAX" , &emitterInfo.maxPopPos },
				{ Type::Vector3, "POSMIN" , &emitterInfo.minPopPos },
				{ Type::Vector3, "MOVE" , &particleInfo.move },
				{ Type::Vector3, "MOVE_TRANSITION" , &particleInfo.moveTransition },
				{ Type::Vector3, "ROT" , &particleInfo.rot },
				{ Type::UseChack, "COLOR" , &color },
				{ Type::Float, "ANGLE" , &particleInfo.fAngle },
				{ Type::Float, "ATTENUATION" , &particleInfo.fAttenuation },
				{ Type::Bool, "BACKROT" , &particleInfo.bBackrot },
				{ Type::Int, "LIFE" , &particleInfo.nLife },
				{ Type::Float, "RADIUS" , &particleInfo.fRadius },
				{ Type::UseChack, "SCALE" , &scale},
				{ Type::Int, "TYPE" , &particleInfo.type },
				{ Type::Float, "WEIGHT" , &particleInfo.fWeight },
				{ Type::Float, "WEIGHTTRANSITION" , &particleInfo.fWeightTransition },
			},nullptr
		};

		LoadValueBundledData(&inData,table);
	}
	else
	{ // 旧バージョン
		emitterInfo.maxPopPos = D3DXVECTOR3(inData["POSMAX"]["X"], inData["POSMAX"]["Y"], inData["POSMAX"]["Z"]);
		emitterInfo.minPopPos = D3DXVECTOR3(inData["POSMIN"]["X"], inData["POSMIN"]["Y"], inData["POSMIN"]["Z"]);

		particleInfo.move = D3DXVECTOR3(inData["MOVE"]["X"], inData["MOVE"]["Y"], inData["MOVE"]["Z"]);
		particleInfo.rot = D3DXVECTOR3(inData["ROT"]["X"], inData["ROT"]["Y"], inData["ROT"]["Z"]);
		particleInfo.moveTransition = D3DXVECTOR3(inData["MOVETRANSITION"]["X"], inData["MOVETRANSITION"]["Y"], inData["MOVETRANSITION"]["Z"]);;

		particleInfo.color.colBigin = D3DXCOLOR(inData["COL"]["R"], inData["COL"]["G"], inData["COL"]["B"], inData["COL"]["A"]);
		particleInfo.color.colRandamMax = D3DXCOLOR(inData["COLRANDAMMAX"]["R"], inData["COLRANDAMMAX"]["G"], inData["COLRANDAMMAX"]["B"], inData["COLRANDAMMAX"]["A"]);
		particleInfo.color.colRandamMin = D3DXCOLOR(inData["COLRANDAMMIN"]["R"], inData["COLRANDAMMIN"]["G"], inData["COLRANDAMMIN"]["B"], inData["COLRANDAMMIN"]["A"]);
		particleInfo.color.colTransition = D3DXCOLOR(inData["COLTRANSITION"]["R"], inData["COLTRANSITION"]["G"], inData["COLTRANSITION"]["B"], inData["COLTRANSITION"]["A"]);
		particleInfo.color.destCol = D3DXCOLOR(inData["DESTCOL"]["R"], inData["DESTCOL"]["G"], inData["DESTCOL"]["B"], inData["DESTCOL"]["A"]);
		particleInfo.color.nEndTime = inData["ENDTIME"];
		particleInfo.color.nCntTransitionTime = inData["CNTTRANSITIONTIME"];
		particleInfo.color.bColTransition = inData["BCOLTRANSITION"];
		particleInfo.color.bColRandom = inData["COLRANDOM"];
		particleInfo.color.bRandomTransitionTime = inData["RANDOMTRANSITIONTIME"];

		particleInfo.type = inData["TYPE"];
		particleInfo.scaleTransition = D3DXVECTOR3(inData["SCALE_TRANSITION"]["X"], inData["SCALE_TRANSITION"]["Y"], inData["SCALE_TRANSITION"]["Z"]);
		particleInfo.fWidth = inData["WIDTH"];
		particleInfo.fHeight = inData["HEIGHT"];
		particleInfo.fRadius = inData["RADIUS"];
		particleInfo.fAngle = inData["ANGLE"];
		particleInfo.fWeight = inData["WEIGHT"];
		particleInfo.nLife = inData["LIFE"];
		particleInfo.fAttenuation = inData["ATTENUATION"];
		particleInfo.fWeightTransition = inData["WEIGHTTRANSITION"];
		particleInfo.nLife = inData["LIFE"];
		particleInfo.bBackrot = inData["BACKROT"];
		particleInfo.fScale = inData["SCALE"];
	}

	SetBundledData(loadData);
}
