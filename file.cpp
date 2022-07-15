//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include <fstream>
#include "nlohmann/json.hpp"

#include "application.h"
#include "texture.h"
#include "particle_manager.h"

namespace nl = nlohmann;

nl::json j;//リストの生成

nl::json TEX;//リストの生成

std::string Lync[256];
int index = 0;

//============================
// 全部出力（置くだけ）
//============================
void OutputStatus()
{
	CParticleManager::BundledData& loadData = CApplication::GetInstance()->GetParticleManager()->GetBundledData()[0];

	CParticle::Info& particleInfo = loadData.particleData;

	j["POSMAX"] = {{ "X", particleInfo.maxPopPos.x } ,{ "Y", particleInfo.maxPopPos.y } ,{ "Z", particleInfo.maxPopPos.z } };
	j["POSMIN"] = {{ "X", particleInfo.minPopPos.x } ,{ "Y", particleInfo.minPopPos.y } ,{ "Z", particleInfo.minPopPos.z } };
	j["MOVE"] = { { "X", particleInfo.move.x } ,{ "Y", particleInfo.move.y } ,{ "Z", particleInfo.move.z } };
	j["MOVETRANSITION"] = { { "X", particleInfo.moveTransition.x } ,{ "Y", particleInfo.moveTransition.y } ,{ "Z", particleInfo.moveTransition.z } };
	j["ROT"] = { {"X", particleInfo.rot.x} ,{ "Y", particleInfo.rot.y },{ "Z", particleInfo.rot.z } };
	
	j["COL"] = { { "R", particleInfo.color.colBigin.r }, {"G" ,particleInfo.color.colBigin.g} ,{ "B", particleInfo.color.colBigin.b } ,{ "A", particleInfo.color.colBigin.a } };
	j["COLRANDAMMAX"] = { { "R", particleInfo.color.colRandamMax.r },{ "G" ,particleInfo.color.colRandamMax.g } ,{ "B", particleInfo.color.colRandamMax.b } ,{ "A", particleInfo.color.colRandamMax.a } };
	j["COLRANDAMMIN"] = { { "R", particleInfo.color.colRandamMin.r },{ "G" ,particleInfo.color.colRandamMin.g } ,{ "B", particleInfo.color.colRandamMin.b } ,{ "A", particleInfo.color.colRandamMin.a } };
	j["COLTRANSITION"] = { { "R", particleInfo.color.colTransition.r },{ "G" ,particleInfo.color.colTransition.g } ,{ "B", particleInfo.color.colTransition.b } ,{ "A", particleInfo.color.colTransition.a } };
	j["DESTCOL"] = { { "R", particleInfo.color.destCol.r },{ "G" ,particleInfo.color.destCol.g } ,{ "B", particleInfo.color.destCol.b } ,{ "A", particleInfo.color.destCol.a } };
	j["ENDTIME"] = particleInfo.color.nEndTime;
	j["CNTTRANSITIONTIME"] = particleInfo.color.nCntTransitionTime;
	j["BCOLTRANSITION"] = particleInfo.color.bColTransition;
	j["COLRANDOM"] = particleInfo.color.bColRandom;
	j["RANDOMTRANSITIONTIME"] = particleInfo.color.bRandomTransitionTime;
	
	j["TYPE"] = particleInfo.type;
	j["WIDTH"] = particleInfo.fWidth;
	j["HEIGHT"] = particleInfo.fHeight;
	j["ANGLE"] = particleInfo.fAngle;
	j["ATTENUATION"] = particleInfo.fAttenuation;
	j["RADIUS"] = particleInfo.fRadius;
	j["WEIGHT"] = particleInfo.fWeight;
	j["WEIGHTTRANSITION"] = particleInfo.fWeightTransition;
	j["LIFE"] = particleInfo.nLife;
	j["BACKROT"] = particleInfo.bBackrot;
	j["SCALE"] = particleInfo.fScale;


	auto jobj = j.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data/FILE/DataEffectOutput.json";
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

	if (ifs)
	{
		CParticleManager::BundledData loadData = {};
		CParticle::Info& particleInfo = loadData.particleData;

		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> j;

		//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
		particleInfo.move = D3DXVECTOR3(j["MOVE"]["X"], j["MOVE"]["Y"], j["MOVE"]["Z"]);
		particleInfo.rot = D3DXVECTOR3(j["ROT"] ["X"], j["ROT"] ["Y"], j["ROT"] ["Z"]);
		particleInfo.moveTransition = D3DXVECTOR3(j["MOVETRANSITION"]["X"], j["MOVETRANSITION"]["Y"], j["MOVETRANSITION"]["Z"]);;
		
		particleInfo.color.colBigin = D3DXCOLOR(j["COL"] ["R"], j["COL"] ["G"], j["COL"] ["B"], j["COL"] ["A"]);
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

		static bool chack = true;

		if (chack)
		{
			CApplication::GetInstance()->GetParticleManager()->SetBundledData(loadData);
			chack = false;
		}
		else
		{
			CApplication::GetInstance()->GetParticleManager()->ChangeBundledData(CParticleManager::DEBUG_TYPE, loadData);
		}
	}
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