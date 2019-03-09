//=============================================================================
//
// 弾処理 [bullet.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"	// シーン3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_MAX_TEXTURE	(2)			// テクスチャの最大数
#define BULLET_SIZE_X		(6)			// 大きさ（X)
#define BULLET_SIZE_Y		(6)			// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBullet : public CScene3D
{// 弾（親：CScene3D）
public:
	typedef enum
	{// 弾の種類
		BULLETTYPE_PLAYER_1 = 0,		// プレイヤー1
		BULLETTYPE_PLAYER_2,			// プレイヤー2
		BULLETTYPE_ENEMY,				// 敵
		BULLETTYPE_MAX
	}BULLETTYPE;

	typedef enum
	{// 弾の種類
		BULLETSTATE_NORMAL = 0,			// 通常
		BULLETSTATE_BOMB,				// ボム
		BULLETSTATE_MAX
	}BULLETSTATE;

	CBullet();									// コンストラクタ
	~CBullet();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState);				// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[BULLET_MAX_TEXTURE];	// テクスチャ情報へのポインタ

	BULLETTYPE m_bulletType;					// 種類
	BULLETSTATE m_bulletState;					// 状態
	D3DXVECTOR3 m_pos;							// 移動量
	D3DXVECTOR3 m_move;							// 移動量
	int m_Life;									// 体力

protected:

};

#endif