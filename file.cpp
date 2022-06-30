//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================
#include "file.h"
#include "letter.h"
#include "particle.h"
#include "imgui_property.h"
#include "texture.h"
#include "application.h"

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "nlohmann/json.hpp"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <sstream>

static CParticle::Particle loadData;
namespace nl = nlohmann;

nl::json j;//リストの生成

nl::json TEX;//リストの生成

std::string Lync[256];
int index = 0;

//============================
//ゲット関数
//============================
CParticle::Particle GetStatus(void)
{
	return loadData;
}

//============================
// 全部出力（置くだけ）
//============================
void OutputStatus()
{
	loadData = GetImguiParticle();

	j["POSMAX"] = {{ "X", loadData.maxPopPos.x } ,{ "Y", loadData.maxPopPos.y } ,{ "Z", loadData.maxPopPos.z } };
	j["POSMIN"] = {{ "X", loadData.minPopPos.x } ,{ "Y", loadData.minPopPos.y } ,{ "Z", loadData.minPopPos.z } };
	j["MOVE"] = { { "X", loadData.move.x } ,{ "Y", loadData.move.y } ,{ "Z", loadData.move.z } };
	j["MOVETRANSITION"] = { { "X", loadData.moveTransition.x } ,{ "Y", loadData.moveTransition.y } ,{ "Z", loadData.moveTransition.z } };
	j["ROT"] = { {"X", loadData.rot.x} ,{ "Y", loadData.rot.y },{ "Z", loadData.rot.z } };
	
	j["COL"] = { { "R", loadData.color.colBigin.r }, {"G" ,loadData.color.colBigin.g} ,{ "B", loadData.color.colBigin.b } ,{ "A", loadData.color.colBigin.a } };
	j["COLRANDAMMAX"] = { { "R", loadData.color.colRandamMax.r },{ "G" ,loadData.color.colRandamMax.g } ,{ "B", loadData.color.colRandamMax.b } ,{ "A", loadData.color.colRandamMax.a } };
	j["COLRANDAMMIN"] = { { "R", loadData.color.colRandamMin.r },{ "G" ,loadData.color.colRandamMin.g } ,{ "B", loadData.color.colRandamMin.b } ,{ "A", loadData.color.colRandamMin.a } };
	j["COLTRANSITION"] = { { "R", loadData.color.colTransition.r },{ "G" ,loadData.color.colTransition.g } ,{ "B", loadData.color.colTransition.b } ,{ "A", loadData.color.colTransition.a } };
	j["DESTCOL"] = { { "R", loadData.color.destCol.r },{ "G" ,loadData.color.destCol.g } ,{ "B", loadData.color.destCol.b } ,{ "A", loadData.color.destCol.a } };
	j["ENDTIME"] = loadData.color.nEndTime;
	j["CNTTRANSITIONTIME"] = loadData.color.nCntTransitionTime;
	j["BCOLTRANSITION"] = loadData.color.bColTransition;
	j["COLRANDOM"] = loadData.color.bColRandom;
	j["RANDOMTRANSITIONTIME"] = loadData.color.bRandomTransitionTime;
	
	j["TYPE"] = loadData.type;
	j["WIDTH"] = loadData.fWidth;
	j["HEIGHT"] = loadData.fHeight;
	j["ANGLE"] = loadData.fAngle;
	j["ATTENUATION"] = loadData.fAttenuation;
	j["RADIUS"] = loadData.fRadius;
	j["WEIGHT"] = loadData.fWeight;
	j["WEIGHTTRANSITION"] = loadData.fWeightTransition;
	j["LIFE"] = loadData.nLife;
	j["BACKROT"] = loadData.bBackrot;
	j["SCALE"] = loadData.fScale;


	auto jobj = j.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data/FILE/DataEffectOutput.json";
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

void LoadJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		loadData = GetImguiParticle();

		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> j;

		//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
		loadData.move = D3DXVECTOR3(j["MOVE"]["X"], j["MOVE"]["Y"], j["MOVE"]["Z"]);
		loadData.rot = D3DXVECTOR3(j["ROT"] ["X"], j["ROT"] ["Y"], j["ROT"] ["Z"]);
		loadData.moveTransition = D3DXVECTOR3(j["MOVETRANSITION"]["X"], j["MOVETRANSITION"]["Y"], j["MOVETRANSITION"]["Z"]);;
		
		loadData.color.colBigin = D3DXCOLOR(j["COL"] ["R"], j["COL"] ["G"], j["COL"] ["B"], j["COL"] ["A"]);
		loadData.color.colRandamMax = D3DXCOLOR(j["COLRANDAMMAX"]["R"], j["COLRANDAMMAX"]["G"], j["COLRANDAMMAX"]["B"], j["COLRANDAMMAX"]["A"]);
		loadData.color.colRandamMin = D3DXCOLOR(j["COLRANDAMMIN"]["R"], j["COLRANDAMMIN"]["G"], j["COLRANDAMMIN"]["B"], j["COLRANDAMMIN"]["A"]);
		loadData.color.colTransition = D3DXCOLOR(j["COLTRANSITION"]["R"], j["COLTRANSITION"]["G"], j["COLTRANSITION"]["B"], j["COLTRANSITION"]["A"]);
		loadData.color.destCol = D3DXCOLOR(j["DESTCOL"]["R"], j["DESTCOL"]["G"], j["DESTCOL"]["B"], j["DESTCOL"]["A"]);
		loadData.color.nEndTime = j["ENDTIME"];
		loadData.color.nCntTransitionTime = j["CNTTRANSITIONTIME"];
		loadData.color.bColTransition = j["BCOLTRANSITION"];
		loadData.color.bColRandom = j["COLRANDOM"];
		loadData.color.bRandomTransitionTime = j["RANDOMTRANSITIONTIME"];
		
		loadData.type = j["TYPE"];
		loadData.fWidth = j["WIDTH"];
		loadData.fHeight = j["HEIGHT"];
		loadData.fRadius = j["RADIUS"];
		loadData.fAngle = j["ANGLE"];
		loadData.fWeight = j["WEIGHT"];
		loadData.nLife = j["LIFE"];
		loadData.fAttenuation = j["ATTENUATION"];
		loadData.fWeightTransition = j["WEIGHTTRANSITION"];
		loadData.nLife = j["LIFE"];
		loadData.bBackrot = j["BACKROT"];
		loadData.fScale = j["SCALE"];

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