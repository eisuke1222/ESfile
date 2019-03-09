//=========================================================================================================================
// 
// �v���C���[���� [player.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "player.h"		// �v���C���[
#include "enemy.h"		// �G
#include "block.h"		// �u���b�N
#include "bullet.h"		// �e
#include "item.h"		// �A�C�e��
#include "score.h"		// �X�R�A
#include "bgwindow.h"	// �w�i
#include "fade.h"		// �t�F�[�h
#include "input.h"		// �L�[�{�[�h
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define PLAYER_TEXTURE_NAME1	"data\\TEXTURE\\all001.png"				// �ǂݍ��ރe�N�X�`���t�@�C��1
#define PLAYER_WIDTH			(40)									// �v���C���[����
#define PLAYER_HEIGHT			(80)									// �v���C���[����
#define PLAYER_NUMPLAYER		(3)										// �v���C���[�c�@
#define PLAYER_LIFE				(3)										// �v���C���[�̗�
#define PLAYER_TEXTURE_SPEED	(6)										// �e�N�X�`���A�j���[�V�����X�s�[�h�i�e���x�j
#define PLAYER_TEXTURE_PATTERN	(5)										// �e�N�X�`���A�j���[�V�����p�^�[����
#define PLAYER_TEXTURE_UV_U		(0.2f)									// �e�N�X�`���A�j���[�V����U�͈�
#define PLAYER_TEXTURE_UV_V		(0.1f)									// �e�N�X�`���A�j���[�V����V�͈�
#define PLAYER_UPSPEED			(1.2f)									// �v���C���[�����X�s�[�h
#define PLAYER_DOWNSPEED		(0.2f)									// �v���C���[�����X�s�[�h
#define PLAYER_JUMPSPEED		(12.0f)									// �v���C���[�㏸�X�s�[�h
#define PLAYER_GRAVITY			(0.5f)									// �v���C���[�d��

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTexturePlayer = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPlayer = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
PLAYER							g_Player;								// �v���C���[���
int								g_nCntColor;							// �F����

//=========================================================================================================================
// �v���C���[����������
//=========================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���C���[��񏉊���
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu
	g_Player.move = D3DXVECTOR3(0, 0, 0.0f);			// �ړ��l
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	g_Player.fWidth = PLAYER_WIDTH;						// ����
	g_Player.fHeight = PLAYER_HEIGHT;					// ����
	g_Player.nNumPlayer = PLAYER_NUMPLAYER;				// �c�@
	g_Player.nLife = PLAYER_LIFE;						// �̗�
	g_Player.fGravity = PLAYER_GRAVITY;					// �d��
	g_Player.State = PLAYERSTATE_APPEAR;				// ���
	g_Player.nCounterState = 0;							// ��ԃJ�E���^
	g_Player.AnimType = PLAYERANIMTYPE_NORMAL;			// �A�j���[�V�������
	g_Player.nCounterAnim = 0;							// �A�j���[�V�����J�E���^
	g_Player.nNumAnim = 0;								// �A�j���[�V�����ԍ�
	g_Player.nSpeedAnim = 1;							// �A�j���[�V�������x
	g_Player.nPatternAnim = 1;							// �A�j���[�V�����p�^�[��
	g_Player.nDinectionMove = 0;						// ����
	g_Player.bAttack = false;							// �U�����Ă��邩
	g_Player.bJump = false;								// �W�����v���Ă��邩
	g_Player.bDisp = false;								// �\�����Ă��邩
	g_Player.pBlock = NULL;								// �u���b�N�|�C���^

	g_nCntColor = 0;									// �F����

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURE_NAME1, &g_pTexturePlayer);	// �v���C���[

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

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
	pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, PLAYER_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// �v���C���[�I������
//=========================================================================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=========================================================================================================================
// �v���C���[�X�V����
//=========================================================================================================================
void UpdatePlayer(void)
{
	//�ϐ��錾
	float fSpeed = PLAYER_UPSPEED;		// �ړ������x

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);
	
	MODE mode = GetMode();				// ��ʏ��
	GAMESTATE game = GetGameState();	// �Q�[�����
	FADE fade = GetFade();				// �t�F�[�h���

	if (g_Player.bDisp == true)
	{// �\�����Ă���ꍇ
		switch (g_Player.State)
		{// ���
		case PLAYERSTATE_APPEAR:		// �o��
			break;
		case PLAYERSTATE_NORMAL:		// �m�[�}��
			break;
		case PLAYERSTATE_DAMAGE:		// �_���[�W
			
			g_Player.nCounterState++;	// �J�E���^��i�߂�
			
			if (g_Player.nCounterState % 5 == 0)
			{// �F�ؑ�
				g_nCntColor = (g_nCntColor + 1) % 2;	// �F�ύX
			}

			if (g_nCntColor == 0) { g_Player.col.a = 0.0f; }// ����
			else { g_Player.col.a = 1.0f; }					// �s����

			if (g_Player.nCounterState > 90)
			{// 1.5�b��
				g_Player.State = PLAYERSTATE_NORMAL;	// �m�[�}����
				g_Player.nCounterState = 0;				// ��ԃJ�E���^��������
				g_Player.col.a = 1.0f;					// �s����
			}

			break;
		case PLAYERSTATE_DEATH:			// �f�X
			break;
		}

		g_Player.nCounterAnim++;	// �J�E���^��i�߂�

		if (mode == MODE_TITLE)
		{// �^�C�g���̏ꍇ
			// ����
			g_Player.AnimType = PLAYERANIMTYPE_WALK;
			g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
			g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;

			if (g_Player.nCounterAnim % g_Player.nSpeedAnim == 0)
			{// �A�j���[�V����
				g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
			}

			// �e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
			pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
			pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
		}
		else if (mode == MODE_GAME)
		{// �Q�[���̏ꍇ
			if (game == GAMESTATE_MAP)
			{
				// �ړ���������
				if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_LEFT) == true)
				{// �C�ӂ̃L�[�iA�L�[�A���L�[�j���������ꍇ
					if (fade != FADE_OUT)
					{
						if (g_Player.bJump == false)
						{// �\���̐���
							g_Player.nNumAnim = 0;					// �A�j���[�V�������ŏ�����

							// ����
							g_Player.AnimType = PLAYERANIMTYPE_RUN;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_RUN;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
					}
				}
				else if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_D) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_RIGHT) == true)
				{// �C�ӂ̃L�[�iD�L�[�A�E�L�[�j���������ꍇ
					if (fade != FADE_OUT)
					{
						if (g_Player.bJump == false && g_Player.bAttack == false)
						{// �\���̐���
							g_Player.nNumAnim = 0;					// �A�j���[�V�������ŏ�����

							// ����
							g_Player.AnimType = PLAYERANIMTYPE_RUN;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_RUN;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
					}
				}
				else if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
				{
					if (g_Player.move.x == 0.0f && g_Player.move.y == 0.0f && fade == FADE_NONE)
					{
						g_Player.nNumAnim = 0;					// �A�j���[�V�������ŏ�����

						// �W�����v
						g_Player.AnimType = PLAYERANIMTYPE_JUMP;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_JUMP;
						g_Player.nSpeedAnim = 20;

						g_Player.bJump = true;					// �W�����v���g�p���Ă���
					}
				}
				else
				{// �L�[�������Ă��Ȃ��Ƃ�
					if (g_Player.bJump == false)
					{
						if (g_Player.move.x == 0.0f &&
							g_Player.move.y == 0.0f)
						{
							if (g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
							{// �A�j���[�V�������ς�����ꍇ
								g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
							}

							// ����
							g_Player.AnimType = PLAYERANIMTYPE_NORMAL;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_NORMAL;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
					}
				}

				// �ړ�����
				g_Player.pos += g_Player.move;

				if (g_Player.nCounterAnim % g_Player.nSpeedAnim == 0 && g_Player.bJump == false)
				{// �A�j���[�V����
					g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
				}

				// ���_�ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);

				// �e�N�X�`���ݒ�
				if (g_Player.nDinectionMove == 0)
				{// �E
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
				else if (g_Player.nDinectionMove == 1)
				{// ��
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
			}
			else
			{
				if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
				{// �C�ӂ̃L�[�iSHIFT)���������ꍇ
					fSpeed *= 2;	// �����x���Q�{
					if (g_Player.bJump == false && g_Player.bAttack == false && g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
					{// �\���̐���
						if (g_Player.AnimType != PLAYERANIMTYPE_RUN)
						{// �A�j���[�V�������ς�����ꍇ
							g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
						}

						// ����
						g_Player.AnimType = PLAYERANIMTYPE_RUN;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_RUN;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
					}
				}
				else if (GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
				{// �C�ӂ̃L�[�iSHIFT)�𗣂����ꍇ
					if (g_Player.bJump == false && g_Player.bAttack == false && g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
					{// �\���̐���
						if (g_Player.AnimType != PLAYERANIMTYPE_WALK)
						{//�@�A�j���[�V�������ς�����ꍇ
							g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
						}

						// ����
						g_Player.AnimType = PLAYERANIMTYPE_WALK;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
					}
				}

				// �ړ���������
				if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
				{// �C�ӂ̃L�[�iA�L�[�A���L�[�j���������ꍇ
					g_Player.move.x += sinf(-D3DX_PI * 0.5f) * fSpeed;		// �ړ��l����
					if (g_Player.bJump == false && g_Player.bAttack == false)
					{// �\���̐���
						if (g_Player.AnimType != PLAYERANIMTYPE_RUN)
						{// �ؑւ̒���
							if (g_Player.AnimType != PLAYERANIMTYPE_WALK)
							{// �A�j���[�V�������ς�����ꍇ
								g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
							}

							// ����
							g_Player.AnimType = PLAYERANIMTYPE_WALK;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
						g_Player.nDinectionMove = 1;	// �����𒲐�
					}
				}
				else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
				{// �C�ӂ̃L�[�iD�L�[�A�E�L�[�j���������ꍇ
					g_Player.move.x += sinf(D3DX_PI * 0.5f) * fSpeed;		// �ړ��l����
					if (g_Player.bJump == false && g_Player.bAttack == false)
					{// �\���̐���
						if (g_Player.AnimType != PLAYERANIMTYPE_RUN)
						{// �ؑւ̒���
							if (g_Player.AnimType != PLAYERANIMTYPE_WALK)
							{// �A�j���[�V�������ς�����ꍇ
								g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
							}

							// ����
							g_Player.AnimType = PLAYERANIMTYPE_WALK;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
						g_Player.nDinectionMove = 0;	// �����𒲐�
					}
				}
				else
				{// �L�[�������Ă��Ȃ��Ƃ�
					if (g_Player.bJump == false && g_Player.bAttack == false)
					{// �\���̐���
						if (g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
						{// �A�j���[�V�������ς�����ꍇ
							g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
						}

						// ����
						g_Player.AnimType = PLAYERANIMTYPE_NORMAL;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_NORMAL;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
					}
				}

				// �W�����v����
				if ((GetKeyboardTrigger(DIK_SPACE) == true || GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true) && g_Player.bJump == false && g_Player.bAttack == false)
				{// �C�ӂ̃L�[�i�X�y�[�X�L�[�j���������ꍇ
					g_Player.move.y -= PLAYER_JUMPSPEED;	// �ړ��l����
					g_Player.nNumAnim = 0;					// �A�j���[�V�������ŏ�����

					// �W�����v
					g_Player.AnimType = PLAYERANIMTYPE_JUMP;
					g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_JUMP;
					g_Player.nSpeedAnim = 20;
					PlaySound(SOUND_LABEL_SE_JUMP000);

					g_Player.bJump = true;					// �W�����v���g�p���Ă���
				}
				else if (g_Player.bJump == true && g_Player.bAttack == false)
				{// �W�����v���g�p���Ă���ꍇ
					if (g_Player.move.y == 0.0f)
					{// �A�j���[�V����
						g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
					}
				}

				if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true && g_Player.bAttack == false)
				{// �C�ӂ̃L�[�iZ�AENTER�j���������ꍇ
					if (g_Player.bAttack == false)
					{//�U�����Ă��Ȃ��ꍇ
						if (g_Player.AnimType != PLAYERANIMTYPE_PATTERN_THROW)
						{// �A�j���[�V�������ς�����ꍇ
							g_Player.nNumAnim = 0;		// �A�j���[�V�������ŏ�����
						}

						// ������
						g_Player.AnimType = PLAYERANIMTYPE_THROW;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_THROW;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;

						g_Player.bAttack = true;		// �U�����g�p���Ă���
					}
				}
				else if (g_Player.bAttack == true)
				{// �U�����g�p���Ă���ꍇ
					if (g_Player.nNumAnim == g_Player.nPatternAnim - 1)
					{// �A�j���[�V�������Ō�܂Ői�񂾏ꍇ
						g_Player.bAttack = false;		// �U�����g�p���Ă��Ȃ���ԃw
					}
					else if (g_Player.nNumAnim == g_Player.nPatternAnim - 4 && g_Player.nCounterAnim % g_Player.nSpeedAnim == 0)
					{// ����̃A�j���[�V�����ɕς�����ꍇ
						// �e���ˏ���
						if (g_Player.nDinectionMove == 0)
						{// �E
							SetBullet(D3DXVECTOR3(g_Player.pos.x + (g_Player.fWidth / 2), g_Player.pos.y - (g_Player.fHeight / 2) - 20, 0.0f),
								D3DXVECTOR3(20.0f, -0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BULLETTYPE_PLAYER, 8.0f, 16.0f, BULLETTEXTURETYPE_PLAYER_NORMAL);
						}
						else
						{// ��
							SetBullet(D3DXVECTOR3(g_Player.pos.x + (g_Player.fWidth / 2), g_Player.pos.y - (g_Player.fHeight / 2) - 20, 0.0f),
								D3DXVECTOR3(-20.0f, -0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BULLETTYPE_PLAYER, 8.0f, 16.0f, BULLETTEXTURETYPE_PLAYER_NORMAL);
						}
						PlaySound(SOUND_LABEL_SE_SHOT000);
					}
				}

				if (g_Player.nCounterAnim % g_Player.nSpeedAnim == 0 && (g_Player.bJump == false || g_Player.bAttack == true))
				{// �A�j���[�V����
					g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
				}

				// �ʒu���L��
				g_Player.posOld = g_Player.pos;
				// �d�͉��Z
				g_Player.move.y += g_Player.fGravity;
				// �ړ�����
				g_Player.pos.y += g_Player.move.y;
				// ��������
				g_Player.move.x += (0.0f - g_Player.move.x) * PLAYER_DOWNSPEED;

				// �u���b�N�Ƃ̏Փˏ���
				if ((CollisionBGWindow(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true) ||
					(CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight - 30.0f, &g_Player.pBlock, COLLISIONBLOCKRETURN_PLAYER) == true))
				{// �Փ˂��Ă���ꍇ
					g_Player.bJump = false;		// �W�����v���g�p���Ă��Ȃ�
				}
				else
				{// �Փ˂��Ă��Ȃ��ꍇ
					g_Player.bJump = true;		// �W�����v���g�p���Ă���
				}

				if (g_Player.pBlock != NULL)
				{// pBlock��NULL�łȂ��ꍇ
					g_Player.pos += g_Player.pBlock->move;

					if (g_Player.pBlock->type == BLOCKTYPE_NORMAL_DAMAGE && g_Player.State == PLAYERSTATE_NORMAL)
					{
						// �m�b�N�o�b�N�ݒ�
						g_Player.move.x = 0.0f;
					
						if (g_Player.move.y <= 0.0f)
						{
							g_Player.move.y *= -1;
						}
						if (g_Player.pos.x < g_Player.pBlock->pos.x + (g_Player.pBlock->fWidth / 2))
						{
							g_Player.move.x -= 50;
						}
						else
						{
							g_Player.move.x += 50;
						}
						
						g_Player.nLife--;	// �̗͌���
						g_Player.State = PLAYERSTATE_DAMAGE;	// �_���[�W��Ԃ�
						PlaySound(SOUND_LABEL_SE_HIT000);
						HitPlayerLife();	// �e�N�X�`���ݒ�

						if (g_Player.nLife <= 0)
						{// �̗͂�0�ȉ��̏ꍇ
							g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_Player.nNumPlayer--;				// �c�@�����炷
							if (g_Player.nNumPlayer < 0)
							{// �c�@��0�ȉ��̏ꍇ
								SetGameState(GAMESTATE_GAMEOVER);			// �Q�[���I�[�o�[��
							}
							else
							{// ����ȊO
								DeletePlayer();
								SetFadeGame(GAMESTATE_MAP);		// �Q�[���}�b�v��
							}
						}
					}
				}

				// �A�C�e���Ƃ̏Փˏ���
				CollisionItem(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight);

				// �G�Ƃ̏Փˏ���
				if (CollisionEnemy(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true && g_Player.State == PLAYERSTATE_NORMAL)
				{// �Փ˂��Ă���ꍇ
					g_Player.nLife--;	// �̗͌���
					g_Player.State = PLAYERSTATE_DAMAGE;	// �_���[�W��Ԃ�
					PlaySound(SOUND_LABEL_SE_HIT000);
					HitPlayerLife();	// �e�N�X�`���ݒ�

					if (g_Player.nLife <= 0)
					{// �̗͂�0�ȉ��̏ꍇ
						g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_Player.nNumPlayer--;				// �c�@�����炷
						if (g_Player.nNumPlayer < 0)
						{// �c�@��0�ȉ��̏ꍇ
							SetGameState(GAMESTATE_GAMEOVER);			// �Q�[���I�[�o�[��
						}
						else
						{// ����ȊO
							DeletePlayer();
							SetFadeGame(GAMESTATE_MAP);		// �Q�[���}�b�v��
						}
					}
				}
				
				if (g_Player.pos.y - g_Player.fHeight > SCREEN_HEIGHT)
				{// ��ʊO����
					g_Player.move.x = 0.0f;				// ���ړ��l���O
					g_Player.bDisp = false;				// �v���C���[��\�����Ȃ�
					
					g_Player.nNumPlayer--;				// �c�@�����炷
					if (g_Player.nNumPlayer < 0)
					{// �c�@��0�ȉ��̏ꍇ
						SetGameState(GAMESTATE_GAMEOVER);			// �Q�[���I�[�o�[��
					}
					else
					{// ����ȊO
						DeletePlayer();
						SetFadeGame(GAMESTATE_MAP);		// �Q�[���}�b�v��
					}
				}

				// ���_�ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);
				// ���_�J���[�ݒ�
				pVtx[0].col = g_Player.col;
				pVtx[1].col = g_Player.col;
				pVtx[2].col = g_Player.col;
				pVtx[3].col = g_Player.col;

				// �e�N�X�`���ݒ�
				if (g_Player.nDinectionMove == 0)
				{// �E
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
				else if (g_Player.nDinectionMove == 1)
				{// ��
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// �v���C���[�`�揈��
//=========================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	// �v���C���[�̕`��
	if (g_Player.bDisp == true)
	{// true�̏ꍇ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================================================================================================
// �v���C���[�ݒ菈��
//=========================================================================================================================
void SetPlayer(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	g_Player.pos = pos;								// �ʒu
	g_Player.move = D3DXVECTOR3(0, 0, 0.0f);		// �ړ��l
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	g_Player.fWidth = PLAYER_WIDTH;					// ����
	g_Player.fHeight = PLAYER_HEIGHT;				// ����
	g_Player.nLife = PLAYER_LIFE;					// �̗�
	g_Player.fGravity = PLAYER_GRAVITY;				// �d��
	g_Player.State = PLAYERSTATE_NORMAL;			// ���
	g_Player.nCounterState = 0;						// ��ԃJ�E���^
	g_Player.AnimType = PLAYERANIMTYPE_NORMAL;		// �A�j���[�V�������
	g_Player.nCounterAnim = 0;						// �A�j���[�V�����J�E���^
	g_Player.nNumAnim = 0;							// �A�j���[�V�����ԍ�
	g_Player.nSpeedAnim = 1;						// �A�j���[�V�������x
	g_Player.nPatternAnim = 1;						// �A�j���[�V�����p�^�[��
	g_Player.nDinectionMove = 0;					// ����
	g_Player.bAttack = false;						// �U�����g�p���Ă��Ȃ�
	g_Player.bJump = false;							// �W�����v���g�p���Ă��Ȃ�
	g_Player.bDisp = true;							// �\������
	g_Player.pBlock = NULL;							// �u���b�N�|�C���^��NULL

	AddScore(SCORETYPE_PLAYER, g_Player.nNumPlayer);// �c�@�ݒ�

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// �v���C���[�擾����
//=========================================================================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;		// �l��Ԃ�
}	

//=========================================================================================================================
// �v���C���[�j������
//=========================================================================================================================
void DeletePlayer(void)
{
	if (g_Player.bDisp == true)
	{
		// �v���C���[��񏉊���
		g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu
		g_Player.move = D3DXVECTOR3(0, 0, 0.0f);			// �ړ��l
		g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_Player.fWidth = PLAYER_WIDTH;						// ����
		g_Player.fHeight = PLAYER_HEIGHT;					// ����
		g_Player.fGravity = PLAYER_GRAVITY;					// �d��
		g_Player.State = PLAYERSTATE_APPEAR;				// ���
		g_Player.nCounterState = 0;							// ��ԃJ�E���^
		g_Player.AnimType = PLAYERANIMTYPE_NORMAL;			// �A�j���[�V�������
		g_Player.nCounterAnim = 0;							// �A�j���[�V�����J�E���^
		g_Player.nNumAnim = 0;								// �A�j���[�V�����ԍ�
		g_Player.nSpeedAnim = 1;							// �A�j���[�V�������x
		g_Player.nPatternAnim = 1;							// �A�j���[�V�����p�^�[��
		g_Player.nDinectionMove = 0;						// ����
		g_Player.bAttack = false;							// �U�����Ă��邩
		g_Player.bJump = false;								// �W�����v���Ă��邩
		g_Player.bDisp = false;								// �\�����Ă��邩
		g_Player.pBlock = NULL;								// �u���b�N�|�C���^
	}
}