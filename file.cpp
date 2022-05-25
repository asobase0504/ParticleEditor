//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================

#include "file.h"
#include "letter.h"

EffectData Effect;
namespace nl = nlohmann;

nl::json j;//リストの生成


//============================
//ゲット関数
//============================
EffectData *GetStatus(void)
{
	return &Effect;
}

//============================
// 全部出力（置くだけ）
//============================
void OutputStatus()
{
	j["POPPOS"] = {{ "X", Effect.nPopPos.x} ,{ "Y", Effect.nPopPos.y} ,{ "Z", Effect.nPopPos.z } };
	j["ROT"] = { {"X", Effect.ROT.x} ,{ "Y", Effect.ROT.y },{ "Z", Effect.ROT.z } };
	j["DIVISION"] = { {"X", Effect.DIVISION.x} ,{"Y", Effect.DIVISION.y}};
	j["COL"] = { { "R", Effect.COL.r }, {"G" ,Effect.COL.g} ,{ "B", Effect.COL.b } ,{ "A", Effect.COL.a } };
	j["EFFECTTYPE"] = Effect.Effecttype ;
	j["ALPHABLEND_TYPE"] = Effect.Alphablend_TYPE ;
	j["RADIUS"] = Effect.fRadius;
	j["SPEED"] = Effect.Speed;
	j["WEIGHT"] = Effect.Weight;
	j["LIFE"] = Effect.nLife;
	j["TEXTURE"] = Effect.TEXTURE;
	j["TEXTURE2"] = Effect.TEXTURE2;
	j["TIME"] = Effect.TIME;
	j["NUMPARTICLE"] = Effect.NUMPARTICLE;
	j["NUMPARTICLE"] = Effect.MULTITEX;

	auto jobj = j.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data/FILE/EffectOutput.json";
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

		Effect.nPopPos = D3DXVECTOR3(j["POPPOS"]["X"], j["POPPOS"]["Y"], j["POPPOS"]["Z"]);
		Effect.ROT = D3DXVECTOR3(j["ROT"] ["X"], j["ROT"] ["Y"], j["ROT"] ["Z"]);
		Effect.DIVISION = D3DXVECTOR2(j["DIVISION"] ["X"], j["DIVISION"] ["Y"]);
		Effect.COL = D3DXCOLOR(j["COL"] ["R"], j["COL"] ["G"], j["COL"] ["B"], j["COL"] ["A"]);
		Effect.Effecttype = j["EFFECTTYPE"];
		Effect.Alphablend_TYPE = j["ALPHABLEND_TYPE"];
		Effect.fRadius = j["RADIUS"];
		Effect.Speed = j["SPEED"];
		Effect.Weight = j["WEIGHT"];
		Effect.nLife = j["LIFE"];
		Effect.TEXTURE = j["TEXTURE"];
		Effect.TEXTURE2 = j["TEXTURE2"];
		Effect.TIME = j["TIME"];
		Effect.NUMPARTICLE = j["NUMPARTICLE"];
		Effect.MULTITEX = j["NUMPARTICLE"];

	}
	OutputStatus();
}