//=========================================================================================================================
// 
// �}�b�v���� [map.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "map.h"		// �}�b�v
#include "player.h"		// �v���C���[
#include "input.h"		// ����
#include "bgwindow.h"	// �w�i
#include "fade.h"		// �t�F�[�h
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_MAP_POLYGON			(10)									// �|���S���̍ő吔
#define MAP_TEXTURE_NAME1		"data\\TEXTURE\\gamemappoint000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��1
#define MAP_WIDTH				(40)									// �v���C���[����
#define MAP_HEIGHT				(40)									// �v���C���[����
#define MAP_TEXTURE_SPEED		(10)									// �e�N�X�`���A�j���[�V�����X�s�[�h�i�e���x�j
#define MAP_TEXTURE_PATTERN		(5)										// �e�N�X�`���A�j���[�V�����p�^�[����
#define MAP_TEXTURE_UV_U		(0.2f)									// �e�N�X�`���A�j���[�V����U�͈�
#define MAP_TEXTURE_UV_V		(0.5f)									// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureMap = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffMap = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
MAP								g_Map[MAX_MAP_POLYGON];					// �}�b�v���
int								g_nCntSetMap;							// �}�b�v����
int								g_nSelectMap;							// �}�b�v�I��

//=========================================================================================================================
// �}�b�v����������
//=========================================================================================================================
void InitMap(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntMap;		// �}�b�v�J�E���^

	// �}�b�v��񏉊���
	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{// �}�b�v���J�E���g
		g_Map[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Map[nCntMap].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Map[nCntMap].nCntAnim = 0;
		g_Map[nCntMap].nNumAnim = 0;
		g_Map[nCntMap].bSelect = false;
		g_Map[nCntMap].bUse = false;
	}
	g_nCntSetMap = 0;
	g_nSelectMap = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MAP_TEXTURE_NAME1, &g_pTextureMap);	// �v���C���[

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_MAP_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffMap->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{// �}�b�v���J�E���g
		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// ���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(MAP_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, MAP_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(MAP_TEXTURE_UV_U, MAP_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMap->Unlock();
}

//=========================================================================================================================
// �}�b�v�I������
//=========================================================================================================================
void UninitMap(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureMap != NULL)
	{
		g_pTextureMap->Release();
		g_pTextureMap = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMap != NULL)
	{
		g_pVtxBuffMap->Release();
		g_pVtxBuffMap = NULL;
	}
}

//=========================================================================================================================
// �}�b�v�X�V����
//=========================================================================================================================
void UpdateMap(void)
{
	//�ϐ��錾
	int nCntMap;
	PLAYER *pPlayer = GetPlayer();		// �v���C���[���
	FADE fade = GetFade();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffMap->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (fade != FADE_OUT)
	{
		if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_LEFT) == true || GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_A) == true)
		{// �C�ӂ̃L�[�i��A���AW�AD�j���������ꍇ
			PlaySound(SOUND_LABEL_SE_SELECT002);

			g_nSelectMap = (g_nSelectMap + (g_nCntSetMap - 1)) % g_nCntSetMap;	// �I����i�߂�

			pPlayer->move.x = sinf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;
			pPlayer->move.y = cosf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;

			if (pPlayer->pos.x < g_Map[g_nSelectMap].pos.x)
			{
				pPlayer->nDinectionMove = 0;
			}
			else
			{
				pPlayer->nDinectionMove = 1;
			}
		}
		if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_RIGHT) == true || GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_D) == true)
		{// �C�ӂ̃L�[�i���A�E�AS�AA�j���������ꍇ
			PlaySound(SOUND_LABEL_SE_SELECT002);

			g_nSelectMap = (g_nSelectMap + 1) % g_nCntSetMap;	// �I����i�߂�

			pPlayer->move.x = sinf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;
			pPlayer->move.y = cosf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;

			if (pPlayer->pos.x < g_Map[g_nSelectMap].pos.x)
			{
				pPlayer->nDinectionMove = 0;
			}
			else
			{
				pPlayer->nDinectionMove = 1;
			}
		}
	}

	if (pPlayer->pos.x >= g_Map[g_nSelectMap].pos.x - 5 && pPlayer->pos.x <= g_Map[g_nSelectMap].pos.x + 5 &&
		pPlayer->pos.y >= g_Map[g_nSelectMap].pos.y - 5 && pPlayer->pos.y <= g_Map[g_nSelectMap].pos.y + 5)
	{
		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{// �}�b�v���J�E���^
		if (g_Map[nCntMap].bUse == true)
		{// �g�p���Ă���ꍇ
			if (nCntMap == g_nSelectMap)
			{// �I���}�b�v
				g_Map[nCntMap].nCntAnim++;		// �J�E���^��i�߂�
				if (g_Map[nCntMap].nCntAnim % MAP_TEXTURE_SPEED == 0)
				{// �A�j���[�V����
					g_Map[nCntMap].nNumAnim = (g_Map[nCntMap].nNumAnim + 1) % MAP_TEXTURE_PATTERN;
				}

				// �e�N�X�`���ݒ�
				pVtx[nCntMap * 4].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5)* MAP_TEXTURE_UV_U), (MAP_TEXTURE_UV_V));
				pVtx[nCntMap * 4 + 1].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5) * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, (MAP_TEXTURE_UV_V));
				pVtx[nCntMap * 4 + 2].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5) * MAP_TEXTURE_UV_U), (MAP_TEXTURE_UV_V + MAP_TEXTURE_UV_V));
				pVtx[nCntMap * 4 + 3].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5) * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, (MAP_TEXTURE_UV_V + MAP_TEXTURE_UV_V));
			}
			else
			{// ����ȊO
				g_Map[nCntMap].nNumAnim = 4;		// �A�j���[�V�����ݒ�

				// �e�N�X�`���ݒ�
				pVtx[nCntMap * 4].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim* MAP_TEXTURE_UV_U), 0.0f);
				pVtx[nCntMap * 4 + 1].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, 0.0f);
				pVtx[nCntMap * 4 + 2].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim * MAP_TEXTURE_UV_U), MAP_TEXTURE_UV_V);
				pVtx[nCntMap * 4 + 3].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, MAP_TEXTURE_UV_V);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMap->Unlock();

	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{// �C�ӂ̃L�[�iENTER�j���������ꍇ
			PlaySound(SOUND_LABEL_SE_DECIDE);
			SetNumStage(g_nSelectMap);				// �Q�[���X�e�[�W�ݒ�
			SetFadeGame(GAMESTATE_GAMEPLAY);		// �Q�[���v���C��
		}
	}
}

//=========================================================================================================================
// �}�b�v�`�揈��
//=========================================================================================================================
void DrawMap(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntMap;		// �}�b�v�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffMap,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureMap);

	// �}�b�v�̕`��
	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{
		if (g_Map[nCntMap].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMap * 4, 2);
		}
	}
}

//=========================================================================================================================
// �}�b�v�ݒ菈��
//=========================================================================================================================
void SetMap(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	int nCntMap;
	PLAYER *pPlayer = GetPlayer();
	bool *aClearStage = GetStageClearState();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffMap->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{
		if (g_Map[nCntMap].bUse == false)
		{
			g_Map[nCntMap].pos = pos;
			g_Map[nCntMap].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_Map[nCntMap].nCntAnim = 0;
			g_Map[nCntMap].nNumAnim = 0;
			g_Map[nCntMap].bSelect = false;
			g_Map[nCntMap].bUse = true;
			
			g_nCntSetMap++;

			if (aClearStage[nCntMap] == true)
			{
				SetBGWindow(D3DXVECTOR3(g_Map[nCntMap].pos.x - 30, g_Map[nCntMap].pos.y - 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f, 50.0f, BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL);
			}

			// ���_�ݒ�
			pVtx[nCntMap * 4].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x - MAP_WIDTH, g_Map[nCntMap].pos.y - MAP_HEIGHT, 0.0f);
			pVtx[nCntMap * 4 + 1].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x + MAP_WIDTH, g_Map[nCntMap].pos.y - MAP_HEIGHT, 0.0f);
			pVtx[nCntMap * 4 + 2].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x - MAP_WIDTH, g_Map[nCntMap].pos.y + MAP_HEIGHT, 0.0f);
			pVtx[nCntMap * 4 + 3].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x + MAP_WIDTH, g_Map[nCntMap].pos.y + MAP_HEIGHT, 0.0f);

			break;
		}
	}

	if (g_Map[g_nSelectMap].bUse == true && pPlayer->bDisp == false)
	{
		// �v���C���[�ݒ�
		SetPlayer(g_Map[g_nSelectMap].pos);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMap->Unlock();
}

//=========================================================================================================================
// �}�b�v�j������
//=========================================================================================================================
int  GetSelectMap(void)
{
	return g_nSelectMap;
}

//=========================================================================================================================
// �}�b�v�j������
//=========================================================================================================================
void DeleteMap(void)
{
	// �ϐ��錾
	int nCntMap;

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{
		if (g_Map[nCntMap].bUse == true)
		{
			g_Map[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Map[nCntMap].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_Map[nCntMap].nCntAnim = 0;
			g_Map[nCntMap].nNumAnim = 0;
			g_Map[nCntMap].bSelect = false;
			g_Map[nCntMap].bUse = false;
		}
	}
	g_nCntSetMap = 0;
}