//=============================================================================
//
// 体力（残機）処理 [life.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFE			(3)			// 残機数
#define LIFE_SIZE_X			(25)		// 大きさ（X)
#define LIFE_SIZE_Y			(25)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;		// シーン2D

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLife : public CScene
{// 体力（親：CScene）
public:
	CLife();									// コンストラクタ
	~CLife();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLifeNum);		// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void AddLife(void);							// 体力加算
	void DeleteLife(void);						// 体力破棄

	static void SetAdd(int nPlayerNum, bool bAdd) { m_bAddLife[nPlayerNum] = bAdd; }				// 加算
	static void SetRespawn(int nPlayerNum, bool bRespawn) { m_bRespawn[nPlayerNum] = bRespawn; }	// リスポーン

	static void SetLiveState(int nPlayerNum) { if (m_nLifeAll[nPlayerNum] == 0) m_bLiveState[nPlayerNum] = false; else m_bLiveState[nPlayerNum] = true; }	// 生存人数

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ情報へのポインタ
	CScene2D *m_apScene2D[MAX_LIFE];			// シーン2D

	int m_nLifeNum;								// 体力番号

	static bool m_bAddLife[MAX_PLAYER];			// 加算
	static bool m_bRespawn[MAX_PLAYER];			// リスポーン可能か
	static int m_nLifeAll[MAX_PLAYER];			// 体力の最大値

	static bool m_bLiveState[MAX_PLAYER];		// 生存人数

protected:

};

#endif