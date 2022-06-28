//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"
#include "texture.h"

class CObject2D : public CObject
{
public:
	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	// 頂点フォーマット
	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

public:	// 静的関数
	static CObject2D* Create();

public:
	CObject2D();
	~CObject2D();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPos(const D3DXVECTOR3& inPos) override;
	void SetSize(const D3DXVECTOR2& inSize);
	void SetColor(const D3DXCOLOR& inColor);
	D3DXCOLOR GetColor() { return m_col; }
	void SetTexture(CTexture::TEXTURE inTex) { m_texture = inTex; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// VtxBuff
	CTexture::TEXTURE m_texture;
	float m_rotY;	// 回転
	float m_fLength;	// 長さ
	float fAngle;	// 角度
	D3DXVECTOR2 size;	// 大きさ
	D3DXCOLOR m_col;	// 色

};

#endif // !_OBJECT2D_H_
