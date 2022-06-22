//==================================================
//
// ÉQÅ[ÉÄÉÇÅ[Éh
// Author : Yuda Kaito
//
//==================================================

//==================================================
// ÉCÉìÉNÉãÅ[Éh
//==================================================
// éÂóvånìù
#include "main.h"
#include "game.h"
#include "input.h"
#include <assert.h>
// èàóùånìù
#include "BG.h"
#include "particle.h"
#include "imgui_property.h"

//==================================================
// ê√ìIïœêî
//==================================================
CParticle* particle;

//--------------------------------------------------
// èâä˙âª
//--------------------------------------------------
void InitGame(void)
{
	particle = new CParticle[CParticle::maxNumber];
	InitBg();		// îwåi
}

//--------------------------------------------------
// èIóπ
//--------------------------------------------------
void UninitGame(void)
{
	UninitBg();			// îwåi
}

//--------------------------------------------------
// çXêV
//--------------------------------------------------
void UpdateGame(void)
{
	UpdateBg();			// îwåi
	CParticle::AllUpdate();

	if (bSetImguiParticle())
	{
		CParticle::Create(GetImguiParticle(), D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	}
}

//--------------------------------------------------
// ï`âÊ
//--------------------------------------------------
void DrawGame(void)
{
	DrawBg();		// îwåi
	CParticle::AllDraw();
}
