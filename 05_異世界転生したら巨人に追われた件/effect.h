//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"			// メイン
#include "scene3d.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EFFECTTYPE (15)
#define MAX_EFFECT (1000)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect : public CScene
{// シーン3D（親：CScene）
public:
	CEffect();																// コンストラクタ
	~CEffect();																// デストラクタ
	HRESULT Init(void);														// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CEffect *Create(D3DXVECTOR3 pos, int nTextureType,int nTxstType);// 生成
	static HRESULT Load(void);												// テクスチャの読み込み
	static void Unload(void);												// 破棄
	void LoadSetEffect(int nTxsttype);										// エフェクトの情報読み込み
	void LoadDataEffect(FILE *pFile,char read[128]);						// エフェクトの情報読み込み(データ読み込み)
	static void LoadNameEffect(void);										// エフェクトの情報読み込み(テキストの名前読み込み)
	static void LoadTexNameEffect(void);									// エフェクトの情報読み込み(エフェクトの画像の名前読み込み)
private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_EFFECTTYPE];					// テクスチャ情報へのポインタ
	D3DXMATRIX					m_mtxWorld;									// ワールドマトリックス
	CScene3D					*m_apScene3D[MAX_EFFECT];								// シーン3Dのポインタ型変数4
	static int					m_nEffectType;								// エフェクトの種類
	static D3DXVECTOR3			m_pos;										// 位置
	D3DXVECTOR2					m_TexUV;									// UV
	D3DXCOLOR					m_col;										// 色
	D3DXVECTOR3					m_rot;										// 向き
	D3DXVECTOR3					m_size;										// 大きさ
	int							m_nSetPoriMAX;								// ポリゴンの数
	int							m_nLife;									// 体力
	D3DXVECTOR3					m_posRange;									// 位置の範囲
	D3DXVECTOR3					m_move;										// 動き
	int							m_nBillType;								// 加算合成あるかないか種類
	float						m_fGravity;									// 重力
	D3DXVECTOR3					m_NumMove[MAX_EFFECT];						// 動きの割り当て
	int							m_nNumLife[MAX_EFFECT];						// 体力割り当ての設定
	static char					m_cTextName[MAX_EFFECTTYPE][128];			// テキストの名前
	static char					m_cTextureName[MAX_EFFECTTYPE][128];		// テキストの名前
	static int					m_nBindText;								// どのテキストをエフェクトに割り当てるか
	static int					m_nTexttureType;							// テクスチャの種類
	int							m_nAnimCounter;								// アニメーションカウンター
	int							m_nPatternCounter;							// パターンカウンター
protected:

};

#endif