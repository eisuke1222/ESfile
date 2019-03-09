//=========================================================================================================================
//
// �v���C���[���� [xplayer.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanf�G���[����
#include <stdio.h>					// stdio
#include "xplayer.h"
#include "xweapon.h"
#include "xenemy.h"
#include "xobject.h"
#include "billboardeffect.h"
#include "billboardObject.h"
#include "meshWall.h"
#include "meshfield.h"
#include "shadow.h"
#include "camera.h"
#include "fade.h"
#include "sound.h"
#include "input.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define XPLAYER_UPSPPED_RUN		(1.2f)						// �v���C���[�������x�i����j
#define XPLAYER_DOWNSPPED		(0.15f)						// �v���C���[�������x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
XPLAYER							g_XPlayer;					// �v���C���[���
int								g_nCntTimeXPlayer;			// �v���C���[�J�E���^�[

//=========================================================================================================================
// �v���C���[�̏���������
//=========================================================================================================================
void InitXPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXPlayer;

	g_XPlayer.nMaxPartsNum = 0;		// �v���C���[�p�[�c��
	g_nCntTimeXPlayer = 0;	// �v���C���[�J�E���^�[

	// �t�@�C���ݒ�
	pFile = fopen(XPLAYER_TEXT_INFO, "r");

	// �v���C���[�e�L�X�g�f�[�^�ǂݍ���
	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &g_XPlayer.nMaxPartsNum);	// �ǂݍ���

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			fscanf(pFile, "%s", &g_XPlayer.aModel[nCntXPlayer].FileName[0]);	// �ǂݍ���
		}

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			fscanf(pFile, "%d", &g_XPlayer.aModel[nCntXPlayer].nIndexParent);		// �ǂݍ���
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initpos.x);			// �ǂݍ���
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initpos.y);			// �ǂݍ���
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initpos.z);			// �ǂݍ���
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initrot.x);			// �ǂݍ���
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initrot.y);			// �ǂݍ���
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initrot.z);			// �ǂݍ���
		}

		fscanf(pFile, "%d", &g_XPlayer.nMaxAnimNum);	// �ǂݍ���

		for (int nCntXPlayerAnim = 0; nCntXPlayerAnim < g_XPlayer.nMaxAnimNum; nCntXPlayerAnim++)
		{
			fscanf(pFile, "%d", &g_XPlayer.nAnimloop[nCntXPlayerAnim]);				// �ǂݍ���
			fscanf(pFile, "%d", &g_XPlayer.nAnimkey[nCntXPlayerAnim]);				// �ǂݍ���
			fscanf(pFile, "%d", &g_XPlayer.nAnimBlend[nCntXPlayerAnim]);			// �ǂݍ���

			for (int nCntXPlayerAnimKey = 0; nCntXPlayerAnimKey < g_XPlayer.nAnimkey[nCntXPlayerAnim]; nCntXPlayerAnimKey++)
			{
				fscanf(pFile, "%d", &g_XPlayer.nAnimfram[nCntXPlayerAnim][nCntXPlayerAnimKey]);			// �ǂݍ���

				for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
				{
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].posAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].x);	// �ǂݍ���
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].posAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].y);	// �ǂݍ���
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].posAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].z);	// �ǂݍ���
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].rotAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].x);	// �ǂݍ���
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].rotAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].y);	// �ǂݍ���
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].rotAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].z);	// �ǂݍ���
				}
			}
		}

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	// X�t�@�C���̓ǂݍ���
	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		D3DXLoadMeshFromX
		(
			&g_XPlayer.aModel[nCntXPlayer].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_XPlayer.aModel[nCntXPlayer].pBuffMat,
			NULL,
			&g_XPlayer.aModel[nCntXPlayer].nNumMat,
			&g_XPlayer.aModel[nCntXPlayer].pMesh
		);
	}

	// �ʒu�E�����̏����ݒ�
	g_XPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_XPlayer.vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_XPlayer.vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	g_XPlayer.nAnimChange = 0;
	g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
	g_XPlayer.nAnimkeynow = 0;
	g_XPlayer.nIdxShadow = 0;
	g_XPlayer.nWeapon = WEAPONTYPE_NONE;
	g_XPlayer.nChargeNum = 0;
	g_XPlayer.nCntstate = 0;
	g_XPlayer.state = PLAYERSTATE_NONE;
	g_XPlayer.nLife = 0;
	g_XPlayer.nInitLife = 0;
	g_XPlayer.nWeapon_1Lv = 0;
	g_XPlayer.nWeapon_2Lv = 0;
	g_XPlayer.nWeapon_3Lv = 0;
	g_XPlayer.bAttack = false;
	g_XPlayer.bNextAttack = false;
	g_XPlayer.nCntChargeAttack = 0;
	g_XPlayer.bChargeAttack = false;
	g_XPlayer.bChargeOn = false;
	g_XPlayer.bMove = false;
	g_XPlayer.bUse = false;

	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
		g_XPlayer.aModel[nCntXPlayer].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
		g_XPlayer.aModel[nCntXPlayer].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.aModel[nCntXPlayer].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XPlayer.aModel[nCntXPlayer].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	}

	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�̃|�C���^

	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		// ���_���̎擾
		nNumVtx = g_XPlayer.aModel[nCntXPlayer].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_XPlayer.aModel[nCntXPlayer].pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_XPlayer.aModel[nCntXPlayer].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��
			{
				if (vtx.x > g_XPlayer.aModel[nCntXPlayer].vtxMax.x)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMax.x = vtx.x;
				}
				if (vtx.y > g_XPlayer.aModel[nCntXPlayer].vtxMax.y)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMax.y = vtx.y;
				}
				if (vtx.z > g_XPlayer.aModel[nCntXPlayer].vtxMax.z)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMax.z = vtx.z;
				}

				if (vtx.x < g_XPlayer.aModel[nCntXPlayer].vtxMin.x)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMin.x = vtx.x;
				}
				if (vtx.y < g_XPlayer.aModel[nCntXPlayer].vtxMin.y)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMin.y = vtx.y;
				}
				if (vtx.z < g_XPlayer.aModel[nCntXPlayer].vtxMin.z)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMin.z = vtx.z;
				}
			}

			pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N
		g_XPlayer.aModel[nCntXPlayer].pMesh->UnlockVertexBuffer();
	}

	// ����
	g_XPlayer.vtxMin.x = -20.0f;
	g_XPlayer.vtxMin.y = 0.0f;
	g_XPlayer.vtxMin.z = -20.0f;
	g_XPlayer.vtxMax.x = 20.0f;
	g_XPlayer.vtxMax.y = g_XPlayer.aModel[0].pos.y + g_XPlayer.aModel[0].vtxMax.y + g_XPlayer.aModel[1].pos.y + g_XPlayer.aModel[1].vtxMax.y + g_XPlayer.aModel[2].pos.y + g_XPlayer.aModel[2].vtxMax.y;
	g_XPlayer.vtxMax.z = 20.0f;
}

//=========================================================================================================================
// �v���C���[�̏I������
//=========================================================================================================================
void UninitXPlayer(void)
{
	// �ϐ��錾
	int nCntXPlayer;

	// ���b�V���̊J��
	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		if (g_XPlayer.aModel[nCntXPlayer].pMesh != NULL)
		{
			g_XPlayer.aModel[nCntXPlayer].pMesh->Release();
			g_XPlayer.aModel[nCntXPlayer].pMesh = NULL;
		}
	}

	// �}�e���A���̊J��
	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		if (g_XPlayer.aModel[nCntXPlayer].pBuffMat != NULL)
		{
			g_XPlayer.aModel[nCntXPlayer].pBuffMat->Release();
			g_XPlayer.aModel[nCntXPlayer].pBuffMat = NULL;
		}
	}
}

//=========================================================================================================================
// �v���C���[�̍X�V����
//=========================================================================================================================
void UpdateXPlayer(void)
{
	MODE mode = GetMode();
	GAMESTATE game = GetGameState();
	FADE fade = GetFade();
	Camera *pCamera = GetCamera();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXPlayer;

	DIJOYSTATE pStick = GetStick();

	if (g_XPlayer.bChargeAttack == false)
	{// �U���`�F���W
		g_nCntTimeXPlayer++;
		g_XPlayer.nCntChargeAttack = 0;
	}

	if (g_XPlayer.bUse == true)
	{// �g���Ă���
		switch (mode)
		{
		case MODE_GAME:
			// �Q�[�����[�h
			if (g_XPlayer.bMove == false)
			{// �����Ȃ��ꍇ�@�j���[�g�������[�V����
				g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
			}
			else if (g_XPlayer.bMove == true)
			{
				switch (game)
				{
				case GAMESTATE_TUTORIAL:
					// �Q�[���`���[�g���A��
					if (g_XPlayer.pos.x + g_XPlayer.vtxMax.x > 500.0f)
					{
						g_XPlayer.pos.x = 500.0f - g_XPlayer.vtxMax.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.x + g_XPlayer.vtxMin.x < -500.0f)
					{
						g_XPlayer.pos.x = -500.0f - g_XPlayer.vtxMin.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMax.z > 500.0f)
					{
						g_XPlayer.pos.z = 500.0f - g_XPlayer.vtxMax.z;
						g_XPlayer.move.z = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMin.z < -500.0f)
					{
						g_XPlayer.pos.z = -500.0f - g_XPlayer.vtxMin.z;
						g_XPlayer.move.z = 0.0f;
					}

					break;
				case GAMESTATE_PLAY:
					// �Q�[���v���C
					switch (g_XPlayer.state)
					{// ���
					case PLAYERSTATE_NORMAL:
						break;
					case PLAYERSTATE_DAMAGE:	// �_���[�W

						g_XPlayer.nCntstate++;

						// �_��
						if (g_XPlayer.nCntstate % 5 % 2 == 0)
						{
							g_XPlayer.col.a = 0.3f;
						}
						else
						{
							g_XPlayer.col.a = 1.0f;
						}

						if (g_XPlayer.nCntstate > 20)
						{// ��Ԃ�߂�
							g_XPlayer.nCntstate = 0;
							g_XPlayer.state = PLAYERSTATE_NORMAL;
							g_XPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						}

						break;
					case PLAYERSTATE_DIE:
						break;
					}

					if (g_XPlayer.pos.z + g_XPlayer.vtxMin.z > 1650.0f)
					{// �}�b�v�ړ�
						if (g_XPlayer.pos.x < -500.0f)
						{
							SetPlayerRespawn(1);
						}
						else if (g_XPlayer.pos.x > 500.0f)
						{
							SetPlayerRespawn(2);
						}
						else
						{
							SetPlayerRespawn(0);
						}

						SetFadeGame(GAMESTATE_PLAY);

						g_nCntTimeXPlayer = 1;
						g_XPlayer.nAnimkeynow = 0;
						g_XPlayer.nAnimChange = 1;

						g_XPlayer.bMove = false;
					}

					// �͈̓`�F�b�N
					if (g_XPlayer.pos.x + g_XPlayer.vtxMax.x > 1750.0f)
					{
						g_XPlayer.pos.x = 1750.0f - g_XPlayer.vtxMax.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.x + g_XPlayer.vtxMin.x < -1750.0f)
					{
						g_XPlayer.pos.x = -1750.0f - g_XPlayer.vtxMin.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMax.z > 1750.0f)
					{
						g_XPlayer.pos.z = 1750.0f - g_XPlayer.vtxMax.z;
						g_XPlayer.move.z = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMin.z < -1750.0f)
					{
						g_XPlayer.pos.z = -1750.0f - g_XPlayer.vtxMin.z;
						g_XPlayer.move.z = 0.0f;
					}

					break;
				}

				g_XPlayer.posOld = g_XPlayer.pos;	// �ʒu�ۊ�

				if (g_XPlayer.bAttack == false)
				{// �U�����Ă��Ȃ�
					if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0) || pStick.lY < -100)
					{// �C�ӂ̃L�[�i��j���������ꍇ
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// ����

						if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -100)
						{// �C�ӂ̃L�[�i���j���������ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 100)
						{// �C�ӂ̃L�[�i�E�j���������ꍇ
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else
						{// ��݂̂̏ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 1.0f + pCamera->rot.y);
						}
					}
					else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000) || pStick.lY > 100)
					{// �C�ӂ̃L�[�i���j���������ꍇ
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}
						
						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// ����

						if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -100)
						{// �C�ӂ̃L�[�i���j���������ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 100)
						{// �C�ӂ̃L�[�i�E�j���������ꍇ
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else
						{// ���݂̂̏ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.0f + pCamera->rot.y);
						}
					}
					else if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -100)
					{// �C�ӂ̃L�[�i���j���������ꍇ
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// ����

						if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0 || pStick.lY < -100))
						{// �C�ӂ̃L�[�i��j���������ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000 || pStick.lY > 100))
						{// �C�ӂ̃L�[�i���j���������ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else
						{// ���݂̂̏ꍇ
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.5f + pCamera->rot.y);
						}
					}
					else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 100)
					{// �C�ӂ̃L�[�i�E�j���������ꍇ
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// ����

						if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0 || pStick.lY < -100))
						{// �C�ӂ̃L�[�i��j���������ꍇ
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000 || pStick.lY > 100))
						{// �C�ӂ̃L�[�i���j���������ꍇ
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else
						{// �E�݂̂̏ꍇ
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.5f + pCamera->rot.y);
						}
					}
					else
					{// ����L�[�������Ă��Ȃ�
						if (g_XPlayer.bAttack == false && g_XPlayer.bNextAttack == false)
						{// �j���[�g�������[�V������
							if (g_XPlayer.nAnimnow != PLAYERANIM_NEUTRAL)
							{
								g_nCntTimeXPlayer = 1;
								g_XPlayer.nAnimkeynow = 0;
								g_XPlayer.nAnimChange = 1;
							}

							g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;	// �j���[�g����
						}
					}
				}

				if (g_XPlayer.nAnimnow == PLAYERANIM_WALK)
				{// ����
					if (g_XPlayer.nAnimkeynow % 2 == 0)
					{
						PlaySound(SOUND_LABEL_SE_WOLK);
					}
				}

				if (g_XPlayer.bAttack == false)
				{// �U�����Ă��Ȃ�
					// ����؂�ւ�
					if (GetButtonTrigger(3) == true)
					{
						if (((g_XPlayer.nWeapon + 1) % 3) == WEAPONTYPE_SWORD)
						{
							SetXWeapon(WEAPONTYPE_SWORD);
						}
						else if (((g_XPlayer.nWeapon + 1) % 3) == WEAPONTYPE_BOW)
						{
							SetXWeapon(WEAPONTYPE_BOW);
						}
						else if (((g_XPlayer.nWeapon + 1) % 3) == WEAPONTYPE_WAND)
						{
							SetXWeapon(WEAPONTYPE_WAND);
						}
					}

					if (GetKeyboardTrigger(DIK_1) == true)
					{
						SetXWeapon(WEAPONTYPE_SWORD);
					}
					else if (GetKeyboardTrigger(DIK_2) == true)
					{
						SetXWeapon(WEAPONTYPE_BOW);
					}
					else if (GetKeyboardTrigger(DIK_3) == true)
					{
						SetXWeapon(WEAPONTYPE_WAND);
					}
				}

				if (fade == FADETYPE_NONE)
				{
					if ((GetKeyboardTrigger(DIK_SPACE) == true || GetButtonTrigger(0) == true))
					{// �U��
						if (g_XPlayer.nWeapon == WEAPONTYPE_SWORD)
						{// ��
							if (g_XPlayer.bAttack == false)
							{// ����
								if (g_XPlayer.nAnimnow != PLAYERANIM_SWORD_1 ||
									g_XPlayer.nAnimnow != PLAYERANIM_SWORD_2 ||
									g_XPlayer.nAnimnow != PLAYERANIM_SWORD_3)
								{
									g_XPlayer.bAttack = true;

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimChange = 1;
								}

								g_XPlayer.nAnimkeynow = 0;

								g_XPlayer.nAnimnow = PLAYERANIM_SWORD_1;
							}
							else
							{// �A��
								if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1 ||
									g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2)
								{
									g_XPlayer.bNextAttack = true;
								}
							}
						}
						else if (g_XPlayer.nWeapon == WEAPONTYPE_BOW)
						{// �|
							if (g_XPlayer.bAttack == false)
							{// ����
								if (g_XPlayer.nAnimnow != PLAYERANIM_BOW)
								{
									g_XPlayer.bAttack = true;

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimChange = 1;
								}

								g_XPlayer.nAnimkeynow = 0;

								g_XPlayer.nAnimnow = PLAYERANIM_BOW;
							}
						}
						else if (g_XPlayer.nWeapon == WEAPONTYPE_WAND)
						{// ��
							if (g_XPlayer.bAttack == false)
							{// ����
								if (g_XPlayer.nAnimnow != PLAYERANIM_WAND)
								{
									g_XPlayer.bAttack = true;

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimChange = 1;
								}

								g_XPlayer.nAnimkeynow = 0;

								g_XPlayer.nAnimnow = PLAYERANIM_WAND;
							}
						}
					}
				}

				if (g_XPlayer.bAttack == true)
				{// �U�����Ă�����
					if (g_XPlayer.nWeapon == WEAPONTYPE_SWORD)
					{// ��
						if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1)
						{// 1��
							if (g_XPlayer.nAnimkeynow == 1)
							{
								PlaySound(SOUND_LABEL_SE_ATTACK_0);

								if (g_nCntTimeXPlayer == 2 && g_XPlayer.nWeapon_1Lv >= 12)
								{// ���x���Ŏa��
									SetBillboardObject(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 100.0f, 0, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 100.0f),
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 25.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 25.0f), g_XPlayer.rot,
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), BILLBOARDTEXTURE_SWORD);
								}
							}
						}
						else if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2)
						{// 2��
							if (g_XPlayer.nAnimkeynow == 0)
							{
								PlaySound(SOUND_LABEL_SE_ATTACK_0);

								if (g_nCntTimeXPlayer == 2 && g_XPlayer.nWeapon_1Lv >= 12)
								{// ���x���Ŏa��
									SetBillboardObject(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 100.0f, 0, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 100.0f),
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 25.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 25.0f), g_XPlayer.rot,
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), BILLBOARDTEXTURE_SWORD);
								}
							}
						}
						else if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_3)
						{// 3��
							if (g_XPlayer.nAnimkeynow == 2)
							{
								PlaySound(SOUND_LABEL_SE_ATTACK_0);

								if (g_nCntTimeXPlayer == 2 && g_XPlayer.nWeapon_1Lv >= 6)
								{// ���x���Ŏa��
									SetBillboardObject(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 100.0f, 0, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 100.0f),
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 25.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 25.0f), g_XPlayer.rot,
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), BILLBOARDTEXTURE_SWORD);
								}
							}
						}
					}
					else if (g_XPlayer.nWeapon == WEAPONTYPE_BOW)
					{// ��
 						if (g_XPlayer.nAnimkeynow == 1)
						{
							if ((GetKeyboardPress(DIK_SPACE) == true || GetButtonPress(0) == true))
							{// �`���[�W
								g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

								if (g_XPlayer.bChargeOn == false)
								{
									g_XPlayer.bChargeAttack = true;
									g_XPlayer.bChargeOn = true;
								}
							}
							else
							{// ����
								if (g_XPlayer.bChargeAttack == true && g_XPlayer.nChargeNum != 0)
								{
									g_nCntTimeXPlayer = 0;
									g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
								}

								g_XPlayer.bChargeAttack = false;
							}

							if (g_XPlayer.bChargeAttack == true)
							{// �`���[�W
								if (g_XPlayer.nWeapon_2Lv < 6)
								{// ���x���Ń`���[�W�Ԋu�ύX
									if (g_XPlayer.nCntChargeAttack == 0)
									{
										g_XPlayer.nChargeNum = 0;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 40)
									{
										g_XPlayer.nChargeNum = 1;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 80)
									{
										g_XPlayer.nChargeNum = 2;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);

										g_nCntTimeXPlayer = 0;
										g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
										g_XPlayer.bChargeAttack = false;
									}
								}
								else
								{// ���x���Ń`���[�W�Ԋu�ύX
									if (g_XPlayer.nCntChargeAttack == 0)
									{
										g_XPlayer.nChargeNum = 0;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 20)
									{
										g_XPlayer.nChargeNum = 1;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 40)
									{
										g_XPlayer.nChargeNum = 2;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);

										g_nCntTimeXPlayer = 0;
										g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
										g_XPlayer.bChargeAttack = false;
									}
								}

								g_XPlayer.nCntChargeAttack++;
							}
						}

						// ���x���őł�����ς���
						if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == 1)
						{
							if (g_XPlayer.bChargeAttack == false)
							{
								if (g_XPlayer.nChargeNum == 0)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}
								else if (g_XPlayer.nChargeNum == 1)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
								else if (g_XPlayer.nChargeNum == 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}

								PlaySound(SOUND_LABEL_SE_ATTACK_1);
							}
						}
						else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2 && g_XPlayer.nChargeNum == 2)
						{
							SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));

							PlaySound(SOUND_LABEL_SE_ATTACK_1);
						}
						else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] && g_XPlayer.nChargeNum == 2)
						{
							SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));

							PlaySound(SOUND_LABEL_SE_ATTACK_1);
						}

						// ���x���őł�����ς���
						if (g_XPlayer.nWeapon_2Lv >= 12)
						{
							if (g_XPlayer.nChargeNum == 0)
							{
								if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}
								else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)])
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}
							}
							else if (g_XPlayer.nChargeNum == 1)
							{
								if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
								else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)])
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
							}
							else if (g_XPlayer.nChargeNum == 2)
							{
								if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2 - g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 4)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
								else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] - g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 8)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 8)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 8)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 8)) * 15.0f));
								}
							}
						}
					}
					else if (g_XPlayer.nWeapon == WEAPONTYPE_WAND)
					{// ��
						if (g_XPlayer.nAnimkeynow == 0)
						{
							if ((GetKeyboardPress(DIK_SPACE) == true || GetButtonPress(0) == true))
							{// �`���[�W
								g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

								if (g_XPlayer.bChargeOn == false)
								{
									g_XPlayer.bChargeAttack = true;
									g_XPlayer.bChargeOn = true;
								}
							}
							else
							{// ����
								if (g_XPlayer.bChargeAttack == true)
								{
									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
								}

								g_XPlayer.bChargeAttack = false;
							}

							if (g_XPlayer.bChargeAttack == true)
							{// ���x���ƃ`���[�W�ŕύX
								if (g_XPlayer.nCntChargeAttack == 0)
								{
									g_XPlayer.nChargeNum = 0;
									SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);
									SetBillboardObject(g_XPlayer.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_XPlayer.rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARDTEXTURE_WAND_1);

									PlaySound(SOUND_LABEL_SE_ATTACK_99);

									if (g_XPlayer.nWeapon_3Lv >= 12)
									{
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);

										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
									}
								}
								else if (g_XPlayer.nCntChargeAttack == 60)
								{
									g_XPlayer.nChargeNum = 1;
									SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);
									SetBillboardObject(g_XPlayer.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_XPlayer.rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARDTEXTURE_WAND_2);

									PlaySound(SOUND_LABEL_SE_ATTACK_99);

									if (g_XPlayer.nWeapon_3Lv >= 12)
									{
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);

										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
									}
								}
								else if (g_XPlayer.nCntChargeAttack == 120)
								{
									g_XPlayer.nChargeNum = 2;
									SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);
									SetBillboardObject(g_XPlayer.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_XPlayer.rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARDTEXTURE_WAND_3);

									PlaySound(SOUND_LABEL_SE_ATTACK_99);

									if (g_XPlayer.nWeapon_3Lv >= 12)
									{
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);

										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
									}

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
									g_XPlayer.bChargeAttack = false;
								}

								g_XPlayer.nCntChargeAttack++;
							}
						}
						else if (g_XPlayer.nAnimkeynow == 2 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow])
						{
							if (g_XPlayer.bChargeAttack == false)
							{
								if (g_XPlayer.nChargeNum == 0)
								{
									SetBillboardBulletEffect(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 50.0f, 50, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 50.0f),
										g_XPlayer.rot,
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 5.0f),
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER);
								}
								else if (g_XPlayer.nChargeNum == 1)
								{
									SetBillboardBulletEffect(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 50.0f, 50, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 50.0f),
										g_XPlayer.rot,
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 5.0f),
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), 0.1f, 120, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER);
								}
								else if (g_XPlayer.nChargeNum == 2)
								{
									SetBillboardBulletEffect(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 50.0f, 50, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 50.0f),
										g_XPlayer.rot,
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 5.0f),
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), 0.1f, 100, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER);
								}

								PlaySound(SOUND_LABEL_SE_ATTACK_2);
							}
						}
					}
				}
			}

			break;
		case MODE_RESULT:
			break;
		}

		{
			// �ڕW��������
			if (g_XPlayer.rotDest.y > D3DX_PI)
			{
				g_XPlayer.rotDest.y -= D3DX_PI * 2.0f;
			}
			if (g_XPlayer.rotDest.y < -D3DX_PI)
			{
				g_XPlayer.rotDest.y += D3DX_PI * 2.0f;
			}
			// �����ݒ�
			fDiff = (g_XPlayer.rotDest.y - g_XPlayer.rot.y);

			// ������������
			if (fDiff > D3DX_PI)
			{
				fDiff -= D3DX_PI * 2.0f;
			}
			if (fDiff < -D3DX_PI)
			{
				fDiff += D3DX_PI * 2.0f;
			}
			// ���݌����ݒ�
			g_XPlayer.rot.y += fDiff * 0.1f;

			// ���݌�������
			if (g_XPlayer.rot.y > D3DX_PI)
			{
				g_XPlayer.rot.y -= D3DX_PI * 2.0f;
			}
			if (g_XPlayer.rot.y < -D3DX_PI)
			{
				g_XPlayer.rot.y += D3DX_PI * 2.0f;
			}
		}

		// �ړ��l���
		g_XPlayer.pos += g_XPlayer.move;

		// ��������
		g_XPlayer.move.x += (0.0f - g_XPlayer.move.x) * XPLAYER_DOWNSPPED;
		g_XPlayer.move.z += (0.0f - g_XPlayer.move.z) * XPLAYER_DOWNSPPED;

		if (g_XPlayer.bMove == true)
		{
			// �I�u�W�F�N�g�����蔻��
			CollisionXObject(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
			CollisionItem(&g_XPlayer.pos, &g_XPlayer.move, &g_XPlayer.vtxMax, &g_XPlayer.vtxMin);
			CollisionMeshField(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
			CollisionXEnemy(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);

			if (CollisionXEnemyWeapon(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax) == true ||
				CollisionXWeaponBullet(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax, WEAPON_ENEMY) == true)
			{
				if (g_XPlayer.state == ENEMYSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					g_XPlayer.state = PLAYERSTATE_DAMAGE;
					g_XPlayer.nCntstate = 0;

					g_XPlayer.nLife -= 1;

					if (g_XPlayer.nLife <= 0)
					{
						SetGameState(GAMESTATE_GAMEOVER);
						DeleteXPlayer();
					}
				}
			}

			// �G�����蔻��
			if (CollisionEffect(&g_XPlayer.pos, &g_XPlayer.move, &g_XPlayer.vtxMax, &g_XPlayer.vtxMin, EFFECTBULLET_ENEMY) == true)
			{
				if (g_XPlayer.state == ENEMYSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					g_XPlayer.state = PLAYERSTATE_DAMAGE;
					g_XPlayer.nCntstate = 0;

					if (g_XPlayer.nLife <= 0)
					{
						SetGameState(GAMESTATE_GAMEOVER);
						DeleteXPlayer();
					}
				}
			}
		}

		/*if (CollisionWall(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, NULL, D3DXVECTOR3(0.0f, 0.0f, 0.0f)) == true)
		{
			g_XPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}*/

		// �e
		D3DXVECTOR3 Shadowpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		Shadowpos.x = g_XPlayer.aModel[0].mtxWorld._41;
		Shadowpos.y = g_XPlayer.aModel[0].mtxWorld._42;
		Shadowpos.z = g_XPlayer.aModel[0].mtxWorld._43;

		SetPositionShadow(g_XPlayer.nIdxShadow, Shadowpos, g_XPlayer.rot);	// �e�ʒu�X�V

		// �A�j���[�V����
		if (g_XPlayer.nAnimChange == 1)
		{
			if (g_nCntTimeXPlayer % g_XPlayer.nAnimBlend[g_XPlayer.nAnimnow] == 0)
			{
				g_nCntTimeXPlayer = 0;
				//g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
				g_XPlayer.nAnimChange = 0;
			}
		}
		else
		{
			if (g_nCntTimeXPlayer % g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow] == 0)
			{
				g_nCntTimeXPlayer = 0;
				g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];

				if (g_XPlayer.nAnimloop[g_XPlayer.nAnimnow] != 1 && g_XPlayer.nAnimkeynow == g_XPlayer.nAnimkey[g_XPlayer.nAnimnow] - 1)
				{
					g_nCntTimeXPlayer = 1;
					g_XPlayer.nAnimkeynow = 0;
					g_XPlayer.nAnimChange = 1;

					// �A�j���[�V�����ɂ���ĕύX
					if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1 ||
						g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2 ||
						g_XPlayer.nAnimnow == PLAYERANIM_SWORD_3 ||
						g_XPlayer.nAnimnow == PLAYERANIM_BOW ||
						g_XPlayer.nAnimnow == PLAYERANIM_WAND)
					{
						g_XPlayer.nChargeNum = 0;
						g_XPlayer.bChargeOn = false;
						g_XPlayer.bChargeAttack = false;
						g_XPlayer.bAttack = false;
						DeleteBillboardObject(BILLBOARDTEXTURE_WAND_1);
						DeleteBillboardObject(BILLBOARDTEXTURE_WAND_2);
						DeleteBillboardObject(BILLBOARDTEXTURE_WAND_3);
					}

					if (g_XPlayer.bNextAttack == true)
					{
						if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1)
						{
							g_XPlayer.nAnimnow = PLAYERANIM_SWORD_2;
							g_XPlayer.bNextAttack = false;
							g_XPlayer.bAttack = true;
							g_XPlayer.nAnimChange = 0;
						}
						else if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2)
						{
							g_XPlayer.nAnimnow = PLAYERANIM_SWORD_3;
							g_XPlayer.bNextAttack = false;
							g_XPlayer.bAttack = true;
							g_XPlayer.nAnimChange = 0;
						}
					}
					else
					{
						if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_3)
						{
							g_XPlayer.move.x -= sinf(g_XPlayer.rot.y) * 7.0f;
							g_XPlayer.move.z -= cosf(g_XPlayer.rot.y) * 7.0f;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
					}
				}
			}
		}

		// �u�����h
		if (g_XPlayer.nAnimChange == 1)
		{
			frac = (g_XPlayer.nAnimBlend[g_XPlayer.nAnimnow] - g_nCntTimeXPlayer) * 1.0f;
			g_XPlayer.aModel[0].posDest = g_XPlayer.aModel[0].Initpos + g_XPlayer.aModel[0].posAnim[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow];
		}
		else
		{
			frac = (g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] - g_nCntTimeXPlayer) * 1.0f;
			g_XPlayer.aModel[0].posDest = g_XPlayer.aModel[0].Initpos + g_XPlayer.aModel[0].posAnim[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow + 1) % (g_XPlayer.nAnimkey[g_XPlayer.nAnimnow])];
		}

		// �����ݒ�
		fDiff = (g_XPlayer.aModel[0].posDest.x - g_XPlayer.aModel[0].pos.x);
		// ���݌����ݒ�
		g_XPlayer.aModel[0].pos.x += fDiff / frac;

		// �����ݒ�
		fDiff = (g_XPlayer.aModel[0].posDest.y - g_XPlayer.aModel[0].pos.y);
		// ���݌����ݒ�
		g_XPlayer.aModel[0].pos.y += fDiff / frac;

		// �����ݒ�
		fDiff = (g_XPlayer.aModel[0].posDest.z - g_XPlayer.aModel[0].pos.z);
		// ���݌����ݒ�
		g_XPlayer.aModel[0].pos.z += fDiff / frac;

		// �����ύX
		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			if (g_XPlayer.nAnimChange == 1)
			{
				g_XPlayer.aModel[nCntXPlayer].rotDest = g_XPlayer.aModel[nCntXPlayer].rotAnim[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow];
			}
			else
			{
				g_XPlayer.aModel[nCntXPlayer].rotDest = g_XPlayer.aModel[nCntXPlayer].rotAnim[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow + 1) % (g_XPlayer.nAnimkey[g_XPlayer.nAnimnow])];
			}

			{
				// �ڕW��������
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.x > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.x -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.x < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.x += D3DX_PI * 2.0f;
				}
				// �����ݒ�
				fDiff = (g_XPlayer.aModel[nCntXPlayer].rotDest.x - g_XPlayer.aModel[nCntXPlayer].rot.x);

				// ������������
				if (fDiff > D3DX_PI)
				{
					fDiff -= D3DX_PI * 2.0f;
				}
				if (fDiff < -D3DX_PI)
				{
					fDiff += D3DX_PI * 2.0f;
				}
				// ���݌����ݒ�
				g_XPlayer.aModel[nCntXPlayer].rot.x += fDiff / frac;

				// ���݌�������
				if (g_XPlayer.aModel[nCntXPlayer].rot.x > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.x -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rot.x < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.x += D3DX_PI * 2.0f;
				}

				// �ڕW��������
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.y > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.y -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.y < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.y += D3DX_PI * 2.0f;
				}
				// �����ݒ�
				fDiff = (g_XPlayer.aModel[nCntXPlayer].rotDest.y - g_XPlayer.aModel[nCntXPlayer].rot.y);

				// ������������
				if (fDiff > D3DX_PI)
				{
					fDiff -= D3DX_PI * 2.0f;
				}
				if (fDiff < -D3DX_PI)
				{
					fDiff += D3DX_PI * 2.0f;
				}
				// ���݌����ݒ�
				g_XPlayer.aModel[nCntXPlayer].rot.y += fDiff / frac;

				// ���݌�������
				if (g_XPlayer.aModel[nCntXPlayer].rot.y > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.y -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rot.y < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.y += D3DX_PI * 2.0f;
				}

				// �ڕW��������
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.z > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.z -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.z < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.z += D3DX_PI * 2.0f;
				}
				// �����ݒ�
				fDiff = (g_XPlayer.aModel[nCntXPlayer].rotDest.z - g_XPlayer.aModel[nCntXPlayer].rot.z);

				// ������������
				if (fDiff > D3DX_PI)
				{
					fDiff -= D3DX_PI * 2.0f;
				}
				if (fDiff < -D3DX_PI)
				{
					fDiff += D3DX_PI * 2.0f;
				}
				// ���݌����ݒ�
				g_XPlayer.aModel[nCntXPlayer].rot.z += fDiff / frac;

				// ���݌�������
				if (g_XPlayer.aModel[nCntXPlayer].rot.z > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.z -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rot.z < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.z += D3DX_PI * 2.0f;
				}
			}
		}
	}
}

//=========================================================================================================================
// �v���C���[�̕`�揈��
//=========================================================================================================================
void DrawXPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						//
	int nCntXPlayer;

	if (g_XPlayer.bUse == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_XPlayer.mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll
		(
			&mtxRot,
			g_XPlayer.rot.y,
			g_XPlayer.rot.x,
			g_XPlayer.rot.z
		);

		D3DXMatrixMultiply
		(
			&g_XPlayer.mtxWorld,
			&g_XPlayer.mtxWorld,
			&mtxRot
		);

		// �ʒu�𔽉f
		D3DXMatrixTranslation
		(
			&mtxTrans,
			g_XPlayer.pos.x,
			g_XPlayer.pos.y,
			g_XPlayer.pos.z
		);

		D3DXMatrixMultiply
		(
			&g_XPlayer.mtxWorld,
			&g_XPlayer.mtxWorld,
			&mtxTrans
		);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_XPlayer.mtxWorld);

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			if (g_XPlayer.aModel[nCntXPlayer].nIndexParent < 0)
			{
				mtxParent = g_XPlayer.mtxWorld;
			}
			else
			{
				mtxParent = g_XPlayer.aModel[g_XPlayer.aModel[nCntXPlayer].nIndexParent].mtxWorld;
			}

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_XPlayer.aModel[nCntXPlayer].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_XPlayer.aModel[nCntXPlayer].rot.y,
				g_XPlayer.aModel[nCntXPlayer].rot.x,
				g_XPlayer.aModel[nCntXPlayer].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&mtxRot
			);

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_XPlayer.aModel[nCntXPlayer].pos.x,
				g_XPlayer.aModel[nCntXPlayer].pos.y,
				g_XPlayer.aModel[nCntXPlayer].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&mtxTrans
			);

			D3DXMatrixMultiply
			(
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&mtxParent
			);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_XPlayer.aModel[nCntXPlayer].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pmat = (D3DXMATERIAL*)g_XPlayer.aModel[nCntXPlayer].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_XPlayer.aModel[nCntXPlayer].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pmat[nCntMat].MatD3D.Diffuse.a = g_XPlayer.col.a;
				pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

				// �e�N�X�`���ݒ�
				pDevice->SetTexture(0, NULL);

				// �v���C���[(�p�[�c)�̕`��
				g_XPlayer.aModel[nCntXPlayer].pMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// �v���C���[�̐ݒ菈��
//=========================================================================================================================
void SetXPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	Camera *pCamera = GetCamera();
	int nCntXPlayer;

	if (g_XPlayer.bUse == false)
	{
		g_nCntTimeXPlayer = 0;

		// �ݒ�
		g_XPlayer.pos = pos;
		g_XPlayer.Initpos = g_XPlayer.pos;
		g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.rot = rot;
		g_XPlayer.rotDest = g_XPlayer.rot;
		g_XPlayer.Initrot = g_XPlayer.rot;

		g_XPlayer.nAnimChange = 0;
		g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
		g_XPlayer.nAnimkeynow = 0;
		g_XPlayer.nWeapon = WEAPONTYPE_NONE;
		g_XPlayer.state = PLAYERSTATE_NORMAL;

		g_XPlayer.nLife = 30;
		g_XPlayer.nInitLife = 30;

		g_XPlayer.bAttack = false;

		g_XPlayer.nIdxShadow = SetShadow(g_XPlayer.pos, g_XPlayer.rot,
			D3DXVECTOR3(20.0f, 0.0f, 20.0f));
		g_XPlayer.bUse = true;

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
			g_XPlayer.aModel[nCntXPlayer].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
			g_XPlayer.aModel[nCntXPlayer].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		// �J��������
		if (pCamera->type == CAMERATYPE_GAME)
		{
			pCamera->posV += g_XPlayer.pos;
			pCamera->posR += g_XPlayer.pos;
		}
	}
}

//=========================================================================================================================
// �v���C���[�̎擾����
//=========================================================================================================================
XPLAYER *GetXPlayer(void)
{
	return &g_XPlayer;
}

//=========================================================================================================================
// �v���C���[�̗̑͐ݒ菈��
//=========================================================================================================================
void SetXPlayerHP(int nLife)
{
	g_XPlayer.nLife = nLife;
}

//=========================================================================================================================
// �v���C���[�̗͎̑擾����
//=========================================================================================================================
int GetXPlayerHP(void)
{
	return g_XPlayer.nLife;
}

//=========================================================================================================================
// �v���C���[�̏Փˏ���
//=========================================================================================================================
bool CollisionXPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	bool bLand = false;

	if (g_XPlayer.bUse == true)
	{// �g�p����Ă���
		if (pPos->y + vtxMax->y > g_XPlayer.pos.y + g_XPlayer.vtxMin.y && pPos->y + vtxMin->y < g_XPlayer.pos.y + g_XPlayer.vtxMax.y)
		{// X�͈͊m�F
			if (pPos->x + vtxMax->x > g_XPlayer.pos.x + g_XPlayer.vtxMin.x && pPos->x + vtxMin->x < g_XPlayer.pos.x + g_XPlayer.vtxMax.x)
			{// X�͈͊m�F
				if (pPosOld->z + vtxMin->z >= g_XPlayer.pos.z + g_XPlayer.vtxMax.z && g_XPlayer.pos.z + g_XPlayer.vtxMax.z > pPos->z + vtxMin->z)
				{// Z��ꔻ��
					pPos->z = g_XPlayer.pos.z + g_XPlayer.vtxMax.z - vtxMin->z;
					//pMove->x = 0.0f;
					pMove->z = 0.0f;

					bLand = true;
				}
				else if (pPosOld->z + vtxMax->z <= g_XPlayer.pos.z + g_XPlayer.vtxMin.z && g_XPlayer.pos.z + g_XPlayer.vtxMin.z < pPos->z + vtxMax->z)
				{// Z���ꔻ��
					pPos->z = g_XPlayer.pos.z + g_XPlayer.vtxMin.z - vtxMax->z;
					//pMove->x = 0.0f;
					pMove->z = 0.0f;

					bLand = true;
				}
			}

			if (pPos->z + vtxMax->z > g_XPlayer.pos.z + g_XPlayer.vtxMin.z && pPos->z + vtxMin->z < g_XPlayer.pos.z + g_XPlayer.vtxMax.z)
			{// Z�͈͊m�F
				if (pPosOld->x + vtxMin->x >= g_XPlayer.pos.x + g_XPlayer.vtxMax.x && g_XPlayer.pos.x + g_XPlayer.vtxMax.x > pPos->x + vtxMin->x)
				{// X�E�Ӕ���
					pPos->x = g_XPlayer.pos.x + g_XPlayer.vtxMax.x - vtxMin->x;
					pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
				else if (pPosOld->x + vtxMax->x <= g_XPlayer.pos.x + g_XPlayer.vtxMin.x && g_XPlayer.pos.x + g_XPlayer.vtxMin.x < pPos->x + vtxMax->x)
				{// X���Ӕ���
					pPos->x = g_XPlayer.pos.x + g_XPlayer.vtxMin.x - vtxMax->x;
					pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
			}
		}

		if (pPos->x + vtxMax->x > g_XPlayer.pos.x + g_XPlayer.vtxMin.x && pPos->x + vtxMin->x < g_XPlayer.pos.x + g_XPlayer.vtxMax.x)
		{// X�͈͊m�F
			if (pPos->z + vtxMax->z > g_XPlayer.pos.z + g_XPlayer.vtxMin.z && pPos->z + vtxMin->z < g_XPlayer.pos.z + g_XPlayer.vtxMax.z)
			{// Z�͈͊m�F
				if (pPosOld->y + vtxMin->y >= g_XPlayer.pos.y + g_XPlayer.vtxMax.y && g_XPlayer.pos.y + g_XPlayer.vtxMax.y > pPos->y + vtxMin->y)
				{// X�E�Ӕ���
					pPos->y = g_XPlayer.pos.y + g_XPlayer.vtxMax.y - vtxMin->y;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
				else if (pPosOld->y + vtxMax->y <= g_XPlayer.pos.y + g_XPlayer.vtxMin.y && g_XPlayer.pos.y + g_XPlayer.vtxMin.y < pPos->x + vtxMax->y)
				{// X���Ӕ���
					pPos->y = g_XPlayer.pos.y + g_XPlayer.vtxMin.y - vtxMax->y;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// �v���C���[�̃A�C�e�����菈��
//=========================================================================================================================
void SetHitItem(int nItem)
{
	// ���탌�x��
	if (nItem == 0)
	{
		if (g_XPlayer.nWeapon_1Lv < 15)
		{
			g_XPlayer.nWeapon_1Lv += 1;
		}

		if (g_XPlayer.nWeapon_1Lv % 3 == 0)
		{
			PlaySound(SOUND_LABEL_SE_LVUP);
		}
	}
	else if (nItem == 1)
	{
		if (g_XPlayer.nWeapon_2Lv < 15)
		{
			g_XPlayer.nWeapon_2Lv += 1;
		}

		if (g_XPlayer.nWeapon_2Lv % 3 == 0)
		{
			PlaySound(SOUND_LABEL_SE_LVUP);
		}
	}
	else if (nItem == 2)
	{
		if (g_XPlayer.nWeapon_3Lv < 15)
		{
			g_XPlayer.nWeapon_3Lv += 1;
		}

		if (g_XPlayer.nWeapon_3Lv % 3 == 0)
		{
			PlaySound(SOUND_LABEL_SE_LVUP);
		}
	}
	else if (nItem == 3)
	{
		if (g_XPlayer.nLife < g_XPlayer.nInitLife)
		{
			g_XPlayer.nLife += 3;

			if (g_XPlayer.nLife > g_XPlayer.nInitLife)
			{
				g_XPlayer.nLife = g_XPlayer.nInitLife;
			}
		}
	}
}

void SetWeaponLv(int nWeapon_1, int nWeapon_2, int nWeapon_3)
{
	// ���탌�x���ݒ�
	g_XPlayer.nWeapon_1Lv = nWeapon_1;
	g_XPlayer.nWeapon_2Lv = nWeapon_2;
	g_XPlayer.nWeapon_3Lv = nWeapon_3;
}

//=========================================================================================================================
// �v���C���[�̈ړ�����
//=========================================================================================================================
void SetMove(bool bMove)
{
	g_XPlayer.bMove = bMove;
}

//=========================================================================================================================
// �v���C���[�̔j������
//=========================================================================================================================
void DeleteXPlayer(void)
{
	GAMESTATE game = GetGameState();

	int nCntXPlayer;

	// �����ݒ�
	SetXWeapon(WEAPONTYPE_NONE);
	DeleteShadow(g_XPlayer.nIdxShadow);

	g_nCntTimeXPlayer = 0;

	g_XPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_XPlayer.nAnimChange = 0;
	g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
	g_XPlayer.nAnimkeynow = 0;
	g_XPlayer.nIdxShadow = 0;
	g_XPlayer.nWeapon = WEAPONTYPE_NONE;
	g_XPlayer.nChargeNum = 0;
	g_XPlayer.nCntstate = 0;
	g_XPlayer.state = PLAYERSTATE_NONE;
	g_XPlayer.nLife = 0;
	g_XPlayer.nInitLife = 0;

	if (game == GAMESTATE_PLAY)
	{
		g_XPlayer.nWeapon_1Lv = 0;
		g_XPlayer.nWeapon_2Lv = 0;
		g_XPlayer.nWeapon_3Lv = 0;
	}

	g_XPlayer.bAttack = false;
	g_XPlayer.bNextAttack = false;
	g_XPlayer.nCntChargeAttack = 0;
	g_XPlayer.bChargeAttack = false;
	g_XPlayer.bChargeOn = false;
	g_XPlayer.bMove = false;
	g_XPlayer.bUse = false;

	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
		g_XPlayer.aModel[nCntXPlayer].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
		g_XPlayer.aModel[nCntXPlayer].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}