//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include <fstream>
#include <assert.h>

#include "application.h"
#include "texture.h"
#include "particle_manager.h"
#include "particle_edit.h"

nlohmann::json j;//リストの生成

nlohmann::json TEX;//リストの生成

std::string Lync[256];
int index = 0;

//============================
// 書込み
//============================
void SaveJson(nlohmann::json inJson, const std::string cUrl)
{
	auto jobj = inJson.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = cUrl;
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

//============================
// 読込み
//============================
void LoadJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (!ifs)
	{
		return;
	}
	CParticleManager::BundledData loadData = {};
	CParticle::Info& particleInfo = loadData.particleData;
	CParticleEmitter::Info& emitterInfo = loadData.emitterData;

	//StringToWString(UTF8toSjis(j["name"]));
	//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
	ifs >> j;

	if (j.count("POSMIN") == 0)
	{
		assert(true);
	}
	else
	{
		assert(true);
	}

	//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
	emitterInfo.maxPopPos = D3DXVECTOR3(j["POSMAX"]["X"], j["POSMAX"]["Y"], j["POSMAX"]["Z"]);
	emitterInfo.minPopPos = D3DXVECTOR3(j["POSMIN"]["X"], j["POSMIN"]["Y"], j["POSMIN"]["Z"]);

	particleInfo.move = D3DXVECTOR3(j["MOVE"]["X"], j["MOVE"]["Y"], j["MOVE"]["Z"]);
	particleInfo.rot = D3DXVECTOR3(j["ROT"]["X"], j["ROT"]["Y"], j["ROT"]["Z"]);
	particleInfo.moveTransition = D3DXVECTOR3(j["MOVETRANSITION"]["X"], j["MOVETRANSITION"]["Y"], j["MOVETRANSITION"]["Z"]);;

	particleInfo.color.colBigin = D3DXCOLOR(j["COL"]["R"], j["COL"]["G"], j["COL"]["B"], j["COL"]["A"]);
	particleInfo.color.colRandamMax = D3DXCOLOR(j["COLRANDAMMAX"]["R"], j["COLRANDAMMAX"]["G"], j["COLRANDAMMAX"]["B"], j["COLRANDAMMAX"]["A"]);
	particleInfo.color.colRandamMin = D3DXCOLOR(j["COLRANDAMMIN"]["R"], j["COLRANDAMMIN"]["G"], j["COLRANDAMMIN"]["B"], j["COLRANDAMMIN"]["A"]);
	particleInfo.color.colTransition = D3DXCOLOR(j["COLTRANSITION"]["R"], j["COLTRANSITION"]["G"], j["COLTRANSITION"]["B"], j["COLTRANSITION"]["A"]);
	particleInfo.color.destCol = D3DXCOLOR(j["DESTCOL"]["R"], j["DESTCOL"]["G"], j["DESTCOL"]["B"], j["DESTCOL"]["A"]);
	particleInfo.color.nEndTime = j["ENDTIME"];
	particleInfo.color.nCntTransitionTime = j["CNTTRANSITIONTIME"];
	particleInfo.color.bColTransition = j["BCOLTRANSITION"];
	particleInfo.color.bColRandom = j["COLRANDOM"];
	particleInfo.color.bRandomTransitionTime = j["RANDOMTRANSITIONTIME"];

	particleInfo.type = j["TYPE"];
	particleInfo.scaleTransition = D3DXVECTOR3(j["SCALE_TRANSITION"]["X"], j["SCALE_TRANSITION"]["Y"], j["SCALE_TRANSITION"]["Z"]);
	particleInfo.fWidth = j["WIDTH"];
	particleInfo.fHeight = j["HEIGHT"];
	particleInfo.fRadius = j["RADIUS"];
	particleInfo.fAngle = j["ANGLE"];
	particleInfo.fWeight = j["WEIGHT"];
	particleInfo.nLife = j["LIFE"];
	particleInfo.fAttenuation = j["ATTENUATION"];
	particleInfo.fWeightTransition = j["WEIGHTTRANSITION"];
	particleInfo.nLife = j["LIFE"];
	particleInfo.bBackrot = j["BACKROT"];
	particleInfo.fScale = j["SCALE"];

	CApplication::GetInstance()->GetParticleManager()->SetBundledData(loadData);
}

void LoadJsonTex(const char* cUrl)
{
	std::string name = "LINK";
	std::string Number = std::to_string(index);
	name += Number;
	TEX[name] = { { "LINK", cUrl } };
	
	index++;
}

void OutputStatusTex()
{
	TEX["Index"] = index;
	auto jobj = TEX.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data/FILE/Textest.json";
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

void SetTex()
{
	const std::string pathToJSON = "data/FILE/Textest.json";
	std::ifstream ifs(pathToJSON);

	if (ifs)
	{
		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> TEX;
		index = TEX["Index"];
		for (int nCnt = 0; nCnt < index; nCnt++)
		{
			
			std::string name = "LINK";
			std::string Number = std::to_string(nCnt);
			name += Number;

			Lync[nCnt] = TEX[name]["LINK"];

			CTexture* pTexture = CApplication::GetInstance()->GetTextureClass();

			pTexture->SetPath(Lync[nCnt]);
		}
	}

	index = 0;
}