//============================
//
// �t�@�C���ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_	// ���̃}�N����`������ĂȂ�������
#define _FILE_H_	// ��d�C���N���[�h�h�~�̃}�N����`

#include "nlohmann/json.hpp"

//--------------------------------------------------
// �������ϐ��̌^
//--------------------------------------------------
enum Type {
	UseChack,	// �z��
	Vector3,	// D3DXVECTOR3�^
	Color,		// D3DXCOLOR�^
	Float,		// float�^
	Int,		// int�^
	Bool		// bool�^
};

//--------------------------------------------------
// ������ɕK�v�ȏ��
//--------------------------------------------------
struct ConvTable {
	Type type;			// ����`��
	const char *name;	// Key
	void *store;		// ����������l
};

//--------------------------------------------------
// �������l�𕡐��ɑΉ�����B
//--------------------------------------------------
struct DeepTable {
	std::vector<ConvTable> table;
	bool* use;
};

//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�
void SaveJson(nlohmann::json inJson, const std::string cUrl);
nlohmann::json LoadJson(const wchar_t* cUrl);
void LoadJsonTex(const char* cUrl);
void OutputStatusTex();
void SetTex();
void LoadValueBundledData(nlohmann::json*, DeepTable&);
#endif
