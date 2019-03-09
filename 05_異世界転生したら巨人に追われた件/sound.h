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
	 // BGM
		SOUND_LABEL_BGM000 = 0,	// BGM(タイトル)
		SOUND_LABEL_BGM001,		// BGM(ゲーム)
		SOUND_LABEL_BGM002,		// BGM(ゲーム2)
		SOUND_LABEL_BGM003,		// BGM(ゲームクリア)
		SOUND_LABEL_BGM004,		// BGM(ゲームオーバー)
		SOUND_LABEL_BGM005,		// BGM(チュートリアル)

								// SE(システム)
								SOUND_LABEL_SE000,		// SE(ポーズメニュー表示)
								SOUND_LABEL_SE001,		// SE(決定)
								SOUND_LABEL_SE002,		// SE(セレクト)
								SOUND_LABEL_SE003,		// SE(キャンセル)
								SOUND_LABEL_SE004,		// SE(スピードアップ)
								SOUND_LABEL_SE005,		// SE(スピードダウン)
								SOUND_LABEL_SE006,		// SE(コイン取得)
								SOUND_LABEL_SE007,		// SE(ライフ回復)

														// SE(プレイヤー周辺)
														SOUND_LABEL_SE008,		// SE(歩行(平地))
														SOUND_LABEL_SE009,		// SE(ダッシュ(平地))
														SOUND_LABEL_SE010,		// SE(歩行(木・ブロック上))
														SOUND_LABEL_SE011,		// SE(ダッシュ(木・ブロック上))
														SOUND_LABEL_SE012,		// SE(歩行(草むら))
														SOUND_LABEL_SE013,		// SE(ダッシュ(草むら))
														SOUND_LABEL_SE014,		// SE(歩行(水上))
														SOUND_LABEL_SE015,		// SE(ブレーキ(ダッシュキャンセル))
														SOUND_LABEL_SE016,		// SE(ジャンプ)
														SOUND_LABEL_SE017,		// SE(着地)
														SOUND_LABEL_SE018,		// SE(倒れる)
														SOUND_LABEL_SE019,		// SE(死亡)
														SOUND_LABEL_SE020,		// SE(昇天)

																				// SE(環境音・巨人の足音)
																				SOUND_LABEL_SE021,		// SE(巨人の足音)
																				SOUND_LABEL_SE022,		// SE(風)
																				SOUND_LABEL_SE023,		// SE(強風)
																				SOUND_LABEL_SE024,		// SE(そよ風)
																				SOUND_LABEL_SE025,		// SE(ブロックを押す)
																				SOUND_LABEL_SE026,		// SE(ツタを登る)
																				SOUND_LABEL_SE027,		// SE(シーソー)
																				SOUND_LABEL_SE028,		// SE(水飛込み)
																				SOUND_LABEL_SE029,		// SE(水流)
																				SOUND_LABEL_SE030,		// SE(水流2)
																				SOUND_LABEL_SE031,		// SE(滝(川))
																				SOUND_LABEL_SE032,		// SE(ステージ崩壊音)
																				SOUND_LABEL_SE033,		// SE(雨)
																				SOUND_LABEL_SE034,		// SE(鳥)
																				SOUND_LABEL_SE035,		// SE(鳥2)
																				SOUND_LABEL_SE036,		// SE(鳥3)
																				SOUND_LABEL_SE037,		// SE(虫)

																										// 追加分
																										SOUND_LABEL_SE038,	// SE(つかむ)
																										SOUND_LABEL_SE039,	// SE(おちる)
																										SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	void ChangeVolume(SOUND_LABEL label, float fVolume);
	void ChagePitch(SOUND_LABEL label, float fPitch);

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
		{ "Data/SOUND/BGM/Title002.wav", -1 },				// BGM0	// タイトル
		{ "Data/SOUND/BGM/Game000.wav", -1 },				// BGM1	// ゲーム
		{ "Data/SOUND/BGM/Game003.wav", -1 },				// BGM2	// ゲーム(巨人接近時)
		{ "Data/SOUND/BGM/GameClear.wav", -1 },				// BGM3	// リザルト(ゲームクリア)
		{ "Data/SOUND/BGM/Result000.wav", -1 },				// BGM4 // リザルト(ゲームオーバー)
		{ "Data/SOUND/BGM/Tutorial_Loop.wav", -1 },			// BGM4 // リザルト(チュートリアル)
		{ "Data/SOUND/SE/SYSTEM/Pause_Open.wav", 0 },		// SE0	// ポーズメニュー表示
		{ "Data/SOUND/SE/SYSTEM/Pause_Decision.wav", 0 },	// SE1	// 決定
		{ "Data/SOUND/SE/SYSTEM/Pause_Select.wav", 0 },		// SE2	// セレクト
		{ "Data/SOUND/SE/SYSTEM/Pause_Cancel.wav", 0 },		// SE3	// キャンセル
		{ "Data/SOUND/SE/SYSTEM/SpeedUp.wav", 0 },			// SE4	// スピードアップ
		{ "Data/SOUND/SE/SYSTEM/SpeedDown.wav", 0 },		// SE5	// スピードダウン
		{ "Data/SOUND/SE/SYSTEM/Coin.wav", 0 },				// SE6	// コイン取得
		{ "Data/SOUND/SE/SYSTEM/Recovery.wav", 0 },			// SE7	// ライフ取得
		{ "Data/SOUND/SE/PLAYER/Walk_Normal.wav", 0 },		// SE8	// 歩行(平地)
		{ "Data/SOUND/SE/PLAYER/Dash_Normal.wav", 0 },		// SE9	// ダッシュ(平地)
		{ "Data/SOUND/SE/PLAYER/Walk_Tree.wav", 0 },		// SE10	// 歩行(木・ブロック上)
		{ "Data/SOUND/SE/PLAYER/Dash_Tree.wav", 0 },		// SE11	// ダッシュ(木・ブロック上)
		{ "Data/SOUND/SE/PLAYER/Walk_Grassy.wav", 0 },		// SE12	// 歩行(草むら)
		{ "Data/SOUND/SE/PLAYER/Dash_Grassy.wav", 0 },		// SE13	// ダッシュ(草むら)
		{ "Data/SOUND/SE/PLAYER/Walk_Water.wav", 0 },		// SE14	// 歩行(水上)
		{ "Data/SOUND/SE/PLAYER/Action_Brake.wav", 0 },		// SE15	// ブレーキ(ダッシュキャンセル)
		{ "Data/SOUND/SE/PLAYER/Action_Jump.wav", 0 },		// SE16	// ジャンプ
		{ "Data/SOUND/SE/PLAYER/Action_Landing.wav", 0 },	// SE17	// 着地
		{ "Data/SOUND/SE/PLAYER/Action_Down.wav", 0 },		// SE18	// 倒れる
		{ "Data/SOUND/SE/PLAYER/Action_Death.wav", 0 },		// SE19	// 死亡
		{ "Data/SOUND/SE/PLAYER/Action_Ascension.wav", 0 },	// SE20	// 昇天
		{ "Data/SOUND/SE/GAME/Walk_Giant.wav", 0 },			// SE21	// 巨人の足音
		{ "Data/SOUND/SE/GAME/Wind000.wav", 0 },			// SE22	// 風
		{ "Data/SOUND/SE/GAME/Wind001.wav", 0 },			// SE23	// 強風
		{ "Data/SOUND/SE/GAME/Wind002.wav", 0 },			// SE24	// そよ風
		{ "Data/SOUND/SE/GAME/Push.wav", 0 },				// SE25	// ブロックを押す
		{ "Data/SOUND/SE/GAME/Ivy.wav", 0 },				// SE26	// ツタを登る
		{ "Data/SOUND/SE/GAME/Seesaw.wav", 0 },				// SE27	// シーソー
		{ "Data/SOUND/SE/GAME/DiveWater.wav", 0 },			// SE28	// 水飛込み
		{ "Data/SOUND/SE/GAME/Waterflow000.wav", 0 },		// SE29	// 水流
		{ "Data/SOUND/SE/GAME/Waterflow001.wav", 0 },		// SE30	// 水流2
		{ "Data/SOUND/SE/GAME/River.wav", 0 },				// SE31	// 滝(川)
		{ "Data/SOUND/SE/GAME/Collapse.wav", 0 },			// SE32	// ステージ崩壊音
		{ "Data/SOUND/SE/GAME/Rain.wav", 0 },				// SE33	// 雨
		{ "Data/SOUND/SE/GAME/Bird000.wav", 0 },			// SE34	// 鳥
		{ "Data/SOUND/SE/GAME/Bird001.wav", 0 },			// SE35	// 鳥2
		{ "Data/SOUND/SE/GAME/Bird002.wav", 0 },			// SE36	// 鳥3
		{ "Data/SOUND/SE/GAME/Insect.wav", 0 },				// SE37	// 虫
		{ "Data/SOUND/SE/GAME/catch.wav", 0 },				// SE38	// 虫
		{ "Data/SOUND/SE/GAME/fall.wav", 0 },				// SE39	// 虫
	};

protected:
};

#endif