//*****************************************************************************
//
//     サーバーの処理[server.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SERVER_H_
#define _SERVER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_SERVER_CONNECTING_NUM  (2)  // サーバーにアクセスできる最大人数
#define MAX_SERVER_DATA            (64) // 一度にやり取りできるデータの最大バイト数

// サーバーのコマンド
#define CD_NONE                "CD_NONE"
#define CD_HELLO               "CD_HELLO"
#define CD_SEND_CLIENT         "CD_SEND_CLIENT"
#define CD_DELETE              "CD_DELETE"
#define CD_SE_DATA             "CD_SE_DATA"
#define CD_USERID              "CD_USERID"

// メッセージ読み取り用
#define USER_ID                "USER_ID"

//*****************************************************************************
//     サーバークラスの定義
//*****************************************************************************
class CServer
{
public:    // 誰でもアクセス可能
	CServer();
	~CServer();

	static HRESULT ServerStartUp(void);
	static void ServerCleanUp(void);
	static CServer *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void WaitConnect(int nIdxClient);
	void ExChangeMessage(int nIdxClient);
	void Command_Hello(int nIdxClient);
	void Command_Send_Client(int nIdxClient);
	void Command_DeleteClient(int nIdxClient);

	struct sockaddr_in m_addrServer;                                       // ソケットアドレス(サーバー)の情報
	struct sockaddr_in m_addrClient[MAX_SERVER_CONNECTING_NUM];            // ソケットアドレス(クライアント)の情報
	SOCKET m_sockServer;                                                   // ソケット(サーバー用)
	SOCKET m_sockClient[MAX_SERVER_CONNECTING_NUM];                        // ソケット(クライアント用)
	int m_nLengthAddrClient[MAX_SERVER_CONNECTING_NUM];                    // ソケットアドレス(クライアント)のサイズ
	char m_aReceiveMessage[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_DATA];    // 受信データ
	char m_aSendMessage[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_DATA];       // 送信データ
	int m_nLengthData[MAX_SERVER_CONNECTING_NUM];                          // 受信データのサイズ
	bool m_bConnected[MAX_SERVER_CONNECTING_NUM];                          // クライアントと接続したかどうか
};

//*****************************************************************************
//    クライアントクラスの定義
//*****************************************************************************
class CClient
{
public:    // 誰でもアクセス可能
	CClient();
	~CClient();

	static CClient *Create(char *pSeverAddr);

	HRESULT Init(char *pSeverAddr);
	void Uninit(void);
	void Update(void);

	int GetClientId(void);
	float GetPlayerPos(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SetServerAddr(char *pServerAddr);
	void SetClientUserId(void);
	void WaitConnect(void);
	void ExChangeMessage(void);
	void Command_Hello(void);
	void Command_Send_Client(void);
	void Command_DeleteClient(void);
	void Command_SendDataClient(void);

	void LoadData_FromMesseage(void);
	void LoadPlayerPos_FromMesseage(char *pSrc);

	int m_nClientId;                          // 自分のクライアント番号(プレイヤーの番号と紐づけ)
	char m_aServerAddr[64];                   // サーバーのIPアドレス
	struct sockaddr_in m_addrServer;          // ソケットアドレス(サーバー)の情報
	SOCKET m_sockClient;                      // ソケット(クライアント用)
	char m_aReceiveMessage[MAX_SERVER_DATA];  // 受信データ
	char m_aSendMessage[MAX_SERVER_DATA];     // 送信データ
	int m_nLengthData;                        // 受信データのサイズ
	bool m_bConnected;                        // サーバーに接続したかどうか


	float m_fPlayerPos[3];
};

#endif
