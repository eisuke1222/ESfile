//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"	// シーン3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_SIZE_X	(10)			// 大きさ（X)
#define EFFECT_SIZE_Y	(10)			// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect : public CScene3D
{// エフェクト（親：CScene3D）
public:
	typedef enum
	{// エフェクトタイプ
		EFFECTTYPE_NONE = 0,		// 何もしていない
		EFFECTTYPE_PLAYER_T,		// プレイヤータイトル
		EFFECTTYPE_PLAYER,			// プレイヤー
		EFFECTTYPE_ENEMY,			// 敵
		EFFECTTYPE_BULLET,			// 弾
		EFFECTTYPE_EXPLOSION,		// 爆発
		EFFECTTYPE_SMOKE,			// 煙
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	typedef enum
	{// テクスチャ
		TEXTURE_EFFECT = 0,
		TEXTURE_WATER,
		TEXTURE_SMOKE,
		TEXTURE_TANK,
		TEXTURE_MAX
	}TEXTURE;

	CEffect();									// コンストラクタ
	CEffect(int);								// コンストラクタ
	~CEffect();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture);	// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture);				// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[TEXTURE_MAX];	// テクスチャ情報へのポインタ

	TEXTURE		m_TexNum;						// テクスチャ
	EFFECTTYPE	m_effectType;					// エフェクトタイプ
	int			m_nCntTimer;					// 時間カウンタ
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_moveInit;						// 移動量初期
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_sizeDown;						// 大きさ減少
	float		m_faDown;						// アルファ減少

protected:

};

#endif