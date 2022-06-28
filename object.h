//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

class CObject
{
public:	// �񋓌^

	// CObject�N���X�̔h����
	enum TYPE
	{
		OBJECT_2D
	};
public:	// �萔
	static const int NUM_MAX = 4096;
public:	// �ÓI�֐�
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

public:
	CObject();
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	const D3DXVECTOR3& GetPos() { return pos; }	// �ʒu�̎擾
	virtual void SetPos(const D3DXVECTOR3& inPos) { pos = inPos; }	// �ʒu�̐ݒ�
	void Release();
protected:
	static CObject* object[NUM_MAX];	// �I�u�W�F�N�g
	static int numAll;	// �ő吔
	int createIdx;		// �����ԍ�
	D3DXVECTOR3 pos;	// �ʒu
};

#endif // !_RENDERER_H_
