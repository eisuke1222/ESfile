//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_MAX_NUM		(5)			// ランキング数
#define RANKING_MAX_DIGIT	(6)			// ランキング桁数
#define RANKING_SIZE_X		(10)		// 大きさ（X)
#define RANKING_SIZE_Y		(10)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber3D;		// 数字3D
class CScene3D;			// シーン3D

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRanking : public CScene
{// ランキング（親：CScene）
public:
	CRanking();									// コンストラクタ
	~CRanking();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	static void SaveRanking(void);				// ランキングセーブ
	static void LoadRanking(void);				// ランキングロード

	static CRanking *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval, int nNum);		// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ情報へのポインタ

	static CNumber3D *m_apNumber3D[RANKING_MAX_NUM][RANKING_MAX_DIGIT];		// 数字3D
	static CScene3D *m_apScene3D[RANKING_MAX_NUM];							// シーン3D
	static int m_nRanking[RANKING_MAX_NUM];		// ランキング

	D3DXVECTOR3 m_pos[RANKING_MAX_NUM];			// 位置
	float m_fScene3DDiff;						// 順位とスコアの差

	static int m_RankInNum;

protected:

};

#endif