//=========================================
// 
// �I�u�W�F�N�g�N���X
// Author YudaKaito
// 
//=========================================
#include "object2d.h"
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include "main.h"
#include <assert.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject2D::CObject2D() :
	m_pVtxBuff(nullptr),
	m_nTexture(-1),
	m_rotY(0.0f),
	m_fLength(0.0f),
	fAngle(0.0f),
	m_col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject2D::~CObject2D()
{
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CObject2D::Init()
{
	// ���_�o�b�t�@�̐���
	CApplication::GetInstance()->GetRenderer()->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	m_fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	fAngle = atan2f(size.x, size.y);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// tex�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void CObject2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void CObject2D::Update()
{
	m_rotY += -0.01f;
	size.x += 5.0f * sinf(m_rotY);
	size.y += 5.0f * sinf(m_rotY);

	SetSize(size);
	SetPos(pos);
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 texture = CApplication::GetInstance()->GetTextureClass()->GetTexture(m_nTexture);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �e�N�X�`���̔j��
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CObject2D::SetPos(const D3DXVECTOR3 & inPos)
{
	CObject::SetPos(inPos);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = pos.x + sinf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.y = pos.y + cosf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = pos.y + cosf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.y = pos.y + cosf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.y = pos.y + cosf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �傫���̐ݒ�
//=============================================================================
void CObject2D::SetSize(const D3DXVECTOR2 & inSize)
{
	size = inSize;
	m_fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	fAngle = atan2f(size.x, size.y);

	SetPos(pos);
}

void CObject2D::SetColor(const D3DXCOLOR & inColor)
{
	m_col = inColor;

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �|���S���N���X�̍쐬
//=============================================================================
CObject2D* CObject2D::Create()
{
	if (numAll >= NUM_MAX)
	{
		return nullptr;
	}

	int idx = numAll;
	CObject2D* objectCreate = new CObject2D;
	objectCreate->createIdx = idx;

	if (objectCreate != nullptr)
	{
		if (FAILED(objectCreate->Init()))
		{
			assert(false);
		}
	}

	return objectCreate;
}
