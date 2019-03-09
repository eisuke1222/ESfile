//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTexture
{// �e�N�X�`��
public:
	typedef enum
	{// ���
		TYPE_MODE_PRESS = 0,				// �v���X
		TYPE_MODE_NUMBER,					// �ԍ�
		TYPE_MODE_END,						// �I���m�F
		TYPE_MODE_YESNO,					// YESNO
		TYPE_MODE_BUTTON,					// �{�^��
		TYPE_TITLE_LOGO,					// �^�C�g��
		TYPE_TITLE_LOGO_BG,					// �^�C�g���G�t�F�N�g�p
		TYPE_TITLE_MENU,					// �^�C�g�����j���[
		TYPE_TITLE_MENU_BG,					// �^�C�g�����j���[BG
		TYPE_GAME_FIELD,					// �t�B�[���h
		TYPE_GAME_EFFECT,					// �G�t�F�N�g
		TYPE_GAME_MAP,						// �}�b�v
		TYPE_GAME_MAPICON,					// �}�b�v�A�C�R��
		TYPE_GAME_SKY_1,					// ��P
		TYPE_GAME_SKY_2,					// ��Q
		TYPE_GAME_GRASS,					// ��
		TYPE_GAME_WARP,						// ���[�v
		TYPE_GAME_GOAL,						// �S�[��
		TYPE_GAME_PLAYER_STEP,				// �X�e�b�v
		TYPE_GAME_PLAYER_WALK,				// ����
		TYPE_GAME_PLAYER_SKILL,				// �X�L��
		TYPE_GAME_CAMERA_ROCKON_FRAME,		// ���b�N�I���t���[��
		TYPE_GAME_CAMERA_ROCKON_PLAY,		// ���b�N�I���v���C
		TYPE_GAME_UI_FRAME,					// UI�t���[��
		TYPE_GAME_UI_CHAR,					// UI�L����
		TYPE_GAME_UI_HPSP,					// UIHPSP
		TYPE_GAME_UI_WEAPON,				// UI����
		TYPE_GAME_UI_PLAYER_NAME,			// UI�v���C���[��
		TYPE_GAME_UI_ENEMY_NAME,			// UI�G��
		TYPE_GAME_UI_RANK,					// UI�����N
		TYPE_GAME_UI_CHECK,					// �`�F�b�N
		TYPE_GAME_UI_TUTORIAL_PLUS,			// �`���[�g���A���v���X
		TYPE_GAME_UI_TUTORIAL_LOGO,			// �`���[�g���A�����S
		TYPE_GAME_UI_TUTORIAL,				// �`���[�g���A��
		TYPE_GAME_UI_CLEAR,					// �N���A
		TYPE_GAME_UI_MISION,				// �~�b�V����
		TYPE_GAME_UI_SKIP,					// �X�L�b�v
		TYPE_GAME_PAUSE_LOGO,				// �|�[�Y���S
		TYPE_GAME_PAUSE_MENU,				// �|�[�Y���j���[
		TYPE_GAME_PAUSE_BG,					// �|�[�Y�w�i
		TYPE_RESULT_CLEAR,					// �N���A
		TYPE_RESULT_CLEAR_BG,				// �N���A�G�t�F�N�g�p
		TYPE_RESULT_OVER,					// �I�[�o�[
		TYPE_RESULT_OVER_BG,				// �I�[�o�[�G�t�F�N�g�p
		TYPE_RESULT_RANK,					// �����N
		TYPE_MAX							// �ő吔
	}TYPE;

	CTexture();											// �R���X�g���N�^
	~CTexture();										// �f�X�g���N�^

	static void ReleaseAll(void);						// �A�����[�h

	static LPDIRECT3DTEXTURE9 GetTexture(int nNum);		// ���f���擾

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];	// �e�N�X�`�����ւ̃|�C���^
	static int					m_pTexType[TYPE_MAX];	// �e�N�X�`�����
	static int					m_nMaxTexture;			// �e�N�X�`����

	const static char			*m_TexName[TYPE_MAX];	// �e�N�X�`����


protected:

};

#endif