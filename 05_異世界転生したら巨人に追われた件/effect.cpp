//=============================================================================
//
// エフェクト(パーティクル)処理 [effect.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "font.h"			// フォント
#include "effect.h"			// エフェクト
#include "scene.h"			// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_nCounteraaa = 0;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9   CEffect::m_pTexture[MAX_EFFECTTYPE] = {};				// テクスチャ情報へのポインタ
D3DXVECTOR3			 CEffect::m_pos = {};									// 位置
int					 CEffect::m_nEffectType = {};							// 種類
char				 CEffect::m_cTextName[MAX_EFFECTTYPE][128] = {};		// テキスト名
char				 CEffect::m_cTextureName[MAX_EFFECTTYPE][128] = {};		// テクスチャ名
int					 CEffect::m_nBindText = 0;								// バインド番号
int					 CEffect::m_nTexttureType = 0;							// テクスチャ種類

//=============================================================================
// コンストラクタ								(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect() : CScene(CScene::PRIORITY_5, CScene::OBJTYPE_EFFECT)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{// カウント
		m_apScene3D[nCount] = NULL;								// シーン３D
		m_NumMove[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		m_nNumLife[nCount] = 0;									// 寿命
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 色
	m_TexUV = D3DXVECTOR2(0.0f, 0.0f);				// UV
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 大きさ
	m_posRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置範囲
	m_nSetPoriMAX = 0;								// ポリゴン数
	m_nEffectType = 0;								// 種類
	m_nLife = 0;									// 寿命
	m_fGravity = 0;									// 重力
	m_nBillType = 0;								// ビルボード種類
	m_nBindText = 0;								// バインドテキスト
	m_nTexttureType = 0;							// テクスチャ種類
	m_nAnimCounter = 0;								// アニメーションカウンター
	m_nPatternCounter = 0;							// パターンカウンター
}

//=============================================================================
// デストラクタ									(public)	*** CEffect ***
//=============================================================================
CEffect::~CEffect()
{
}
//=============================================================================
//	エフェクトの生成
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, int textype, int txsttype)
{
	CEffect *pEffect = NULL;
	if (pEffect == NULL)
	{//	アイテムの生成
		pEffect = new CEffect;						// メモリ確保
		if (pEffect != NULL)
		{//アイテムの初期化
			pEffect->m_pos = pos;					//	位置の設定
			pEffect->m_nEffectType = textype;		//	エフェクトの画像の種類
			pEffect->m_nBindText = txsttype;		//	エフェクトの演出割り当て
			pEffect->Init();						// 初期化処理
		}
	}
	return pEffect;
}

//=============================================================================
// 初期化処理									(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Init(void)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{// カウント
		m_apScene3D[nCount] = NULL;								// シーン３D
		m_NumMove[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		m_nNumLife[nCount] = 0;									// 寿命
	}

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 角度
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					// 色
	m_TexUV = D3DXVECTOR2(0.0f, 0.0f);							// UV
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 大きさ
	m_nSetPoriMAX = 0;											// ポリゴン数
	m_nLife = 0;												// 寿命
	m_fGravity = 0;												// 重力
	m_nAnimCounter = 0;											// アニメーション
	m_nPatternCounter = 0;										// パターン
	m_posRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置範囲

	//	テキストのデータの読み込み
	LoadSetEffect(m_nBindText);

	for (int nCount = 0; nCount < m_nSetPoriMAX; nCount++)
	{//	最大数分回す
		m_apScene3D[nCount] = NULL;
		if (m_apScene3D[nCount] == NULL)
		{
			m_apScene3D[nCount] = new CScene3D(CScene::PRIORITY_5, CScene::OBJTYPE_EFFECT);
			if (m_apScene3D[nCount] != NULL)
			{//	3Dポリゴンのメモリが確保出来たら
				if (m_nBindText != 2 && m_nBindText != 6 && m_nBindText != 7 && m_nBindText != 9 && m_nBindText != 10)
				{//	スモーク以外
				 //	動きの初期設定
					m_NumMove[nCount] = D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.x)),
						(cosf((rand() % 628 / 100.0f)) * ((rand() % (int)1) + m_move.y)),
						(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.z)));
				}
				else
				{//	スモーク
				 //	動きの初期設定
					m_NumMove[nCount] = D3DXVECTOR3(
						(sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.x)),
						(cosf((rand() % 314 / 2) / 100.0f) * ((rand() % (int)1) + m_move.y)),
						(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.z)));
				}
				if (m_nBindText == 11)
				{
					m_rot += D3DXVECTOR3(0.0f, 0.0f, cosf((float)(rand() % 628 / 100) * ((rand() % (int)10))));
				}
				//	位置の初期設定
				m_pos += D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.x)),
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.y)),
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.z)));
				//	寿命の設定
				m_nNumLife[nCount] = m_nLife;
				//	テクスチャの割り当て
				m_apScene3D[nCount]->BindTexture(m_pTexture[m_nEffectType]);
				if (m_nBillType == 0)
				{//	加算合成なし
					m_apScene3D[nCount]->SetInitAll(m_pos, m_rot, m_size, m_col, m_TexUV, CScene3D::SCENE3DTYPE_BILLEFFECTNONE);
				}
				else if (m_nBillType == 1)
				{//	加算合成あり
					m_apScene3D[nCount]->SetInitAll(m_pos, m_rot, m_size, m_col, m_TexUV, CScene3D::SCENE3DTYPE_BILLEFFECT);
				}
				m_apScene3D[nCount]->Init();
			}
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CEffect ***
//=============================================================================
void CEffect::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{//	最大数分回す
		if (m_apScene3D[nCount] != NULL)
		{//	使用されていなかったら
			m_apScene3D[nCount]->Uninit();
			m_apScene3D[nCount] = NULL;
		}
	}
	// シーン開放
	CScene::SetDeath();
}

//=============================================================================
// 更新処理										(public)	*** CEffect ***
//=============================================================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos[MAX_EFFECT];							//	位置の取得
	D3DXVECTOR3 rot[MAX_EFFECT];							//	向きの設定
	D3DXVECTOR3 size[MAX_EFFECT];							//	大きさの取得
	D3DXCOLOR   col[MAX_EFFECT];							//	色の設定
												//	動的確保
	for (int nCount = 0; nCount < m_nSetPoriMAX; nCount++)
	{//	ポリゴンの数文回す
		if (m_apScene3D[nCount] != NULL)
		{//	空じゃないか調べる
			pos[nCount] = m_apScene3D[nCount]->Getpos();			//	位置の取得
			rot[nCount] = m_apScene3D[nCount]->Getrot();			//	向きの設定
			size[nCount] = m_apScene3D[nCount]->Getsize();			//	大きさの取得
			col[nCount] = m_apScene3D[nCount]->Getcol();			//	色の設定

			if (m_nBindText == 1)
			{//	岩だったら
			 //m_NumMove[nCount].y += 0.25;
			}
			else
			{
				size[nCount].x += 0.25f;
				size[nCount].y += 0.25f;
			}

			//	位置に動きを代入する
			pos[nCount] += m_NumMove[nCount];
			//	重力の設定
			m_NumMove[nCount].y -= m_fGravity;

			m_apScene3D[nCount]->Setpos(pos[nCount]);		//	位置の設定
			m_apScene3D[nCount]->Setsize(size[nCount]);		//	大きさの設定
			m_apScene3D[nCount]->Setcol(col[nCount]);		//	色の設定
			m_apScene3D[nCount]->Setrot(rot[nCount]);		//	向きの設定
															//	体力の減少
			m_nNumLife[nCount]--;
			if (m_nNumLife[nCount] < 0)
			{
				Uninit();
			}
		}
	}
}

//=============================================================================
// 描画処理										(public)	*** CEffect ***
//=============================================================================
void CEffect::Draw(void)
{

}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CEffect::Load(void)
{
	LoadTexNameEffect();
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	for (int nCount = 0; nCount < m_nTexttureType; nCount++)
	{//	テクスチャの数文回す
		D3DXCreateTextureFromFile(pDevice, &m_cTextureName[nCount][0], &m_pTexture[nCount]);
	}
	return S_OK;
}
//=============================================================================
// アンロード処理								(public)	*** CEffect ***
//=============================================================================
void CEffect::Unload(void)
{
	//	使っていないメモリの開放
	for (int nCount = 0; nCount < m_nTexttureType; nCount++)
	{//	テクスチャの数文回す
	 // テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

}
//=============================================================================
// エフェクトの情報読み込み(テキストの名前読み込み)
//=============================================================================
void CEffect::LoadNameEffect(void)
{
	FILE *pFile = NULL;
	int nNumTxst = 0;	//	読み込むテキストの数
	int nCount = 0;
	char *pStrCur;		//	文字列の先頭へのポインタ
	char aLine[256];	//	文字列の読み込み用[1行分]
	char aStr[256];		//	文字列の抜き出し用
	char *pCheck;
	char *pStrLen;

	pFile = fopen("data/TEXT/EFFECT/EFFECT_SET_TEXT.txt", "r");

	if (pFile != NULL)
	{//	ファイルの読み込みが設定出来たら
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1行分分読み込む
			pStrCur = aLine;			//	先頭アドレスを読み込む
			strcpy(aStr, pStrCur);		//	先頭アドレスを抜き出す
			pStrLen = "NUM_TEXT = ";
			if (memcmp(pStrCur, "NUM_TEXT = ", strlen(pStrLen)) == 0)
			{//	モデルの番号
				pStrCur += strlen(pStrLen);			//	文字列のところまでアドレスをずらす
				strcpy(aStr, pStrCur);				//	ずらしたところまでの文字列を代入する
				nNumTxst = atoi(aStr);			//	文字列をint型に変換する
				break;
			}
		}
		nCount = 0;
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1行分分読み込む
		 //	改行文字があるかないか
			pCheck = strchr(aLine, '\n');
			if (pCheck != NULL)
			{//	改行文字があった場合
				*pCheck = '\0';
			}//	改行文字を終端文字に入れ替える
			pStrCur = aLine;			//	先頭アドレスを読み込む
			strcpy(aStr, pStrCur);		//	先頭アドレスを抜き出す
			pStrLen = "TEXT_NAME = ";
			if (memcmp(pStrCur, "TEXT_NAME = ", strlen(pStrLen)) == 0)
			{//	モデルの名前
				pStrCur += strlen(pStrLen);
				strcpy(&m_cTextName[nCount][0], pStrCur);
				if (nCount > nNumTxst)
				{
					break;
				}
			}
			nCount += 1;
		}
		fclose(pFile);
	}
}
//=============================================================================
//	エフェクトの情報読み込み(データ読み込み)
//=============================================================================
void  CEffect::LoadSetEffect(int nTxsttype)
{
	FILE *pFile;
	char read[128];					//	テキストの文字列読み込み
									//	テキストの読み込み
	pFile = fopen(&m_cTextName[nTxsttype][0], "r");

	if (pFile != NULL)
	{//	ファイルの読み込みが設定出来たら
	 //	テキストのデータ読み込み
		LoadDataEffect(pFile, &read[0]);
		fclose(pFile);
	}

}
//=============================================================================
// エフェクトの情報読み込み(データ読み込み)
//=============================================================================
void CEffect::LoadDataEffect(FILE *pFile, char read[128])
{
	fscanf(pFile, "%s", &read[0]);
	if (strcmp(&read[0], "EFFECTSET_STATE") == 0)
	{//	エフェクトの情報の読み込み
		do
		{
			fscanf(pFile, "%s", &read[0]);
			if (strcmp(&read[0], "POSRANGE") == 0)
			{//	回転の設定
				fscanf(pFile, "%s%f%f%f", &read[0], &m_posRange.x, &m_posRange.y, &m_posRange.z);
			}
			else if (strcmp(&read[0], "ROT") == 0)
			{//	回転の設定
				fscanf(pFile, "%s%f%f%f", &read[0], &m_rot.x, &m_rot.y, &m_rot.z);
			}
			else if (strcmp(&read[0], "MOVE") == 0)
			{//	大きさの設定
				fscanf(pFile, "%s%f%f%f", &read[0], &m_move.x, &m_move.y, &m_move.z);
			}
			else if (strcmp(&read[0], "SIZE") == 0)
			{//	大きさの設定
				fscanf(pFile, "%s%f%f%f", &read[0], &m_size.x, &m_size.y, &m_size.z);
			}
			else if (strcmp(&read[0], "COL") == 0)
			{//	色の設定
				fscanf(pFile, "%s%f%f%f%f", &read[0], &m_col.r, &m_col.g, &m_col.b, &m_col.a);
			}
			else if (strcmp(&read[0], "TEXUV") == 0)
			{//	UVの設定
				fscanf(pFile, "%s%f%f", &read[0], &m_TexUV.x, &m_TexUV.y);
			}
			else if (strcmp(&read[0], "PRIGONMAX") == 0)
			{//	ポリゴンの数の設定
				fscanf(pFile, "%s%d", &read[0], &m_nSetPoriMAX);
			}
			else if (strcmp(&read[0], "GRAVITY") == 0)
			{//	エフェクトが消える時間の設定
				fscanf(pFile, "%s%f", &read[0], &m_fGravity);
			}
			else if (strcmp(&read[0], "LIFE") == 0)
			{//	エフェクトが消える時間の設定
				fscanf(pFile, "%s%d", &read[0], &m_nLife);
			}
			else if (strcmp(&read[0], "BILLTYPE") == 0)
			{//	エフェクト加算合成の設定
				fscanf(pFile, "%s%d", &read[0], &m_nBillType);
			}
		} while (strcmp(&read[0], "EFFECT_END") != 0);
	}
}
//=============================================================================
// エフェクトの情報読み込み(エフェクトの画像の名前読み込み)
//=============================================================================
void CEffect::LoadTexNameEffect(void)
{
	FILE *pFile = NULL;
	int nNumTex = 0;	//	読み込むテクスチャの数
	int nCount = 0;
	char *pStrCur;		//	文字列の先頭へのポインタ
	char aLine[256];	//	文字列の読み込み用[1行分]
	char aStr[256];		//	文字列の抜き出し用
	char *pCheck;		//	文字列の中のゴミcheck
	char *pStrLen;		//	文字列の長さ

	pFile = fopen("data/TEXT/EFFECT/EFFECT_SET_TEXTURE.txt", "r");

	if (pFile != NULL)
	{//	ファイルの読み込みが設定出来たら
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1行分分読み込む
			pStrCur = aLine;			//	先頭アドレスを読み込む
			strcpy(aStr, pStrCur);		//	先頭アドレスを抜き出す
			pStrLen = "MAX_TEXTURE = ";
			if (memcmp(pStrCur, "MAX_TEXTURE = ", strlen(pStrLen)) == 0)
			{//	モデルの番号
				pStrCur += strlen(pStrLen);			//	文字列のところまでアドレスをずらす
				strcpy(aStr, pStrCur);				//	ずらしたところまでの文字列を代入する
				nNumTex = atoi(aStr);			//	文字列をint型に変換する
				break;
			}
		}
		m_nTexttureType = nNumTex;
		nCount = 0;
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1行分分読み込む
		 //	改行文字があるかないか
			pCheck = strchr(aLine, '\n');
			if (pCheck != NULL)
			{//	改行文字があった場合
				*pCheck = '\0';
			}//	改行文字を終端文字に入れ替える
			pStrCur = aLine;			//	先頭アドレスを読み込む
			strcpy(aStr, pStrCur);		//	先頭アドレスを抜き出す
			pStrLen = "TEXTURE_NAME = ";
			if (memcmp(pStrCur, "TEXTURE_NAME = ", strlen(pStrLen)) == 0)
			{//	モデルの名前
				pStrCur += strlen(pStrLen);
				strcpy(&m_cTextureName[nCount][0], pStrCur);
				if (nCount > nNumTex)
				{
					break;
				}
			}
			nCount += 1;
		}
		fclose(pFile);
	}

}