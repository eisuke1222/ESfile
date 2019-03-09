//=========================================================================================================================

//
// �J�������� [camera.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "camera.h"				// �J����
#include "input.h"				// ����
#include "billboardobject.h"	// �I�u�W�F�N�g
#include "xplayer.h"			// �v���C���[
#include "xenemy.h"				// �G

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define CAMERA_POS_MOVE		(2.0f)								// �J�����ړ���
#define CAMERA_ROT_MOVE		(0.03f)								// �J������]��
#define CAMERA_ROT_AUTOMOVE	(0.0015f)							// �J����������]��
#define CAMERA_RESETSPEED	(0.08f)								// �J�������Z�b�g���x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
Camera						g_Camera;							// �J�����̏��
int							g_nCntCameraTimer;					// �J�����^�C�}�[
D3DXVECTOR3					g_RockOnpos;

//=========================================================================================================================
// �J�����̏���������
//=========================================================================================================================
void InitCamera(void)
{
	// �����l�ݒ�
	g_Camera.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���_
	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;				// �����_
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// �x�N�g��
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	g_Camera.fDistance = g_Camera.posV.z - g_Camera.posR.z;		// ����
	g_Camera.fResetRot = 0.0f;									// ���Z�b�g����
	g_Camera.fResetRotDest = 0.0f;								// �ڕW���Z�b�g����
	g_Camera.bReset= false;										// ���Z�b�g
	g_Camera.nCntRockOn = 0;									// ���b�N�I���J�E���g
	g_Camera.fRockOnRot = 0.0f;									// ���b�N�I������
	g_Camera.fRockOnRotDest = 0.0f;								// �ڕW���b�N�I������
	g_Camera.bRockOn = false;									// ���b�N�I�����Ă邩
	g_Camera.bRockOnMove = false;								// ���b�N�I�����Ă邩
	g_Camera.type = CAMERATYPE_NONE;							// ���

	g_nCntCameraTimer = 0;										// �J�����^�C�}�[
	g_RockOnpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================================================================================================
// �J�����̏I������
//=========================================================================================================================
void UninitCamera(void)
{

}

//=========================================================================================================================
// �J�����̍X�V����
//=========================================================================================================================
void UpdateCamera(void)
{
	XPLAYER *pXPlayer = GetXPlayer();
	XENEMY *pXEnemy = GetXEnemy();
	int nXEnemyNum = GetXEnemyNum();

	float fDiff = 0.0f, fcof = 0.0f;

	//DIMOUSESTATE2 pMouse = GetMousePointer();
	DIJOYSTATE pStick = GetStick();

	switch (g_Camera.type)
	{
	case CAMERATYPE_NONE:
		break;
	case CAMERATYPE_TITLE:			// �^�C�g��

		// ������]
		g_Camera.rot.y -= CAMERA_ROT_AUTOMOVE;
		g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		break;
	case CAMERATYPE_TITLE_GAME:			// �^�C�g���Q�[��

		g_Camera.posR.z += 5.0f;
		g_Camera.posV.z += 5.0f;

		break;
	case CAMERATYPE_GAME:			// �Q�[��
	{
		if (pXPlayer->bMove == true)
		{// �v���C���[������悤�ɂȂ�����
			if ((GetKeyboardTrigger(DIK_R) == true || GetButtonTrigger(11) == true))
			{// �J�������Z�b�g
				if (g_Camera.bReset == false)
				{// �g�p���Ă��Ȃ��ꍇ
					fDiff = (pXPlayer->rot.y - g_Camera.rot.y + D3DX_PI);
					g_Camera.fResetRot = pXPlayer->rot.y;
					g_Camera.fResetRotDest = fDiff + g_Camera.rot.y;
					// �l����
					if (g_Camera.fResetRotDest > D3DX_PI)
					{
						g_Camera.fResetRotDest -= D3DX_PI * 2.0f;
					}
					if (g_Camera.fResetRotDest < -D3DX_PI)
					{
						g_Camera.fResetRotDest += D3DX_PI * 2.0f;
					}

					g_Camera.bReset = true;
					g_Camera.bRockOn = false;
					g_Camera.bRockOnMove = false;
				}
			}

			if (g_Camera.bReset == true)
			{// �g�p���Ă���ꍇ
				fDiff = (g_Camera.fResetRot - g_Camera.rot.y + D3DX_PI);
				if (fDiff > D3DX_PI)
				{
					fDiff -= D3DX_PI * 2.0f;
				}
				if (fDiff < -D3DX_PI)
				{
					fDiff += D3DX_PI * 2.0f;
				}

				g_Camera.rot.y += fDiff * CAMERA_RESETSPEED;
				if (g_Camera.rot.y > D3DX_PI)
				{
					g_Camera.rot.y -= D3DX_PI * 2.0f;
				}
				if (g_Camera.rot.y < -D3DX_PI)
				{
					g_Camera.rot.y += D3DX_PI * 2.0f;
				}

				// ���_����
				if (pXPlayer->move.x > 0.7f || pXPlayer->move.x < -0.7f || pXPlayer->move.z > 0.7f || pXPlayer->move.z < -0.7f)
				{// �ړ���
					g_Camera.posVDest.x = pXPlayer->pos.x + g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
					g_Camera.posVDest.z = pXPlayer->pos.z + g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
				}
				else
				{// ��~��
					g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
					g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
				}

				if (g_Camera.rot.y > g_Camera.fResetRotDest - 0.05f && g_Camera.rot.y < g_Camera.fResetRotDest + 0.05f)
				{// �l����
					g_Camera.rot.y = g_Camera.fResetRotDest;
					g_Camera.bReset = false;
					g_Camera.bRockOnMove = false;
				}
			}

			// ���_��]
			if ((GetKeyboardPress(DIK_Q) == true || GetButtonPress(4) == true || pStick.lZ < -300))
			{
				g_Camera.rot.y -= CAMERA_ROT_MOVE;
				g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				g_Camera.bRockOn = false;
				g_Camera.bRockOnMove = false;
			}
			if ((GetKeyboardPress(DIK_E) == true || GetButtonPress(5) == true || pStick.lZ > 300))
			{
				g_Camera.rot.y += CAMERA_ROT_MOVE;
				g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				g_Camera.bRockOn = false;
				g_Camera.bRockOnMove = false;
			}

			// �J�����Ǐ]
			if (pXPlayer->move.x > 0.7f || pXPlayer->move.x < -0.7f || pXPlayer->move.z > 0.7f || pXPlayer->move.z < -0.7f)
			{// �ړ���
				g_Camera.posRDest.x = pXPlayer->pos.x - sinf(pXPlayer->rot.y) * 40.0f;
				g_Camera.posRDest.y = pXPlayer->pos.y;
				g_Camera.posRDest.z = pXPlayer->pos.z - cosf(pXPlayer->rot.y) * 40.0f;
				g_Camera.posVDest.x = pXPlayer->pos.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.y = pXPlayer->pos.y;
				g_Camera.posVDest.z = pXPlayer->pos.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				if (g_nCntCameraTimer < 30)
				{
					g_nCntCameraTimer++;
				}

				fcof = 0.1f + (float)(g_nCntCameraTimer * 0.001f);
			}
			else
			{// ��~��
				g_Camera.posRDest.x = pXPlayer->pos.x - sinf(pXPlayer->rot.y);
				g_Camera.posRDest.y = pXPlayer->pos.y;
				g_Camera.posRDest.z = pXPlayer->pos.z - cosf(pXPlayer->rot.y);
				g_Camera.posVDest.x = pXPlayer->pos.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.y = pXPlayer->pos.y;
				g_Camera.posVDest.z = pXPlayer->pos.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				g_nCntCameraTimer = 0;

				fcof = 0.03f;
			}

			if (((GetKeyboardPress(DIK_C) == true || GetKeyboardPress(DIK_V) == true) ||
				(GetButtonPress(6) == true || GetButtonPress(7) == true || GetButtonPress(10) == true)) && nXEnemyNum > 0)
			{// ���b�N�I��
				g_Camera.nCntRockOn++;

				if (g_Camera.nCntRockOn == 20)
				{// �������m�F
					if (g_Camera.bRockOn == false)
					{
						g_RockOnpos = GetRockOnXEnemy(0);

						g_Camera.bRockOnMove = true;

						SetBillboardObject(D3DXVECTOR3(g_RockOnpos.x, g_RockOnpos.y + 100, g_RockOnpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), BILLBOARDTEXTURE_ROCKON);
					}

					g_Camera.bRockOn = g_Camera.bRockOn ? false : true;
				}
			}
			else
			{// �����Ă��Ȃ�
				g_Camera.nCntRockOn = 0;
			}

			if (g_Camera.bRockOn == true)
			{
				if ((GetKeyboardTrigger(DIK_C) == true || GetButtonTrigger(6) == true))
				{
					ChangeRockOnXEnemy(1);
				}
				if ((GetKeyboardTrigger(DIK_V) == true || GetButtonTrigger(7) == true))
				{
					ChangeRockOnXEnemy(0);
				}

				g_RockOnpos = GetRockOnXEnemy(1);

				if (g_Camera.bRockOnMove == true)
				{// ���b�N�I���g�p���Ă���
					pXPlayer->rotDest.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z) + D3DX_PI;

					fDiff = (pXPlayer->rotDest.y - g_Camera.rot.y + D3DX_PI);
					g_Camera.fRockOnRot = pXPlayer->rotDest.y;
					g_Camera.fRockOnRotDest = fDiff + g_Camera.rot.y;
					// �l����
					if (g_Camera.fRockOnRotDest > D3DX_PI)
					{
						g_Camera.fRockOnRotDest -= D3DX_PI * 2.0f;
					}
					if (g_Camera.fRockOnRotDest < -D3DX_PI)
					{
						g_Camera.fRockOnRotDest += D3DX_PI * 2.0f;
					}

					fDiff = (g_Camera.fRockOnRot - g_Camera.rot.y + D3DX_PI);
					if (fDiff > D3DX_PI)
					{
						fDiff -= D3DX_PI * 2.0f;
					}
					if (fDiff < -D3DX_PI)
					{
						fDiff += D3DX_PI * 2.0f;
					}

					g_Camera.rot.y += fDiff * 0.2f;
					if (g_Camera.rot.y > D3DX_PI)
					{
						g_Camera.rot.y -= D3DX_PI * 2.0f;
					}
					if (g_Camera.rot.y < -D3DX_PI)
					{
						g_Camera.rot.y += D3DX_PI * 2.0f;
					}

					// ���_����

					{// ��~��
						g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
						g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
					}

					if (g_Camera.rot.y > g_Camera.fRockOnRotDest - 0.03f && g_Camera.rot.y < g_Camera.fRockOnRotDest + 0.03f)
					{// �l����
						g_Camera.rot.y = g_Camera.fRockOnRotDest;

						pXPlayer->rotDest.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z) + D3DX_PI;
						g_Camera.rot.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z);

						g_Camera.bRockOnMove = false;
					}

					fcof = 1.0f;
				}
				else
				{
					pXPlayer->rotDest.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z) + D3DX_PI;
					g_Camera.rot.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z);

					// ���_����
					g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
					g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
				}

				SetPosionRockOn(D3DXVECTOR3(g_RockOnpos.x, g_RockOnpos.y + 100, g_RockOnpos.z));
			}
			else
			{
				DeleteBillboardObject(BILLBOARDTEXTURE_ROCKON);
			}

			// �l���
			g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * fcof;
			g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * fcof;
			g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * fcof;
			g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * fcof;
		}
	}

	break;
	case CAMERATYPE_RESULT:			// ���U���g
		break;
	case CAMERATYPE_RANKING:		// �����L���O

		// ������]
		g_Camera.rot.y += CAMERA_ROT_AUTOMOVE;
		g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		break;
	}

	// �J������������
	if (g_Camera.rot.y > D3DX_PI)
	{
		g_Camera.rot.y -= D3DX_PI * 2.0f;
	}
	if (g_Camera.rot.y < -D3DX_PI)
	{
		g_Camera.rot.y += D3DX_PI * 2.0f;
	}
}

//=========================================================================================================================
// �J�����̐ݒ菈��
//=========================================================================================================================
void SetUpCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	(
		&g_Camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		7500.0f
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH
	(
		&g_Camera.mtxView,
		&g_Camera.posV,
		&g_Camera.posR,
		&g_Camera.vecU
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//=========================================================================================================================
// �J�����̎擾����
//=========================================================================================================================
Camera *GetCamera(void)
{
	return &g_Camera;
}

//=========================================================================================================================
// �J�����̐ݒ菈��
//=========================================================================================================================
void SetCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR, CAMERATYPE type)
{
	g_Camera.posV = posV;										// ���_
	g_Camera.posR = posR;										// �����_
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// �x�N�g��
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	g_Camera.fDistance = g_Camera.posV.z - g_Camera.posR.z;		// ����
	g_Camera.type = type;										// ���

	g_nCntCameraTimer = 0;										// �J�����^�C�}�[
}

//=========================================================================================================================
// ���b�N�I����������
//=========================================================================================================================
void SetCancelRockOn(void)
{
	g_Camera.bRockOn = false;
	g_Camera.bRockOnMove = false;
}