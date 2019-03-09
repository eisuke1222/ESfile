//=============================================================================
//
// ブロック処理 [block.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scenex.h"		// シーンx

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBlock : public CSceneX
{// ブロック親：CSceneX）
public:
	typedef enum
	{// モデル種類
		MODELTYPE_BOX_R = 0,	// 岩(ブロック)
		MODELTYPE_BOX_W_1,		// 木
		MODELTYPE_BOX_W_2,		// 木(背景用)
		MODELTYPE_SKY,			// 空中床(正式採用視野)
		MODELTYPE_ICE,			// 滑る床
		MODELTYPE_LOG,			// 丸太
		MODELTYPE_SEESAW,		// シーソー
		MODELTYPE_IVY,			// ツタ
		MODELTYPE_IVY_2,		// ツタ(背景用)
		MODELTYPE_BOX_W_3,		// 木ブロック4つ分
		MODELTYPE_TREE,			// 背景の木
		MODELTYPE_TREE2,		// 背景の木2
		MODELTYPE_TREE3,		// 背景の木3
		MODELTYPE_TREE4,		// 背景の木4
		MODELTYPE_GRASS,		// 草
		MODELTYPE_GOAL,			// ゴール
		MODELTYPE_PBROCK,		// ゴール
		MODELTYPE_MAX
	}MODELTYPE;

	typedef enum
	{// 衝突種類
		COLTYPE_NONE = 0,		// 判定なし
		COLTYPE_BOX,			// 矩形
		COLTYPE_PUSH,			// 押す
		COLTYPE_BREAK,			// 破壊
		COLTYPE_SKY,			// 空中
		COLTYPE_ICE,			// 氷
		COLTYPE_FALL,			// 倒れる
		COLTYPE_SEESAW,			// シーソー
		COLTYPE_CLIMB,			// 登れる
		COLTYPE_WALLJUMP,		// 壁キック
		COLTYPE_MAX
	}COLTYPE;

	typedef enum
	{// 衝突場所
		HIT_UP = 0,				// 上
		HIT_DOWN,				// 下
		HIT_LEFT,				// 左
		HIT_RIGHT,				// 右
		HIT_MAX
	}HIT;

	CBlock();										// コンストラクタ
	~CBlock();										// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CBlock *Create(D3DXVECTOR3 pos, MODELTYPE modelType, COLTYPE coltype);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, MODELTYPE modelType, COLTYPE coltype);				// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	void Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float *fDownSpeed, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);
	bool OtherBlockCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);

	COLTYPE GetType(void) { return m_colType; }		// 衝突種類取得
	
	bool *GetHit(void) { return m_bHit; }			// 衝突場所取得

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MODELTYPE_MAX];		// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh[MODELTYPE_MAX];			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MODELTYPE_MAX];		// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MODELTYPE_MAX];		// マテリアル情報の数

	MODELTYPE					m_modelType;		// モデル種類
	COLTYPE						m_colType;			// 衝突種類
	D3DXVECTOR3					m_posOld;			// 移動量
	D3DXVECTOR3					m_move;				// 移動量
	bool						m_bHit[HIT_MAX];	// 衝突場所

	float						m_shadowHeight;		// 影高さ

protected:

};

#endif