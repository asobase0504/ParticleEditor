//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include "particle.h"
#include "imgui_property.h"
#include "texture.h"
#include "application.h"

#include <fstream>
#include "nlohmann/json.hpp"

#include "particle_manager.h"
#include "particle.h"

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

	j["POSMAX"] = {{ "X", loadData.particleData.maxPopPos.x } ,{ "Y", loadData.particleData.maxPopPos.y } ,{ "Z", loadData.particleData.maxPopPos.z } };
	j["POSMIN"] = {{ "X", loadData.particleData.minPopPos.x } ,{ "Y", loadData.particleData.minPopPos.y } ,{ "Z", loadData.particleData.minPopPos.z } };
	j["MOVE"] = { { "X", loadData.particleData.move.x } ,{ "Y", loadData.particleData.move.y } ,{ "Z", loadData.particleData.move.z } };
	j["MOVETRANSITION"] = { { "X", loadData.particleData.moveTransition.x } ,{ "Y", loadData.particleData.moveTransition.y } ,{ "Z", loadData.particleData.moveTransition.z } };
	j["ROT"] = { {"X", loadData.particleData.rot.x} ,{ "Y", loadData.particleData.rot.y },{ "Z", loadData.particleData.rot.z } };
	
	j["COL"] = { { "R", loadData.particleData.color.colBigin.r }, {"G" ,loadData.particleData.color.colBigin.g} ,{ "B", loadData.particleData.color.colBigin.b } ,{ "A", loadData.particleData.color.colBigin.a } };
	j["COLRANDAMMAX"] = { { "R", loadData.particleData.color.colRandamMax.r },{ "G" ,loadData.particleData.color.colRandamMax.g } ,{ "B", loadData.particleData.color.colRandamMax.b } ,{ "A", loadData.particleData.color.colRandamMax.a } };
	j["COLRANDAMMIN"] = { { "R", loadData.particleData.color.colRandamMin.r },{ "G" ,loadData.particleData.color.colRandamMin.g } ,{ "B", loadData.particleData.color.colRandamMin.b } ,{ "A", loadData.particleData.color.colRandamMin.a } };
	j["COLTRANSITION"] = { { "R", loadData.particleData.color.colTransition.r },{ "G" ,loadData.particleData.color.colTransition.g } ,{ "B", loadData.particleData.color.colTransition.b } ,{ "A", loadData.particleData.color.colTransition.a } };
	j["DESTCOL"] = { { "R", loadData.particleData.color.destCol.r },{ "G" ,loadData.particleData.color.destCol.g } ,{ "B", loadData.particleData.color.destCol.b } ,{ "A", loadData.particleData.color.destCol.a } };
	j["ENDTIME"] = loadData.particleData.color.nEndTime;
	j["CNTTRANSITIONTIME"] = loadData.particleData.color.nCntTransitionTime;
	j["BCOLTRANSITION"] = loadData.particleData.color.bColTransition;
	j["COLRANDOM"] = loadData.particleData.color.bColRandom;
	j["RANDOMTRANSITIONTIME"] = loadData.particleData.color.bRandomTransitionTime;
	
	j["TYPE"] = loadData.particleData.type;
	j["WIDTH"] = loadData.particleData.fWidth;
	j["HEIGHT"] = loadData.particleData.fHeight;
	j["ANGLE"] = loadData.particleData.fAngle;
	j["ATTENUATION"] = loadData.particleData.fAttenuation;
	j["RADIUS"] = loadData.particleData.fRadius;
	j["WEIGHT"] = loadData.particleData.fWeight;
	j["WEIGHTTRANSITION"] = loadData.particleData.fWeightTransition;
	j["LIFE"] = loadData.particleData.nLife;
	j["BACKROT"] = loadData.particleData.bBackrot;
	j["SCALE"] = loadData.particleData.fScale;


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

		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> j;

		//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
		loadData.particleData.move = D3DXVECTOR3(j["MOVE"]["X"], j["MOVE"]["Y"], j["MOVE"]["Z"]);
		loadData.particleData.rot = D3DXVECTOR3(j["ROT"] ["X"], j["ROT"] ["Y"], j["ROT"] ["Z"]);
		loadData.particleData.moveTransition = D3DXVECTOR3(j["MOVETRANSITION"]["X"], j["MOVETRANSITION"]["Y"], j["MOVETRANSITION"]["Z"]);;
		
		loadData.particleData.color.colBigin = D3DXCOLOR(j["COL"] ["R"], j["COL"] ["G"], j["COL"] ["B"], j["COL"] ["A"]);
		loadData.particleData.color.colRandamMax = D3DXCOLOR(j["COLRANDAMMAX"]["R"], j["COLRANDAMMAX"]["G"], j["COLRANDAMMAX"]["B"], j["COLRANDAMMAX"]["A"]);
		loadData.particleData.color.colRandamMin = D3DXCOLOR(j["COLRANDAMMIN"]["R"], j["COLRANDAMMIN"]["G"], j["COLRANDAMMIN"]["B"], j["COLRANDAMMIN"]["A"]);
		loadData.particleData.color.colTransition = D3DXCOLOR(j["COLTRANSITION"]["R"], j["COLTRANSITION"]["G"], j["COLTRANSITION"]["B"], j["COLTRANSITION"]["A"]);
		loadData.particleData.color.destCol = D3DXCOLOR(j["DESTCOL"]["R"], j["DESTCOL"]["G"], j["DESTCOL"]["B"], j["DESTCOL"]["A"]);
		loadData.particleData.color.nEndTime = j["ENDTIME"];
		loadData.particleData.color.nCntTransitionTime = j["CNTTRANSITIONTIME"];
		loadData.particleData.color.bColTransition = j["BCOLTRANSITION"];
		loadData.particleData.color.bColRandom = j["COLRANDOM"];
		loadData.particleData.color.bRandomTransitionTime = j["RANDOMTRANSITIONTIME"];
		
		loadData.particleData.type = j["TYPE"];
		loadData.particleData.fWidth = j["WIDTH"];
		loadData.particleData.fHeight = j["HEIGHT"];
		loadData.particleData.fRadius = j["RADIUS"];
		loadData.particleData.fAngle = j["ANGLE"];
		loadData.particleData.fWeight = j["WEIGHT"];
		loadData.particleData.nLife = j["LIFE"];
		loadData.particleData.fAttenuation = j["ATTENUATION"];
		loadData.particleData.fWeightTransition = j["WEIGHTTRANSITION"];
		loadData.particleData.nLife = j["LIFE"];
		loadData.particleData.bBackrot = j["BACKROT"];
		loadData.particleData.fScale = j["SCALE"];

		CApplication::GetInstance()->GetParticleManager()->SetBundledData(loadData);
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