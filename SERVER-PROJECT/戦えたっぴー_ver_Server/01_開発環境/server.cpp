//*****************************************************************************
//
//     サーバーの処理[server.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "server.h"
#include "serverlib.h"
#include "manager.h"
#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define HINAEVENT_DROP_ITEM (2)

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************


//*****************************************************************************
//    CServerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
//    WinSockの初期化処理
//=============================================================================
HRESULT CServer::ServerStartUp(void)
{
	WSADATA wsaData;  // winsockの情報
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
//    WinSockの終了処理
//=============================================================================
void CServer::ServerCleanUp(void)
{
	WSACleanup();
}

//=============================================================================
//    生成処理
//=============================================================================
CServer *CServer::Create(void)
{
	CServer *pServer = NULL;      // サーバークラス型のポインタ
	if (pServer == NULL)
	{// メモリが空になっている
		pServer = new CServer;
		if (pServer != NULL)
		{// インスタンスを生成できた
			if (FAILED(pServer->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pServer;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CServer::Init(void)
{
	// 変数をクリアする
	ClearVariable();

	// ソケットの作成
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	// ソケットに名称を付与
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	// クライアントからの接続要求を待てる状態にする
	listen(m_sockServer, 5);
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_hWaitConnectHander[nCntConnect] = 0;
		m_bConnected[nCntConnect] = false;
	}

	// スレッドを切り替えて接続待機処理をする
	m_hWaitConnectAllHander = (HANDLE)_beginthreadex(NULL, 0, WaitConnectAllHandler, this, 0, NULL);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CServer::Uninit(void)
{
	// マルチスレッド終了のためにクライアントを生成し破棄する
	CClient *pClient[MAX_SERVER_CONNECTING_NUM] = {};
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM - m_nNumConnect; nCntConnect++)
	{
		pClient[nCntConnect] = CClient::Create("127.0.1.1");
	}
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM - m_nNumConnect; nCntConnect++)
	{
		if (pClient[nCntConnect] != NULL)
		{
			pClient[nCntConnect]->Uninit();
			delete pClient[nCntConnect];
			pClient[nCntConnect] = NULL;
		}
	}

	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		// ソケット(クライアント)の開放
		closesocket(m_sockClient[nCntConnect]);

		// ハンドルの開放
		if (m_hWaitConnectHander[nCntConnect] != 0)
		{
			CloseHandle(m_hWaitConnectHander[nCntConnect]);
		}
	}

	// ハンドルの開放
	if (m_hWaitConnectAllHander != 0)
	{
		CloseHandle(m_hWaitConnectAllHander);
	}

	// ソケット(サーバー)の開放
	closesocket(m_sockServer);
}

//=============================================================================
//    更新処理
//=============================================================================
void CServer::Update(void)
{
	// メッセージ受信処理
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// クライアントと接続できた
			CDebugProc::Print("%d番と接続完了!!\n", nCntConnect);
			RecieveMassageFromClient(nCntConnect);
		}
		else
		{// クライアントと接続できていない
			CDebugProc::Print("サーバー受付中...\n");
		}
	}


	// メッセージ交換処理
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// クライアントと接続できた
			ExChangeMessage(nCntConnect);
		}
	}


	// メッセージ送信処理
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// クライアントと接続できた
			SendMessageToClient(nCntConnect);
		}
	}

	CDebugProc::Print("サーバー接続人数 : %d人", m_nNumConnect);
}

//=============================================================================
//    接続待機処理
//=============================================================================
void CServer::WaitConnect(void)
{
	// クライアントからの接続要求を受け付ける
	SOCKET sockClient;
	struct sockaddr_in addrClient;
	int nLenghtAddrClient = 0;
	nLenghtAddrClient = sizeof(addrClient);
	sockClient = accept(m_sockServer, (struct sockaddr *)&addrClient, &nLenghtAddrClient);
	if (sockClient == INVALID_SOCKET)
	{
		return;
	}

	// 開いている場所に情報を格納
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == false)
		{
			m_sockClient[nCntConnect] = sockClient;
			m_addrClient[nCntConnect] = addrClient;
			m_nLengthAddrClient[nCntConnect] = nLenghtAddrClient;
			memset(m_aSendMessage[nCntConnect], 0, sizeof(m_aSendMessage[nCntConnect]));
			m_bConnected[nCntConnect] = true;
			m_nNumConnect++;
			break;
		}
	}

	return;
}

//=============================================================================
//    接続待機処理
//=============================================================================
void CServer::WaitConnectAll(void)
{
	// クライアントからの接続要求を受け付ける
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_nLengthAddrClient[nCntConnect] = sizeof(m_addrClient);
		m_sockClient[nCntConnect] = accept(m_sockServer, (struct sockaddr *)&m_addrClient[nCntConnect], &m_nLengthAddrClient[nCntConnect]);
		if (m_sockClient[nCntConnect] == INVALID_SOCKET)
		{
			return;
		}
		memset(m_aSendMessage[nCntConnect], 0, sizeof(m_aSendMessage[nCntConnect]));
		m_bConnected[nCntConnect] = true;
		m_nNumConnect++;
	}

	return;
}

//=============================================================================
//    接続待機処理を呼ぶ関数(マルチスレッド対応のため)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnect();     // CServer型にキャストして呼ぶ
	return 0;
}

//=============================================================================
//    接続待機処理を呼ぶ関数(マルチスレッド対応のため)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectAllHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnectAll();  // CServer型にキャストして呼ぶ
	return 0;
}

//=============================================================================
//    変数をクリアする
//=============================================================================
void CServer::ClearVariable(void)
{
	m_nNumConnect = 0;
	m_hWaitConnectAllHander = 0;
	m_nNumEnemyBulletData = 0;

	for (int nCnt = 0; nCnt < MAX_SERVER_CONNECTING_NUM; nCnt++)
	{
		m_hWaitConnectHander[nCnt] = 0;
		m_nNumPlayerBulletData[nCnt] = 0;
		memset(m_aReceiveMessage[nCnt], 0, MAX_SERVER_DATA);
		memset(m_aSendMessage[nCnt], 0, MAX_SERVER_DATA);
	}

	for (int nCnt = 0; nCnt < MAX_SERVER_CONNECTING_NUM; nCnt++)
	{
		for (int nCntData = 0; nCntData < MAX_SERVER_BULLETDATA; nCntData++)
		{
			ZeroMemory(&m_PlayerBulletData[nCnt][nCntData], sizeof(m_PlayerBulletData[nCnt][nCntData]));
		}
	}
	for (int nCntData = 0; nCntData < MAX_SERVER_BULLETDATA; nCntData++)
	{
		ZeroMemory(&m_EnemyBulletData[nCntData], sizeof(m_EnemyBulletData[nCntData]));
	}
}

//=============================================================================
//    クライアントからメッセージを受け取る処理
//=============================================================================
void CServer::RecieveMassageFromClient(int nIdxClient)
{
	// クライアントからデータ(メッセージ)を受信
	memset(m_aReceiveMessage[nIdxClient], 0, MAX_SERVER_DATA);
	m_nLengthData[nIdxClient] = recv(m_sockClient[nIdxClient], m_aReceiveMessage[nIdxClient], MAX_SERVER_DATA, 0);
}

//=============================================================================
//    クライアントとのメッセージ交換処理
//=============================================================================
void CServer::ExChangeMessage(int nIdxClient)
{
	// メッセージを表示する
	CDebugProc::Print("%s\n受信バイト数[%d]\n", m_aReceiveMessage[nIdxClient], m_nLengthData[nIdxClient]);

	// メッセージからコマンドを適用
	if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_HELLO) == 0)
	{
		Command_Hello(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_DELETE) == 0)
	{
		Command_DeleteClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SEND_CLIENT) == 0)
	{
		Command_Send_Client(nIdxClient);
		strcat(m_aSendMessage[nIdxClient], "メッセージ送信");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SE_DATA) == 0)
	{
		Command_SendDataClient(nIdxClient);

		strcat(m_aSendMessage[nIdxClient], " ");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_USERID) == 0)
	{
		Command_UserIdClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_CHARA_DATA) == 0)
	{
		Command_CharaDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_GAME_DATA) == 0)
	{
		Command_GameDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_TUTO_DATA) == 0)
	{
		Command_TutorialDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_NONE) == 0
		&& CServerLib::Memcmp(m_aReceiveMessage[(nIdxClient + 1) % 2], CD_SEND_CLIENT) != 0)
	{
		strcat(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    クライアントへメッセージを送信する処理
//=============================================================================
void CServer::SendMessageToClient(int nIdxClient)
{
	// クライアントへメッセージを送信
	send(m_sockClient[nIdxClient], m_aSendMessage[nIdxClient], strlen(m_aSendMessage[nIdxClient]), 0);
	memset(m_aSendMessage[nIdxClient], 0, MAX_SERVER_DATA);
}

//=============================================================================
//    Helloメッセージをクライアントへ送信する処理
//=============================================================================
void CServer::Command_Hello(int nIdxClient)
{
	// クライアントへのメッセージを設定
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    クライアントからクライアントへメッセージ送信する処理
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
}

//=============================================================================
//    クライアントとの交信を終了する処理
//=============================================================================
void CServer::Command_DeleteClient(int nIdxClient)
{
	// ソケット(クライアント)の開放
	closesocket(m_sockClient[nIdxClient]);
	m_bConnected[nIdxClient] = false;
	m_nNumConnect--;
	m_hWaitConnectHander[nIdxClient] = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandler, this, 0, NULL);
	CDebugProc::ClearString();
	CDebugProc::Print("%d番の接続が途絶えました・・・\n", nIdxClient);
	CDebugProc::Print("接続受付開始\n");
}

//=============================================================================
//    クライアントからデータを読み取る処理
//=============================================================================
void CServer::Command_SendDataClient(int nIdxClient)
{
	float aPlayerPos[3];        // プレイヤーの座標
	char aStr[MAX_SERVER_DATA];
	char *pStr;
	pStr = &m_aReceiveMessage[nIdxClient][0];

	// 受信したメッセージからデータを読み取り
	pStr= CServerLib::ReadFloat3(pStr, CD_SE_DATA, &aPlayerPos[0]);

	// 受信した座標を別クライアントへ送信するためのメッセージを作成
	strcpy(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat3(&aPlayerPos[0], aStr));
}

//=============================================================================
//    クライアントからキャラデータを読み取る処理
//=============================================================================
void CServer::Command_CharaDataClient(int nIdxClient)
{
	int nWord = 0;
	int nState = 0;
	int nSelectPlayer = 0;
	int nSelectStageSide = 0;
	int nSelectStageVertical = 0;
	char aStr[MAX_SERVER_DATA];
	char *pStr;
	pStr = &m_aReceiveMessage[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][0];

	// 受信したメッセージからデータを読み取り
	nState = CServerLib::ReadInt(pStr, CD_CHARA_DATA);
	nWord = CServerLib::PopString(pStr, CD_CHARA_DATA);
	pStr += nWord;
	nSelectPlayer = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// データを別クライアントへ送信するためのメッセージを作成
	strcpy(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nState, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSelectPlayer, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	if (nIdxClient != 0)
	{
		nSelectStageSide = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nSelectStageVertical = CServerLib::ReadInt(pStr, "");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSelectStageSide, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSelectStageVertical, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	if (m_bConnected[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == false)
	{// まだもう一人が接続できていない
		strcpy(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    クライアントからゲームデータを読み取る処理
//=============================================================================
void CServer::Command_GameDataClient(int nIdxClient)
{
	char *pStr;
	pStr = &m_aReceiveMessage[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][0];

	// 受信したメッセージからデータを読み取り
	if (CServerLib::Memcmp(pStr, CD_GAME_DATA) == 0)
	{
		// 頭出し
		pStr = CServerLib::HeadPutout(pStr, CD_GAME_DATA);

		// プレイヤーのデータを設定する
		pStr = SetGameData_Player(nIdxClient, pStr);

		if (nIdxClient != 0)
		{
			// ゲームのデータを設定する
			pStr = SetGameData_Game(nIdxClient, pStr);

			// 敵のデータを設定する
			pStr = SetGameData_Enemy(nIdxClient, pStr);

			// 敵の弾データを設定する
			pStr = PushBulletData_Enemy(nIdxClient, pStr);
			SetGameData_EnemyBullet(nIdxClient, pStr);

			// 消すブロックデータを設定する
			pStr = SetGameData_Block(nIdxClient, pStr);

			// アイテムのデータを設定する
			pStr = SetGameData_Item(nIdxClient, pStr);

			// スポーンした敵の数を設定する
			pStr = SetGameData_SpawnEnemyCount(nIdxClient, pStr);

			// マップイベントのデータを設定する
			pStr = SetGameData_MapEvent(nIdxClient, pStr);
		}

		// プレイヤー同士の弾データを設定する
		pStr = PushBulletData_Player(nIdxClient, pStr);
		SetGameData_PlayerBullet(nIdxClient, pStr);

		if (nIdxClient == 0)
		{
			// 消すブロックデータを設定する
			pStr = SetGameData_Block(nIdxClient, pStr);

			// 消す敵データを設定する
			pStr = SetGameData_DeleteEnemy(nIdxClient, pStr);

			// 相手の状態を設定する
			pStr = SetGameData_ClientState(nIdxClient, pStr);

			// 消すアイテムデータを設定する
			pStr = SetGameData_DeleteItem(nIdxClient, pStr);
		}

		// 相手の倒した敵の数を設定する
		pStr = SetGameData_BreakEnemy(nIdxClient, pStr);

		// 弾が当たった情報を設定する
		pStr = SetGameData_HitBullet(nIdxClient, pStr);
	}
	else
	{
		strcpy(m_aSendMessage[nIdxClient], "???");
	}

	if (m_bConnected[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == false)
	{// まだもう一人が接続できていない
		strcpy(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    クライアントからチュートリアルデータを読み取る処理
//=============================================================================
void CServer::Command_TutorialDataClient(int nIdxClient)
{
	char *pStr;
	pStr = &m_aReceiveMessage[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][0];

	// 受信したメッセージからデータを読み取り
	if (CServerLib::Memcmp(pStr, CD_TUTO_DATA) == 0)
	{
		// 頭出し
		pStr = CServerLib::HeadPutout(pStr, CD_TUTO_DATA);

		// プレイヤーのデータを設定する
		pStr = SetGameData_Player(nIdxClient, pStr);

		if (nIdxClient != 0)
		{
			// ゲームのデータを設定する
			pStr = SetGameData_Game(nIdxClient, pStr);

			// 敵のデータを設定する
			pStr = SetGameData_Enemy(nIdxClient, pStr);

			// 敵の弾データを設定する
			pStr = PushBulletData_Enemy(nIdxClient, pStr);
			SetGameData_EnemyBullet(nIdxClient, pStr);

			// 消すブロックデータを設定する
			pStr = SetGameData_Block(nIdxClient, pStr);
		}

		// プレイヤー同士の弾データを設定する
		pStr = PushBulletData_Player(nIdxClient, pStr);
		SetGameData_PlayerBullet(nIdxClient, pStr);

		if (nIdxClient == 0)
		{
			// 消すブロックデータを設定する
			pStr = SetGameData_Block(nIdxClient, pStr);

			// 消す敵データを設定する
			pStr = SetGameData_DeleteEnemy(nIdxClient, pStr);
		}

		// 弾が当たった情報を設定する
		pStr = SetGameData_HitBullet(nIdxClient, pStr);
	}
	else
	{
		strcpy(m_aSendMessage[nIdxClient], "???");
	}

	if (m_bConnected[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == false)
	{// まだもう一人が接続できていない
		strcpy(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    クライアントへプレイヤーのデータを設定する処理
//=============================================================================
char *CServer::SetGameData_Player(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;
	float fPlayerPos[3] = { 0.0f ,0.0f, 0.0f };
	float fPlayerRot[3] = { 0.0f ,0.0f, 0.0f };
	int nPlayerState = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// プレイヤーの人数を読み取る
	nNumPlayer = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcpy(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumPlayer, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumPlayer != 0)
	{// 相手のプレイヤーが生きている
		pStr = CServerLib::ReadFloat3(pStr, "", &fPlayerPos[0]);
		pStr = CServerLib::ReadFloat3(pStr, "", &fPlayerRot[0]);
		nPlayerState = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データを別クライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fPlayerPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fPlayerRot[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nPlayerState, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}


	// ホストじゃなかったら
	if (nIdxClient != 0)
	{
		nNumPlayer = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumPlayer, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    クライアントへゲームのデータを設定する処理
//=============================================================================
char *CServer::SetGameData_Game(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nGameState = 0;
	int nMapIdx = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// データを読み取る
	nGameState = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	nMapIdx = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// データを別クライアントへ送信するためのメッセージを作成
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nGameState, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nMapIdx, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    クライアントへ敵のデータを設定する処理
//=============================================================================
char *CServer::SetGameData_Enemy(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumEnemy = 0;
	float fEnemyPos[3] = { 0.0f ,0.0f,0.0f };
	float fEnemyRot[3] = { 0.0f ,0.0f,0.0f };
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// 敵のデータ数を読み取る
	nNumEnemy = CServerLib::ReadInt(pStr, "");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumEnemy, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumEnemy == 0) return pStr;

	// 敵の数分データを設定
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		// データを読み取る
		pStr = CServerLib::ReadFloat3(pStr, "", &fEnemyPos[0]);
		pStr = CServerLib::ReadFloat3(pStr, "", &fEnemyRot[0]);
		nEnemyType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nEnemyIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fEnemyPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fEnemyRot[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nEnemyType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nEnemyIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}

	return pStr;
}

//=============================================================================
//    クライアントへアイテムのデータを設定する処理
//=============================================================================
char *CServer::SetGameData_Item(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumItem = 0;
	float fItemPos[3] = { 0.0f ,0.0f,0.0f };
	int nItemIdx = 0;
	int nItemType = 0;

	// アイテムのデータ数を読み取る
	nNumItem = CServerLib::ReadInt(pStr, "");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumItem, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumItem == 0) return pStr;

	// アイテムの数分データを設定
	for (int nCntEnemy = 0; nCntEnemy < nNumItem; nCntEnemy++)
	{
		// データを読み取る
		pStr = CServerLib::ReadFloat3(pStr, "", &fItemPos[0]);
		nItemIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nItemType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fItemPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nItemIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nItemType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}

	return pStr;
}

//=============================================================================
//    クライアントへスポーンした敵の情報を設定する処理
//=============================================================================
char *CServer::SetGameData_SpawnEnemyCount(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nSpawn = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// データを読み取る
	nSpawn = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// データを別クライアントへ送信するためのメッセージを作成
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSpawn, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    クライアントへマップイベント情報を設定する処理
//=============================================================================
char *CServer::SetGameData_MapEvent(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nMapEvent = 0;
	int nNextEvent = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// データを読み取る
	nMapEvent = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	nNextEvent = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// データを別クライアントへ送信するためのメッセージを作成
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nMapEvent, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNextEvent, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    クライアントへ弾が当たった情報を設定する処理
//=============================================================================
char *CServer::SetGameData_HitBullet(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nHitBullet = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// データを読み取る
	nHitBullet = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// データを別クライアントへ送信するためのメッセージを作成
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nHitBullet, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    プレイヤーの弾情報をプッシュする処理
//=============================================================================
char *CServer::PushBulletData_Player(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f, 0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の数を設定
	m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == 0) return pStr;

	// 弾の数だけデータをプッシュ
	for (int nCntData = 0; nCntData < m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM]; nCntData++)
	{
		// データ読み取り
		pStr = CServerLib::ReadFloat3(pStr, "", &fBulletPos[0]);
		nBulletIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nBulletType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データ設定
		m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos = D3DXVECTOR3(fBulletPos[0], fBulletPos[1], fBulletPos[2]);
		m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nIdx = nBulletIdx;
		m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nType = nBulletType;
	}

	return pStr;
}

//=============================================================================
//    敵の弾情報をプッシュする処理
//=============================================================================
char *CServer::PushBulletData_Enemy(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f,0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の数を設定
	m_nNumEnemyBulletData = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (m_nNumEnemyBulletData == 0) return pStr;

	// 弾の数だけデータをプッシュ
	for (int nCntData = 0; nCntData < m_nNumEnemyBulletData; nCntData++)
	{
		// データ読み取り
		pStr = CServerLib::ReadFloat3(pStr, "", &fBulletPos[0]);
		nBulletIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nBulletType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データ設定
		m_EnemyBulletData[nCntData].pos = D3DXVECTOR3(fBulletPos[0], fBulletPos[1], fBulletPos[2]);
		m_EnemyBulletData[nCntData].nIdx = nBulletIdx;
		m_EnemyBulletData[nCntData].nType = nBulletType;
	}

	return pStr;
}

//=============================================================================
//    クライアントへ敵の弾データを設定する処理
//=============================================================================
void CServer::SetGameData_EnemyBullet(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f, 0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 敵の弾数を設定
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(m_nNumEnemyBulletData, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (m_nNumEnemyBulletData == 0) return;

	// 敵の弾数分データを設定
	for (int nCntData = 0; nCntData < m_nNumEnemyBulletData; nCntData++)
	{
		// データを作成
		fBulletPos[0] = m_EnemyBulletData[nCntData].pos.x;
		fBulletPos[1] = m_EnemyBulletData[nCntData].pos.y;
		fBulletPos[2] = m_EnemyBulletData[nCntData].pos.z;
		nBulletIdx = m_EnemyBulletData[nCntData].nIdx;
		nBulletType = m_EnemyBulletData[nCntData].nType;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fBulletPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}
}

//=============================================================================
//    クライアントへプレイヤーの弾データを設定する処理
//=============================================================================
void CServer::SetGameData_PlayerBullet(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f, 0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// プレイヤーの弾数を設定
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM], aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == 0) return;

	// プレイヤーの弾数分データを設定
	for (int nCntData = 0; nCntData < m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM]; nCntData++)
	{
		// データを作成
		fBulletPos[0] = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos.x;
		fBulletPos[1] = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos.y;
		fBulletPos[2] = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos.z;
		nBulletIdx = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nIdx;
		nBulletType = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nType;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fBulletPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}
}

//=============================================================================
//    クライアントへ消すブロックのデータを設定する処理
//=============================================================================
char *CServer::SetGameData_Block(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int nDeleteIdx = 0;

	// 消すブロックの数を取得
	nNumDeleteBlock = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumDeleteBlock, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumDeleteBlock == 0)return pStr;

	// 消すブロックの数だけデータを設定
	for (int nCntData = 0; nCntData < nNumDeleteBlock; nCntData++)
	{
		// データ読み取り
		nDeleteIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nDeleteIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    クライアントへ消す敵のデータを設定する処理
//=============================================================================
char *CServer::SetGameData_DeleteEnemy(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nDeleteIdx = 0;

	// 消す敵の数を取得
	nNumDeleteEnemy = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumDeleteEnemy, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumDeleteEnemy == 0)return pStr;

	// 消す敵の数だけデータを設定
	for (int nCntData = 0; nCntData < nNumDeleteEnemy; nCntData++)
	{
		// データ読み取り
		nDeleteIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nDeleteIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    クライアントへ消すアイテムのデータを設定する処理
//=============================================================================
char *CServer::SetGameData_DeleteItem(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumDeleteItem = 0;
	int nDeleteIdx = 0;

	// 消すアイテムの数を取得
	nNumDeleteItem = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumDeleteItem, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumDeleteItem == 0)return pStr;

	// 消すアイテムの数だけデータを設定
	for (int nCntData = 0; nCntData < nNumDeleteItem; nCntData++)
	{
		// データ読み取り
		nDeleteIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// データをクライアントへ送信するためのメッセージを作成
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nDeleteIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    クライアントへ相手の状態を送信する処理
//=============================================================================
char *CServer::SetGameData_ClientState(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nGameState = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// データを読み取る
	nGameState = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// データを別クライアントへ送信するためのメッセージを作成
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nGameState, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    クライアントへ相手の倒した敵の数を送信する処理
//=============================================================================
char *CServer::SetGameData_BreakEnemy(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nNumBreak = 0;
	int nScore = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// 通常の敵
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// 装甲車タンク
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// 速射砲タンク
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// ヘビータンク
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// 相手のスコア
	nScore = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nScore, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    クライアントへ番号を送信する処理
//=============================================================================
void CServer::Command_UserIdClient(int nIdxClient)
{
	// クライアント番号を送信
	char aStr[MAX_SERVER_DATA] = "\0";
	strcpy(m_aSendMessage[nIdxClient], USER_ID);
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nIdxClient, aStr));
}


//*****************************************************************************
//    CClientの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CClient *CClient::Create(char *pSeverAddr)
{
	CClient *pClient = NULL;      // クライアントクラス型のポインタ
	if (pClient == NULL)
	{// メモリが空になっている
		pClient = new CClient;
		if (pClient != NULL)
		{// インスタンスを生成できた
			if (FAILED(pClient->Init(pSeverAddr)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pClient;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CClient::Init(char *pSeverAddr)
{
	// IPアドレスを設定
	if (pSeverAddr == NULL) return E_FAIL;
	SetServerAddr(pSeverAddr);

	// ソケットの作成
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);
	m_bConnected = false;

	// サーバーへ接続
	connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));
	m_bConnected = true;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CClient::Uninit(void)
{
	// サーバーとの接続を終了
	Command_DeleteClient();

	// ソケット(クライアント)の開放
	closesocket(m_sockClient);
}

//=============================================================================
//    更新処理
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected != true)
	{// サーバーと接続できていない
		WaitConnect();
	}
	else
	{// サーバーと接続できた
		ExChangeMessage();
	}
}

//=============================================================================
//    サーバーIPアドレスを設定する処理
//=============================================================================
void CClient::SetServerAddr(char *pServerAddr)
{
	strcpy(m_aServerAddr, pServerAddr);
}

//=============================================================================
//    サーバーとの接続待機処理
//=============================================================================
void CClient::WaitConnect(void)
{
	// ソケットの作成
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);

	if (connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer)) == SOCKET_ERROR)
	{
		CDebugProc::Print("接続が途絶えました・・・\n");
		CDebugProc::Print("接続開始\n");
		m_bConnected = false;
	}
	else
	{
		m_bConnected = true;
		m_nLengthData = 0;
	}
}

//=============================================================================
//    サーバーとのメッセージ交換処理
//=============================================================================
void CClient::ExChangeMessage(void)
{
	// サーバーへ送るメッセージを作成
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	if (pKeyboard->GetPress(DIK_RETURN) == true)
	{
		Command_Hello();
	}
	else if (pKeyboard->GetPress(DIK_SPACE) == true)
	{
		Command_Send_Client();
	}
	else if (pKeyboard->GetPress(DIK_1) == true)
	{
		Command_DeleteClient();
	}
	else
	{
		strcat(m_aSendMessage, CD_NONE);
	}

	// サーバーへデータ(メッセージ)を送信
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// サーバーからデータ(メッセージ)を受信
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// メッセージを表示する
		CDebugProc::Print("%s\n", m_aReceiveMessage);

		// メッセージを表示する
		CDebugProc::Print("受信バイト数[%d]\n", m_nLengthData);
	}
}

//=============================================================================
//    Helloメッセージを受け取る処理
//=============================================================================
void CClient::Command_Hello(void)
{
	strcat(m_aSendMessage, CD_HELLO);
}

//=============================================================================
//    別のクライアントへメッセージを送信する処理
//=============================================================================
void CClient::Command_Send_Client(void)
{
	strcat(m_aSendMessage, CD_SEND_CLIENT);
}

//=============================================================================
//    サーバーとの接続を終了する処理
//=============================================================================
void CClient::Command_DeleteClient(void)
{
	strcat(m_aSendMessage, CD_DELETE);
}