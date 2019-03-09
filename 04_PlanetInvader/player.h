//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "model3d.h"	// モデル3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CModel3D
{// プレイヤー（親：CModel3D）
public:
	CPlayer();									// コンストラクタ
	~CPlayer();									// デストラクタ

	typedef enum
	{// 状態
		PLAYERSTATE_NONE = 0,		// 何もしていない
		PLAYERSTATE_NORMAL,			// 通常
		PLAYERSTATE_DAMAGE,			// ダメージ
		PLAYERSTATE_RESPAWN,		// 復活
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum);					// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	bool HitDamage(int nDamage)
	{// ヒットダメージ
		if (m_state == PLAYERSTATE_NORMAL)
		{// 通常状態
			m_nLife -= nDamage;					// 体力を減らす
			m_state = PLAYERSTATE_DAMAGE;		// ダメージ状態へ
		}
		if (m_nLife < 1) return true;	// 値を返す
		return false;		// 値を返す
	}

	int GetPlayerNum(void) { return m_nPlayerNum; }			// プレイヤー番号取得

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;	// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh;		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD				m_nNumMat;		// マテリアル情報の数

	int				m_nPlayerNum;				// プレイヤー番号
	bool			m_bmove;					// 移動可
	D3DXVECTOR3		m_pos;						// 位置
	D3DXVECTOR3		m_move;						// 移動量
	int				m_nCntTimer;				// 時間カウンタ
	float			m_frotDest;					// 向き差分
	int				m_nCntstate;				// 状態カウント
	PLAYERSTATE		m_state;					// 状態
	int				m_nLife;					// 体力

	int				m_nCntShot;					// 発射カウンタ

protected:

};

#endif