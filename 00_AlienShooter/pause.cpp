//=========================================================================================================================
// 
// �|�[�Y���� [bg.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "pause.h"		// �|�[�Y
#include "fade.h"		// �t�F�[�h
#include "sound.h"		// �T�E���h
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_PAUSE_TEXTURE		(5)								// �|�[�Y�e�N�X�`���ő吔
#define PAUSE_TEXTURE_NAME_1	"data\\TEXTURE\\brack.png"		// �ǂݍ��ރe�N�X�`���t�@�C��1
#define PAUSE_TEXTURE_NAME_2	"data\\TEXTURE\\pause100.png"	// �ǂݍ��ރe�N�X�`���t�@�C��2
#define PAUSE_TEXTURE_NAME_3	"data\\TEXTURE\\pause000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��3
#define PAUSE_TEXTURE_NAME_4	"data\\TEXTURE\\pause001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��4
#define PAUSE_TEXTURE_NAME_5	"data\\TEXTURE\\pause002.png"	// �ǂݍ��ރe�N�X�`���t�@�C��5
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
LPDIRECT3DTEXTURE9				g_pTexturePause[MAX_PAUSE_TEXTURE] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPause = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3						g_posPause;						// �|�[�Y�̈ʒu
int								g_SelectNum;					// �I��ԍ�
float							g_SelectColor;					// �I��F
float							g_SelectColorChange;			// �I��F�ω�

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
	int nCntPause;								// �|�[�Y�J�E���^
	float fPosPause = -150;						// �|�[�Y�ʒu

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// BG
		pDevice,
		PAUSE_TEXTURE_NAME_1,
		&g_pTexturePause[0]
	);
	D3DXCreateTextureFromFile
	(// �|�[�Y
		pDevice,
		PAUSE_TEXTURE_NAME_2,
		&g_pTexturePause[1]
	);
	D3DXCreateTextureFromFile
	(// �R���e�j���[
		pDevice,
		PAUSE_TEXTURE_NAME_3,
		&g_pTexturePause[2]
	);
	D3DXCreateTextureFromFile
	(// ���g���C
		pDevice,
		PAUSE_TEXTURE_NAME_4,
		&g_pTexturePause[3]
	);
	D3DXCreateTextureFromFile
	(// �N�E�B�b�g
		pDevice,
		PAUSE_TEXTURE_NAME_5,
		&g_pTexturePause[4]
	);

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
		if (nCntPause == 0)
		{// BG
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
		}
		else if (nCntPause == 1)
		{// �|�[�Y
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) - 180, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) - 180, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) + 180, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) + 180, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
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

			fPosPause += 100;
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
		{// NULL�ȊO�̏ꍇ
			g_pTexturePause[nCntTex]->Release();	// �J��
			g_pTexturePause[nCntTex] = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffPause->Release();		// �J��
		g_pVtxBuffPause = NULL;			// NULL��
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

	if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true)
	{// �i��j��������
		// ���_�J���[�ݒ�
		pVtx[g_SelectNum * 4 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[g_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[g_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[g_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		PlaySound(SOUND_LABEL_SE_SELECT);		// �I����

		g_SelectNum = (g_SelectNum + 2) % 3;	// �I��ԍ���ς���
		g_SelectColor = 1.0f;					// �F
	}
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true)
	{// �i���j��������
		// ���_�J���[�ݒ�
		pVtx[g_SelectNum * 4 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[g_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[g_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[g_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		PlaySound(SOUND_LABEL_SE_SELECT);		// �I����

		g_SelectNum = (g_SelectNum + 1) % 3;	// �I��ԍ���ς���
		g_SelectColor = 1.0f;					// �F
	}

	g_SelectColor += g_SelectColorChange;

	if (g_SelectColor > 1.0f || g_SelectColor < 0.5f)
	{// �؂�ւ�
		g_SelectColorChange *= -1;
	}

	// ���_�J���[�ݒ�
	pVtx[g_SelectNum * 4 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	pVtx[g_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	pVtx[g_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	pVtx[g_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{// �iENTER�j��������
		PlaySound(SOUND_LABEL_SE_DECIDE);	// ���艹
		
		MODE mode = GetMode();

		if (g_SelectNum == 0)
		{// �|�[�Y����
			if (mode == MODE_TUTORIAL)
			{// �`���[�g���A����������
				SetTutorialPauseState(false);
			}
			else if (mode == MODE_GAME)
			{// �Q�[����������
				SetPauseState(false);
			}
		}
		else if (g_SelectNum == 1)
		{// ���g���C
			if (mode == MODE_TUTORIAL)
			{// �`���[�g���A����������
				SetFade(MODE_TUTORIAL);
			}
			else if (mode == MODE_GAME)
			{// �Q�[����������
				SetFade(MODE_GAME);
			}
		}
		else if (g_SelectNum == 2)
		{// �^�C�g����
			SetFade(MODE_TITLE);
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
}

//=========================================================================================================================
// �|�[�Y�ݒ菈��
//=========================================================================================================================
void SetPause(void)
{
	g_SelectNum = 0;							// �I��ԍ�
	g_SelectColor = 1.0f;						// �I���J���[
	g_SelectColorChange = -0.008f;				// �I���J���[�ω�
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
		{// �w�i
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// �|�[�Y���j���[
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