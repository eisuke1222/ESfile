//=========================================================================================================================
// 
// �t�F�[�h���� [fade.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "fade.h"		// �t�F�[�h
#include "input.h"		// ���͏���
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define FADE_TEXTURE_SPEED			(5)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define FADE_TEXTURE_PATTERN		(10)							// �e�N�X�`���A�j���[�V�����p�^�[����
#define FADE_TEXTURE_UV_U			(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define FADE_TEXTURE_UV_V			(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define FADE_SPEED					(0.002f)						// �t�F�[�h�X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9					g_pTextureFade = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9				g_pVtxBuffFade = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
FADETYPE							g_Type;							// �t�F�[�h���
FADE								g_fade;							// �t�F�[�h���
MODE								g_modeNext;						// ���̃��[�h
GAMESTATE							g_gameNext;						// ���̃Q�[��
D3DXCOLOR							g_colorFade;					// �t�F�[�h�F
int									g_nCntFadeTimer;				// ���ԃJ�E���^

//=========================================================================================================================
// �t�F�[�h����������
//=========================================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̎擾

	g_Type = FADETYPE_NONE;								// �t�F�[�h���
	g_fade = FADE_IN;									// �t�F�[�h���
	g_modeNext = modeNext;								// ��ʏ��
	g_gameNext = GAMESTATE_NONE;						// �Q�[�����
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �t�F�[�h�F
	g_nCntFadeTimer = 0;								// ���ԃJ�E���^

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffFade->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// ���_�J���[�ݒ�
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(FADE_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, FADE_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(FADE_TEXTURE_UV_U, FADE_TEXTURE_UV_V);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffFade->Unlock();
}

//=========================================================================================================================
// �t�F�[�h�I������
//=========================================================================================================================
void UninitFade(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=========================================================================================================================
// �t�F�[�h�X�V����
//=========================================================================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffFade->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0

	);
	
	if (g_fade != FADE_NONE)
	{// �t�F�[�h���J�n
		if (g_fade == FADE_IN)
		{// �t�F�[�h�C��
			g_colorFade.a -= 0.03f;

			if (g_colorFade.a <= 0.0f)
			{// ����
				g_colorFade.a = 0.0f;	// �����x�𒲐�
				g_fade = FADE_NONE;		// �������Ă��Ȃ����
				g_nCntFadeTimer = 0;	// ���ԃJ�E���^

			}
		}
		else if (g_fade == FADE_OUT)
		{// �t�F�[�h�A�E�g
			g_colorFade.a += 0.03f;

			if (g_colorFade.a >= 1.0f)
			{// �s����
				g_colorFade.a = 1.0f;	// �����x�𒲐�

				g_nCntFadeTimer++;		// �J�E���^�����Z
				if (g_nCntFadeTimer > 30)
				{
					g_fade = FADE_IN;	// �t�F�[�h�C��

					if (g_Type == FADETYPE_MODE)
					{
						// ���[�h�̐ݒ�
						SetMode(g_modeNext);
					}
					else if (g_Type == FADETYPE_GAME)
					{
						// �Q�[���̐ݒ�
						SetGameState(g_gameNext);
					}
				}
			}
		}
	}

	// ���_�J���[�ݒ�
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffFade->Unlock();
}

//=========================================================================================================================
// �t�F�[�h�`�揈��
//=========================================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffFade,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	// �t�F�[�h�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=========================================================================================================================
// �t�F�[�h�̐ݒ�
//=========================================================================================================================
void SetFadeMode(MODE modeNext)
{// ���[�h
	g_Type = FADETYPE_MODE;		// �t�F�[�h���
	g_fade = FADE_OUT;			// �t�F�[�h�A�E�g
	g_modeNext = modeNext;		// ���̉��
}
void SetFadeGame(GAMESTATE gameNext)
{// �Q�[��
	g_Type = FADETYPE_GAME;		// �t�F�[�h���
	g_fade = FADE_OUT;			// �t�F�[�h�A�E�g
	g_gameNext = gameNext;		// ���̃Q�[��
}

//=========================================================================================================================
// �t�F�[�h�̎擾
//=========================================================================================================================
FADE GetFade(void)
{
	return g_fade;		// �l��Ԃ�
}