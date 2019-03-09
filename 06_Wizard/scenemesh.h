//=============================================================================
//
// シーンメッシュ処理 [sceneMesh.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENEMESH_H_
#define _SCENEMESH_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMesh : public CScene
{// シーンメッシュ（親：CScene）
public:
	typedef enum
	{// メッシュタイプ
		MESHTYPE_FIELD = 0,			// フィールド
		MESHTYPE_WALL,				// ウォール
		MESHTYPE_CYLINDER,			// シリンダー
		MESHTYPE_DOME,				// ドーム
		MESHTYPE_MAX				// 最大数
	}MESHTYPE;

	typedef enum
	{// テクスチャタイプ
		MESHTEX_ALL = 0,			// 全面
		MESHTEX_NUMSIZE,			// 分割
		MESHTEX_MAX					// 最大数
	}MESHTEX;

	typedef enum
	{// サイン種類
		SINTYPE_CIRCLE = 0,			// 円
		SINTYPE_RIGHT,				// 右
		SINTYPE_LEFT,				// 左
		SINTYPE_UP,					// 上
		SINTYPE_DOWN,				// 下
		SINTYPE_MAX					// 最大数
	}SINTYPE;

	typedef enum
	{// カーブ中心点
		SINSTART_POS = 0,			// 位置
		SINSTART_CENTER,			// 円
		SINSTART_LU,				// 左上
		SINSTART_RU,				// 右上
		SINSTART_LD,				// 左下
		SINSTART_RD,				// 右下
		SINSTART_MAX				// 最大数
	}SINSTART;

	CSceneMesh(int nPriority = 3, CScene::OBJTYPE objType = (CScene::OBJTYPE)0);	// コンストラクタ
	~CSceneMesh();										// デストラクタ

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)		// テクスチャ反映
		{ m_pTexture = pTexture; }		

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	// 描画編集
	void DrawEdit(bool bAdd, bool bSubtract, bool bATest, bool bZWrite, bool bLight, int nATest = 0, int nCullMode = 3, DWORD dCmpValue = D3DCMP_LESSEQUAL)
	{
		m_bDrawAdd		= bAdd;				// 加算合成
		m_bDrawSubtract	= bSubtract;		// 減産合成
		m_bDrawATest	= bATest;			// αテスト
		m_bDrawZWrite	= bZWrite;			// Zライト
		m_bDrawLight	= bLight;			// ライト影響
		m_nATest		= nATest;			// αテスト数値
		m_nCullMode		= nCullMode;		// カリング
		m_dCmpValue = dCmpValue;			// Zライト設定
	}

														// 初期値設定
	void SetInitAll(MESHTYPE meshType, MESHTEX meshTex, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV);		// 初期値設定

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// 位置取得
	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// 回転設定
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// 回転取得
	D3DXVECTOR3 Getnum(void)		{ return m_num; }	// ブロック数取得
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// 大きさ取得

	void SetStart(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV);	// ステータス設定

	void SetTex(D3DXVECTOR2 TexMoveUV);					// アニメーション情報設定

	void SetVtx(int nNum, D3DXVECTOR3 pos);				// 頂点設定
	D3DXVECTOR3 GetVtx(int nNum);						// 頂点取得
	D3DXVECTOR3 GetNor(int nNum);						// 法線取得
	void SetCol(int nNum, D3DXCOLOR col);				// 色設定

	int GetMaxVtx(void)			{ return m_nMaxVtx; }	// 頂点数取得

	bool ColInMesh(D3DXVECTOR3 pos);					// メッシュ内にいるか

														// サインカーブ更新
	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine);
	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange);

														// 高さ設定
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);
	float GetHeight(D3DXVECTOR3 pos);					// 高さ取得
	float GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// 傾き取得
	float GetBlockNor(D3DXVECTOR3 pos);					// ブロック法線取得

	void SetVecNor(void);								// 法線計算
private:

	LPDIRECT3DTEXTURE9			m_pTexture;				// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// 頂点バッファ情報へのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// インデックス情報へのポインタ
	D3DXMATRIX					m_mtxWorld;				// ワールドマトリックス
	int							m_nMaxVtx;				// 頂点の最大数
	int							m_nMaxIdx;				// インデックスの最大数
	int							m_nMaxPolygon;			// ポリゴンの最大数

	MESHTYPE					m_meshType;				// メッシュタイプ
	MESHTEX						m_meshTex;				// テクスチャタイプ
	D3DXVECTOR3					m_num;					// 枚数
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_rot;					// 向き
	D3DXVECTOR3					m_size;					// 大きさ
	D3DXVECTOR2					m_TexUV;				// UV

	bool						m_bDrawAdd;				// 加算合成
	bool						m_bDrawSubtract;		// 減産合成
	bool						m_bDrawATest;			// αテスト
	bool						m_bDrawZWrite;			// Zライト
	bool						m_bDrawLight;			// ライト影響
	int							m_nATest;				// αテスト数値
	int							m_nCullMode;			// カリング番号
	DWORD						m_dCmpValue;			// Zライト設定

protected:

};

#endif