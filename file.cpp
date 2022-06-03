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

Particle DataEffect;
namespace nl = nlohmann;

nl::json j;//リストの生成


//============================
//ゲット関数
//============================
Particle GetStatus(void)
{
	return DataEffect;
}

//============================
// 全部出力（置くだけ）
//============================
void OutputStatus()
{
	DataEffect = GetImguiParticle();

	j["POS"] = {{ "X", DataEffect.pos.x} ,{ "Y", DataEffect.pos.y} ,{ "Z", DataEffect.pos.z } };
	j["MOVE"] = { { "X", DataEffect.move.x } ,{ "Y", DataEffect.move.y } ,{ "Z", DataEffect.move.z } };
	j["MOVETRANSITION"] = { { "X", DataEffect.moveTransition.x } ,{ "Y", DataEffect.moveTransition.y } ,{ "Z", DataEffect.moveTransition.z } };
	j["ROT"] = { {"X", DataEffect.rot.x} ,{ "Y", DataEffect.rot.y },{ "Z", DataEffect.rot.z } };
	j["COL"] = { { "R", DataEffect.color.col.r }, {"G" ,DataEffect.color.col.g} ,{ "B", DataEffect.color.col.b } ,{ "A", DataEffect.color.col.a } };
	j["COLRANDAMMAX"] = { { "R", DataEffect.color.colRandamMax.r },{ "G" ,DataEffect.color.colRandamMax.g } ,{ "B", DataEffect.color.colRandamMax.b } ,{ "A", DataEffect.color.colRandamMax.a } };
	j["COLRANDAMMIN"] = { { "R", DataEffect.color.colRandamMin.r },{ "G" ,DataEffect.color.colRandamMin.g } ,{ "B", DataEffect.color.colRandamMin.b } ,{ "A", DataEffect.color.colRandamMin.a } };
	j["COLTRANSITION"] = { { "R", DataEffect.color.colTransition.r },{ "G" ,DataEffect.color.colTransition.g } ,{ "B", DataEffect.color.colTransition.b } ,{ "A", DataEffect.color.colTransition.a } };
	j["TYPE"] = DataEffect.type;
	j["WIDTH"] = DataEffect.fWidth;
	j["HEIGHT"] = DataEffect.fHeight;
	j["ANGLE"] = DataEffect.fAngle;
	j["ATTENUATION"] = DataEffect.fAttenuation;
	j["RADIUS"] = DataEffect.fRadius;
	j["WEIGHT"] = DataEffect.fWeight;
	j["WEIGHTTRANSITION"] = DataEffect.fWeightTransition;
	j["LIFE"] = DataEffect.nLife;
	j["BACKROT"] = DataEffect.bBackrot;
	j["SCALE"] = DataEffect.fScale;


	auto jobj = j.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data/FILE/DataEffectOutput.json";
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

void LoodJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		//StringToWString(UTF8toSjis(j["name"]));
		//DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"] ["name"]));
		ifs >> j;

		//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
		DataEffect.pos = D3DXVECTOR3(j["POS"]["X"], j["POS"]["Y"], j["POS"]["Z"]);
		DataEffect.move = D3DXVECTOR3(j["MOVE"]["X"], j["MOVE"]["Y"], j["MOVE"]["Z"]);
		DataEffect.rot = D3DXVECTOR3(j["ROT"] ["X"], j["ROT"] ["Y"], j["ROT"] ["Z"]);
		DataEffect.moveTransition = D3DXVECTOR3(j["MOVETRANSITION"]["X"], j["MOVETRANSITION"]["Y"], j["MOVETRANSITION"]["Z"]);;
		DataEffect.color.col = D3DXCOLOR(j["COL"] ["R"], j["COL"] ["G"], j["COL"] ["B"], j["COL"] ["A"]);
		DataEffect.color.colRandamMax = D3DXCOLOR(j["COLRANDAMMAX"]["R"], j["COLRANDAMMAX"]["G"], j["COLRANDAMMAX"]["B"], j["COLRANDAMMAX"]["A"]);
		DataEffect.color.colRandamMin = D3DXCOLOR(j["COLRANDAMMIN"]["R"], j["COLRANDAMMIN"]["G"], j["COLRANDAMMIN"]["B"], j["COLRANDAMMIN"]["A"]);
		DataEffect.color.colTransition = D3DXCOLOR(j["COLTRANSITION"]["R"], j["COLTRANSITION"]["G"], j["COLTRANSITION"]["B"], j["COLTRANSITION"]["A"]);
		DataEffect.type = j["TYPE"];
		DataEffect.fWidth = j["WIDTH"];
		DataEffect.fHeight = j["HEIGHT"];
		DataEffect.fRadius = j["RADIUS"];
		DataEffect.fAngle = j["ANGLE"];
		DataEffect.fWeight = j["WEIGHT"];
		DataEffect.nLife = j["LIFE"];
		DataEffect.fAttenuation = j["ATTENUATION"];
		DataEffect.fWeightTransition = j["WEIGHTTRANSITION"];
		DataEffect.nLife = j["LIFE"];
		DataEffect.bBackrot = j["BACKROT"];
		DataEffect.fScale = j["SCALE"];

	}

}