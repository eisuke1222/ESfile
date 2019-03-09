//*****************************************************************************
//
//				タイムの処理[time.cpp]
//				Auther : Shun Yokomichi
//
//*****************************************************************************

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "time.h"		// タイム
#include "number.h"		// 数字

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_WIDHT			(14.5f)		// 幅の大きさ
#define	NUMBER_HIGHT			(14.5f)		// 高さの大きさ
#define POS_X					(1257.0f)	// 位置座標(X軸)
#define POS_Y					(69.0f)		// 位置座標(Y軸)
#define POS_X_INTERVAL			(35.0f)		// 距離間隔

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int			CTime::m_nSecond = 0;
int			CTime::m_nMinute = 0;
int			CTime::m_nTime = 0;

bool		CTime::m_bTimer = false;

CNumber2D	*CTime::m_apNumber[MAX_NUMBER] = {};

//*****************************************************************************
// タイムの生成
//*****************************************************************************
CTime *CTime::Create()
{
	CTime *pTime = NULL;		// ポインタを生成

	if (pTime == NULL)
	{
		pTime = new CTime;
		if (pTime != NULL)
		{
			pTime->Init();
		}
	}

	return pTime;
}

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTime::CTime() : CScene(PRIORITY_UI, OBJTYPE_NUMBER)
{
	// デフォルト値
	for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_apNumber[nCntNum] = NULL;
	}
	m_nSecond = 0;
	m_nCntTime = 0;
	m_nMinute = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTime::~CTime() {}

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT CTime::Init(void)
{
	CNumber::Load();		//　数字の読み込み

	for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{	// 桁数文生成
		m_apNumber[nCntNum] = new CNumber2D;
		if (nCntNum < MAX_NUMBER - 1)
		{	// 秒単位の位置
			m_apNumber[nCntNum]->Init(D3DXVECTOR3(POS_X - (nCntNum * POS_X_INTERVAL), POS_Y, 0.0f), D3DXVECTOR3(NUMBER_WIDHT, NUMBER_HIGHT, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{	//　分単位の位置
			m_apNumber[nCntNum]->Init(D3DXVECTOR3(POS_X - (nCntNum * POS_X_INTERVAL + 20.0f), POS_Y, 0.0f), D3DXVECTOR3(NUMBER_WIDHT, NUMBER_HIGHT, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	// 変数の初期化
	m_nSecond = 0;
	m_nMinute = 0;
	m_nCntTime = 0;

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CTime::Uninit(void)
{
	CNumber::Unload();		// Textureの破棄

	for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{	// 桁数分の開放
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Uninit();
			m_apNumber[nCntNum] = NULL;
		}
	}

	CScene::SetDeath();		// 死亡フラグ
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CTime::Update(void)
{
	if (m_bTimer == true)
	{
		m_nCntTime++;		// カウンターの増加

		if ((m_nCntTime % 60) == 0)
		{	// 60フレーム 1秒
			AddTime(1);
		}
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CTime::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{	// 桁数分の描画
		m_apNumber[nCntNum]->Draw();
	}
}

//*****************************************************************************
//		タイムの設定
//*****************************************************************************
void CTime::SetTime(int nTime)
{
	m_nSecond = nTime;

	if ((m_nSecond % 60) == 0)
	{	//60秒たったら
		m_nMinute++;
		m_nSecond = 0;
	}

	int nTexData000 = m_nSecond;
	int nTexData001 = m_nMinute;

	for (int Number = 0; Number < MAX_NUMBER; Number++)
	{
		if (Number < MAX_NUMBER - 1)
		{	// 秒単位
			m_apNumber[Number]->SetTex((nTexData000 % 1), 1, D3DXVECTOR2((nTexData000 * 0.1f), 1.0f));
		}
		else
		{	// 分単位
			m_apNumber[Number]->SetTex((nTexData001 % 1), 1, D3DXVECTOR2((nTexData001 * 0.1f), 1.0f));
		}
		nTexData000 /= 10;
	}
}

//*****************************************************************************
//		タイムの加算
//*****************************************************************************
void CTime::AddTime(int nValue)
{
	m_nSecond += nValue;		// 加算
	m_nTime += nValue;			// 加算
	SetTime(m_nSecond);
}
