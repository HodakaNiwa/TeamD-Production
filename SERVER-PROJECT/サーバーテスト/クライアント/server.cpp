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

#include "game.h"
#include "player.h"

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
	// �N���C�A���g�փ��b�Z�[�W�𑗐M
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    �N���C�A���g����N���C�A���g�փ��b�Z�[�W���M���鏈��
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
	// �N���C�A���g�փ��b�Z�[�W�𑗐M
	strcat(m_aSendMessage[(nIdxClient + 1) % 2], "Hello Client");
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

	// �����̃N���C�A���g�ԍ����T�[�o�[����ݒ�
	SetClientUserId();


	m_fPlayerPos[0] = 0.0f;
	m_fPlayerPos[1] = 0.0f;
	m_fPlayerPos[2] = 0.0f;

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
//   �����̃N���C�A���g�ԍ����T�[�o�[�����M���鏈��
//=============================================================================
void CClient::SetClientUserId(void)
{
	m_nClientId = 0;  // ���[�U�[ID������

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, CD_USERID);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	// ���b�Z�[�W���N���C�A���g�ԍ��ɕϊ�
	m_nClientId = CServerLib::ReadInt(m_aReceiveMessage, USER_ID);
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
	strcpy(m_aSendMessage, CD_SE_DATA);

	char aStr[64];
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer(m_nClientId);
	strcat(m_aSendMessage, CServerLib::WriteFloat(pPlayer->GetPos().x, aStr));
	strcat(m_aSendMessage, " ");
	strcat(m_aSendMessage, CServerLib::WriteFloat(pPlayer->GetPos().y, aStr));
	strcat(m_aSendMessage, " ");
	strcat(m_aSendMessage, CServerLib::WriteFloat(pPlayer->GetPos().z, aStr));

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// ���b�Z�[�W��\������
		CDebugProc::Print("%s\n", m_aReceiveMessage);
		CDebugProc::Print("��M�o�C�g��[%d]\n", m_nLengthData);

		// ��M�����f�[�^����I�t�Z�b�g�ɏ]���ăf�[�^��ǂݎ��
		LoadData_FromMesseage();
	}
}

//=============================================================================
//    ��M�����f�[�^����I�t�Z�b�g�ɏ]���ăf�[�^��ǂݎ��
//=============================================================================
void CClient::LoadData_FromMesseage(void)
{
	// ���b�Z�[�W�R�s�[
	char aStr[MAX_SERVER_DATA];
	strcpy(aStr, m_aReceiveMessage);

	// �v���C���[�̍��W�ǂݎ��
	LoadPlayerPos_FromMesseage(aStr);
}

//=============================================================================
//    ��M�����f�[�^����v���C���[�̍��W�f�[�^��ǂݎ��
//=============================================================================
void CClient::LoadPlayerPos_FromMesseage(char *pSrc)
{
	// �v���C���[�̍��W��ǂݎ��
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

//=============================================================================
//    �T�[�o�[����f�[�^���擾���鏈��
//=============================================================================
void CClient::Command_SendDataClient(void)
{
	strcat(m_aSendMessage, CD_SE_DATA);
}

//=============================================================================
//    �����̃N���C�A���g�ԍ����擾����
//=============================================================================
int CClient::GetClientId(void)
{
	return m_nClientId;
}

//=============================================================================
//    �v���C���[�̍��W���擾����
//=============================================================================
float CClient::GetPlayerPos(int nIdx)
{
	return m_fPlayerPos[nIdx];
}