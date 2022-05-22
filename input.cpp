//=============================================================================
//
// ���͏��� [input.cpp]
// Author1 : KOZUNA HIROHITO
// Author2 : ISOE JUKIA
// Author3 : YUDA KAITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "input.h"
#include "XInput.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define NUM_KEY_MAX		(256)	// �L�[�̍ő吔�i�L�[�{�[�h�j
#define MOUSE_KEY_MAX	(8)		// �L�[�̍ő吔�i�}�E�X�j
#define PLAYER_MAX		(4)		// �v���C���[�̍ő�l��

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
// �L�[�{�[�h
static LPDIRECTINPUT8 s_pInput = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8 s_pDevKeyboard = NULL;		// ���̓f�o�C�X�i�L�[�{�[�h�i�R���g���[���[�p�͕ʂɍ��j�j�ւ̃|�C���^
static BYTE s_aKeyState[NUM_KEY_MAX];					// �L�[�{�[�h�̃v���X���
static BYTE s_aKeyStateTrigger[NUM_KEY_MAX];			// �L�[�{�[�h�̃g���K�[���
static BYTE s_aKeyStateRelease[NUM_KEY_MAX];			// �L�[�{�[�h�̃����[�X���

														// �W���C�p�b�h
static XINPUT_STATE s_JoyKeyState[PLAYER_MAX];			// �W���C�p�b�g�̃v���X���
static XINPUT_STATE s_JoyKeyStateTrigger[PLAYER_MAX];	// �W���C�p�b�g�̃g���K�[���
static XINPUT_STATE s_JoyKeyStateRelease[PLAYER_MAX];	// �W���C�p�b�g�̃����[�X���
static D3DXVECTOR3 s_JoyStickPos[PLAYER_MAX];			// �W���C�X�e�B�b�N�̌X��
static XINPUT_VIBRATION s_JoyMoter[PLAYER_MAX];			// �W���C�p�b�h�̃��[�^�[
static int s_nTime[PLAYER_MAX];							// �U����������
static WORD s_nStrength[PLAYER_MAX];					// �U���̋��� (0 - 65535)

														// �}�E�X
static LPDIRECTINPUT8 s_pMouseInput = NULL;				// Directinut�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8 s_pDevMouse = NULL;			// ���͂Ńp�X�ւ̃|�C���^
static DIMOUSESTATE2 s_aKeyStateMouse;					// �}�E�X�̃v���X����
static DIMOUSESTATE2 s_aKeyStateMouseTrigger;			// �}�E�X�̃g���K�[����
static DIMOUSESTATE2 s_aKeyStateMouseRelease;			// �}�E�X�̃����[�X����
static POINT s_MousePos;								// �}�E�X�̃J�[�\���p
static HWND s_hMouseWnd;								// �E�B���h�E�n���h��

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
// �L�[�{�[�h
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	// ������
void UninitKeyboard(void);								// �I������
void UpdateKeyboard(void);								// �X�V����

														// �W���C�p�b�h
HRESULT InitJoypad(void);								// ������
void UninitJoypad(void);								// �I������
void UpdateJoypad(void);								// �X�V����

														// �}�E�X
HRESULT InitMouse(HINSTANCE hlnstance, HWND hWnd);		// ������
void UninitMouse(void);									// �I������
void UpdateMouse(void);									// �X�V����

//*************************************************************************************
// ���͏����S��
//*************************************************************************************

//--------------------------------------------------
// ���͏����S���̏�����
//--------------------------------------------------
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �}�E�X�̏���������
	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �W���C�p�b�h������
	InitJoypad();

	return S_OK;
}

//--------------------------------------------------
// ���͏����S���̏I������
//--------------------------------------------------
void UninitInput(void)
{
	// �I������
	UninitKeyboard();	// �L�[�{�[�h
	UninitMouse();		// �}�E�X
	UninitJoypad();		// �W���C�p�b�h

}

//--------------------------------------------------
// ���͏����S���̍X�V����
//--------------------------------------------------
void UpdateInput(void)
{
	// �X�V����
	UpdateKeyboard();	// �L�[�{�[�h
	UpdateMouse();		// �}�E�X
	UpdateJoypad();		// �W���C�p�b�h
}

//*****************************************************************************
// �L�[�{�[�h���͏���
//*****************************************************************************

//--------------------------------------------------
// �L�[�{�[�h����������
//--------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&s_pInput, NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard, &s_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------
// �L�[�{�[�h�̏I������
//--------------------------------------------------
void UninitKeyboard(void)
{
	// ���̓f�o�C�X�i�L�[�{�[�h�j�̕���
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (s_pInput != NULL)
	{
		s_pInput->Release();
		s_pInput = NULL;
	}
}

//--------------------------------------------------
// �L�[�{�[�h�̍X�V����
//--------------------------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��

	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{// ���̓f�o�C�X����f�[�^���擾
		for (int i = 0; i < NUM_KEY_MAX; i++)
		{
			s_aKeyStateTrigger[i] = ~s_aKeyState[i] & aKeyState[i];	// �L�[�{�[�h�̃g���K�[����ۑ�
			s_aKeyStateRelease[i] = s_aKeyState[i] & ~aKeyState[i];	// �L�[�{�[�h�̃����[�X����ۑ�
			s_aKeyState[i] = aKeyState[i];	// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//--------------------------------------------------
// �L�[�{�[�h�̃v���X�����擾
//--------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// �L�[�{�[�h�̃g���K�[�����擾
//--------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// �L�[�{�[�h�̃����[�X�����擾
//--------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// �L�[�{�[�h�̑S�L�[�v���X�����擾
//--------------------------------------------------
bool GetKeyboardAllPress(void)
{
	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		if (GetKeyboardPress(i))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// �L�[�{�[�h�̑S�L�[�g���K�[�����擾
//--------------------------------------------------
bool GetKeyboardAllTrigger(void)
{
	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		if (GetKeyboardTrigger(i))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// �L�[�{�[�h�̑S�L�[�����[�X�����擾
//--------------------------------------------------
bool GetKeyboardAllRelease(void)
{
	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		if (GetKeyboardRelease(i))
		{
			return true;
		}
	}
	return false;
}

//*************************************************************************************
//�W���C�p�b�h���͏���
//*************************************************************************************

//--------------------------------------------------
// �W���C�p�b�h�̏�����
//--------------------------------------------------
HRESULT InitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�i�L���ɂ���j
	XInputEnable(true);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//�������[�̃N���A
		memset(&s_JoyKeyState[i], 0, sizeof(XINPUT_STATE));
		memset(&s_JoyKeyStateTrigger[i], 0, sizeof(XINPUT_STATE));

		//�W���C�p�b�h�̐U������̂O�N���A
		ZeroMemory(&s_JoyMoter[i], sizeof(XINPUT_VIBRATION));

		//�U������p�̏�����
		s_nStrength[i] = 0;
		s_nTime[i] = 0;
	}
	return S_OK;
}

//--------------------------------------------------
// �W���C�p�b�h�̏I��
//--------------------------------------------------
void UninitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�i�����ɂ���j
	XInputEnable(false);
}

//--------------------------------------------------
// �W���C�p�b�h�̍X�V
//--------------------------------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//�W���C�p�b�h���͏��

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//�W���C�p�b�h�̏�Ԃ��擾
		if (XInputGetState(i, &JoyKeyState[i]) == ERROR_SUCCESS)
		{
			WORD wButtons = s_JoyKeyState[i].Gamepad.wButtons;
			WORD wOldButtons = s_JoyKeyState[i].Gamepad.wButtons;
			s_JoyKeyStateTrigger[i].Gamepad.wButtons = ~wButtons & wOldButtons; //�g���K�[����ۑ�
			s_JoyKeyStateRelease[i].Gamepad.wButtons = wButtons & ~wOldButtons; //�����[�X����ۑ�
			s_JoyKeyState[i] = JoyKeyState[i];  //�v���X����
		}

		//�W���C�p�b�h�̐U��
		s_JoyMoter[i].wLeftMotorSpeed = s_nStrength[i];
		s_JoyMoter[i].wRightMotorSpeed = s_nStrength[i];
		XInputSetState(i, &s_JoyMoter[i]);

		if (s_nTime[i] > 0)
		{
			s_nTime[i]--;
		}
		else
		{
			s_nStrength[i] = 0;
			s_nTime[i] = 0;
		}
	}
}

//--------------------------------------------------
// �W���C�p�b�h�̃v���X����
//--------------------------------------------------
bool GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (s_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//--------------------------------------------------
// �W���C�p�b�h�̃g���K�[����
//--------------------------------------------------
bool GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (s_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//--------------------------------------------------
// �W���C�p�b�g�i�X�e�B�b�N�v���X�j����
//--------------------------------------------------
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		s_JoyStickPos[nPlayer] = D3DXVECTOR3(s_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -s_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		s_JoyStickPos[nPlayer] = D3DXVECTOR3(s_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -s_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return s_JoyStickPos[nPlayer];
}

//--------------------------------------------------
// �W���C�p�b�g�i�g���K�[�y�_���j����
//--------------------------------------------------
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = s_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = s_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//--------------------------------------------------
// �R���g���[���[�̐U������
//--------------------------------------------------
void JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	s_nTime[nPlayer] = nTime;			//�U����������
	s_nStrength[nPlayer] = nStrength;	//�U���̋���
}

//*************************************************************************************
//�}�E�X���͏���
//*************************************************************************************

//--------------------------------------------------
// �}�E�X�̏�����
//--------------------------------------------------
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&s_pMouseInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�}�E�X�j�̐���
	if (FAILED(s_pMouseInput->CreateDevice(GUID_SysMouse, &s_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(s_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�E�B���h�E�n���h���̕ۊ�
	s_hMouseWnd = hWnd;

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	s_pDevMouse->Acquire();

	return S_OK;
}

//--------------------------------------------------
// �}�E�X�̏I������
//--------------------------------------------------
void UninitMouse(void)
{
	if (s_pDevMouse != NULL)
	{
		s_pDevMouse->Unacquire();
		s_pDevMouse = NULL;
	}
}

//--------------------------------------------------
//�}�E�X�̍X�V����
//--------------------------------------------------
void UpdateMouse(void)
{
	DIMOUSESTATE2 aKeyState;	// ���͏��

	if (SUCCEEDED(s_pDevMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{// ���̓f�o�C�X����f�[�^���擾
		for (int nCntKey = 0; nCntKey < MOUSE_KEY_MAX; nCntKey++)
		{
			WORD wButtons = s_aKeyStateMouse.rgbButtons[nCntKey];
			WORD wOldButtons = aKeyState.rgbButtons[nCntKey];
			s_aKeyStateMouseTrigger.rgbButtons[nCntKey] = ~wButtons & wOldButtons; // �g���K�[����ۑ�
			s_aKeyStateMouseRelease.rgbButtons[nCntKey] = wButtons & ~wOldButtons; // �����[�X����ۑ�
		}

		s_aKeyStateMouse = aKeyState;		// �v���X����ۑ�
	}
	else
	{
		s_pDevMouse->Acquire();			// �A�N�Z�X�����l��
	}
}

//--------------------------------------------------
// �}�E�X�̃v���X����
//--------------------------------------------------
bool GetMousePress(MOUSE mouse)
{
	return (s_aKeyStateMouse.rgbButtons[mouse] & 0x80) ? true : false;
}

//--------------------------------------------------
// �}�E�X�̃g���K�[����
//--------------------------------------------------
bool GetMouseTrigger(MOUSE mouse)
{
	return (s_aKeyStateMouseTrigger.rgbButtons[mouse] & 0x80) ? true : false;
}

//--------------------------------------------------
// �}�E�X�̃����[�X����
//--------------------------------------------------
bool GetMouseRelease(MOUSE mouse)
{
	return (s_aKeyStateMouseRelease.rgbButtons[mouse] & 0x80) ? true : false;
}

//--------------------------------------------------
//�@�}�E�X�|�C���^�[�̈ʒu
//--------------------------------------------------
D3DXVECTOR3 GetMouse(void)
{
	//��ʏ�̃}�E�X�|�C���^�[�̈ʒu
	GetCursorPos(&s_MousePos);
	//�E�B���h�E��̃}�E�X�|�C���^�[�̈ʒu
	ScreenToClient(s_hMouseWnd, &s_MousePos);

	return D3DXVECTOR3((float)s_MousePos.x, (float)s_MousePos.y, 0.0f);
}

//--------------------------------------------------
// �}�E�X�̃z�C�[���̓������m
//--------------------------------------------------
int GetMouseWheel(void)
{
	return (int)s_aKeyStateMouse.lZ;
}