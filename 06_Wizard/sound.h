//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Shouta Sano
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
		SOUND_LABEL_BGM_TITLE000 = 0,	// BGM(タイトル)
		SOUND_LABEL_BGM_GAME000,		// BGM(ゲーム)
		SOUND_LABEL_BGM_GAME001,		// BGM(ゲーム)
		SOUND_LABEL_BGM_GAME002,		// BGM(ゲーム)
		SOUND_LABEL_SE_GAMEEND,			// SE(エンド)
		SOUND_LABEL_SE_SHOT000,			// SE(撃つ)
		SOUND_LABEL_SE_RANGE000,		// SE(範囲)
		SOUND_LABEL_SE_DAMAGE,			// SE(ダメージ)
		SOUND_LABEL_SE_SKILL000,		// SE(スキル)
		SOUND_LABEL_SE_SKILL001,		// SE(スキル)
		SOUND_LABEL_SE_JUMP,			// SE(ワープ)
		SOUND_LABEL_SE_LANDING,			// SE(ワープ)
		SOUND_LABEL_SE_GRASS,			// SE(ワープ)
		SOUND_LABEL_SE_WORP000,			// SE(ワープ)
		SOUND_LABEL_SE_SLIME000,		// SE(スライム)
		SOUND_LABEL_SE_ENEMY_ATTACK000,	// SE(敵攻撃0)
		SOUND_LABEL_SE_ENEMY_ATTACK001,	// SE(敵攻撃1)
		SOUND_LABEL_SE_DON,				// SE(ドン)
		SOUND_LABEL_SE_WOLK,			// SE(歩く)
		SOUND_LABEL_SE_EREA_WORP,		// SE(エリアワープ)
		SOUND_LABEL_SE_TUTORIAL_CLEAR,	// SE(チュートリアルクリア)
		SOUND_LABEL_SE_PAUSE000,		// SE(ポーズ)
		SOUND_LABEL_SE_SERECT000,		// SE(決定)
		SOUND_LABEL_SE_ENTER000,		// SE(決定)
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();													// コンストラクタ
	~CSound();													// デストラクタ

	HRESULT Init(HWND hWnd);									// 初期化処理	
	void Uninit(void);											// 終了処理
	HRESULT PlaySound(SOUND_LABEL label);						// 再生
	void StopSound(SOUND_LABEL label);							// 停止
	void StopSound(void);										// 全停止
	void ChangeVolume(SOUND_LABEL label, float fVolume);		// ボリューム変更
	void ChagePitch(SOUND_LABEL label, float fPitch);			// ピッチ変更

private:
	typedef struct
	{// パラメータ
		char *pFilename;		// ファイル名
		int nCntLoop;			// ループカウント(-1でループ再生)
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