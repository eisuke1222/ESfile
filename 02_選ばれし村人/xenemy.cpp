//=========================================================================================================================
//
// �G���� [xenemy.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanf�G���[����
#include <stdio.h>					// stdio
#include "xenemy.h"
#include "xobject.h"
#include "xplayer.h"
#include "xweapon.h"
#include "meshfield.h"
#include "billboardobject.h"
#include "billboardeffect.h"
#include "score.h"
#include "shadow.h"
#include "camera.h"
#include "fade.h"
#include "sound.h"
#include "input.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_XENEMY				(10)						// �G�̍ő吔
#define MAX_XENEMY_NUM			(16)						// �e�G�̍ő吔
#define XENEMY_UPSPPED			(1.2f)						// �G�������x
#define XENEMY_DOWNSPPED		(0.15f)						// �G�������x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
XENEMYINI						g_XEnemyIni[MAX_XENEMY];					// �ǂݍ��݃f�[�^
XENEMY							g_XEnemy[MAX_XENEMY][MAX_XENEMY_NUM];		// �G���
int								g_XEnemyNum;								// �G�ő吔
int								g_nSetingNum;								// �ݒ�ԍ�
int								g_RockOn, g_RockOnNum;						// ���b�N�I���ԍ�

//=========================================================================================================================
// �G�̏���������
//=========================================================================================================================
void InitXEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXEnemy, nCntXEnemyParts;

	// �����ݒ�
	g_nSetingNum = 0;
	g_RockOn = 99;
	g_RockOnNum = 99;

	// �t�@�C���ݒ�
	pFile = fopen(XENEMY_TEXT_INFO, "r");

	// �G�e�L�X�g�f�[�^�ǂݍ���
	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &g_XEnemyNum);	// �ǂݍ���

		for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			fscanf(pFile, "%d", &g_XEnemyIni[nCntXEnemy].nSetNum);		// �ǂݍ���
			fscanf(pFile, "%s", &g_XEnemyIni[nCntXEnemy].FileName[0]);	// �ǂݍ���
		}

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�ݒ�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		// �t�@�C���ݒ�
		pFile = fopen(&g_XEnemyIni[nCntXEnemy].FileName[0], "r");

		// �G�e�L�X�g�f�[�^�ǂݍ���
		if (pFile != NULL)
		{// �t�@�C�����J�����ꍇ
			fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nMaxPartsNum);	// �ǂݍ���

			for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
			{
				fscanf(pFile, "%s", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].FileName[0]);	// �ǂݍ���
			}

			for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
			{
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].nIndexParent);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos.x);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos.y);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos.z);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initrot.x);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initrot.y);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initrot.z);	// �ǂݍ���
			}

			fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nMaxAnimNum);	// �ǂݍ���

			for (int nCntXEnemyPartsAnim = 0; nCntXEnemyPartsAnim < g_XEnemy[nCntXEnemy][0].nMaxAnimNum; nCntXEnemyPartsAnim++)
			{
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimloop[nCntXEnemyPartsAnim]);				// �ǂݍ���
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimkey[nCntXEnemyPartsAnim]);				// �ǂݍ���
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimBlend[nCntXEnemyPartsAnim]);				// �ǂݍ���

				for (int nCntXEnemyPartsAnimKey = 0; nCntXEnemyPartsAnimKey < g_XEnemy[nCntXEnemy][0].nAnimkey[nCntXEnemyPartsAnim]; nCntXEnemyPartsAnimKey++)
				{
					fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimfram[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey]);			// �ǂݍ���

					for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
					{
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].posAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].x);	// �ǂݍ���
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].posAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].y);	// �ǂݍ���
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].posAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].z);	// �ǂݍ���
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].x);	// �ǂݍ���
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].y);	// �ǂݍ���
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].z);	// �ǂݍ���
					}
				}
			}

			fclose(pFile);	// �t�@�C�������
		}
		else
		{// �t�@�C�����J���Ȃ��ꍇ
			MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}
	}

	// X�t�@�C���̓ǂݍ���
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			D3DXLoadMeshFromX
			(
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat,
				NULL,
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].nNumMat,
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh
			);
		}
	}

	// �ʒu�E�����̏����ݒ�
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		g_XEnemy[nCntXEnemy][0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_XEnemy[nCntXEnemy][0].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XEnemy[nCntXEnemy][0].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_XEnemy[nCntXEnemy][0].nCntTime = 0;
		g_XEnemy[nCntXEnemy][0].nAnimChange= 0;
		g_XEnemy[nCntXEnemy][0].nAnimnow = ENEMYANIM_NEUTRAL;
		g_XEnemy[nCntXEnemy][0].nAnimkeynow = 0;
		g_XEnemy[nCntXEnemy][0].nIdxShadow = 0;
		g_XEnemy[nCntXEnemy][0].state = ENEMYSTATE_NONE;
		g_XEnemy[nCntXEnemy][0].nCntstate = 0;
		g_XEnemy[nCntXEnemy][0].nLife = 0;
		g_XEnemy[nCntXEnemy][0].nInitLife = 0;
		g_XEnemy[nCntXEnemy][0].nBillBoardHP = 0;
		g_XEnemy[nCntXEnemy][0].bAttack = false;
		g_XEnemy[nCntXEnemy][0].nSetAttack = 1;
		g_XEnemy[nCntXEnemy][0].nCntTimerand = 0;
		g_XEnemy[nCntXEnemy][0].Pattern = ENEMYPATTERN_NONE;
		g_XEnemy[nCntXEnemy][0].bUse = false;

		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pos = g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos;
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rot = g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[0][0];
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		}
	}

	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�̃|�C���^

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			// ���_���̎擾
			nNumVtx = g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��
				{
					if (vtx.x > g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.x)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.x = vtx.x;
					}
					if (vtx.y > g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.y)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.y = vtx.y;
					}
					if (vtx.z > g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.z)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.z = vtx.z;
					}

					if (vtx.x < g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.x)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.x = vtx.x;
					}
					if (vtx.y < g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.y)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.y = vtx.y;
					}
					if (vtx.z < g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.z)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.z = vtx.z;
					}
				}

				pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
			}

			// ���_�o�b�t�@���A�����b�N
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->UnlockVertexBuffer();
		}
	}

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		g_XEnemy[nCntXEnemy][0].vtxMin.x = -20.0f;
		g_XEnemy[nCntXEnemy][0].vtxMin.y = 0.0f;
		g_XEnemy[nCntXEnemy][0].vtxMin.z = -20.0f;
		g_XEnemy[nCntXEnemy][0].vtxMax.x = 20.0f;
		g_XEnemy[nCntXEnemy][0].vtxMax.y = g_XEnemy[nCntXEnemy][0].aModel[0].pos.y + g_XEnemy[nCntXEnemy][0].aModel[1].pos.y + g_XEnemy[nCntXEnemy][0].aModel[2].pos.y + g_XEnemy[nCntXEnemy][0].aModel[2].vtxMax.y;
		g_XEnemy[nCntXEnemy][0].vtxMax.z = 20.0f;

		for (int nCntXEnemyNum = 1; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			g_XEnemy[nCntXEnemy][nCntXEnemyNum] = g_XEnemy[nCntXEnemy][0];
		}
	}
}

//=========================================================================================================================
// �G�̏I������
//=========================================================================================================================
void UninitXEnemy(void)
{
	// �ϐ��錾
	int nCntXEnemy;
	int nCntXEnemyParts;

	// ���b�V���̊J��
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			if (g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh != NULL)
			{
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->Release();
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh = NULL;
			}
		}
	}

	// �}�e���A���̊J��
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			if (g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat != NULL)
			{
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat->Release();
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat = NULL;
			}
		}
	}
}

//=========================================================================================================================
// �G�̍X�V����
//=========================================================================================================================
void UpdateXEnemy(void)
{
	int nCntXEnemy;
	Camera *pCamera = GetCamera();
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXEnemyParts;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// �g���Ă���
				if (game != GAMESTATE_TUTORIAL)
				{// �`���[�g���A���ȊO
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == false)
					{// �U�����Ă��Ȃ�
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand++;

						/*switch (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern)
						{
						case ENEMYPATTERN_NONE:		// �������Ă��Ȃ�

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
							{// ������
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// �j���[�g����
								
							if (nCntXEnemy != ENEMYTYPE_BOSS)
							{// �{�X�ȊO
								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > (rand() % 120 + 60) || g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 240)
								{
									int nRand = rand() % 3;

									// �s���ύX
									if (nRand == 0)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
									}
									else if (nRand == 1)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = (rand() % 628) * 0.01f;
									}

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
								}
							}
							else
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;
							}

							break;
						case ENEMYPATTERN_WALKINIT:		// ��������

							if ((g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x - 5.0f && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x + 5.0f &&
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z - 5.0f && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z + 5.0f) ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 240)
							{// �ύX
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED);

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
							{// ������
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;		// ����

							break;
						case ENEMYPATTERN_WALKRANDOM:	// �ړ������_��

							if (rand() % 120 == 0)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = (rand() % 628) * 0.01f;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED);

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 120)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > (rand() % 60 + 60))
							{
								int nRand = rand() % 3;

								// �s���ύX
								if (nRand == 0)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
								}
								else if (nRand == 1)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
								}
								else
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = (rand() % 628) * 0.01f;
								}

								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
							{// ������
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;			// ����

							break;
						case ENEMYPATTERN_ATTACKSET:	// �U������

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 2.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 2.0f);

							if (nCntXEnemy == ENEMYTYPE_BOSS)
							{// �{�X
								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 1)
								{
									if (pXPlayer->pos.x + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
								else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack >= 2)
								{
									if (pXPlayer->pos.x + 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
							}
							else
							{// ���̑�
								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 300)
								{
									// �s���ύX
									if (rand() % 2 == 0)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}

								if (nCntXEnemy == 0)
								{
									if (pXPlayer->pos.x + 100 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 100 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 100 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 100 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
								else
								{
									if (pXPlayer->pos.x + 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
							}

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
							{// ������
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;		// ����

							break;
						case ENEMYPATTERN_ATTACK:	// �U��

							if (nCntXEnemy == ENEMYTYPE_BOSS)
							{// �{�X
								if (pXPlayer->pos.x + 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
									pXPlayer->pos.z + 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
								}

								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 1)
								{
									// �s���ύX
									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand < 30)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// �j���[�g����
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand == 180)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 2;
										}

 										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 4.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 4.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;	// ����


										if (pXPlayer->pos.x + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
											pXPlayer->pos.z + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

											if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK)
											{
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
											}

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK;	// �U��

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
										}
									}
								}
								else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack >= 2)
								{
									// �s���ύX
									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand < 30)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// �j���[�g����
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK2)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK2;	// �U���Q

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
							}
							else
							{// ���̑�
								if (nCntXEnemy == 0)
								{
									if (pXPlayer->pos.x + 200 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 200 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 200 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 200 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
								else
								{
									if (pXPlayer->pos.x + 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}

								g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand < 38)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
									{// ������
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
									}

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// �j���[�g����
								}
								else
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED);

									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
									{// ������
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
									}

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;	// ����

									if (nCntXEnemy == 0)
									{
										if (pXPlayer->pos.x + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
											pXPlayer->pos.z + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

											if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK)
											{// ������
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
											}

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK;	// �U��

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
										}
									}
									else
									{
										if (pXPlayer->pos.x + 250 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 250 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
											pXPlayer->pos.z + 250 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 250 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

											if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK)
											{// ������
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
											}

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK;	// �U��

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
										}
									}
								}
							}

							break;
						}*/
					}
				}

				switch (g_XEnemy[nCntXEnemy][nCntXEnemyNum].state)
				{
				case ENEMYSTATE_NORMAL:		// �m�[�}��
					break;
				case ENEMYSTATE_DAMAGE:		// �_���[�W

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate++;

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate % 5 % 2 == 0)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].col.a = 0.3f;
					}
					else
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].col.a = 1.0f;
					}

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate > 13)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_NORMAL;		// �m�[�}���ɖ߂�
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;
				case ENEMYSTATE_DIE:	
					break;
				}

				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime++;

				g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos;	// �ʒu�ۊ�

				if (nCntXEnemy == ENEMYTYPE_BOSS)
				{
					// �s���ύX
					/*if (pXPlayer->Initpos.x + 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x && pXPlayer->pos.x - 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x &&
						pXPlayer->Initpos.z + 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z && pXPlayer->pos.z - 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == false)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACKSET ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACK)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 0;
						}
					}
					else */if (pXPlayer->pos.x + 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACKSET &&
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = rand() % 3 + 1;
						}
					}
				}
				else
				{
					// �s���ύX
					if (pXPlayer->pos.x + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACKSET &&
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						}
					}
					else
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACKSET)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						}
						else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						}
					}
				}

				// �ړ��l���
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos += g_XEnemy[nCntXEnemy][nCntXEnemyNum].move;

				// ��������
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x) * XENEMY_DOWNSPPED;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z) * XENEMY_DOWNSPPED;

				{
					// �ڕW��������
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y > D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y -= D3DX_PI * 2.0f;
					}
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y < -D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y += D3DX_PI * 2.0f;
					}
					// �����ݒ�
					fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y - g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y);

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
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y += fDiff * 0.1f;

					// ���݌�������
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y > D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y -= D3DX_PI * 2.0f;
					}
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y < -D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y += D3DX_PI * 2.0f;
					}
				}				

				if (nCntXEnemy == 1)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == true)
					{
						// �|��
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 1)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 1 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == 1)
							{
								SeBulletWeaponEnemy(
								D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41, g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._42 + 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43),
								D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot),
								D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 15.0f));
							}
						}
					}
				}
				else if (nCntXEnemy == 2)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == true)
					{
						// �G�t�F�N�g
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow])
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot,
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
					}
				}
				else if (nCntXEnemy == ENEMYTYPE_BOSS)
				{// �{�X
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == true)
					{
						// �G�t�F�N�g
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 1)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow] ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 6 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow] ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 12 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow])
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
						else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 2)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == 1)
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);

								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
						else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 3)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % 2 == 0)
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
					}
				}

				// �I�u�W�F�N�g�����蔻��
				if (CollisionXObject(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
				}

				if (CollisionXWeapon(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true ||
					CollisionXWeaponBullet(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax, WEAPON_PLAYER) == true)
				{// ���퓖���蔻��
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].state == ENEMYSTATE_NORMAL)
					{
						PlaySound(SOUND_LABEL_SE_DAMAGE);

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_DAMAGE;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;

						if (game == GAMESTATE_PLAY)
						{
							if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_1 ||
								pXPlayer->nAnimnow == PLAYERANIM_SWORD_2)
							{
								HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_1Lv / 3 + 1);
							}
							else if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_3)
							{
								HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_1Lv / 3 + 2);
							}
							else
							{
								HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_2Lv / 3 + 1);
							}
						}

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife <= 0)
						{
							DeleteXEnemy(nCntXEnemy, nCntXEnemyNum);
						}
					}
				}

				// ���b�V���t�B�[���h�����蔻��
				CollisionMeshField(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax);
				
				// �G���m
				Collision(nCntXEnemy, nCntXEnemyNum);

				SetXEnemyCollNum(nCntXEnemy, nCntXEnemyNum);

				// �U�������蔻��
				if (CollisionSword(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos) == true)
				{// ��
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_DAMAGE;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;

					if (game == GAMESTATE_PLAY)
					{
						HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_1Lv / 3);
					}

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife <= 0)
					{
						DeleteXEnemy(nCntXEnemy, nCntXEnemyNum);
					}
				}

				if (CollisionEffect(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, EFFECTBULLET_PLAYER) == true)
				{// �G�t�F�N�g
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].state == ENEMYSTATE_NORMAL)
					{
						PlaySound(SOUND_LABEL_SE_DAMAGE);

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_DAMAGE;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;					

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife <= 0)
						{
							DeleteXEnemy(nCntXEnemy, nCntXEnemyNum);
						}
					}
				}

				// �v���C���[�����蔻��
				if (CollisionXPlayer(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x -= sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z -= cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;
				}

				// �ړ��l���
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos += g_XEnemy[nCntXEnemy][nCntXEnemyNum].move;

				// ��������
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x) * XENEMY_DOWNSPPED;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z) * XENEMY_DOWNSPPED;

				SetPosionHP(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + 80, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP);
				
				// �e
				D3DXVECTOR3 Shadowpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				Shadowpos.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].mtxWorld._41;
				Shadowpos.y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].mtxWorld._42;
				Shadowpos.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].mtxWorld._43;

				SetPositionShadow(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow, Shadowpos, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot);	// �e�ʒu�X�V

				// �A�j���[�V����
				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange == 1)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimBlend[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] == 0)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 0;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow];
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 0;
					}
				}
				else
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow] == 0)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 0;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow];

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimloop[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] != 1 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] - 1)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK2)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = false;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 1;

								if (nCntXEnemy == ENEMYTYPE_BOSS)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = rand() % 3 + 1;
								}
							}

							//g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
						}
					}
				}

				// �u�����h
				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange == 1)
				{
					frac = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimBlend[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] - g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime) * 1.0f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].Initpos + g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow];
				}
				else
				{
					frac = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow)] - g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime) * 1.0f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].Initpos + g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow])];
				}

				// �����ݒ�
				fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.x);
				// ���݌����ݒ�
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.x += fDiff / frac;

				// �����ݒ�
				fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest.y - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.y);
				// ���݌����ݒ�
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.y += fDiff / frac;

				// �����ݒ�
				fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.z);
				// ���݌����ݒ�
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.z += fDiff / frac;

				for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][nCntXEnemyNum].nMaxPartsNum; nCntXEnemyParts++)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow])];

					{
						// �ڕW��������
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x += D3DX_PI * 2.0f;
						}
						// �����ݒ�
						fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x);

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
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x += fDiff / frac;

						// ���݌�������
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x += D3DX_PI * 2.0f;
						}

						// �ڕW��������
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y += D3DX_PI * 2.0f;
						}
						// �����ݒ�
						fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y);

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
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y += fDiff / frac;

						// ���݌�������
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y += D3DX_PI * 2.0f;
						}

						// �ڕW��������
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z += D3DX_PI * 2.0f;
						}
						// �����ݒ�
						fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z);

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
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z += fDiff / frac;

						// ���݌�������
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z += D3DX_PI * 2.0f;
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================================
// �G�̕`�揈��
//=========================================================================================================================
void DrawXEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						// 
	int nCntXEnemy;
	int nCntXEnemyParts;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z
				);

				D3DXMatrixMultiply
				(
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&mtxRot
				);

				// �ʒu�𔽉f
				D3DXMatrixTranslation
				(
					&mtxTrans,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z
				);

				D3DXMatrixMultiply
				(
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&mtxTrans
				);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld);

				for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][nCntXEnemyNum].nMaxPartsNum; nCntXEnemyParts++)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].nIndexParent < 0)
					{
						mtxParent = g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld;
					}
					else
					{
						mtxParent = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].nIndexParent].mtxWorld;
					}

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld);

					// ��]�𔽉f
					D3DXMatrixRotationYawPitchRoll
					(
						&mtxRot,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z
					);

					D3DXMatrixMultiply
					(
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&mtxRot
					);

					// �ʒu�𔽉f
					D3DXMatrixTranslation
					(
						&mtxTrans,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pos.x,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pos.y,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pos.z
					);

					D3DXMatrixMultiply
					(
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&mtxTrans
					);

					D3DXMatrixMultiply
					(
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&mtxParent
					);

					// ���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld);

					// ���݂̃}�e���A�����擾
					pDevice->GetMaterial(&matDef);

					// �}�e���A���f�[�^�ւ̃|�C���^���擾
					pmat = (D3DXMATERIAL*)g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].nNumMat; nCntMat++)
					{
						// �}�e���A���̐ݒ�
						pmat[nCntMat].MatD3D.Diffuse.a = g_XEnemy[nCntXEnemy][nCntXEnemyNum].col.a;
						pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

						// �e�N�X�`���ݒ�
						pDevice->SetTexture(0, NULL);

						// �G(�p�[�c)�̕`��
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pMesh->DrawSubset(nCntMat);
					}

					// �}�e���A�����f�t�H���g�ɖ߂�
					pDevice->SetMaterial(&matDef);
				}
			}
		}
	}
}

//=========================================================================================================================
// �G�̐ݒ菈��
//=========================================================================================================================
void SetXEnemy(int type, D3DXVECTOR3 pos)
{
	GAMESTATE game = GetGameState();
	int nNowMap = GetNowMap();

	for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[type].nSetNum; nCntXEnemyNum++)
	{
		if (g_XEnemy[type][nCntXEnemyNum].bUse == false)
		{
			if (g_RockOn == 99 && g_RockOnNum == 99)
			{
				g_RockOn = type;
				g_RockOnNum = nCntXEnemyNum;
			}

			// �ʒu
			g_XEnemy[type][nCntXEnemyNum].pos = pos;
			g_XEnemy[type][nCntXEnemyNum].Initpos = pos;
			g_XEnemy[type][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// ����
			if (game == GAMESTATE_TUTORIAL)
			{
				g_XEnemy[type][nCntXEnemyNum].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
				g_XEnemy[type][nCntXEnemyNum].rotDest = g_XEnemy[type][nCntXEnemyNum].rot;
			}
			else
			{
				g_XEnemy[type][nCntXEnemyNum].rot = g_XEnemy[type][nCntXEnemyNum].Initrot;
				g_XEnemy[type][nCntXEnemyNum].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			
			// �����ݒ�
			g_XEnemy[type][nCntXEnemyNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_XEnemy[type][nCntXEnemyNum].nIdxShadow = SetShadow(g_XEnemy[type][nCntXEnemyNum].pos, g_XEnemy[type][nCntXEnemyNum].rot,
				D3DXVECTOR3(30.0f, 0.0f, 30.0f));
			g_XEnemy[type][nCntXEnemyNum].nCntTimerand = 0;
			g_XEnemy[type][nCntXEnemyNum].nCntTime = 0;
			g_XEnemy[type][nCntXEnemyNum].state = ENEMYSTATE_NORMAL;
			g_XEnemy[type][nCntXEnemyNum].nCntstate = 0;

			// ���C�t
			if (type == ENEMYTYPE_BOSS)
			{
				g_XEnemy[type][nCntXEnemyNum].nLife = 40;
				g_XEnemy[type][nCntXEnemyNum].nInitLife = 40;
			}
			else
			{
				if (nNowMap <= 3)
				{
					g_XEnemy[type][nCntXEnemyNum].nLife = 10;
					g_XEnemy[type][nCntXEnemyNum].nInitLife = 10;
				}
				else
				{
					g_XEnemy[type][nCntXEnemyNum].nLife = 20;
					g_XEnemy[type][nCntXEnemyNum].nInitLife = 20;
				}
			}

			g_XEnemy[type][nCntXEnemyNum].nBillBoardHP = SetHPNum(D3DXVECTOR3(g_XEnemy[type][nCntXEnemyNum].pos.x, g_XEnemy[type][nCntXEnemyNum].pos.y + 80, g_XEnemy[type][nCntXEnemyNum].pos.z), 
				D3DXVECTOR3(40, 10, 0.0f));
			g_XEnemy[type][nCntXEnemyNum].nAnimChange = 0;
			g_XEnemy[type][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
			g_XEnemy[type][nCntXEnemyNum].nAnimkeynow = 0;
			g_XEnemy[type][nCntXEnemyNum].bAttack = false;
			g_XEnemy[type][nCntXEnemyNum].nSetAttack = 1;
			g_XEnemy[type][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
			g_XEnemy[type][nCntXEnemyNum].bUse = true;

			g_nSetingNum++;

			break;
		}
	}
}

//=========================================================================================================================
// �G�̎擾����
//=========================================================================================================================
XENEMY *GetXEnemy(void)
{
	return &g_XEnemy[0][0];
}

//=========================================================================================================================
// �G�̑����擾����
//=========================================================================================================================
int GetXEnemyNum(void)
{
	return g_nSetingNum;
}

//=========================================================================================================================
// �G�̃��b�N�I���ԍ��擾����
//=========================================================================================================================
D3DXVECTOR3 GetRockOnXEnemy(int nNum)
{
	XPLAYER *pXPlayer = GetXPlayer();
	D3DXVECTOR3 pos1, pos2;

	int nFor = 0;
	int nCntXEnemy;

	// ���b�N�I���ԍ�����
	if (nNum == 0)
	{
		for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			if (nFor == 0)
			{
				for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
					{// �g�p����Ă���
						pos1.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x - pXPlayer->pos.x;
						pos1.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z - pXPlayer->pos.z;

						if (pos1.x < 0)
						{
							pos1.x *= -1;
						}
						if (pos1.z < 0)
						{
							pos1.z *= -1;
						}
						g_RockOn = nCntXEnemy;
						g_RockOnNum = nCntXEnemyNum;

						nFor = 1;

						break;
					}
				}
			}
		}

		for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
			{
				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
				{// �g�p����Ă���
					pos2.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x - pXPlayer->pos.x;
					pos2.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z - pXPlayer->pos.z;

					if (pos2.x < 0)
					{
						pos2.x *= -1;
					}

					if (pos2.z < 0)
					{
						pos2.z *= -1;
					}

					if (pos1.x >= pos2.x &&
						pos1.z >= pos2.z)
					{
						g_RockOn = nCntXEnemy;
						g_RockOnNum = nCntXEnemyNum;
					}
				}
			}
		}
	}

	if (g_XEnemy[g_RockOn][g_RockOnNum].bUse == false)
	{// �g�p����Ă���
		SetCancelRockOn();
	}

	return g_XEnemy[g_RockOn][g_RockOnNum].pos;
}

//=========================================================================================================================
// �G�̃��b�N�I���ύX�擾����
//=========================================================================================================================
void ChangeRockOnXEnemy(int nNum)
{
	int nCntXEnemy;
	bool bChange = false;

	if (nNum == 0)
	{
		for (nCntXEnemy = g_RockOn; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			if (bChange == false)
			{
				if (nCntXEnemy == g_RockOn)
				{
					for (int nCntXEnemyNum = g_RockOnNum + 1; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// �g�p����Ă���
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
				else
				{
					for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// �g�p����Ă���
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}

		if (bChange == false)
		{
			for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
			{
				if (bChange == false)
				{
					for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
					{
						if (nCntXEnemy == g_RockOn && nCntXEnemyNum == g_RockOnNum)
						{
							break;
						}

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// �g�p����Ă���
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}
	}
	if (nNum == 1)
	{
		for (nCntXEnemy = g_RockOn; nCntXEnemy >= 0; nCntXEnemy--)
		{
			if (bChange == false)
			{
				if (nCntXEnemy == g_RockOn)
				{
					for (int nCntXEnemyNum = g_RockOnNum - 1; nCntXEnemyNum >= 0; nCntXEnemyNum--)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// �g�p����Ă���
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
				else
				{
					for (int nCntXEnemyNum = g_XEnemyIni[nCntXEnemy].nSetNum - 1; nCntXEnemyNum >= 0; nCntXEnemyNum--)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// �g�p����Ă���
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}

		if (bChange == false)
		{
			for (nCntXEnemy = g_XEnemyNum - 1; nCntXEnemy >= 0; nCntXEnemy--)
			{
				if (bChange == false)
				{
					for (int nCntXEnemyNum = g_XEnemyIni[nCntXEnemy].nSetNum - 1; nCntXEnemyNum >= 0; nCntXEnemyNum--)
					{
						if (nCntXEnemy == g_RockOn && nCntXEnemyNum == g_RockOnNum)
						{
							break;
						}

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// �g�p����Ă���
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================================
// �G�̏Փˏ���
//=========================================================================================================================
bool Collision(int nCntXEnemy1, int nCntXEnemyNum1)
{
	int nCntXEnemy;
	int nCntHit = 0;
	bool bLand = false;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true && (nCntXEnemy != nCntXEnemy1 || nCntXEnemyNum != nCntXEnemyNum1))
			{// �g�p����Ă���
			 //if (pPos->y + vtxMax->y > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && pPos->y + vtxMin->y < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y)
				{// X�͈͊m�F
					if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
					{// X�͈͊m�F
						if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z >= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z)
						{// Z��ꔻ��
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z;
							//pMove->x = 0.0f;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.z *= -1;

							nCntHit++;

							bLand = true;
						}
						else if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z <= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z)
						{// Z���ꔻ��
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z;
							//pMove->x = 0.0f;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.z *= -1;

							nCntHit++;

							bLand = true;
						}
					}

					if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z�͈͊m�F
						if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x >= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x)
						{// X�E�Ӕ���
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.x *= -1;
							//pMove->z = 0.0f;

							nCntHit++;

							bLand = true;
						}
						else if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x <= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x)
						{// X���Ӕ���
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.x *= -1;
							//pMove->z = 0.0f;

							nCntHit++;

							bLand = true;
						}
					}
				}

				/*if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
				{// X�͈͊m�F
				if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
				{// Z�͈͊m�F
				if (pPosOld->y + vtxMin->y >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y > pPos->y + vtxMin->y)
				{// X�E�Ӕ���
				pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y - vtxMin->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;

				bLand = true;
				}
				else if (pPosOld->y + vtxMax->y <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y < pPos->x + vtxMax->y)
				{// X���Ӕ���
				pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y - vtxMax->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;

				bLand = true;
				}
				}
				}*/


				if (bLand == true && nCntHit % 2 == 0)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x -= sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -1.2f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z -= cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -1.2f;
				}
			}
		}
	}

	return bLand;
}
bool CollisionXEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	int nCntXEnemy;
	bool bLand = false;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// �g�p����Ă���
				//if (pPos->y + vtxMax->y > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && pPos->y + vtxMin->y < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y)
				{// X�͈͊m�F
					if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
					{// X�͈͊m�F
						if (pPosOld->z + vtxMin->z >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z >= pPos->z + vtxMin->z)
						{// Z��ꔻ��
							pPos->z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z - vtxMin->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->z + vtxMax->z <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z <= pPos->z + vtxMax->z)
						{// Z���ꔻ��
							pPos->z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z - vtxMax->z;
							//pMove->x = 0.0f;
 							pMove->z = 0.0f;

							bLand = true;
						}
					}

					if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z�͈͊m�F
						if (pPosOld->x + vtxMin->x >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x >= pPos->x + vtxMin->x)
						{// X�E�Ӕ���
							pPos->x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x - vtxMin->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->x + vtxMax->x <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x <= pPos->x + vtxMax->x)
						{// X���Ӕ���
							pPos->x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x - vtxMax->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
					}
				}

				/*if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
				{// X�͈͊m�F
					if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z�͈͊m�F
						if (pPosOld->y + vtxMin->y >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y > pPos->y + vtxMin->y)
						{// X�E�Ӕ���
							pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y - vtxMin->y;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->y + vtxMax->y <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y < pPos->x + vtxMax->y)
						{// X���Ӕ���
							pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y - vtxMax->y;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
					}
				}*/
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// ����̏Փ˔��菈��
//=========================================================================================================================
bool CollisionXEnemyWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	XPLAYER *pXPlayer = GetXPlayer();
	int nCntXEnemy;
	bool bLand = false;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true && nCntXEnemy == 0 &&
				((g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2)))
			{// �g�p����Ă���
			 //if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
				{// X�͈͊m�F
					//if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 - 10.0f && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 + 10.0f &&
					//	pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 - 10.0f && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 + 10.0f)
					if (pXPlayer->pos.x + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)					
					{// X�͈͊m�F

						pMove->x = sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -5.0f;
						pMove->z = cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -5.0f;

						bLand = true;
					}
				}
			}
			else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true && nCntXEnemy == ENEMYTYPE_BOSS &&
				((g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2) ||
				(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 6) ||
					(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 12)))
			{// �g�p����Ă���
			 //if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
				{// X�͈͊m�F
				 //if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 - 10.0f && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 + 10.0f &&
				 //	pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 - 10.0f && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 + 10.0f)
					if (pXPlayer->pos.x + 150 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 150 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 150 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 150 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
					{// X�͈͊m�F

						pMove->x = sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;
						pMove->z = cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;

						bLand = true;
					}
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// �G�̓����菈��
//=========================================================================================================================
void HitXEnemy(int nCntXEnemy, int nCntXEnemyNum, int nDamage)
{
	if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
	{
		g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife -= nDamage;

		SetUpdateHP(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP, g_XEnemy[nCntXEnemy][nCntXEnemyNum].nInitLife, g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife);
	}
}

//=========================================================================================================================
// �G�̔j������
//=========================================================================================================================
void DeleteXEnemy(int nCntXEnemy, int nCntXEnemyNum)
{
	int nNowMap = GetNowMap();

	g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse = false;
	DeleteShadow(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow);
	DeleteBillboardObject(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP);
	g_nSetingNum--;

	if (nCntXEnemy == ENEMYTYPE_BOSS)
	{
		AddScore(SCORETYPE_SCORE, 5000);
	}
	else if (nNowMap >= 4)
	{
		AddScore(SCORETYPE_SCORE, 3000);
	}
	else
	{
		AddScore(SCORETYPE_SCORE, 1500);
	}

	if (g_RockOn == nCntXEnemy && g_RockOnNum == nCntXEnemyNum)
	{// �g�p����Ă���
		SetCancelRockOn();
	}

	if (rand() % 2 == 0)
	{
		if (nCntXEnemy == 0)
		{
			SetBillboardObject(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_WEAPON_1);
		}
		else if (nCntXEnemy == 1)
		{
			SetBillboardObject(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_WEAPON_2);
		}
		else if (nCntXEnemy == 2)
		{
			SetBillboardObject(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_WEAPON_3);
		}
	}

	if (g_nSetingNum <= 0 && nNowMap >= 5)
	{
		SetGameState(GAMESTATE_GAMECLEAR);
	}
}
void DeleteXEnemy(void)
{
	int nCntXEnemy;

	g_nSetingNum = 0;
	g_RockOn = 99;
	g_RockOnNum = 99;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// �g�p����Ă���
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_NONE;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nInitLife = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = false;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 1;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse = false;
			}
		}
	}
}