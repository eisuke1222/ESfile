//=========================================================================================================================
// 
// ���U���g���� [resule.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "result.h"		// ���U���g
#include "game.h"		// �Q�[��		
#include "sound.h"		// �T�E���h
#include "fade.h"		// �t�F�[�h
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define RESULT_TEXTURE_NAME_1	"data\\TEXTURE\\gameclear000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define RESULT_TEXTURE_NAME_2	"data\\TEXTURE\\gameover000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define RESULT_POS_X			(0)									// ���U���g�̍���X���W
#define RESULT_POS_Y			(0)									// ���U���g�̍���Y���W
#define RESULT_WIDTH			(SCREEN_WIDTH)						// ���U���g�̕�
#define RESULT_HEIGHT			(SCREEN_HEIGHT)						// ���U���g�̍���
#define RESULT_TEXTURE_UV_U		(1.0f)								// �e�N�X�`���A�j���[�V����U�͈�
#define RESULT_TEXTURE_UV_V		(1.0f)								// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureResult = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3					g_posResult;							// ���U���g�̈ʒu
int							g_nCntResultState;						// ��ԃJ�E���^

//=========================================================================================================================
// ���U���g����������
//=========================================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_posResult = D3DXVECTOR3((RESULT_POS_X + RESULT_WIDTH) / 2, (RESULT_POS_Y + RESULT_HEIGHT) / 2, 0.0f);		// �|���S���̒��S�l
	g_nCntResultState = 0;						//��ԃJ�E���^��������

	GAMESTATE GameStateNow = GetGameState();	// �Q�[���̏�Ԃ��擾

	// �e�N�X�`���̓ǂݍ���
	if (GameStateNow == GAMESTATE_GAMECLEAR)
	{// �Q�[���N���A�̏ꍇ
		D3DXCreateTextureFromFile
		(// ���U���g1
			pDevice,
			RESULT_TEXTURE_NAME_1,
			&g_pTextureResult
		);

		PlaySound(SOUND_LABEL_SE_CLEAR);	// �Q�[���N���A
	}
	else
	{// �Q�[���I�[�o�[
		D3DXCreateTextureFromFile
		(// ���U���g2
			pDevice,
			RESULT_TEXTURE_NAME_2,
			&g_pTextureResult
		);

		PlaySound(SOUND_LABEL_SE_OVER);		// �Q�[���I�[�o�[
	}

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffResult->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_HEIGHT, 0.0f);
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
	pVtx[1].tex = D3DXVECTOR2(RESULT_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, RESULT_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(RESULT_TEXTURE_UV_U, RESULT_TEXTURE_UV_V);
	
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();
}

//=========================================================================================================================
// ���U���g�I������
//=========================================================================================================================
void UninitResult(void)
{
	// �e�N�X�`���[�̔j��
	if (g_pTextureResult != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pTextureResult->Release();	// �J��
		g_pTextureResult = NULL;		// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffResult->Release();	// �J��
		g_pVtxBuffResult = NULL;		// NULL��
	}
}

//=========================================================================================================================
// ���U���g�X�V����
//=========================================================================================================================
void UpdateResult(void)
{
	g_nCntResultState++;			// ��Ԃ��J�E���g
	
	FADE Fade = GetFade();			// �t�F�[�h���擾

	if (Fade == FADE_NONE)
	{// �t�F�[�h���g�p���Ă��Ȃ��ꍇ
		if (g_nCntResultState > 180 || GetKeyboardTrigger(DIK_RETURN) == true)
		{// ��ʐ؂�ւ�
			SetFade(MODE_RANKING);		// �����L���O
		}
	}
}

//=========================================================================================================================
// ���U���g�`�揈��
//=========================================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureResult);

	// ���U���g�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}