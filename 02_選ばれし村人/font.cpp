//=========================================================================================================================
//
// �t�H���g���� [font.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanf�G���[����
#include "stdio.h"					// io
#include "font.h"					// �t�H���g
#include "title.h"					// �^�C�g��
#include "game.h"					// �Q�[��
#include "xplayer.h"				// �v���C���[
#include "sound.h"					// �T�E���h
#include "window.h"					// �E�B���h�E
#include "input.h"					// ����

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_FONT		(8)								// �t�H���g�̍ő吔
#define MAX_MASSAGE		(64)							// �����̍ő吔
#define FONTFAILE_NAME "data\\TEXT\\messagedata.txt"	// �t�H���g�t�@�C����

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
FONTSTATE g_aFont[MAX_FONT];							// �t�H���g
char g_aMassage[MAX_MASSAGE][MAX_FONTNUM];				// ���b�Z�[�W
int	g_nCntFontTimer;									// �J�E���g
bool g_bMassageSet;										// set�`�F�b�N
int	g_MassageFontNum;
int	g_MassageNum;
int g_MaxMassageNum;

//=========================================================================================================================
// �t�H���g�̏���������
//=========================================================================================================================
void InitFont(void)
{
	int nCntFont;
	FILE *pFile;

	g_bMassageSet = false;
	g_nCntFontTimer = 0;
	g_MassageFontNum = 0;
	g_MassageNum = 0;

	// �t�@�C���ݒ�
	pFile = fopen(FONTFAILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &g_MaxMassageNum);	// �ǂݍ���

		for (nCntFont = 0; nCntFont < g_MaxMassageNum; nCntFont++)
		{
			fscanf(pFile, "%s", &g_aMassage[nCntFont][0]);	// �ǂݍ���
		}

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�ݒ�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// �J�E���g
		g_aFont[nCntFont].pFont = NULL;
		g_aFont[nCntFont].nWidth = 0;
		g_aFont[nCntFont].nHeight = 0;
		g_aFont[nCntFont].nFont = SHIFTJIS_CHARSET;
		g_aFont[nCntFont].rect = { 0, 0, 0, 0 };
		g_aFont[nCntFont].nSet = DT_LEFT;
		g_aFont[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aFont[nCntFont].bUse = false;
	}
}

//=========================================================================================================================
// �t�H���g�̏I������
//=========================================================================================================================
void UninitFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_aFont[nCntFont].pFont != NULL)
		{// �f�o�b�O�\���p�t�H���g�̊J��
			g_aFont[nCntFont].pFont->Release();
			g_aFont[nCntFont].pFont = NULL;
		}
	}
}

//=========================================================================================================================
// �t�H���g�̍X�V����
//=========================================================================================================================
void UpdateFont(void)
{
	MODE mode = GetMode();
	GAMESTATE game = GetGameState();
	int nTitleView = GetViewNum();
	char cData[128];
	bool bUpMessage = false;

	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// �J�E���g
		if (g_aFont[nCntFont].bUse == true)
		{//
			bUpMessage = true;

			break;
		}
	}
	if (bUpMessage == true)
	{
		if ((mode == MODE_TITLE && nTitleView == 1) || game == GAMESTATE_TUTORIAL)
		{// �`���[�g���A�����
			g_nCntFontTimer++;

			if (g_nCntFontTimer >= 30)
			{// �\���J�n
				if (g_bMassageSet == true)
				{// �g�p����Ȃ�
					if (g_nCntFontTimer % 3 == 0)
					{// �\���Ԋu
						wsprintf(&cData[0], &g_aMassage[g_MassageNum][g_MassageFontNum]);
						cData[2] = '\0';
						SetwsFont((g_MassageNum + 1) % 2 == 0, 1, &cData[0]);

						PlaySound(SOUND_LABEL_SE_MESSAGE);

						g_MassageFontNum += 2;

						if (g_aMassage[g_MassageNum][g_MassageFontNum] == NULL)
						{// NULL�ɂȂ�����
							g_MassageNum++;
							g_MassageFontNum = 0;
							g_nCntFontTimer = 0;

							if (g_MassageNum % 2 == 0)
							{
								g_bMassageSet = false;
							}
						}
					}
				}
				else
				{// �\���I��
					if (g_MassageNum < g_MaxMassageNum)
					{
						if (g_MassageNum == 6 && g_nCntFontTimer == 30)
						{// �t�F�[�h
							SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOW_TUTORIAL, SETTEXTURE_FRONT);
						}

						if (g_nCntFontTimer >= 60)
						{// ���������Ď���
							g_bMassageSet = true;
							g_nCntFontTimer = 0;

							SetwsFont(0, 0, "");
							SetwsFont(1, 0, "");
						}
					}
					else
					{// �S���I��
						if (g_nCntFontTimer == 60)
						{// ���b�Z�[�W�j��
							DeleteWindow(WINDOWTEXTURE_MESSAGE);
							SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0, WINDOWTEXTURE_GAME_TUTORIAL, SETTEXTURE_FRONT);

							SetwsFont(0, 0, "");
							SetwsFont(1, 0, "");

							SetMove(true);	// �v���C���[������悤��
						}
						else if (g_nCntFontTimer == 420)
						{// ���상�j���[�j��
							//if (GetKeyboardTrigger(DIK_RETURN) == true)
							{
								DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL);
								DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_MESSAGE);

								SetTutorialStep(true);

								SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 430, SCREEN_HEIGHT - 80, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_2, SETTEXTURE_FRONT);
							}
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================================
// �t�H���g�̕`�揈��
//=========================================================================================================================
void DrawFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// �J�E���g
		if (g_aFont[nCntFont].bUse == true)
		{// �g�p���Ă���Ȃ�
			// �e�L�X�g�`��
			// hDC      : �f�o�C�X�R���e�L�X�g�̃n���h��
			// lpString : �`�悷�镶����ւ̃|�C���^
			// nCount   : ������̕�����
			// lpRect   : �����`�̈�
			// uFormat  : �e�L�X�g�̐��`���@���w��
			g_aFont[nCntFont].pFont->DrawText(NULL, &g_aFont[nCntFont].aFont[0], -1, &g_aFont[nCntFont].rect, g_aFont[nCntFont].nSet, g_aFont[nCntFont].col);
		}
	}
}

//=========================================================================================================================
// �t�H���g�̕`�揈��
//=========================================================================================================================
void SetFont(int nWidth, int nHeight, int nFont, RECT rect, int nSet, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// �J�E���g
		if (g_aFont[nCntFont].bUse == false)
		{// �g�p���Ă��Ȃ��Ȃ�
			g_aFont[nCntFont].nWidth = nWidth;
			g_aFont[nCntFont].nHeight = nHeight;
			g_aFont[nCntFont].nFont = nFont;
			g_aFont[nCntFont].rect = rect;
			g_aFont[nCntFont].nSet = nSet;
			g_aFont[nCntFont].col = col;
			g_aFont[nCntFont].bUse = true;

			// �f�o�b�O�\���p�t�H���g��ݒ�
			// pDevice         : �f�o�C�X�ւ̃|�C���^
			// Height          : �����̍���
			// Width           : �����̕�
			// Weight          : �t�H���g�̑���
			// MipLevels       : �~�b�v�}�b�v���x����
			// Italic          : �C�^���b�N�t�H���g
			// CharSet         : �t�H���g�̕����Z�b�g
			// OutputPrecision : ���ۂ̃t�H���g�ƖړI�̃t�H���g�̃T�C�Y����ѓ����̈�v���@���w��
			// Quality         : ���ۂ̃t�H���g�ƖړI�̃t�H���g�Ƃ̈�v���@���w��
			// PitchAndFamily  : �s�b�`�ƃt�@�~���C���f�b�N�X
			// pFacename       : �t�H���g�̖��O��ێ����镶����
			// ppFont          : �t�H���g�ւ̃|�C���^
			D3DXCreateFont(pDevice, nWidth, nHeight, 0, 0, FALSE, nFont,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Font", &g_aFont[nCntFont].pFont);

			break;
		}
	}
}

//=========================================================================================================================
// �t�H���g�̑������
//=========================================================================================================================
void SetwsFont(int nSetNum, int nSettype, char *aFont)
{
	if (g_aFont[nSetNum].bUse == true)
	{
		// ���������
		if (nSettype == 0)
		{// �S��
			wsprintf(&g_aFont[nSetNum].aFont[0], &aFont[0]);
		}
		else if (nSettype == 1)
		{// �ǋL
			strcat(&g_aFont[nSetNum].aFont[0], &aFont[0]);
		}
	}
}

//=========================================================================================================================
// �t�H���g�̔j������
//=========================================================================================================================
void DeleteFont(void)
{
	int nCntFont;

	g_bMassageSet = false;
	g_nCntFontTimer = 0;
	g_MassageFontNum = 0;
	g_MassageNum = 0;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_aFont[nCntFont].bUse == true)
		{
			SetwsFont(nCntFont, 0, "");

			g_aFont[nCntFont].pFont = NULL;
			g_aFont[nCntFont].nWidth = 0;
			g_aFont[nCntFont].nHeight = 0;
			g_aFont[nCntFont].nFont = SHIFTJIS_CHARSET;
			g_aFont[nCntFont].rect = { 0, 0, 0, 0 };
			g_aFont[nCntFont].nSet = DT_LEFT;
			g_aFont[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aFont[nCntFont].bUse = false;
		}
	}
}

//=========================================================================================================================
// ���b�Z�[�W�\������
//=========================================================================================================================
void SetMessage(bool bMassageSet)
{
	g_bMassageSet = bMassageSet;
}