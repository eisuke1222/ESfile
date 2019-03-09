//=========================================================================================================================
// 
// �E�B���h�E���� [frontwindow.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "window.h"		// �E�B���h�E
#include "xweapon.h"
#include "xplayer.h"	
#include "input.h"		// ���͏���
#include "fade.h"		// �t�F�[�h
#include "sound.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_WINDOW_POLYGON				(64)							// �|���S���̍ő吔
#define WINDOW_TEXTURE_SPEED			(5)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define WINDOW_TEXTURE_PATTERN			(5)								// �e�N�X�`���A�j���[�V�����p�^�[����
#define WINDOW_TEXTURE_UV_U				(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define WINDOW_TEXTURE_UV_V				(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define WINDOW_TEXTURE_SPEED_TEX2		(3)								// �e�N�X�`���A�j���[�V����
#define WINDOW_TEXTURE_PATTERN_TEX2		(20)							// �e�N�X�`���A�j���[�V����
#define WINDOW_TEXTURE_PATTERN_U_TEX2	(2)								// �e�N�X�`���A�j���[�V����U
#define WINDOW_TEXTURE_UV_U_TEX2		(0.5f)							// �e�N�X�`���A�j���[�V����U�͈�
#define WINDOW_TEXTURE_UV_V_TEX2		(0.1f)							// �e�N�X�`���A�j���[�V����V�͈�
#define WINDOW_TEXTURE_PATTERN_1		(4)								// �e�N�X�`���A�j���[�V����
#define WINDOW_TEXTURE_UV_U_1			(0.25f)							// �e�N�X�`���A�j���[�V����U�͈�
#define WINDOW_TEXTURE_UV_V_1			(0.2f)							// �e�N�X�`���A�j���[�V����V�͈�
#define WINDOW_TEXTURE_UV_U_3			(0.2f)							// �e�N�X�`���A�j���[�V����U�͈�
#define WINDOW_TEXTURE_UV_V_3			(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define WINDOW_SPEED					(0.002f)						// �E�B���h�E�X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureWindow[WINDOWTEXTURE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffWindow = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
WINDOW							g_Window[MAX_WINDOW_POLYGON];				// �E�B���h�E�̏��
SELECTSTATE						g_SelectState;								// �I���̏��
float							g_fAngleUp;									// �p�x���Z
float							g_fAngleDown;								// �p�x���Z

//=========================================================================================================================
// �E�B���h�E����������
//=========================================================================================================================
void InitWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾
	int nCntWindow;							// �E�B���h�E�J�E���^

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		g_Window[nCntWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�B���h�E�̒��S�l
		g_Window[nCntWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�B���h�E�̒��S�l
		g_Window[nCntWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
		g_Window[nCntWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�B���h�E�̈ړ��l
		g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �E�B���h�E�F
		g_Window[nCntWindow].fWidth = 0;								// ����
		g_Window[nCntWindow].fHeight = 0;								// ����
		g_Window[nCntWindow].fAngle = 0;								// �p�x
		g_Window[nCntWindow].fLenght = 0;								// ����
		g_Window[nCntWindow].nCntTimer = 0;								// ���ԃJ�E���^
		g_Window[nCntWindow].nCounterAnim = 0;							// �A�j���[�V�����J�E���^
		g_Window[nCntWindow].nNumAnim = 0;								// �A�j���[�V�����ԍ�
		g_Window[nCntWindow].nSpeedAnim = 0;							// �A�j���[�V�������x
		g_Window[nCntWindow].nPatternAnim = 0;							// �A�j���[�V�����p�^�[����
		g_Window[nCntWindow].bUse = false;								// �g�p���Ă��邩
	}
	g_SelectState = SELECTSTATE_NORMAL;									// �I�����	
	g_fAngleUp = 0.0f;													// ������
	g_fAngleDown = 0.0f;												// ������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_1, &g_pTextureWindow[0]);		// �^�C�g���i�^�C�g���j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_2, &g_pTextureWindow[1]);		// �^�C�g���i�A�j���[�V�����j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_3, &g_pTextureWindow[2]);		// �Q�[���i�`���[�g���A���j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_4, &g_pTextureWindow[3]);		// �Q�[���i�`���[�g���A���j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_5, &g_pTextureWindow[4]);		// �Q�[���i�Q�[���`���[�g���A���j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_6, &g_pTextureWindow[5]);		// �Q�[���i�`���[�g���A���R�����g�J���j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_7, &g_pTextureWindow[6]);		// �Q�[���i�`���[�g���A���R�����g����j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_8, &g_pTextureWindow[7]);		// �Q�[���i�`���[�g���A�����b�Z�[�W�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_9, &g_pTextureWindow[8]);		// �Q�[���i�}�b�v�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_10, &g_pTextureWindow[9]);	// �Q�[���i�}�b�v�ʒu�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_11, &g_pTextureWindow[10]);	// �Q�[���iHPBG�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_12, &g_pTextureWindow[11]);	// �Q�[���iHP�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_13, &g_pTextureWindow[12]);	// �Q�[���iHP�}�[�N�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_14, &g_pTextureWindow[13]);	// �Q�[���i����1�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_15, &g_pTextureWindow[14]);	// �Q�[���i����2�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_16, &g_pTextureWindow[15]);	// �Q�[���i����3�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_17, &g_pTextureWindow[16]);	// �Q�[���i����g�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_18, &g_pTextureWindow[17]);	// ���U���g�i�w�i�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_19, &g_pTextureWindow[18]);	// ���U���g�i�N���A�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_20, &g_pTextureWindow[19]);	// ���U���g�i�I�[�o�[�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_21, &g_pTextureWindow[20]);	// ���U���g�i���܂��j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_22, &g_pTextureWindow[21]);	// �����L���O�i�����j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_23, &g_pTextureWindow[22]);	// �����L���O�i1�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_24, &g_pTextureWindow[23]);	// �����L���O�i2�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_25, &g_pTextureWindow[24]);	// �����L���O�i3�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_26, &g_pTextureWindow[25]);	// �����L���O�i4�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_27, &g_pTextureWindow[26]);	// �����L���O�i5�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_28, &g_pTextureWindow[27]);	// �����L���O�i���b�Z�[�WA�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_29, &g_pTextureWindow[28]);	// �����L���O�i���b�Z�[�WB�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_30, &g_pTextureWindow[29]);	// �����L���O�i���b�Z�[�WC�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_31, &g_pTextureWindow[30]);	// �����L���O�i���b�Z�[�WD�j
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_32, &g_pTextureWindow[31]);	// ���b�Z�[�W
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_33, &g_pTextureWindow[32]);	// �I��
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_34, &g_pTextureWindow[33]);	// �I��

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_WINDOW_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWindow,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, WINDOW_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffWindow->Unlock();
}

//=========================================================================================================================
// �E�B���h�E�I������
//=========================================================================================================================
void UninitWindow(void)
{
	// �ϐ��錾
	int nCntTex;		// �e�N�X�`�����J�E���g

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < WINDOWTEXTURE_MAX; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTextureWindow[nCntTex] != NULL)
		{
			g_pTextureWindow[nCntTex]->Release();
			g_pTextureWindow[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffWindow != NULL)
	{
		g_pVtxBuffWindow->Release();
		g_pVtxBuffWindow = NULL;
	}
}

//=========================================================================================================================
// �E�B���h�E�X�V����
//=========================================================================================================================
void UpdateWindow(void)
{
	int nCntWindow;
	MODE mode = GetMode();
	XPLAYER *pXPlayer = GetXPlayer();
	float fHP;
	float fHPRadius;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true)
		{// �g�p���Ă���ꍇ
			switch (g_Window[nCntWindow].texture)
			{
			case WINDOWTEXTURE_TITLE:	// �^�C�g��

				// �g��
				if (g_Window[nCntWindow].fWidth != SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight != SCREEN_HEIGHT / 2 - 100)
				{
					if (g_Window[nCntWindow].fWidth >= SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight >= SCREEN_HEIGHT / 2 - 100)
					{
						g_Window[nCntWindow].fWidth = SCREEN_WIDTH / 2 - 100;
						g_Window[nCntWindow].fHeight = SCREEN_HEIGHT / 2 - 100;
					}
					else
					{
						g_Window[nCntWindow].fWidth += (SCREEN_WIDTH / 2 - 100) * 0.1f;
						g_Window[nCntWindow].fHeight += (SCREEN_HEIGHT / 2 - 100) * 0.1f;
					}
				}

				break;
			case WINDOWTEXTURE_TITLE_ANIM:	// �^�C�g���A�j���[�V����

				// �A�j���[�V����
				g_Window[nCntWindow].nCounterAnim++;

				if (g_Window[nCntWindow].nCounterAnim % WINDOW_TEXTURE_SPEED_TEX2 == 0)
				{
					if (g_Window[nCntWindow].nNumAnim < WINDOW_TEXTURE_PATTERN_TEX2)
					{
						g_Window[nCntWindow].nNumAnim += 1;
					}
					else
					{
						DeleteWindow(nCntWindow);
					}

					if (g_Window[nCntWindow].nNumAnim == 15)
					{
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0, WINDOWTEXTURE_TITLE, SETTEXTURE_TOPFRONT);
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_1, SETTEXTURE_TOPFRONT);
					}
				}

				break;
			case WINDOWTEXTURE_GAME_TUTORIAL:	// �`���[�g���A��

				// �`���[�g���A��
				if (g_Window[nCntWindow].fWidth != SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight != SCREEN_HEIGHT / 2 - 100)
				{
					if (g_Window[nCntWindow].fWidth >= SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight >= SCREEN_HEIGHT / 2 - 100)
					{
						g_Window[nCntWindow].fWidth = SCREEN_WIDTH / 2 - 100;
						g_Window[nCntWindow].fHeight = SCREEN_HEIGHT / 2 - 100;
					}
					else
					{
						g_Window[nCntWindow].fWidth += (SCREEN_WIDTH / 2 - 100) * 0.1f;
						g_Window[nCntWindow].fHeight += (SCREEN_HEIGHT / 2 - 100) * 0.1f;

						if (g_Window[nCntWindow].fWidth + (SCREEN_WIDTH / 2 - 100) * 0.1f == SCREEN_WIDTH / 2 - 100 &&
							g_Window[nCntWindow].fHeight + (SCREEN_HEIGHT / 2 - 100) * 0.1f == SCREEN_HEIGHT / 2 - 100)
						{
							SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 200, 30, WINDOWTEXTURE_GAME_TUTORIAL_MESSAGE, SETTEXTURE_FRONT);
						}
					}
				}

				break;
			case WINDOWTEXTURE_GAME_TUTORIAL_P:
				break;
			case WINDOWTEXTURE_GAME_HP:		// HP

				fHP = (float)pXPlayer->nLife / (float)pXPlayer->nInitLife;

				fHPRadius = g_Window[nCntWindow].fHPWidth * fHP;

				if (fHP < 0.3f)
				{
					g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else if (fHP < 0.5f)
				{
					g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 0.8f, 0.1f, 1.0f);
				}
				else if (fHP < 1.0f)
				{
					g_Window[nCntWindow].col = D3DXCOLOR(0.3f, 1.0f, 0.1f, 1.0f);
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_1:	// ����

				if (pXPlayer->nWeapon_1Lv < 3)
				{
					g_Window[nCntWindow].nNumAnim = 0;
				}
				else if (pXPlayer->nWeapon_1Lv < 6)
				{
					g_Window[nCntWindow].nNumAnim = 1;
				}
				else if (pXPlayer->nWeapon_1Lv < 9)
				{
					g_Window[nCntWindow].nNumAnim = 2;
				}
				else if (pXPlayer->nWeapon_1Lv < 12)
				{
					g_Window[nCntWindow].nNumAnim = 3;
				}
				else
				{
					g_Window[nCntWindow].nNumAnim = 4;
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_2:	// ����Q

				if (pXPlayer->nWeapon_2Lv < 3)
				{
					g_Window[nCntWindow].nNumAnim = 0;
				}
				else if (pXPlayer->nWeapon_2Lv < 6)
				{
					g_Window[nCntWindow].nNumAnim = 1;
				}
				else if (pXPlayer->nWeapon_2Lv < 9)
				{
					g_Window[nCntWindow].nNumAnim = 2;
				}
				else if (pXPlayer->nWeapon_2Lv < 12)
				{
					g_Window[nCntWindow].nNumAnim = 3;
				}
				else
				{
					g_Window[nCntWindow].nNumAnim = 4;
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_3:	// ����R

				if (pXPlayer->nWeapon_3Lv < 3)
				{
					g_Window[nCntWindow].nNumAnim = 0;
				}
				else if (pXPlayer->nWeapon_3Lv < 6)
				{
					g_Window[nCntWindow].nNumAnim = 1;
				}
				else if (pXPlayer->nWeapon_3Lv < 9)
				{
					g_Window[nCntWindow].nNumAnim = 2;
				}
				else if (pXPlayer->nWeapon_3Lv < 12)
				{
					g_Window[nCntWindow].nNumAnim = 3;
				}
				else
				{
					g_Window[nCntWindow].nNumAnim = 4;
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_99:

				if (pXPlayer->nWeapon == 0)
				{
					for (int nCnt = 0; nCnt < MAX_WINDOW_POLYGON; nCnt++)
					{// �E�B���h�E���J�E���g
						if (g_Window[nCnt].bUse == true && g_Window[nCnt].texture == WINDOWTEXTURE_GAME_WEAPON_1)
						{// �g�p���Ă���ꍇ
							g_Window[nCntWindow].pos = g_Window[nCnt].pos;

							break;
						}
					}
				}
				else if (pXPlayer->nWeapon == 1)
				{
					for (int nCnt = 0; nCnt < MAX_WINDOW_POLYGON; nCnt++)
					{// �E�B���h�E���J�E���g
						if (g_Window[nCnt].bUse == true && g_Window[nCnt].texture == WINDOWTEXTURE_GAME_WEAPON_2)
						{// �g�p���Ă���ꍇ
							g_Window[nCntWindow].pos = g_Window[nCnt].pos;

							break;
						}
					}
				}
				else if (pXPlayer->nWeapon == 2)
				{
					for (int nCnt = 0; nCnt < MAX_WINDOW_POLYGON; nCnt++)
					{// �E�B���h�E���J�E���g
						if (g_Window[nCnt].bUse == true && g_Window[nCnt].texture == WINDOWTEXTURE_GAME_WEAPON_3)
						{// �g�p���Ă���ꍇ
							g_Window[nCntWindow].pos = g_Window[nCnt].pos;

							break;
						}
					}
				}

				break;
			case WINDOWTEXTURE_RESULT_BG:	// ���U���g

				g_Window[nCntWindow].nCntTimer++;

				if (g_Window[nCntWindow].nCntTimer > 60)
				{
					if (g_Window[nCntWindow].pos.x <= SCREEN_WIDTH / 2)
					{
						g_Window[nCntWindow].pos.x = SCREEN_WIDTH / 2;
					}
					else
					{
						g_Window[nCntWindow].pos.x -= 20.0f;
					}
				}

				break;
			case WINDOWTEXTURE_RESULT_CLEAR:	// �N���A
			case WINDOWTEXTURE_RESULT_OVER:		// �I�[�o�[

				if (g_Window[nCntWindow].fWidth != SCREEN_WIDTH / 2 && g_Window[nCntWindow].fHeight != SCREEN_HEIGHT / 2)
				{
					g_Window[nCntWindow].fWidth -= (SCREEN_WIDTH / 4);
					g_Window[nCntWindow].fHeight -= (SCREEN_HEIGHT / 4);

					if (g_Window[nCntWindow].fWidth == SCREEN_WIDTH / 2 && g_Window[nCntWindow].fHeight == SCREEN_HEIGHT / 2)
					{
						PlaySound(SOUND_LABEL_SE_RESULT_3);
					}
				}

				if (mode == MODE_RESULT)
				{
					if (g_Window[nCntWindow].pos.y > 180.0f)
					{
						g_Window[nCntWindow].pos.y -= 10.0f;
					}
				}

				break;
			case WINDOWTEXTURE_RESULT_OTHER:

				g_Window[nCntWindow].nCounterAnim++;

				if (g_Window[nCntWindow].nCounterAnim % 3 == 0)
				{
					if (g_Window[nCntWindow].nNumAnim < 19)
					{
						g_Window[nCntWindow].nNumAnim += 1;
					}
					//g_Window[nCntWindow].nNumAnim = (g_Window[nCntWindow].nNumAnim + 1) % 49;
				}

				//if (GetKeyboardTrigger(DIK_1) == true)
				{
					//g_Window[nCntWindow].nNumAnim = 0;
				}

				break;
			case WINDOWTEXTURE_SELECT_1:		// �I��
			case WINDOWTEXTURE_SELECT_2:		// �I��

				g_Window[nCntWindow].nCntTimer++;

				if (g_SelectState == SELECTSTATE_NORMAL)
				{// �I���m�[�}��
					if (g_Window[nCntWindow].nCntTimer == 80) { g_Window[nCntWindow].nCntTimer = 0; }

					// �_��
					if (g_Window[nCntWindow].nCntTimer < 40) { g_Window[nCntWindow].col.a = 1.0f; }
					else { g_Window[nCntWindow].col.a = 0.0f; }
				}
				else if (g_SelectState == SELECTSTATE_ENTER)
				{// �I���G���^�[
					if (g_Window[nCntWindow].nCntTimer == 10)
					{
						g_Window[nCntWindow].nCntTimer = 0;
					}

					// �_��
					if (g_Window[nCntWindow].nCntTimer < 5) { g_Window[nCntWindow].col.a = 1.0f; }
					else { g_Window[nCntWindow].col.a = 0.0f; }
				}

				break;
			case WINDOW_TUTORIAL:	// �`���[�g���A��

				g_Window[nCntWindow].nCntTimer++;

				if (g_Window[nCntWindow].nCntTimer < 30)
				{
					g_Window[nCntWindow].col.a += (1.0f / 30.0f);
				}
				if (g_Window[nCntWindow].nCntTimer == 35)
				{
					SetXWeapon(WEAPONTYPE_SWORD);
				}
				else if (g_Window[nCntWindow].nCntTimer >= 40)
				{
					g_Window[nCntWindow].col.a -= (1.0f / 30.0f);
				}

				if (g_Window[nCntWindow].nCntTimer == 70)
				{
					DeleteWindow(WINDOW_TUTORIAL);
				}

				break;
			case WINDOW_FADE:	// �t�F�[�h

				g_Window[nCntWindow].nCntTimer++;

				if (g_Window[nCntWindow].nCntTimer < 30)
				{
					g_Window[nCntWindow].col.a += (1.0f / 30.0f);
				}
				else if (g_Window[nCntWindow].nCntTimer >= 40)
				{
					g_Window[nCntWindow].col.a -= (1.0f / 30.0f);
				}

				if (g_Window[nCntWindow].nCntTimer == 70)
				{
					DeleteWindow(WINDOW_FADE);
				}

				break;
			}

			// ���_�ݒ�
			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_GAME_HP)
			{// HP
				pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + (fHPRadius - g_Window[nCntWindow].fWidth), g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + (fHPRadius - g_Window[nCntWindow].fWidth), g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
			}
			else
			{// ���̑�
				pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
			}
			
			// ���_�J���[�ݒ�
			pVtx[nCntWindow * 4].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 1].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_Window[nCntWindow].col;

			// �e�N�X�`���ݒ�
			if (WINDOWTEXTURE_TITLE_ANIM == g_Window[nCntWindow].texture)
			{// �^�C�g���A�j���[�V����
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2 + WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2 + WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2 + WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2 + WINDOW_TEXTURE_UV_V_TEX2);
			}
			else if (WINDOWTEXTURE_RESULT_OTHER == g_Window[nCntWindow].texture)
			{// ���U���g
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1 + WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1 + WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1 + WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1 + WINDOW_TEXTURE_UV_V_1);
			}
			else if (
				WINDOWTEXTURE_GAME_WEAPON_1 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_2 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_3 == g_Window[nCntWindow].texture)
			{// ����
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3 + WINDOW_TEXTURE_UV_U_3, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3, WINDOW_TEXTURE_UV_V_3);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3 + WINDOW_TEXTURE_UV_U_3, WINDOW_TEXTURE_UV_V_3);
			}
			else
			{// ���̑�
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, WINDOW_TEXTURE_UV_V);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffWindow->Unlock();	
}

//=========================================================================================================================
// �E�B���h�E�`�揈��
//=========================================================================================================================
void DrawTopFrontWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntWindow;		// �E�B���h�E�J�E���^

						// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_TOPFRONT)
		{// �g�p���Ă���ꍇ
			// �e�N�X�`���ݒ�
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// �E�B���h�E�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);
		}
	}
}
void DrawFrontWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntWindow;		// �E�B���h�E�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_FRONT)
		{// �g�p���Ă���ꍇ
			// �e�N�X�`���ݒ�
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// �E�B���h�E�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);
		}
	}
}
void DrawTopBGWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntWindow;		// �E�B���h�E�J�E���^

						// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_TOPBG)
		{// �g�p���Ă���ꍇ

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// �A���t�@�e�X�g�̐ݒ�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}

			// �e�N�X�`���ݒ�
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// �E�B���h�E�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// �A���t�@�e�X�g�𖳌�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}
}
void DrawBGWindow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntWindow;		// �E�B���h�E�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_BG)
		{// �g�p���Ă���ꍇ

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// �A���t�@�e�X�g�̐ݒ�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}

			// �e�N�X�`���ݒ�
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// �E�B���h�E�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// �A���t�@�e�X�g�𖳌�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}
}

//=========================================================================================================================
// �E�B���h�E�ݒ菈��
//=========================================================================================================================
void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, WINDOWTEXTURE texture, SETTEXTURE type)
{
	// �ϐ��錾
	int nCntWindow;	// �E�B���h�E�J�E���^

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == false)
		{// �g�p���Ă��Ȃ��ꍇ
			g_Window[nCntWindow].pos = pos;				// ���S�l
			g_Window[nCntWindow].tex = pos;				// ���S�l
			g_Window[nCntWindow].col = col;				// �F
			g_Window[nCntWindow].fWidth = fWidth;		// ����
			if (texture == WINDOWTEXTURE_GAME_HP)
			{
				g_Window[nCntWindow].fHPWidth = fWidth * 2;	// ����
			}
			g_Window[nCntWindow].fHeight = fHeight;		// ����
			g_Window[nCntWindow].texture = texture;		// �e�N�X�`�����
			g_Window[nCntWindow].type = type;			// �\���^�C�v
			g_Window[nCntWindow].nCntTimer = 0;			// ���ԃJ�E���^
			g_Window[nCntWindow].nCounterAnim = 0;		// �A�j���[�V�����J�E���^
			g_Window[nCntWindow].nNumAnim = 0;			// �A�j���[�V�����ԍ�
			g_Window[nCntWindow].nSpeedAnim = 0;		// �A�j���[�V�������x
			g_Window[nCntWindow].nPatternAnim = 0;		// �A�j���[�V�����p�^�[����
			g_Window[nCntWindow].bUse = true;			// �g�p���Ă����Ԃ�

			// ���_�ݒ�
			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);

			// ���_�J���[�ݒ�
			pVtx[nCntWindow * 4].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 1].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_Window[nCntWindow].col;

			// �e�N�X�`���ݒ�
			if (WINDOWTEXTURE_TITLE_ANIM == g_Window[nCntWindow].texture)
			{// �^�C�g���A�j���[�V����
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_TEX2, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_TEX2, WINDOW_TEXTURE_UV_V_TEX2);
			}
			else if (WINDOWTEXTURE_RESULT_OTHER == g_Window[nCntWindow].texture)
			{// ���U���g
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_1, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_1, WINDOW_TEXTURE_UV_V_1);
			}
			else if (WINDOWTEXTURE_GAME_WEAPON_1 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_2 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_3 == g_Window[nCntWindow].texture)
			{// ����
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_3, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V_3);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_3, WINDOW_TEXTURE_UV_V_3);
			}
			else
			{// ���̑�
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, WINDOW_TEXTURE_UV_V);
			}
			
			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffWindow->Unlock();
}

//=========================================================================================================================
// �I��ݒ菈��
//=========================================================================================================================
void SetSelectState(SELECTSTATE state)
{
	// �ϐ��錾
	int nCntWindow;		// �E�B���h�E�J�E���^

	g_SelectState = state;		// ��Ԃ��L��

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].texture == WINDOWTEXTURE_SELECT_1 || g_Window[nCntWindow].texture == WINDOWTEXTURE_SELECT_2)
		{// �g�p���Ă��Ȃ��ꍇ
			g_Window[nCntWindow].nCntTimer = 0;			// ���ԃJ�E���^������
		}
	}
}

//=========================================================================================================================
// �E�B���h�E�j������
//=========================================================================================================================
void DeleteWindow(int nCntWindow)
{
	g_Window[nCntWindow].bUse = false;
}
void DeleteWindow(WINDOWTEXTURE texture)
{
	// �ϐ��錾
	int nCntWindow;		// �E�B���h�E�J�E���^

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].texture == texture)
		{// �g�p���Ă���ꍇ
			g_Window[nCntWindow].bUse = false;
			break;
		}
	}
}
void DeleteWindow(void)
{
	// �ϐ��錾
	int nCntWindow;		// �E�B���h�E�J�E���^

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// �E�B���h�E���J�E���g
		if (g_Window[nCntWindow].bUse == true)
		{// �g�p���Ă���ꍇ
			g_Window[nCntWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�B���h�E�̒��S�l
			g_Window[nCntWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�B���h�E�̒��S�l
			g_Window[nCntWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
			g_Window[nCntWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�B���h�E�̈ړ��l
			g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �E�B���h�E�F
			g_Window[nCntWindow].fWidth = 0;								// ����
			g_Window[nCntWindow].fHeight = 0;								// ����
			g_Window[nCntWindow].fAngle = 0;								// �p�x
			g_Window[nCntWindow].fLenght = 0;								// ����
			g_Window[nCntWindow].nCntTimer = 0;								// ���ԃJ�E���^
			g_Window[nCntWindow].nCounterAnim = 0;							// �A�j���[�V�����J�E���^
			g_Window[nCntWindow].nNumAnim = 0;								// �A�j���[�V�����ԍ�
			g_Window[nCntWindow].nSpeedAnim = 0;							// �A�j���[�V�������x
			g_Window[nCntWindow].nPatternAnim = 0;							// �A�j���[�V�����p�^�[����
			g_Window[nCntWindow].bUse = false;								// �g�p���Ă��邩
		}
	}

	g_SelectState = SELECTSTATE_NORMAL;										// �I�����	
	g_fAngleUp = 0.0f;														// ������
	g_fAngleDown = 0.0f;													// ������
}