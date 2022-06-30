//
//BG.h
//

#ifndef _BG_H_
#define _BG_H_

#include <d3dx9.h>
#include "object2d.h"

class CBg : public CObject2D
{
public:
	CBg();
	~CBg();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:

};
#endif