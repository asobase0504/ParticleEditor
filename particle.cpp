//����������������������������������������
//effect.cpp
//����������������������������������������
#include "main.h"
#include "particle.h"
#include <time.h>
#include "imgui_property.h"

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture[NUM_PARTICLE] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Particle g_aParticle[MAX_PARTICLE];
float g_fAngle = 0.0f;

//����������
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&s_pTexture[PARTICLETYPE_NORMAL]);

	// ������
	memset(g_aParticle, 0, sizeof(g_aParticle));

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//�I������
void UninitParticle(void)
{
	for (int i = 0; i < NUM_PARTICLE; i++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//�X�V����
void UpdateParticle(void)
{
	//(ImGui)
	D3DXVECTOR3 ImPos = GetPos();
	D3DXVECTOR3 ImMove = GetMove();
	D3DXCOLOR ImColor = GetColor();
	int ImSelect = GetType();
	float ImAlpha = GetAlpha();
	float ImAttenuation = GetAttenuation();
	float ImAngle = GetAngle();
	bool bEnable = bSetEffect();
	bool bBackRot = BackRot();
	bool bTexRot = TexRot();
	bool bTex = TexUse();
	float ImRadius = GetRadius();

//	g_fAngle = 0;

	if (bSetImguiParticle())
	{
		SetParticleImgui(GetImguiParticle());
//		SetParticle(ImPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ImColor, 60, 50.0f, 50.0f, PARTICLETYPE_NORMAL);
	}

	if (bTex)
	{
		LoadTex();
	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (!pParticle->bUse)
		{//�G�t�F�N�g���g�p����Ă���Ȃ�
			continue;
		}

		/* ���g�p���Ă���Ȃ火 */

		//�G�t�F�N�g�̈ړ�
		pParticle->pos += pParticle->move;

		// �����ȓ���
		{
			//��
			/*g_fAngle += 0.7f;
			pParticle->move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * ImAttenuation;
			pParticle->move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * ImAttenuation;*/

			//�ӂɂ�ӂɂ�
			/*g_fAngle += 0.0996f;
			pParticle->move.x = ImAttenuation * powf(cosf(fGRad), 3.0f);
			pParticle->move.y = ImAttenuation * powf(sinf(fGRad), 3.0f);*/

		}

		// ����
		pParticle->nLife--;	// �̗͂̌���
		pParticle->move.y += pParticle->fWeight;			// �d��
		pParticle->col += pParticle->colTransition;			// �F�̐���
		pParticle->move *= pParticle->fAttenuation;		// �ړ��ʂ̐���
		pParticle->fWeight += pParticle->fWeightTransition;	// �d���̐���

		if (pParticle->nLife <= 0)
		{//�G�t�F�N�g�̎���
			DeleteParticle(i);
		}

		VERTEX_2D *pVtx = nullptr;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		//���_�f�[�^�̃|�C���^��4���W�߂�

		//���_���W�̐ݒ�
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;
		
		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//�`�揈��
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�_�ɓ\��(true)�A�|���S���ɓ\��(false)
	//pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,true);

	//�J�����̈ʒu(true)�A�X�N���[���̈ʒu(false)
	//pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,false);

	//�T�C�Y��ݒ�
	//pDevice->SetRenderState(D3DRS_POINTSIZE,FloattoDword(1.0f));
	//pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloattoDword(0.0f));

	//�|�C���g�T�C�Y�̌v�Z
	//pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloattoDword(0.0f));
	//pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloattoDword(0.0f));
	//pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloattoDword(1.0f));

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (!g_aParticle[nCnt].bUse)
		{
			continue;
		}

		/* ���g�p���Ă���Ȃ火 */

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[g_aParticle[nCnt].type]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		//pDevice->DrawPrimitive(D3DPT_POINTLIST,0, nCnt);
	}

	//�|�C���g�X�v���C�g����������
	//pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���������p���Ȃ�
	pDevice->SetTexture(0, NULL);
}

//�ݒ菈��
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type)
{
	int ImLife = GetLife();
	float ImRadius = GetRadius();

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (pParticle->bUse)
		{
			continue;
		}

		/* ���g�p����ĂȂ��Ȃ火 */

		// �f�[�^�̃��Z�b�g
		DeleteParticle(i);

		pParticle->pos = pos;
		pParticle->move = move;
		pParticle->col = col;
		pParticle->type = type;
		pParticle->fWidth = fWidth;
		pParticle->fHeight = fHeight;

		//(ImGui)
		pParticle->nLife = ImLife;
		pParticle->fRadius = ImRadius;

		pParticle->bUse = true;

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		//���_�f�[�^�̃|�C���^��4���W�߂�

		//���_���W�̐ݒ�
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

void SetParticleImgui(Particle& inParticle)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (pParticle->bUse)
		{
			continue;
		}

		/* ���g�p����ĂȂ��Ȃ火 */

		// �f�[�^�̃��Z�b�g
		DeleteParticle(i);

		*pParticle = inParticle;
		pParticle->type = PARTICLETYPE_NORMAL;

		pParticle->fWidth = g_aParticle->fScale;
		pParticle->fHeight = g_aParticle->fScale;
		pParticle->bUse = true;

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		//���_�f�[�^�̃|�C���^��4���W�߂�

		//���_���W�̐ݒ�
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		float ImAttenuation = GetAttenuation();
		float ImAngle = GetAngle();
		D3DXVECTOR3 ImRot = GetRot();
		bool bBackRot = BackRot();
		float fRad = 0.0f;
		float fGRad = 0.0f;

		if (pParticle->bBackrot)
		{
			//float fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (ImRot.z - g_fAngle);
		}
		else
		{
			fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (ImRot.z + g_fAngle);
		}

		//������������
		g_fAngle += ImAngle;
		pParticle->move.x += (pParticle->fRadius * sinf(fGRad)) * pParticle->fAttenuation;
		pParticle->move.y += (pParticle->fRadius * cosf(fGRad)) * pParticle->fAttenuation;

		//======================
		//���K��
		//======================
		if (pParticle->fRadius > D3DX_PI)
		{
			pParticle->fRadius -= D3DX_PI * 2;
		}
		else if (pParticle->fRadius < -D3DX_PI)
		{
			pParticle->fRadius += D3DX_PI * 2;
		}

		if (g_fAngle > D3DX_PI)
		{
			g_fAngle -= D3DX_PI * 2;
		}
		else if (g_fAngle < -D3DX_PI)
		{
			g_fAngle += D3DX_PI * 2;
		}

		break;
	}
}

// �e�N�X�`���̓Ǎ���
void LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			GetFileName(),
			&s_pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

// �폜����
void DeleteParticle(int nDelete)
{
	// �f�[�^�̃��Z�b�g
	memset(&g_aParticle[nDelete], 0, sizeof(g_aParticle[nDelete]));
}

void DeleteParticleAll()
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		DeleteParticle(i);
	}
}

// �p�x�̏���������
void RemoveAngle(void)
{
	g_fAngle = 0;
}

Particle *GetParticle(void)
{
	return &g_aParticle[0];
}

//Float��DWORD�ɕϊ�����
DWORD FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}