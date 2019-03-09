//=============================================================================
//
// 爆弾（残数）処理 [bomb.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BOMB			(5)			// 残数
#define BOMB_SIZE_X			(25)		// 大きさ（X)
#define BOMB_SIZE_Y			(25)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBomb : public CScene
{// 爆弾（親：CScene）
public:
	CBomb();									// コンストラクタ
	~CBomb();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CBomb *Create(int nMaxBomb, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nBombNum);		// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void AddBomb(void);							// 爆弾加算
	void ResetBomb(void);						// ボムリセット
	void DeleteBomb(void);						// 爆弾破棄

	static bool SetShot(int nPlayerNum) { return m_bShotBomb[nPlayerNum]; }							// 撃つ
	static void SetAdd(int nPlayerNum, bool bAdd) { m_bAddBomb[nPlayerNum] = bAdd; }				// 加算
	static void SetReset(int nPlayerNum, bool bReset) { m_bResetBomb[nPlayerNum] = bReset; }		// リセット
	static void SetDelete(int nPlayerNum, bool bDelete) { m_bDeleteBomb[nPlayerNum] = bDelete; }	// 破棄

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャ情報へのポインタ
	CScene2D *m_apScene2D[MAX_BOMB];		// シーン2D
	
	int	m_nBombNum;								// 爆弾番号

	static bool m_bShotBomb[MAX_PLAYER];		// 爆弾撃つ
	static bool m_bAddBomb[MAX_PLAYER];			// 爆弾加算
	static bool m_bResetBomb[MAX_PLAYER];		// 爆弾リセット
	static bool m_bDeleteBomb[MAX_PLAYER];		// 爆弾破棄
	static int m_nMaxBomb[MAX_PLAYER];			// 爆弾最大数
	static int m_nBombAll[MAX_PLAYER];			// 爆弾現在数

protected:

};

#endif