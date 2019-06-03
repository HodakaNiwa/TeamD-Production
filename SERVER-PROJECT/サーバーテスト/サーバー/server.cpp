//*****************************************************************************
//
//     �T�[�o�[�̏���[server.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "server.h"
#include "serverlib.h"
#include "manager.h"
#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
//    CServer�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
//    WinSock�̏���������
//=============================================================================
HRESULT CServer::ServerStartUp(void)
{
	WSADATA wsaData;  // winsock�̏��
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
//    WinSock�̏I������
//=============================================================================
void CServer::ServerCleanUp(void)
{
	WSACleanup();
}

//=============================================================================
//    ��������
//=============================================================================
CServer *CServer::Create(void)
{
	CServer *pServer = NULL;      // �T�[�o�[�N���X�^�̃|�C���^
	if (pServer == NULL)
	{// ����������ɂȂ��Ă���
		pServer = new CServer;
		if (pServer != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pServer->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pServer;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CServer::Init(void)
{
	// �\�P�b�g�̍쐬
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	// �\�P�b�g�ɖ��̂�t�^
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	// �N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
	listen(m_sockServer, 5);
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_bConnected[nCntConnect] = false;
		WaitConnect(nCntConnect);


		// �N���C�A���g����f�[�^(���b�Z�[�W)����M
		memset(m_aReceiveMessage[nCntConnect], 0, sizeof(m_aReceiveMessage[nCntConnect]));
		m_nLengthData[nCntConnect] = recv(m_sockClient[nCntConnect], m_aReceiveMessage[nCntConnect], MAX_SERVER_DATA, 0);
		Command_UserIdClient(nCntConnect);
		send(m_sockClient[nCntConnect], m_aSendMessage[nCntConnect], strlen(m_aSendMessage[nCntConnect]), 0);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CServer::Uninit(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		// �\�P�b�g(�N���C�A���g)�̊J��
		closesocket(m_sockClient[nCntConnect]);
	}

	// �\�P�b�g(�T�[�o�[)�̊J��
	closesocket(m_sockServer);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CServer::Update(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] != true)
		{// �N���C�A���g�Ɛڑ��ł��Ă��Ȃ�
			WaitConnect(nCntConnect);
		}
		else
		{// �N���C�A���g�Ɛڑ��ł���
#ifdef SERVER_MODE
			CDebugProc::Print("%d�ԂƐڑ�����!!\n", nCntConnect);
#endif
			ExChangeMessage(nCntConnect);
		}
	}
}

//=============================================================================
//    �ڑ��ҋ@����
//=============================================================================
void CServer::WaitConnect(int nIdxClient)
{
	// �N���C�A���g����̐ڑ��v�����󂯕t����
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
//    �N���C�A���g�Ƃ̃��b�Z�[�W��������
//=============================================================================
void CServer::ExChangeMessage(int nIdxClient)
{
	// �N���C�A���g����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage[nIdxClient], 0, sizeof(m_aReceiveMessage[nIdxClient]));
	m_nLengthData[nIdxClient] = recv(m_sockClient[nIdxClient], m_aReceiveMessage[nIdxClient], MAX_SERVER_DATA, 0);

	// ���b�Z�[�W��\������
	CDebugProc::Print("%s\n��M�o�C�g��[%d]\n", m_aReceiveMessage[nIdxClient], m_nLengthData[nIdxClient]);

	// ���b�Z�[�W����R�}���h��K�p
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
		strcat(m_aSendMessage[nIdxClient], "���b�Z�[�W���M");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SE_DATA) == 0)
	{
		Command_SendDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_USERID) == 0)
	{
		Command_UserIdClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_NONE) == 0
		&& CServerLib::Memcmp(m_aReceiveMessage[(nIdxClient + 1) % 2], CD_SEND_CLIENT) != 0)
	{
		strcat(m_aSendMessage[nIdxClient], "???");
	}

	// �N���C�A���g�փ��b�Z�[�W�𑗐M
	send(m_sockClient[nIdxClient], m_aSendMessage[nIdxClient], strlen(m_aSendMessage[nIdxClient]), 0);
	memset(m_aSendMessage[nIdxClient], 0, sizeof(m_aSendMessage[nIdxClient]));
}

//=============================================================================
//    Hello���b�Z�[�W���N���C�A���g�֑��M���鏈��
//=============================================================================
void CServer::Command_Hello(int nIdxClient)
{
	// �N���C�A���g�ւ̃��b�Z�[�W��ݒ�
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    �N���C�A���g����N���C�A���g�փ��b�Z�[�W���M���鏈��
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
}

//=============================================================================
//    �N���C�A���g�Ƃ̌�M���I�����鏈��
//=============================================================================
void CServer::Command_DeleteClient(int nIdxClient)
{
	// �\�P�b�g(�N���C�A���g)�̊J��
	closesocket(m_sockClient[nIdxClient]);
	m_bConnected[nIdxClient] = false;
	CDebugProc::ClearString();
	CDebugProc::Print("%d�Ԃ̐ڑ����r�₦�܂����E�E�E\n", nIdxClient);
	CDebugProc::Print("�ڑ���t�J�n\n");
}

//=============================================================================
//    �N���C�A���g����f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_SendDataClient(int nIdxClient)
{
	float aPlayerPos[3];   // �v���C���[�̍��W
	int nWord = 0;
	char aStr[64];
	char *pStr;
	pStr = &m_aReceiveMessage[nIdxClient][0];

	// ��M�������b�Z�[�W����f�[�^��ǂݎ��
	aPlayerPos[0] = CServerLib::ReadFloat(pStr, CD_SE_DATA); // X���W
	nWord = CServerLib::PopString(pStr, aStr);
	pStr += nWord;
	aPlayerPos[1] = CServerLib::ReadFloat(pStr, "");         // Y���W
	nWord = CServerLib::PopString(pStr, aStr);
	pStr += nWord;
	aPlayerPos[2] = CServerLib::ReadFloat(pStr, "");         // Z���W

	// ��M�������W��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcpy(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat(aPlayerPos[0], aStr));
	strcat(m_aSendMessage[(nIdxClient + 1) % 2], " ");
	strcat(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat(aPlayerPos[1], aStr));
	strcat(m_aSendMessage[(nIdxClient + 1) % 2], " ");
	strcat(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat(aPlayerPos[2], aStr));
}

//=============================================================================
//    �N���C�A���g�֔ԍ��𑗐M���鏈��
//=============================================================================
void CServer::Command_UserIdClient(int nIdxClient)
{
	// �N���C�A���g�ԍ��𑗐M
	char aStr[64];
	strcpy(m_aSendMessage[nIdxClient], USER_ID);
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nIdxClient, aStr));
}


//*****************************************************************************
//    CClient�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
//    ��������
//=============================================================================
CClient *CClient::Create(char *pSeverAddr)
{
	CClient *pClient = NULL;      // �N���C�A���g�N���X�^�̃|�C���^
	if (pClient == NULL)
	{// ����������ɂȂ��Ă���
		pClient = new CClient;
		if (pClient != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pClient->Init(pSeverAddr)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pClient;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CClient::Init(char *pSeverAddr)
{
	// IP�A�h���X��ݒ�
	if (pSeverAddr == NULL) return E_FAIL;
	SetServerAddr(pSeverAddr);

	// �\�P�b�g�̍쐬
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);
	m_bConnected = false;

	// �T�[�o�[�֐ڑ�
	connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));
	m_bConnected = true;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CClient::Uninit(void)
{
	// �T�[�o�[�Ƃ̐ڑ����I��
	Command_DeleteClient();

	// �\�P�b�g(�N���C�A���g)�̊J��
	closesocket(m_sockClient);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected != true)
	{// �T�[�o�[�Ɛڑ��ł��Ă��Ȃ�
		WaitConnect();
	}
	else
	{// �T�[�o�[�Ɛڑ��ł���
		ExChangeMessage();
	}
}

//=============================================================================
//    �T�[�o�[IP�A�h���X��ݒ肷�鏈��
//=============================================================================
void CClient::SetServerAddr(char *pServerAddr)
{
	strcpy(m_aServerAddr, pServerAddr);
}

//=============================================================================
//    �T�[�o�[�Ƃ̐ڑ��ҋ@����
//=============================================================================
void CClient::WaitConnect(void)
{
	// �\�P�b�g�̍쐬
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);

	if (connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer)) == SOCKET_ERROR)
	{
		CDebugProc::Print("�ڑ����r�₦�܂����E�E�E\n");
		CDebugProc::Print("�ڑ��J�n\n");
		m_bConnected = false;
	}
	else
	{
		m_bConnected = true;
		m_nLengthData = 0;
	}
}

//=============================================================================
//    �T�[�o�[�Ƃ̃��b�Z�[�W��������
//=============================================================================
void CClient::ExChangeMessage(void)
{
	// �T�[�o�[�֑��郁�b�Z�[�W���쐬
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

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// ���b�Z�[�W��\������
		CDebugProc::Print("%s\n", m_aReceiveMessage);

		// ���b�Z�[�W��\������
		CDebugProc::Print("��M�o�C�g��[%d]\n", m_nLengthData);
	}
}

//=============================================================================
//    Hello���b�Z�[�W���󂯎�鏈��
//=============================================================================
void CClient::Command_Hello(void)
{
	strcat(m_aSendMessage, CD_HELLO);
}


//=============================================================================
//    �ʂ̃N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��
//=============================================================================
void CClient::Command_Send_Client(void)
{
	strcat(m_aSendMessage, CD_SEND_CLIENT);
}

//=============================================================================
//    �T�[�o�[�Ƃ̐ڑ����I�����鏈��
//=============================================================================
void CClient::Command_DeleteClient(void)
{
	strcat(m_aSendMessage, CD_DELETE);
}