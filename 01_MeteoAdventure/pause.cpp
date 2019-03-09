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
#include "input.h"		// ���͏���
#include "sound.h"		// �T�E���h
#include "help.h"		// �w���v

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_PAUSE_TEXTURE		(6)								// �|�[�Y�e�N�X�`���ő吔
#define PAUSE_TEXTURE_NAME_1	"data\\TEXTURE\\brack000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��1
#define PAUSE_TEXTURE_NAME_2	"data\\TEXTURE\\pause101.png"	// �ǂݍ��ރe�N�X�`���t�@�C��2
#define PAUSE_TEXTURE_NAME_3	"data\\TEXTURE\\pause001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��3
#define PAUSE_TEXTURE_NAME_4	"data\\TEXTURE\\pause002.png"	// �ǂݍ��ރe�N�X�`���t�@�C��4
#define PAUSE_TEXTURE_NAME_5	"data\\TEXTURE\\pause003.png"	// �ǂݍ��ރe�N�X�`���t�@�C��5
#define PAUSE_TEXTURE_NAME_6	"data\\TEXTURE\\pause004.png"	// �ǂݍ��ރe�N�X�`���t�@�C��6
#define PAUSE_POS_X				(0)								// �|�[�Y�̍���X���W
#define PAUSE_POS_Y				(0)								// �|�[�Y�̍���Y���W
#define PAUSE_WIDTH				(SCREEN_WIDTH)					// �|�[�Y�̕�
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)					// �|�[�Y�̍���
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
bool							g_SelectEnter;								// �I�𔻒�
bool							g_bHelp;									// �w���v�\����

//=========================================================================================================================
// �|�[�Y����������
//=========================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾

	g_posPause = D3DXVECTOR3((PAUSE_POS_X + PAUSE_WIDTH) / 2, (PAUSE_POS_Y + PAUSE_HEIGHT) / 2, 0.0f);		// �|�[�Y�̒��S�l
	g_SelectNum = 0;							// �I��ԍ�
	g_SelectColor = 1.0f;						// �I���J���[
	g_SelectColorChange = -0.008f;				// �I���J���[�ω�
	g_SelectEnter = false;						// �I�𔻒�
	int nCntPause;								// �|�[�Y�J�E���^
	float fPosPause = -130;						// �|�[�Y�ʒu
	g_bHelp = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_1, &g_pTexturePause[0]);		// BG
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_2, &g_pTexturePause[1]);		// �|�[�Y
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_3, &g_pTexturePause[2]);		// �R���e�j���[
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_4, &g_pTexturePause[3]);		// �X�e�[�W���g���C
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_5, &g_pTexturePause[4]);		// �Q�[���}�b�v
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_6, &g_pTexturePause[5]);		// �N�E�B�b�g�Q�[��

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
		if (nCntPause == 0 || nCntPause == 1)
		{// BG
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);

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
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) + fPosPause, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) + fPosPause, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) + fPosPause + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) + fPosPause + 100, 0.0f);
			
			if (nCntPause == 2)
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

			fPosPause += 120;
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

	// �w���v����������
	InitHelp();
}

//=========================================================================================================================
// �|�[�Y�I������
//=========================================================================================================================
void UninitPause(void)
{
	// �ϐ��錾
	int nCntTex;		// �e�N�X�`���J�E���^

	// �w���v�I������
	UninitHelp();

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
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPause->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (GetKeyboardTrigger(DIK_H) == true)
	{
		g_bHelp = g_bHelp ? false : true;
	}

	if (g_bHelp == false)
	{
		if (g_SelectEnter == false)
		{
			if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true)
			{
				// ���_�J���[�ݒ�
				pVtx[g_SelectNum * 4 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[g_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[g_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[g_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

				g_SelectNum = (g_SelectNum + 3) % 4;	// �I����i�߂�
				g_SelectColor = 1.0f;					// �J���[�ύX
				PlaySound(SOUND_LABEL_SE_SELECT001);
			}
			if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true)
			{
				// ���_�J���[�ݒ�
				pVtx[g_SelectNum * 4 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[g_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[g_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[g_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

				g_SelectNum = (g_SelectNum + 1) % 4;	// �I����i�߂�
				g_SelectColor = 1.0f;					// �J���[�ύX
				PlaySound(SOUND_LABEL_SE_SELECT001);
			}

			g_SelectColor += g_SelectColorChange;		// �F�ω�

			if (g_SelectColor > 1.0f || g_SelectColor < 0.5f)
			{// �F�ω��J��Ԃ�
				g_SelectColorChange *= -1;
			}

			// ���_�J���[�ݒ�
			pVtx[g_SelectNum * 4 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		}
		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffPause->Unlock();

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{// �C�ӂ̃L�[�iENTER�j���������ꍇ
			MODE mode = GetMode();		// ���[�h�擾
			FADE fade = GetFade();

			if (fade == FADE_NONE)
			{
				if (g_SelectNum == 0)
				{
					PlaySound(SOUND_LABEL_SE_PAUSEOUT);
					SetPauseState(false);						// �g�p���Ă��Ȃ���Ԃ�
				}
				else if (g_SelectNum == 1)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					g_SelectEnter = true;				// �I�𔻒�
					SetFadeGame(GAMESTATE_GAMEPLAY);	// �Q�[���̍ŏ���
				}
				else if (g_SelectNum == 2)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					g_SelectEnter = true;				// �I�𔻒�
					SetFadeGame(GAMESTATE_MAP);			// �Q�[���}�b�v��
				}
				else if (g_SelectNum == 3)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					g_SelectEnter = true;				// �I�𔻒�
					GameSoundStop();
					SetFadeMode(MODE_TITLE);			// �^�C�g����
				}
			}
		}
		if (GetKeyboardTrigger(DIK_X) == true)
		{
			PlaySound(SOUND_LABEL_SE_PAUSEOUT);
			SetPauseState(false);							// �g�p���Ă��Ȃ���Ԃ�
		}
	}
	else if (g_bHelp == true)
	{
		UpdateHelp();

		if (GetKeyboardTrigger(DIK_P) == true || GetKeyboardTrigger(DIK_X) == true || GetKeyboardTrigger(DIK_RETURN) == true)
		{
			g_bHelp = false;
		}
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

	if (g_bHelp == true)
	{
		DrawHelp();
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
	g_SelectEnter = false;						// �I�����
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

	pVtx += 8;

	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE - 2; nCntPause++)
	{// �|�[�Y���J�E���g
		if (nCntPause == 0)
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