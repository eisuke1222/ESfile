//=========================================================================================================================
// 
// �w�i���� [bgwindow.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "bgwindow.h"	// �w�i
#include "input.h"		// ���͏���
#include "map.h"		// �}�b�v
#include "player.h"		// �v���C���[
#include "block.h"		// �u���b�N
#include "item.h"		// �A�C�e��
#include "score.h"		// �X�R�A
#include "fade.h"		// �t�F�[�h
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_BGWINDOW_POLYGON			(64)							// �|���S���̍ő吔
#define BGWINDOW_TEXTURE_SPEED			(5)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define BGWINDOW_TEXTURE_PATTERN		(5)								// �e�N�X�`���A�j���[�V�����p�^�[����
#define BGWINDOW_TEXTURE_UV_U			(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define BGWINDOW_TEXTURE_UV_V			(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define BGWINDOW_TEXTURE_ANIM_UV_U		(0.2f)							// �e�N�X�`���A�j���[�V����U�͈�
#define BGWINDOW_TEXTURE_ANIM_UV_V		(0.5f)							// �e�N�X�`���A�j���[�V����V�͈�
#define BGWINDOW_TEXTURE_STAGE_UV_U		(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define BGWINDOW_TEXTURE_STAGE_UV_V		(0.2f)							// �e�N�X�`���A�j���[�V����V�͈�
#define BGWINDOW_SPEED					(0.002f)						// �w�i�X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBGWindow[BGWINDOWTEXTURE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBGWindow = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
BGWINDOW						g_BGWindow[MAX_BGWINDOW_POLYGON];				// �w�i�̏��
SELECTSTATE						g_SelectState;									// �I���̏��
float							g_fAngleUp;										// �p�x���Z
float							g_fAngleDown;									// �p�x���Z
int								g_nCntBGTimer;									// ���ԃJ�E���^
bool							g_bGoal;										// �S�[������

//=========================================================================================================================
// �w�i����������
//=========================================================================================================================
void InitBGWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾
	int nCntBGWindow;							// �w�i�J�E���^

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		g_BGWindow[nCntBGWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �w�i�̒��S�l
		g_BGWindow[nCntBGWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �w�i�̒��S�l
		g_BGWindow[nCntBGWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
		g_BGWindow[nCntBGWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �w�i�̈ړ��l
		g_BGWindow[nCntBGWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �w�i�F
		g_BGWindow[nCntBGWindow].fWidth = 0;								// ����
		g_BGWindow[nCntBGWindow].fHeight = 0;								// ����
		g_BGWindow[nCntBGWindow].fAngle = 0;								// �p�x
		g_BGWindow[nCntBGWindow].fLenght = 0;								// ����
		g_BGWindow[nCntBGWindow].nCounterAnim = 0;							// �A�j���[�V�����J�E���^
		g_BGWindow[nCntBGWindow].nNumAnim = 0;								// �A�j���[�V�����ԍ�
		g_BGWindow[nCntBGWindow].nSpeedAnim = 0;							// �A�j���[�V�������x
		g_BGWindow[nCntBGWindow].nPatternAnim = 0;							// �A�j���[�V�����p�^�[����
		g_BGWindow[nCntBGWindow].bUse = false;								// �g�p���Ă��邩
	}
	g_SelectState = SELECTSTATE_NORMAL;										// �I�����	
	g_fAngleUp = 0.0f;														// ������
	g_fAngleDown = 0.0f;													// ������
	g_nCntBGTimer = 0;														// ������
	g_bGoal = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_1, &g_pTextureBGWindow[0]);		// �^�C�g���i�w�i�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_2, &g_pTextureBGWindow[1]);		// �^�C�g���i���S�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_3, &g_pTextureBGWindow[2]);		// �^�C�g���i�n�ʁj
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_4, &g_pTextureBGWindow[3]);		// �Q�[���i�}�b�v�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_5, &g_pTextureBGWindow[4]);		// �Q�[���iP�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_6, &g_pTextureBGWindow[5]);		// �Q�[���i�X�e�[�W�ԍ��j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_7, &g_pTextureBGWindow[6]);		// �Q�[���i�n�C�X�R�A�e�L�X�g�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_8, &g_pTextureBGWindow[7]);		// �Q�[���i�c�@�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_9, &g_pTextureBGWindow[8]);		// �Q�[���i�v���C���[�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_10, &g_pTextureBGWindow[9]);	// �Q�[���i�̗́j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_11, &g_pTextureBGWindow[10]);	// �Q�[���i�R�C���j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_12, &g_pTextureBGWindow[11]);	// �Q�[���i�R�C���e�L�X�g�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_13, &g_pTextureBGWindow[12]);	// �Q�[���i�w���v�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_14, &g_pTextureBGWindow[13]);	// �Q�[���i�w���v�i�����j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_15, &g_pTextureBGWindow[14]);	// �Q�[���i�w���v�i����j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_16, &g_pTextureBGWindow[15]);	// �Q�[���i�w���v�i�W�����v�j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_17, &g_pTextureBGWindow[16]);	// �Q�[���i�w���v�i�U���j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_18, &g_pTextureBGWindow[17]);	// �Q�[���i�w�i�i��j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_19, &g_pTextureBGWindow[18]);	// �Q�[���i�w�i�i��j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_20, &g_pTextureBGWindow[19]);	// �Q�[���i�؁i���j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_21, &g_pTextureBGWindow[20]);	// �Q�[���i�؁i���؁j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_22, &g_pTextureBGWindow[21]);	// �Q�[���i�؁i���j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_23, &g_pTextureBGWindow[22]);	// �Q�[���i�����i���j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_24, &g_pTextureBGWindow[23]);	// �Q�[���i�S�[�������j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_25, &g_pTextureBGWindow[24]);	// �Q�[���i�S�[���j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_26, &g_pTextureBGWindow[25]);	// �Q�[���i�S�[���i���j�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_27, &g_pTextureBGWindow[26]);	// ���U���g�i�N���A�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_28, &g_pTextureBGWindow[27]);	// ���U���g�i�I�[�o�[�j
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_29, &g_pTextureBGWindow[28]);	// �I��

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BGWINDOW_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBGWindow,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBGWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		// ���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBGWindow->Unlock();
}

//=========================================================================================================================
// �w�i�I������
//=========================================================================================================================
void UninitBGWindow(void)
{
	// �ϐ��錾
	int nCntTex;		// �e�N�X�`�����J�E���g

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < BGWINDOWTEXTURE_MAX; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTextureBGWindow[nCntTex] != NULL)
		{
			g_pTextureBGWindow[nCntTex]->Release();
			g_pTextureBGWindow[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBGWindow != NULL)
	{
		g_pVtxBuffBGWindow->Release();
		g_pVtxBuffBGWindow = NULL;
	}
}

//=========================================================================================================================
// �w�i�X�V����
//=========================================================================================================================
void UpdateBGWindow(void)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
	int nCntBGWindow;	// �w�i�J�E���^

	MODE mode = GetMode();
	GAMESTATE game = GetGameState();	// �Q�[�����
	bool pPuase = GetPauseState();		// �|�[�Y���
	int nSelectMap = GetSelectMap();	// �I���}�b�v���
	PLAYER *pPlayer = GetPlayer();		// �v���C�����
	int nNumCoin = GetNumCoin();		// �R�C������
	bool bScroll = GetScrollNum();

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBGWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{// �g�p���Ă���ꍇ
			switch (g_BGWindow[nCntBGWindow].texture)
			{// �e�N�X�`�����
			case BGWINDOWTEXTURE_TITLE_BG:		// �^�C�g��BG

				g_BGWindow[nCntBGWindow].tex.x += 0.0008f;

				break;
			case BGWINDOWTEXTURE_TITLE_LOGO:	// �^�C�g�����S
				break;
			case BGWINDOWTEXTURE_TITLE_GOUND:	// �^�C�g���n��

				g_fAngleUp += 0.01f;
				g_fAngleDown -= 0.01f;

				break;
			case BGWINDOWTEXTURE_GAME_MAP:		// �Q�[���}�b�v
				break;
			case BGWINDOWTEXTURE_GAME_NUMPLAYER:// �Q�[���X�e�[�W�ԍ�
				break;
			case BGWINDOWTEXTURE_GAME_HISCORETXT:// �Q�[���n�C�X�R�A�e�L�X�g
				break;
			case BGWINDOWTEXTURE_GAME_NUMSTAGE: // �Q�[���c�@

				g_BGWindow[nCntBGWindow].nNumAnim = nSelectMap;
				
				break;
			case BGWINDOWTEXTURE_GAME_PLAYER:	// �Q�[���v���C���[

				g_BGWindow[nCntBGWindow].nCounterAnim++;
				if (g_BGWindow[nCntBGWindow].nCounterAnim % (BGWINDOW_TEXTURE_SPEED + 1) == 0)
				{
					g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
				}

				break;
			case BGWINDOWTEXTURE_GAME_LIFE:		// �Q�[�����C�t
				break;
			case BGWINDOWTEXTURE_GAME_COIN:		// �Q�[���R�C��

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].nCounterAnim++;

					if (g_BGWindow[nCntBGWindow].nCounterAnim % BGWINDOW_TEXTURE_SPEED == 0)
					{
						g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
						if (g_BGWindow[nCntBGWindow].nNumAnim == 0) { g_BGWindow[nCntBGWindow].nPatternAnim = g_BGWindow[nCntBGWindow].nPatternAnim ^ 1; }
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_COINTXT:		// �Q�[���R�C��
				break;
			case BGWINDOWTEXTURE_GAME_HELP:			// �Q�[���w���v
			case BGWINDOWTEXTURE_GAME_HELPWALK:		// �Q�[���w���v����
			case BGWINDOWTEXTURE_GAME_HELPRUN:		// �Q�[���w���v����
			case BGWINDOWTEXTURE_GAME_HELPJUMP:		// �Q�[���w���v�W�����v
			case BGWINDOWTEXTURE_GAME_HELPATTACK:	// �Q�[���w���v�U��

				if (pPuase == false)
				{
					if (bScroll == false)
					{
						g_BGWindow[nCntBGWindow].pos.x -= pPlayer->move.x / 2;
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE:	// �Q�[���X�e�[�W

				if (pPuase == false)
				{
					if (pPlayer->move.x > 0)
					{
						g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 80000;
					}
					g_BGWindow[nCntBGWindow].tex.x += 0.0001f;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_IWA:	// �Q�[���X�e�[�W�w�i��

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 1050;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER:	// �Q�[���X�e�[�W��(���j

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 3500;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL:	// �Q�[���X�e�[�W�؁i���؁j

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 700;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_TREE_SMALL:	// �Q�[���X�e�[�W�؁i���j
				break;
			case BGWINDOWTEXTURE_GAME_STAGE_SABAKU_YAMA:// �Q�[�������i�R�j

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 10000;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT:	// �Q�[���X�e�[�W�S�[������

				if (pPuase == false)
				{
					if (bScroll == false)
					{
						g_BGWindow[nCntBGWindow].pos.x -= pPlayer->move.x / 2;
					}

					if (nNumCoin <= 0)
					{
						g_BGWindow[nCntBGWindow].col.a -= 0.01f;
						if (g_BGWindow[nCntBGWindow].col.a <= 0.0f)
						{
							DeleteBGWindow(nCntBGWindow);
						}
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGEGOAL:// �Q�[���X�e�[�W�S�[��

				if (pPuase == false || mode == MODE_TITLE)
				{
					if (bScroll == false)
					{
						g_BGWindow[nCntBGWindow].pos.x -= pPlayer->move.x / 2;
					}

					g_BGWindow[nCntBGWindow].nCounterAnim++;

					if (g_BGWindow[nCntBGWindow].nCounterAnim % 8 == 0)
					{
						g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL:// �Q�[���X�e�[�W�S�[���i���j

				g_BGWindow[nCntBGWindow].nCounterAnim++;

				if (g_BGWindow[nCntBGWindow].nCounterAnim % 8 == 0)
				{
					g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
				}

				break;
			case BGWINDOWTEXTURE_RESULT_CLEAR:	// �Q�[���N���A
			case BGWINDOWTEXTURE_RESULT_OVER:	// �Q�[���I�[�o�[
				break;
			case BGWINDOWTEXTURE_SELECT:		// �I��

				g_nCntBGTimer++;

				if (g_SelectState == SELECTSTATE_NORMAL)
				{// �I���m�[�}��
					if (g_nCntBGTimer == 80) { g_nCntBGTimer = 0; }

					// �_��
					if (g_nCntBGTimer < 40) { g_BGWindow[nCntBGWindow].col.a = 1.0f; }
					else { g_BGWindow[nCntBGWindow].col.a = 0.0f; }
				}
				else if (g_SelectState == SELECTSTATE_ENTER)
				{// �I���G���^�[
					if (g_nCntBGTimer == 10)
					{ g_nCntBGTimer = 0; }

					// �_��
					if (g_nCntBGTimer < 5) { g_BGWindow[nCntBGWindow].col.a = 1.0f; }
					else { g_BGWindow[nCntBGWindow].col.a = 0.0f; }
				}

				break;
			}
			
			// ���_�ݒ�
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_TITLE_GOUND)
			{// �^�C�g���n��
				pVtx[nCntBGWindow * 4].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
			}
			else
			{// ����ȊO
				pVtx[nCntBGWindow * 4].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 3].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
			}

			// ���_�J���[�ݒ�
			pVtx[nCntBGWindow * 4].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 1].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 2].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 3].col = g_BGWindow[nCntBGWindow].col;

			// �e�N�X�`���ݒ�
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_PLAYER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_COIN || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{// �Q�[���v���C���[�A�R�C���A�S�[��
				
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V + BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V + BGWINDOW_TEXTURE_ANIM_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_NUMSTAGE)
			{// �Q�[���X�e�[�W�ԍ�
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V + BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V + BGWINDOW_TEXTURE_STAGE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL)
			{
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_IWA || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL)
			{// �Q�[���w�i��A��
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x + g_BGWindow[nCntBGWindow].texUV.x, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x + g_BGWindow[nCntBGWindow].texUV.x, BGWINDOW_TEXTURE_UV_V);
			}
			else
			{// ����ȊO
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U + BGWINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U + BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBGWindow->Unlock();

	if (g_bGoal == true)
	{
		g_nCntBGTimer++;
		if (g_nCntBGTimer == 60)
		{
			PlaySound(SOUND_LABEL_SE_GAME_STAGE_CLEAR);

			SetHiScore();
			SetStageClearState();
			//SetFadeGame(GAMESTATE_MAP);		// �Q�[���}�b�v
		}
	}
}

//=========================================================================================================================
// �w�i�`�揈��
//=========================================================================================================================
void DrawBGWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBGWindow;		// �w�i�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBGWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{// �g�p���Ă���ꍇ
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureBGWindow[g_BGWindow[nCntBGWindow].texture]);

			// �w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBGWindow * 4, 2);
		}
	}
}

//=========================================================================================================================
// �w�i�ݒ菈��
//=========================================================================================================================
void SetBGWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, BGWINDOWTEXTURE texture)
{
	// �ϐ��錾
	int nCntBGWindow;	// �w�i�J�E���^

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBGWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		if (g_BGWindow[nCntBGWindow].bUse == false)
		{// �g�p���Ă��Ȃ��ꍇ
			g_BGWindow[nCntBGWindow].pos = pos;				// ���S�l
			g_BGWindow[nCntBGWindow].tex = pos;				// ���S�l
			g_BGWindow[nCntBGWindow].col = col;				// �F
			g_BGWindow[nCntBGWindow].fWidth = fWidth;		// ����
			g_BGWindow[nCntBGWindow].fHeight = fHeight;		// ����
			g_BGWindow[nCntBGWindow].texture = texture;		// �e�N�X�`��
			g_BGWindow[nCntBGWindow].bUse = true;			// �g�p���Ă����Ԃ�

			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{
				g_bGoal = false;
			}

			// ���_�ݒ�
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_TITLE_GOUND)
			{// ��]
				g_BGWindow[nCntBGWindow].fAngle = atan2f(
					g_BGWindow[nCntBGWindow].pos.x - (g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth),
					g_BGWindow[nCntBGWindow].pos.y - (g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight));
				g_BGWindow[nCntBGWindow].fLenght = sqrtf(
					(g_BGWindow[nCntBGWindow].fWidth * 	g_BGWindow[nCntBGWindow].fWidth) + 
					(g_BGWindow[nCntBGWindow].fHeight * g_BGWindow[nCntBGWindow].fHeight));
				g_fAngleUp = g_BGWindow[nCntBGWindow].fAngle;
				g_fAngleDown = g_BGWindow[nCntBGWindow].fAngle;
				
				pVtx[nCntBGWindow * 4].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
			}
			else
			{// ����ȊO
				pVtx[nCntBGWindow * 4].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 3].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
			}

			// ���_�J���[�ݒ�
			pVtx[nCntBGWindow * 4].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 1].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 2].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 3].col = g_BGWindow[nCntBGWindow].col;
			
			// �e�N�X�`���ݒ�
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_PLAYER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_COIN || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{// �A�j���[�V����
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_ANIM_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_NUMSTAGE)
			{
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, BGWINDOW_TEXTURE_STAGE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL)
			{
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_IWA || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL)
			{
				if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_IWA)
				{
					g_BGWindow[nCntBGWindow].texUV.x = BGWINDOW_TEXTURE_UV_U / (256 / g_BGWindow[nCntBGWindow].fWidth);
				}
				else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER)
				{
					g_BGWindow[nCntBGWindow].texUV.x = BGWINDOW_TEXTURE_UV_U / (400 / g_BGWindow[nCntBGWindow].fWidth);
				}
				else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL)
				{
					g_BGWindow[nCntBGWindow].texUV.x = BGWINDOW_TEXTURE_UV_U / (150 / g_BGWindow[nCntBGWindow].fWidth);
				}

				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].texUV.x, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].texUV.x, BGWINDOW_TEXTURE_UV_V);
			}
			else
			{// ����ȊO
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
			}

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBGWindow->Unlock();
}

//=========================================================================================================================
// �I��ݒ菈��
//=========================================================================================================================
void SetSelectState(SELECTSTATE state)
{
	g_SelectState = state;		// ��Ԃ��L��
	g_nCntBGTimer = 0;			// ���ԃJ�E���^������
}

//=========================================================================================================================
// �I��ݒ菈��
//=========================================================================================================================
void HitPlayerLife(void)
{
	// �ϐ��錾
	int nCntBGWindow;
	
	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		if (g_BGWindow[nCntBGWindow].bUse == true && g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_LIFE)
		{// ���C�t���g�p���Ă���ꍇ
			DeleteBGWindow(nCntBGWindow);		// �w�i�j������
			break;
		}
	}
}

//=========================================================================================================================
// �w�i�Փˏ���
//=========================================================================================================================
bool CollisionBGWindow(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	// �ϐ��錾
	int nCntBGWindow;
	bool bLand = false;
	pPosOld->x -= pMove->x;

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_HELP)
			{
				if (pPos->x + (fWidth / 2) > g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth && pPos->x - (fWidth / 2) < g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth)
				{
					if (pPos->y >= g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight + 23 && g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight + 23 >= pPosOld->y)
					{
						pPos->y = g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight + 23;
						pMove->y = 0.0f;
						bLand = true;
					}
				}
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT)
			{
				if (pPos->x + (fWidth / 2) >= g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth && g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth >= pPosOld->x + (fWidth / 2))
				{
					pPos->x = g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth - (fWidth / 2);
					pMove->x = 0.0f;
				}
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{
				if (pPos->x >= g_BGWindow[nCntBGWindow].pos.x - 10)
				{
					g_bGoal = true;
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// �S�[������擾����
//=========================================================================================================================
bool GetGoalHit(void)
{
	return g_bGoal;
}

//=========================================================================================================================
// �w�i�j������
//=========================================================================================================================
void DeleteBGWindow(int nCntBGWindow)
{
	g_BGWindow[nCntBGWindow].bUse = false;
}
void DeleteBGWindow(void)
{
	// �ϐ��錾
	int nCntBGWindow;		// �w�i�J�E���^

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// �w�i���J�E���g
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{// �g�p���Ă���ꍇ
			g_BGWindow[nCntBGWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �w�i�̒��S�l
			g_BGWindow[nCntBGWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �w�i�̒��S�l
			g_BGWindow[nCntBGWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
			g_BGWindow[nCntBGWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �w�i�̈ړ��l
			g_BGWindow[nCntBGWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �w�i�F
			g_BGWindow[nCntBGWindow].fWidth = 0;								// ����
			g_BGWindow[nCntBGWindow].fHeight = 0;								// ����
			g_BGWindow[nCntBGWindow].fAngle = 0;								// �p�x
			g_BGWindow[nCntBGWindow].fLenght = 0;								// ����
			g_BGWindow[nCntBGWindow].nCounterAnim = 0;							// �A�j���[�V�����J�E���^
			g_BGWindow[nCntBGWindow].nNumAnim = 0;								// �A�j���[�V�����ԍ�
			g_BGWindow[nCntBGWindow].nSpeedAnim = 0;							// �A�j���[�V�������x
			g_BGWindow[nCntBGWindow].nPatternAnim = 0;							// �A�j���[�V�����p�^�[����
			g_BGWindow[nCntBGWindow].bUse = false;								// �g�p���Ă��邩
		}
	}

	g_SelectState = SELECTSTATE_NORMAL;											// �I�����	
	g_fAngleUp = 0.0f;															// ������
	g_fAngleDown = 0.0f;														// ������
	g_nCntBGTimer = 0;															// ������
	g_nCntBGTimer++;
	g_bGoal = false;
}