//=========================================================================================================================
//
// �I�u�W�F�N�g���� [xobject.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanf�G���[����
#include <stdio.h>					// stdio
#include "xobject.h"
#include "shadow.h"
#include "meshfield.h"
#include "fade.h"
#include "camera.h"
#include "input.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_XOBJECT				(20)						// �I�u�W�F�N�g�̍ő吔
#define MAX_XOBJECT_NUM			(64)						// �e�I�u�W�F�N�g�̍ő吔
#define XOBJECT_UPSPPED			(0.7f)						// �I�u�W�F�N�g�������x
#define XOBJECT_DOWNSPPED		(0.2f)						// �I�u�W�F�N�g�������x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
XOBJECTINI						g_XObjectIni[MAX_XOBJECT];					// �ǂݍ��݃f�[�^
XOBJECT							g_XObject[MAX_XOBJECT][MAX_XOBJECT_NUM];	// �I�u�W�F�N�g�f�[�^
int								g_MaxXObject;								// �I�u�W�F�N�g�̍ő吔

//=========================================================================================================================
// �I�u�W�F�N�g�̏���������
//=========================================================================================================================
void InitXObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXObject, nCntXObjectModel;

	// �����ݒ�
	g_MaxXObject = 0;

	// �t�@�C���ݒ�
	pFile = fopen(XOBJECT_TEXT_INFO, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &g_MaxXObject);	// �ǂݍ���

		for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
		{
			fscanf(pFile, "%s", &g_XObjectIni[nCntXObject].FileName[0]);	// �ǂݍ���
		}

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�ݒ�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	// �I�u�W�F�N�g�e�L�X�g�f�[�^�ǂݍ���
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		// �t�@�C���ݒ�
		pFile = fopen(&g_XObjectIni[nCntXObject].FileName[0], "r");

		if (pFile != NULL)
		{// �t�@�C�����J�����ꍇ
			fscanf(pFile, "%d", &g_XObject[nCntXObject][0].nMaxModel);	// �ǂݍ���

			for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
			{
				fscanf(pFile, "%s", &g_XObject[nCntXObject][nCntXObjectModel].FileName[0]);	// �ǂݍ���
			}

			for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
			{
				fscanf(pFile, "%d", &g_XObject[nCntXObject][nCntXObjectModel].nOya);		// �ǂݍ���
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initpos.x);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initpos.y);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initpos.z);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initrot.x);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initrot.y);	// �ǂݍ���
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initrot.z);	// �ǂݍ���
			}

			fclose(pFile);	// �t�@�C�������
		}
		else
		{// �t�@�C�����J���Ȃ��ꍇ
			MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}
	}

	// X�t�@�C���̓ǂݍ���
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		D3DXLoadMeshFromX
		(
			&g_XObject[nCntXObject][0].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_XObject[nCntXObject][0].pBuffMat,
			NULL,
			&g_XObject[nCntXObject][0].nNumMat,
			&g_XObject[nCntXObject][0].pMesh
		);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)g_XObject[nCntXObject][0].pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[g_XObject[nCntXObject][0].nNumMat];
		g_XObject[nCntXObject][0].pTexture = new LPDIRECT3DTEXTURE9[g_XObject[nCntXObject][0].nNumMat];

		for (DWORD tex = 0; tex < g_XObject[nCntXObject][0].nNumMat; tex++)
		{
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			g_XObject[nCntXObject][0].pTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&g_XObject[nCntXObject][0].pTexture[tex])))
				{
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}

		delete matDef;
	}

	// �ʒu�E�����̏����ݒ�
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{
			g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
			g_XObject[nCntXObject][nCntXObjectModel].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
			g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
			g_XObject[nCntXObject][nCntXObjectModel].bUse = false;
		}
	}

	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�̃|�C���^

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		// ���_���̎擾
		nNumVtx = g_XObject[nCntXObject][0].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_XObject[nCntXObject][0].pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_XObject[nCntXObject][0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��

			if (vtx.x > g_XObject[nCntXObject][0].vtxMax.x)
			{
				g_XObject[nCntXObject][0].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_XObject[nCntXObject][0].vtxMax.y)
			{
				g_XObject[nCntXObject][0].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_XObject[nCntXObject][0].vtxMax.z)
			{
				g_XObject[nCntXObject][0].vtxMax.z = vtx.z;
			}

			if (vtx.x < g_XObject[nCntXObject][0].vtxMin.x)
			{
				g_XObject[nCntXObject][0].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_XObject[nCntXObject][0].vtxMin.y)
			{
				g_XObject[nCntXObject][0].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_XObject[nCntXObject][0].vtxMin.z)
			{
				g_XObject[nCntXObject][0].vtxMin.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N
		g_XObject[nCntXObject][0].pMesh->UnlockVertexBuffer();
	}

	// �I�u�W�F�N�g����
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		for (nCntXObjectModel = 1; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{
			g_XObject[nCntXObject][nCntXObjectModel].pMesh = g_XObject[nCntXObject][0].pMesh;
			g_XObject[nCntXObject][nCntXObjectModel].pBuffMat = g_XObject[nCntXObject][0].pBuffMat;
			g_XObject[nCntXObject][nCntXObjectModel].nNumMat = g_XObject[nCntXObject][0].nNumMat;
			g_XObject[nCntXObject][nCntXObjectModel].pTexture = g_XObject[nCntXObject][0].pTexture;
			g_XObject[nCntXObject][nCntXObjectModel].vtxMin = g_XObject[nCntXObject][0].vtxMin;
			g_XObject[nCntXObject][nCntXObjectModel].vtxMax = g_XObject[nCntXObject][0].vtxMax;
		}
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̏I������
//=========================================================================================================================
void UninitXObject(void)
{
	// �ϐ��錾
	int nCntXObject;

	// ���b�V���̊J��
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		if (g_XObject[nCntXObject][0].pMesh != NULL)
		{// NULL�ȊO�̏ꍇ
			g_XObject[nCntXObject][0].pMesh->Release();
			g_XObject[nCntXObject][0].pMesh = NULL;
		}
	}

	// �}�e���A���̊J��
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		if (g_XObject[nCntXObject][0].pBuffMat != NULL)
		{// NULL�ȊO�̏ꍇ
			g_XObject[nCntXObject][0].pBuffMat->Release();
			g_XObject[nCntXObject][0].pBuffMat = NULL;
		}
	}

	// �e�N�X�`���̊J��
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		for (DWORD tex = 0; tex < g_XObject[nCntXObject][0].nNumMat; tex++)
		{// �}�e���A�����J�E���g
			if (g_XObject[nCntXObject][0].pTexture[tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				g_XObject[nCntXObject][0].pTexture[tex]->Release();
				g_XObject[nCntXObject][0].pTexture[tex] = NULL;
			}
		}
		if (g_XObject[nCntXObject][0].pTexture != NULL)
		{
			delete[] g_XObject[nCntXObject][0].pTexture;
			g_XObject[nCntXObject][0].pTexture;
		}
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̍X�V����
//=========================================================================================================================
void UpdateXObject(void)
{
	// �ϐ��錾
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// ���f�����J�E���g
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// �g�p���Ă���ꍇ
				SetPositionShadow(g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow, g_XObject[nCntXObject][nCntXObjectModel].pos, g_XObject[nCntXObject][nCntXObjectModel].rot);
			}
		}
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̕`�揈��
//=========================================================================================================================
void DrawXObject(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// ���f�����J�E���g
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// �g�p���Ă���ꍇ
				if (g_XObject[nCntXObject][nCntXObjectModel].nOya < 0)
				{
					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld);
				}
				else
				{
					g_XObject[nCntXObject][nCntXObjectModel].mtxWorld = g_XObject[nCntXObject][g_XObject[nCntXObject][nCntXObjectModel].nOya].mtxWorld;
				}

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_XObject[nCntXObject][nCntXObjectModel].rot.y,
					g_XObject[nCntXObject][nCntXObjectModel].rot.x,
					g_XObject[nCntXObject][nCntXObjectModel].rot.z
				);

				D3DXMatrixMultiply
				(
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&mtxRot
				);

				// �ʒu�𔽉f
				D3DXMatrixTranslation
				(
					&mtxTrans,
					g_XObject[nCntXObject][nCntXObjectModel].pos.x,
					g_XObject[nCntXObject][nCntXObjectModel].pos.y,
					g_XObject[nCntXObject][nCntXObjectModel].pos.z
				);

				D3DXMatrixMultiply
				(
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&mtxTrans
				);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_XObject[nCntXObject][nCntXObjectModel].mtxWorld);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pmat = (D3DXMATERIAL*)g_XObject[nCntXObject][nCntXObjectModel].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_XObject[nCntXObject][nCntXObjectModel].nNumMat; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

					// �e�N�X�`���ݒ�
					pDevice->SetTexture(0, g_XObject[nCntXObject][nCntXObjectModel].pTexture[nCntMat]);
					//pDevice->SetTexture(0, NULL);

					// �I�u�W�F�N�g(�p�[�c)�̕`��
					g_XObject[nCntXObject][nCntXObjectModel].pMesh->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̐ݒ菈��
//=========================================================================================================================
void SetXObject(TEXTTYPE type)
{
	// �ϐ��錾
	int nCntXObjectModel;

	for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[type][0].nMaxModel; nCntXObjectModel++)
	{// ���f�����J�E���g
		if (g_XObject[type][nCntXObjectModel].bUse == false)
		{// �g�p���Ă���ꍇ
			g_XObject[type][nCntXObjectModel].pos = g_XObject[type][nCntXObjectModel].Initpos;
			g_XObject[type][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[type][nCntXObjectModel].rot = g_XObject[type][nCntXObjectModel].Initrot;
			g_XObject[type][nCntXObjectModel].nIdxShadow = SetShadow(g_XObject[type][nCntXObjectModel].pos, g_XObject[type][nCntXObjectModel].rot, g_XObject[type][nCntXObjectModel].vtxMax);
			g_XObject[type][nCntXObjectModel].bUse = true;
		}
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̏Փˏ���
//=========================================================================================================================
bool CollisionXObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	// �ϐ��錾
	int nCntXObject, nCntXObjectModel;
	bool bLand = false;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// ���f�����J�E���g
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// �g�p����Ă���
				//if (pPos->y + vtxMax->y > g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y && pPos->y + vtxMin->y < g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y)
				{// X�͈͊m�F
					if (pPos->x + vtxMax->x > g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && pPos->x + vtxMin->x < g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x)
					{// X�͈͊m�F
						if (pPosOld->z + vtxMin->z >= g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z && g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z >= pPos->z + vtxMin->z)
						{// Z��ꔻ��
							pPos->z = g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z - vtxMin->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;
						}
						else if (pPosOld->z + vtxMax->z <= g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z <= pPos->z + vtxMax->z)
						{// Z���ꔻ��
							pPos->z = g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z - vtxMax->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;
						}
					}

					if (pPos->z + vtxMax->z > g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && pPos->z + vtxMin->z < g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z)
					{// Z�͈͊m�F
						if (pPosOld->x + vtxMin->x >= g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x && g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x >= pPos->x + vtxMin->x)
						{// X�E�Ӕ���
							pPos->x = g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x - vtxMin->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;
						}
						else if (pPosOld->x + vtxMax->x <= g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x <= pPos->x + vtxMax->x)
						{// X���Ӕ���
							pPos->x = g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x - vtxMax->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;
						}
					}
				}

				/*if (pPos->x + vtxMax->x > g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && pPos->x + vtxMin->x < g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x)
				{// X�͈͊m�F
				if (pPos->z + vtxMax->z > g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && pPos->z + vtxMin->z < g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z)
				{// Z�͈͊m�F
				if (pPosOld->y + vtxMin->y >= g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y && g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y > pPos->y + vtxMin->y)
				{// X�E�Ӕ���
				pPos->y = g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y - vtxMin->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;
				}
				else if (pPosOld->y + vtxMax->y <= g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y && g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y < pPos->x + vtxMax->y)
				{// X���Ӕ���
				pPos->y = g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y - vtxMax->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;
				}
				}
				}*/
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// �I�u�W�F�N�g�̔j������
//=========================================================================================================================
void DeleteXObject(void)
{
	// �ϐ��錾
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// �I�u�W�F�N�g���J�E���g
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// ���f�����J�E���g
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// �g�p����Ă���
				g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
				g_XObject[nCntXObject][nCntXObjectModel].bUse = false;
			}
		}
	}
}