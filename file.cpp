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
nlohmann::json LoadJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (!ifs)
	{
		return nullptr;
	}
	nlohmann::json jsonData;
	ifs >> jsonData;

	return jsonData;
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

//--------------------------------------------------
// 値の読込み
//--------------------------------------------------
void LoadValueBundledData(nlohmann::json* inJson, DeepTable& inTable)
{
	std::vector<ConvTable> table = inTable.table;
	for (int i = 0; i < table.size(); i++)
	{
		if ((*inJson).count(table[i].name) == 0)
		{
			continue;
		}

		switch (table[i].type)
		{
		case UseChack:
			if (((DeepTable*)table[i].store)->use != nullptr)
			{
				*((DeepTable*)table[i].store)->use = true;
			}
			// 再起処理
			LoadValueBundledData(&((*inJson)[table[i].name]), *((DeepTable*)table[i].store));
			break;
		case Vector3:
		{
			auto VectorFloatToVector3 = [](std::vector<float> inVector)->D3DXVECTOR3
			{
				return D3DXVECTOR3(inVector[0], inVector[1], inVector[2]);
			};

			*((D3DXVECTOR3*)table[i].store) = VectorFloatToVector3((*inJson)[table[i].name]);
		}
		break;
		case Color:
		{
			auto VectorFloatToColor = [](std::vector<float> inVector)->D3DXCOLOR
			{
				return D3DXCOLOR(inVector[0], inVector[1], inVector[2], inVector[3]);
			};

			*((D3DXCOLOR*)table[i].store) = VectorFloatToColor((*inJson)[table[i].name]);
		}
		break;
		case Float:
			*((float*)table[i].store) = (*inJson)[table[i].name];
			break;
		case Int:
			*((int*)table[i].store) = (*inJson)[table[i].name];
			break;
		case Bool:
			*((bool*)table[i].store) = (*inJson)[table[i].name];
			break;

		default:
			break;
		}
	}
}
