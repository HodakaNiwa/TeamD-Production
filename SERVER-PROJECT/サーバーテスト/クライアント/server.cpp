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

#include "game.h"
#include "player.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


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
		m_bConnected[nCntConnect] = false;
		WaitConnect(nCntConnect);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CServer::Uninit(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		// ソケット(クライアント)の開放
		closesocket(m_sockClient[nCntConnect]);
	}

	// ソケット(サーバー)の開放
	closesocket(m_sockServer);
}

//=============================================================================
//    更新処理
//=============================================================================
void CServer::Update(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] != true)
		{// クライアントと接続できていない
			WaitConnect(nCntConnect);
		}
		else
		{// クライアントと接続できた
#ifdef SERVER_MODE
			CDebugProc::Print("%d番と接続完了!!\n", nCntConnect);
#endif
			ExChangeMessage(nCntConnect);
		}
	}
}

//=============================================================================
//    接続待機処理
//=============================================================================
void CServer::WaitConnect(int nIdxClient)
{
	// クライアントからの接続要求を受け付ける
	m_nLengthAddrClient[nIdxClient] = sizeof(m_addrClient);
	m_sockClient[nIdxClient] = accept(m_sockServer, (struct sockaddr *)&m_addrClient[nIdxClient], &m_nLengthAddrClient[nIdxClient]);
	if (m_sockClient[nIdxClient] == INVALID_SOCKET)
	{
		return;
	}
	memset(m_aSendMessage[nIdxClient], 0, sizeof(m_aSendMessage[nIdxClient]));
	m_bConnected[nIdxClient] = true;
}

//=============================================================================
//    クライアントとのメッセージ交換処理
//=============================================================================
void CServer::ExChangeMessage(int nIdxClient)
{
	// クライアントからデータ(メッセージ)を受信
	memset(m_aReceiveMessage[nIdxClient], 0, sizeof(m_aReceiveMessage[nIdxClient]));
	m_nLengthData[nIdxClient] = recv(m_sockClient[nIdxClient], m_aReceiveMessage[nIdxClient], MAX_SERVER_DATA, 0);

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
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_NONE) == 0
		&& CServerLib::Memcmp(m_aReceiveMessage[(nIdxClient + 1) % 2], CD_SEND_CLIENT) != 0)
	{
		strcat(m_aSendMessage[nIdxClient], "???");
	}

	// クライアントへメッセージを送信
	send(m_sockClient[nIdxClient], m_aSendMessage[nIdxClient], strlen(m_aSendMessage[nIdxClient]), 0);
	memset(m_aSendMessage[nIdxClient], 0, sizeof(m_aSendMessage[nIdxClient]));
}

//=============================================================================
//    Helloメッセージをクライアントへ送信する処理
//=============================================================================
void CServer::Command_Hello(int nIdxClient)
{
	// クライアントへメッセージを送信
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    クライアントからクライアントへメッセージ送信する処理
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
	// クライアントへメッセージを送信
	strcat(m_aSendMessage[(nIdxClient + 1) % 2], "Hello Client");
}

//=============================================================================
//    クライアントとの交信を終了する処理
//=============================================================================
void CServer::Command_DeleteClient(int nIdxClient)
{
	// ソケット(クライアント)の開放
	closesocket(m_sockClient[nIdxClient]);
	m_bConnected[nIdxClient] = false;
	CDebugProc::ClearString();
	CDebugProc::Print("%d番の接続が途絶えました・・・\n", nIdxClient);
	CDebugProc::Print("接続受付開始\n");
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

	// 自分のクライアント番号をサーバーから設定
	SetClientUserId();


	m_fPlayerPos[0] = 0.0f;
	m_fPlayerPos[1] = 0.0f;
	m_fPlayerPos[2] = 0.0f;

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
//   自分のクライアント番号をサーバーから受信する処理
//=============================================================================
void CClient::SetClientUserId(void)
{
	m_nClientId = 0;  // ユーザーID初期化

	// サーバーへデータ(メッセージ)を送信
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, CD_USERID);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// サーバーからデータ(メッセージ)を受信
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	// メッセージをクライアント番号に変換
	m_nClientId = CServerLib::ReadInt(m_aReceiveMessage, USER_ID);
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
	strcpy(m_aSendMessage, CD_SE_DATA);

	char aStr[64];
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer(m_nClientId);
	strcat(m_aSendMessage, CServerLib::WriteFloat(pPlayer->GetPos().x, aStr));
	strcat(m_aSendMessage, " ");
	strcat(m_aSendMessage, CServerLib::WriteFloat(pPlayer->GetPos().y, aStr));
	strcat(m_aSendMessage, " ");
	strcat(m_aSendMessage, CServerLib::WriteFloat(pPlayer->GetPos().z, aStr));

	// サーバーへデータ(メッセージ)を送信
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// サーバーからデータ(メッセージ)を受信
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// メッセージを表示する
		CDebugProc::Print("%s\n", m_aReceiveMessage);
		CDebugProc::Print("受信バイト数[%d]\n", m_nLengthData);

		// 受信したデータからオフセットに従ってデータを読み取る
		LoadData_FromMesseage();
	}
}

//=============================================================================
//    受信したデータからオフセットに従ってデータを読み取る
//=============================================================================
void CClient::LoadData_FromMesseage(void)
{
	// メッセージコピー
	char aStr[MAX_SERVER_DATA];
	strcpy(aStr, m_aReceiveMessage);

	// プレイヤーの座標読み取り
	LoadPlayerPos_FromMesseage(aStr);
}

//=============================================================================
//    受信したデータからプレイヤーの座標データを読み取る
//=============================================================================
void CClient::LoadPlayerPos_FromMesseage(char *pSrc)
{
	// プレイヤーの座標を読み取る
	int nWord = 0;
	char aStr[64];
	m_fPlayerPos[0] = CServerLib::ReadFloat(pSrc, "");
	nWord = CServerLib::PopString(pSrc, aStr);
	pSrc += nWord;
	m_fPlayerPos[1] = CServerLib::ReadFloat(pSrc, "");
	nWord = CServerLib::PopString(pSrc, aStr);
	pSrc += nWord;
	m_fPlayerPos[2] = CServerLib::ReadFloat(pSrc, "");
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

//=============================================================================
//    サーバーからデータを取得する処理
//=============================================================================
void CClient::Command_SendDataClient(void)
{
	strcat(m_aSendMessage, CD_SE_DATA);
}

//=============================================================================
//    自分のクライアント番号を取得する
//=============================================================================
int CClient::GetClientId(void)
{
	return m_nClientId;
}

//=============================================================================
//    プレイヤーの座標を取得する
//=============================================================================
float CClient::GetPlayerPos(int nIdx)
{
	return m_fPlayerPos[nIdx];
}