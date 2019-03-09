//=========================================================================================================================
// 
// �|�[�Y���� [pause.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "pause.h"		// �|�[�Y
#include "fade.h"		// �t�F�[�h
#include "sound.h"
#include "score.h"
#include "window.h"
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_PAUSE_TEXTURE		(6)								// �|�[�Y�e�N�X�`���ő吔
#define PAUSE_TEXTURE_NAME_1	"data\\TEXTURE\\brack000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��1
#define PAUSE_TEXTURE_NAME_2	"data\\TEXTURE\\pause000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��2
#define PAUSE_TEXTURE_NAME_3	"data\\TEXTURE\\pause101.png"	// �ǂݍ��ރe�N�X�`���t�@�C��3
#define PAUSE_TEXTURE_NAME_4	"data\\TEXTURE\\pause001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��4
#define PAUSE_TEXTURE_NAME_5	"data\\TEXTURE\\pause002.png"	// �ǂݍ��ރe�N�X�`���t�@�C��5
#define PAUSE_TEXTURE_NAME_6	"data\\TEXTURE\\pause003.png"	// �ǂݍ��ރe�N�X�`���t�@�C��6
#define PAUSE_POS_X				(0)								// �|�[�Y�̍���X���W
#define PAUSE_POS_Y				(0)								// �|�[�Y�̍���Y���W
#define PAUSE_WIDTH				(SCREEN_WIDTH)					// �|�[�Y�̕�
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)					// �|�[�Y�̍���
#define PAUSE_POS_X1			(0)								// �|�[�Y�̍���X���W
#define PAUSE_POS_Y1			(0)								// �|�[�Y�̍���Y���W
#define PAUSE_WIDTH1			(100)							// �|�[�Y�̕�
#define PAUSE_HEIGHT1			(100)							// �|�[�Y�̍���
#define PAUSE_TEXTURE_SPEED		(5)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define PAUSE_TEXTURE_PATTERN	(10)							// �e�N�X�`���A�j���[�V�����p�^�[����
#define PAUSE_TEXTURE_UV_U		(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define PAUSE_TEXTURE_UV_V		(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define PAUSE_BACKTEXTURE_UV_U	(1.0f)							// �O�e�N�X�`���A�j���[�V����U�͈�
#define PAUSE_BACKTEXTURE_UV_V	(1.0f)							// �O�e�N�X�`���A�j���[�V����V�͈�
#define PAUSE_SPEED				(0.002f)						// �|�[�Y�X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTexturePause[MAX_PAUSE_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPause = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3						g_posPause;									// �|�[�Y�̈ʒu
int								g_SelectNum;								// �I��ԍ�
float							g_SelectColor;								// �I��F
float							g_SelectColorChange;						// �I��F�ω�
D3DXVECTOR3						g_SelectSize;								// �I���T�C�Y
D3DXVECTOR3						g_SelectSizeChange;							// �I���T�C�Y�ω�
float							g_Selectpos;								// �I���ʒu
float							g_SelectLenght;								// �I��
float							g_SelectAngleUp;							// �I��
float							g_SelectAngleDown;							// �I��
bool							g_SelectEnter;								// �I�𔻒�
bool							g_bHelp;									// �w���v�\����
bool							g_bTriggerStick;

//=========================================================================================================================
// �|�[�Y����������
//=========================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾

	g_posPause = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �|�[�Y�̒��S�l
	g_SelectNum = 0;							// �I��ԍ�
	g_SelectColor = 1.0f;						// �I���J���[
	g_SelectColorChange = -0.008f;				// �I���J���[�ω�
	g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �I���T�C�Y
	g_SelectSizeChange = D3DXVECTOR3(SCREEN_WIDTH / 800.0f, SCREEN_HEIGHT / 800.0f, 0.0f);	// �I���T�C�Y
	g_Selectpos = 0.0f;							// �I���ʒu
	g_SelectLenght = 0.0f;						// �I��
	g_SelectAngleUp = 0.0f;						// �I��
	g_SelectAngleDown = 0.0f;					// �I��
	g_SelectEnter = false;						// �I�𔻒�
	int nCntPause;								// �|�[�Y�J�E���^
	float fPosPause = -130;						// �|�[�Y�ʒu
	g_bHelp = false;
	g_bTriggerStick = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_1, &g_pTexturePause[0]);		// BG
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_2, &g_pTexturePause[1]);		// �|�[�Y
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_3, &g_pTexturePause[2]);		// �R���e�j���[
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_4, &g_pTexturePause[3]);		// �X�e�[�W���g���C
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_5, &g_pTexturePause[4]);		// �N�E�B�b�g�Q�[��
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_6, &g_pTexturePause[5]);		// �I��

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPause->Lock
	(
		0,
		0, 
		(void**)&pVtx,
		0
	);
	
	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE; nCntPause++)
	{// �|�[�Y���J�E���g
		// ���_�ݒ�
		if (nCntPause == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(450 + PAUSE_POS_X1, 330 + PAUSE_POS_Y1, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(450 + PAUSE_WIDTH1, 330 + PAUSE_POS_Y1, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450 + PAUSE_POS_X1, 330 + PAUSE_HEIGHT1, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(450 + PAUSE_WIDTH1, 330 + PAUSE_HEIGHT1, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
		}

		if (nCntPause == 0 || nCntPause == 1)
		{// BG
			if (nCntPause == 0)
			{
				// ���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else
		{// �|�[�Y���j���[		
			if (nCntPause == 2 || nCntPause == 3)
			{
				// ���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				// ���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
		}
		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PAUSE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, PAUSE_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();

	g_SelectAngleUp = atan2f(
		450 - (450 + 50),
		330 - (330 + 50));
	g_SelectLenght = sqrtf(
		(50 * 50) +
		(50 * 50));
	g_SelectAngleDown = g_SelectAngleUp;
}

//=========================================================================================================================
// �|�[�Y�I������
//=========================================================================================================================
void UninitPause(void)
{
	// �ϐ��錾
	int nCntTex;		// �e�N�X�`���J�E���^

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < MAX_PAUSE_TEXTURE; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=========================================================================================================================
// �|�[�Y�X�V����
//=========================================================================================================================
void UpdatePause(void)
{
	DIJOYSTATE pStick = GetStick();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPause->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if ((GetKeyboardTrigger(DIK_H) == true || GetButtonTrigger(11) == true))
	{
		if (g_bHelp == true)
		{
			DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_P);
			DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_GAME_TUTORIAL_IN, SETTEXTURE_TOPFRONT);
		}
		else if (g_bHelp == false)
		{
			DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_GAME_TUTORIAL_P, SETTEXTURE_TOPFRONT);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_GAME_TUTORIAL_OUT, SETTEXTURE_TOPFRONT);
		}

		g_bHelp = g_bHelp ? false : true;
	}

	if (g_SelectEnter == false && g_bHelp == false)
	{
		if ((GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || ((pStick.rgdwPOV[0] == 0 || pStick.lY < -100) && g_bTriggerStick == false)))
		{
			// ���_�ݒ�
			pVtx[g_SelectNum * 4 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[g_SelectNum * 4 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			PlaySound(SOUND_LABEL_SE_PAUSE_1);

			g_SelectNum = (g_SelectNum + 2) % 3;	// �I����i�߂�
			g_SelectColor = 1.0f;					// �J���[�ύX
			g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_bTriggerStick = true;
		}
		if ((GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || ((pStick.rgdwPOV[0] == 18000 || pStick.lY > 100) && g_bTriggerStick == false)))
		{
			// ���_�ݒ�
			pVtx[g_SelectNum * 4 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[g_SelectNum * 4 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			PlaySound(SOUND_LABEL_SE_PAUSE_1);

			g_SelectNum = (g_SelectNum + 1) % 3;	// �I����i�߂�
			g_SelectColor = 1.0f;					// �J���[�ύX
			g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_bTriggerStick = true;
		}

		g_SelectSize -= g_SelectSizeChange;

		g_SelectColor += g_SelectColorChange;		// �F�ω�

		if (g_SelectColor > 1.0f || g_SelectColor < 0.5f)
		{// �F�ω��J��Ԃ�
			g_SelectColorChange *= -1;
			g_SelectSizeChange.x *= -1;
			g_SelectSizeChange.y *= -1;
		}

		// ���_�ݒ�
		pVtx[g_SelectNum * 4 + 12].pos = D3DXVECTOR3(PAUSE_POS_X - 30 - g_SelectSize.x, PAUSE_POS_Y - 30 - g_SelectSize.y, 0.0f);
		pVtx[g_SelectNum * 4 + 1 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH + 30 + g_SelectSize.x, PAUSE_POS_Y - 30 - g_SelectSize.y, 0.0f);
		pVtx[g_SelectNum * 4 + 2 + 12].pos = D3DXVECTOR3(PAUSE_POS_X - 30 - g_SelectSize.x, PAUSE_HEIGHT + 30 + g_SelectSize.y, 0.0f);
		pVtx[g_SelectNum * 4 + 3 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH + 30 + g_SelectSize.x, PAUSE_HEIGHT + 30 + g_SelectSize.y, 0.0f);
		// ���_�J���[�ݒ�
		pVtx[g_SelectNum * 4 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		pVtx[g_SelectNum * 4 + 1 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		pVtx[g_SelectNum * 4 + 2 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		pVtx[g_SelectNum * 4 + 3 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	}

	g_Selectpos = (g_SelectNum) * 120.0f;

	g_SelectAngleUp += 0.01f;
	g_SelectAngleDown -= 0.01f;

	// ���_�ݒ�
	/*pVtx[(2) * 4].pos = D3DXVECTOR3(400 + PAUSE_POS_X1, 280 + PAUSE_POS_Y1 + g_Selectpos, 0.0f);
	pVtx[(2) * 4 + 1].pos = D3DXVECTOR3(400 + PAUSE_WIDTH1, 280 + PAUSE_POS_Y1 + g_Selectpos, 0.0f);
	pVtx[(2) * 4 + 2].pos = D3DXVECTOR3(400 + PAUSE_POS_X1, 280 + PAUSE_HEIGHT1 + g_Selectpos, 0.0f);
	pVtx[(2) * 4 + 3].pos = D3DXVECTOR3(400 + PAUSE_WIDTH1, 280 + PAUSE_HEIGHT1 + g_Selectpos, 0.0f);*/

	pVtx[2 * 4].pos.x = 450 - sinf(g_SelectAngleUp) * g_SelectLenght;
	pVtx[2 * 4].pos.y = 330 + g_Selectpos - cosf(g_SelectAngleUp) * g_SelectLenght;
	pVtx[2 * 4 + 1].pos.x = 450 + sinf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 1].pos.y = 330 + g_Selectpos - cosf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 2].pos.x = 450 - sinf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 2].pos.y = 330 + g_Selectpos + cosf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 3].pos.x = 450 + sinf(g_SelectAngleUp) * g_SelectLenght;
	pVtx[2 * 4 + 3].pos.y = 330 + g_Selectpos + cosf(g_SelectAngleUp) * g_SelectLenght;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
		GetButtonTrigger(2) == true) && g_bHelp == false)
	{// �C�ӂ̃L�[�iENTER�j���������ꍇ
		FADE fade = GetFade();
		GAMESTATE game = GetGameState();

		if (fade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);

			if (g_SelectNum == 0)
			{
				SetPauseState(false);				// �g�p���Ă��Ȃ���Ԃ�
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
			}
			else if (g_SelectNum == 1)
			{
				g_SelectEnter = true;				// �I�𔻒�
				
				if (game == GAMESTATE_TUTORIAL)
				{
					SetFadeGame(GAMESTATE_TUTORIAL);// �Q�[���ցi�`���[�g���A���j
				}
				else if (game == GAMESTATE_PLAY)
				{
					SetFadeGame(GAMESTATE_PLAY);	// �Q�[���ցi�Q�[�����C���j		
					DeleteScore();
					SetMapReset(0);
				}
			}
			else if (g_SelectNum == 2)
			{
				g_SelectEnter = true;				// �I�𔻒�
				SetFadeMode(MODE_TITLE);			// �^�C�g����				
			}
		}
	}
	if ((GetKeyboardTrigger(DIK_X) == true || GetButtonTrigger(1) == true))
	{
		PlaySound(SOUND_LABEL_SE_PAUSE_0);

		DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_P);
		DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
		DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
		SetPauseState(false);						// �g�p���Ă��Ȃ���Ԃ�
	}

	if ((pStick.rgdwPOV[0] != 0 && pStick.rgdwPOV[0] != 18000) && (pStick.lY > -100 && pStick.lY < 100))
	{
		g_bTriggerStick = false;
	}
}

//=========================================================================================================================
// �|�[�Y�`�揈��
//=========================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�|�C���^
	int nCntPause;					// �|�[�Y�J�E���^
	int nCntTex = 0;				// �e�N�X�`���J�E���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE; nCntPause++)
	{// �|�[�Y���J�E���g
		// �e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		// �|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=========================================================================================================================
// �|�[�Y�ݒ菈��
//=========================================================================================================================
void SetPauseState(void)
{
	g_SelectNum = 0;							// �I��ԍ�
	g_SelectColor = 1.0f;						// �I���J���[
	g_SelectColorChange = -0.008f;				// �I���J���[�ω�
	g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SelectEnter = false;						// �I�����
	g_bHelp = false;
	g_bTriggerStick = false;

	int nCntPause;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPause->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += 12;

	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE - 3; nCntPause++)
	{// �|�[�Y���J�E���g
		if (nCntPause == 0)
		{
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();
}

//=========================================================================================================================
// �w���v���擾����
//=========================================================================================================================
bool GetHelpState(void)
{
	return g_bHelp;
}

//=========================================================================================================================
// �X�e�B�b�N��Ԑݒ菈��
//=========================================================================================================================
void SetStickState(bool bTriggerStick)
{
	g_bTriggerStick = bTriggerStick;
}