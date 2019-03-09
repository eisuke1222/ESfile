//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Shouta Sano
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:
	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum
	{// �T�E���h��
		SOUND_LABEL_BGM_TITLE000 = 0,	// BGM(�^�C�g��)
		SOUND_LABEL_BGM_GAME000,		// BGM(�Q�[��)
		SOUND_LABEL_BGM_GAME001,		// BGM(�Q�[��)
		SOUND_LABEL_BGM_GAME002,		// BGM(�Q�[��)
		SOUND_LABEL_SE_GAMEEND,			// SE(�G���h)
		SOUND_LABEL_SE_SHOT000,			// SE(����)
		SOUND_LABEL_SE_RANGE000,		// SE(�͈�)
		SOUND_LABEL_SE_DAMAGE,			// SE(�_���[�W)
		SOUND_LABEL_SE_SKILL000,		// SE(�X�L��)
		SOUND_LABEL_SE_SKILL001,		// SE(�X�L��)
		SOUND_LABEL_SE_JUMP,			// SE(���[�v)
		SOUND_LABEL_SE_LANDING,			// SE(���[�v)
		SOUND_LABEL_SE_GRASS,			// SE(���[�v)
		SOUND_LABEL_SE_WORP000,			// SE(���[�v)
		SOUND_LABEL_SE_SLIME000,		// SE(�X���C��)
		SOUND_LABEL_SE_ENEMY_ATTACK000,	// SE(�G�U��0)
		SOUND_LABEL_SE_ENEMY_ATTACK001,	// SE(�G�U��1)
		SOUND_LABEL_SE_DON,				// SE(�h��)
		SOUND_LABEL_SE_WOLK,			// SE(����)
		SOUND_LABEL_SE_EREA_WORP,		// SE(�G���A���[�v)
		SOUND_LABEL_SE_TUTORIAL_CLEAR,	// SE(�`���[�g���A���N���A)
		SOUND_LABEL_SE_PAUSE000,		// SE(�|�[�Y)
		SOUND_LABEL_SE_SERECT000,		// SE(����)
		SOUND_LABEL_SE_ENTER000,		// SE(����)
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();													// �R���X�g���N�^
	~CSound();													// �f�X�g���N�^

	HRESULT Init(HWND hWnd);									// ����������	
	void Uninit(void);											// �I������
	HRESULT PlaySound(SOUND_LABEL label);						// �Đ�
	void StopSound(SOUND_LABEL label);							// ��~
	void StopSound(void);										// �S��~
	void ChangeVolume(SOUND_LABEL label, float fVolume);		// �{�����[���ύX
	void ChagePitch(SOUND_LABEL label, float fPitch);			// �s�b�`�ύX

private:
	typedef struct
	{// �p�����[�^
		char *pFilename;		// �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{// �f�[�^�p�X
		{ "data/SOUND/BGM/TITLE/Title001.wav", -1 },			// BGM0
		{ "data/SOUND/BGM/GAME/game000.wav", -1 },				// BGM0
		{ "data/SOUND/BGM/GAME/game001.wav", -1 },				// BGM0
		{ "data/SOUND/BGM/GAME/game002.wav", -1 },				// BGM0
		{ "data/SOUND/SE/GAME/gameend.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/shot000.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/range000.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/damage.wav", 0 },					// SE0
		{ "data/SOUND/SE/GAME/skill000.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/skill001.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/Action_Jump.wav", 0 },			// SE0
		{ "data/SOUND/SE/GAME/Action_Landing.wav", 0 },			// SE0
		{ "data/SOUND/SE/GAME/Dash_Grassy.wav", 0 },			// SE0
		{ "data/SOUND/SE/GAME/worp000.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/slime000.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/enemyattack000.wav", 0 },			// SE0
		{ "data/SOUND/SE/GAME/enemyattack001.wav", 0 },			// SE0
		{ "data/SOUND/SE/GAME/don000.wav", 0 },					// SE0
		{ "data/SOUND/SE/GAME/walk.wav", 0 },					// SE0
		{ "data/SOUND/SE/GAME/ereaworp.wav", 0 },				// SE0
		{ "data/SOUND/SE/GAME/tutorialclear.wav", 0 },			// SE0
		{ "data/SOUND/SE/GAME/pause000.wav", 0 },				// SE0
		{ "data/SOUND/SE/OTHER/select000.wav", 0 },				// SE0
		{ "data/SOUND/SE/OTHER/enter000.wav", 0 },				// SE0
	};

protected:
};

#endif