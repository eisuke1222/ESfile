//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Eisuke Sakagawa 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{// サウンド名
		SOUND_LABEL_BGM000 = 0,		// BGM000
		SOUND_LABEL_BGM001,			// BGM001
		SOUND_LABEL_SE000,			// SE000
		SOUND_LABEL_SE001,			// SE001
		SOUND_LABEL_SE002,			// SE002
		SOUND_LABEL_SE003,			// SE003
		SOUND_LABEL_SE004,			// SE004
		SOUND_LABEL_SE005,			// SE005
		SOUND_LABEL_SE006,			// SE006
		SOUND_LABEL_SE007,			// SE007
		SOUND_LABEL_SE008,			// SE008
		SOUND_LABEL_SE009,			// SE009
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{// データパス
		{ "data/BGM/bgm_000.wav", -1 },		// BGM0
		{ "data/BGM/sumia.wav", -1 },		// BGM1
		{ "data/SE/pausein.wav", 0 },		// SE0
		{ "data/SE/secect.wav", 0 },		// SE1
		{ "data/SE/enter.wav", 0 },			// SE2
		{ "data/SE/shoot1.wav", 0 },		// SE3
		{ "data/SE/shoot2.wav", 0 },		// SE4
		{ "data/SE/explosion.wav", 0 },		// SE5
		{ "data/SE/boss.wav", 0 },			// SE6
		{ "data/SE/title.wav", 0 },			// SE7
		{ "data/SE/player.wav", 0 },		// SE8
		{ "data/SE/item.wav", 0 },			// SE9
	};

protected:

};

#endif
