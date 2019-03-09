//=============================================================================
//
// ゲーム処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJ_MAX (1800)
#define TEXT_OBJ_MAX (3)		//	テキストの中のオブジェクトの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CEnemy;
class CMeshField;
class CMeshRiver;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{// ゲーム
public:
	typedef struct
	{
		int				nCnter;					//	オブジェクトの数を数える
		D3DXVECTOR3		pos[OBJ_MAX];			//	オブジェクトポジションの設定
		int				nType[OBJ_MAX];			//	オブジェクト種類の設定
		int				ncolType[OBJ_MAX];		//	オブジェクト判定の設定
		D3DXVECTOR3		size[OBJ_MAX];			//	オブジェクトサイズの設定
		bool			bUse[OBJ_MAX];			// 使ったか
	}TEXTSTATE;

	typedef enum
	{	// ゲームの状態(CLEAR & GameOver)
		MODE_NONE = 0,
		MODE_GAMEOVER000,		// 巨人に捕まる
		MODE_GAMEOVER001,		// 体力ゼロ
		MODE_GAMECLEAR,			// 生還
		MODE_MAX
	}MODEGAME;

	static const int m_MeshFieldMax = 8;		// フィールド数
	static const int m_MeshRiverMax = 4;		// リバー数

	CGame();						// コンストラクタ
	~CGame();						// デストラクタ

	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	void ObjectLoad(void);			// オブジェクトの読み込み
	void SetStage(void);
	void PlayES(int nSoundNum);		// サウンドの番号で環境音を出す

	// プレイヤー（設定、取得）
	static void SetPlayer(void) { m_pPlayer = NULL; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }

	// 敵（設定、取得）
	static void SetEnemy(void) { m_pEnemy = NULL; }
	static CEnemy *GetEnemy(void) { return m_pEnemy; }

	// メッシュフィールド（設定、取得）
	static void SetMeshField(CMeshField *pMeshField)
	{
		for (int nCount = 0; nCount < m_MeshFieldMax; nCount++)
		{
			if (m_pMeshField[nCount] == pMeshField)
			{
				m_pMeshField[nCount] = NULL;
				break;
			}
		}
	}
	static CMeshField *GetMeshField(int nNum) { return m_pMeshField[nNum]; }

	// メッシュリバー（取得）
	static CMeshRiver *GetMeshRiver(int nNum) { return m_pMeshRiver[nNum]; }

	// ゲームの状態取得と設定
	static void SetGame(MODEGAME pGame) { m_game = pGame; };
	static MODEGAME GetGame() { return m_game; };

	static void SetPause(bool bPause);
private:
	static CPlayer *m_pPlayer;							// プレイヤー
	static CEnemy *m_pEnemy;							// 敵
	static CMeshField *m_pMeshField[m_MeshFieldMax];	// メッシュフィールド
	static CMeshRiver *m_pMeshRiver[m_MeshRiverMax];	// メッシュリバー

	TEXTSTATE m_textState[TEXT_OBJ_MAX];				//	テキスト読み込む変数[読み込むオブジェクトの種類]
	static MODEGAME m_game;								// ゲーム
	static CPause *m_pPause;
	static bool m_bPause;

	int m_nCounter;										// カウント

protected:

};

#endif