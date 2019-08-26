//*****************************************************************************
//
//     サウンドの処理[sound.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sound.h"
#include "fileLoader.h"
#include "functionlib.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// データ読み取り用のパス
#define NUM_SOUND    "NUM_SOUND = "
#define SOUNDSET     "SOUNDSET"
#define FILENAME     "FILENAME = "
#define LOOP         "LOOP = "
#define END_SOUNDSET "END_SOUNDSET"

//=============================================================================
//    コンストラクタ
//=============================================================================
CSound::CSound()
{
	// 値のクリア
	m_pXAudio2 = NULL;          // XAudio2オブジェクトへのポインタ
	m_pMasteringVoice = NULL;   // マスターボイスへのポインタ
	m_apSourceVoice = NULL;  	// ソースボイスへのポインタ
	m_apDataAudio = NULL;	    // オーディオデータへのポインタ
	m_apSizeAudio = NULL;		// オーディオデータサイズへのポインタ
	m_nNumSound = 0;            // サウンドデータの数
	m_apSoundParam = NULL;      // 音素材のパラメータへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd, char *pFileName)
{
	HRESULT hr;

	// ファイル名をコピー
	strcpy(m_aFileName, pFileName);

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンド情報をファイルから読み取る
	if (FAILED(LoadSoundData()))
	{
		MessageBox(hWnd, "サウンドデータの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// サウンドデータの初期化
	if (m_apDataAudio != NULL && m_apSizeAudio != NULL && m_apSoundParam != NULL && m_apSourceVoice != NULL)
	{// メモリが確保できている
		for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
		{// サウンドデータの数だけ繰り返し
			HANDLE hFile;
			DWORD dwChunkSize = 0;
			DWORD dwChunkPosition = 0;
			DWORD dwFiletype;
			WAVEFORMATEXTENSIBLE wfx;
			XAUDIO2_BUFFER buffer;

			// バッファのクリア
			memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

			// サウンドデータファイルの生成
			hFile = CreateFile(m_apSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
				return HRESULT_FROM_WIN32(GetLastError());
			}
			if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタを先頭に移動
				MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
				return HRESULT_FROM_WIN32(GetLastError());
			}

			// WAVEファイルのチェック
			hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			if (dwFiletype != 'EVAW')
			{
				MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// フォーマットチェック
			hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// オーディオデータ読み込み
			hr = CheckChunk(hFile, 'atad', &m_apSizeAudio[nCntSound], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			m_apDataAudio[nCntSound] = (BYTE*)malloc(m_apSizeAudio[nCntSound]);
			hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_apSizeAudio[nCntSound], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// ソースボイスの生成
			hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = m_apSizeAudio[nCntSound];
			buffer.pAudioData = m_apDataAudio[nCntSound];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = m_apSoundParam[nCntSound].nCntLoop;

			// オーディオバッファの登録
			m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
	{// サウンドデータの数だけ繰り返し
		if (m_apSourceVoice[nCntSound])
		{// メモリが生成されている
		    // 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの破棄
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	// ソースボイスの開放
	delete[] m_apSourceVoice;
	m_apSourceVoice = NULL;

	// オーディオデータの開放
	delete[] m_apDataAudio;
	m_apDataAudio = NULL;

	// 音素材パラメータの開放
	for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
	{// サウンドデータの数だけ繰り返し
		free(m_apSoundParam[nCntSound].pFilename);
		m_apSoundParam[nCntSound].pFilename = NULL;
	}
	delete[] m_apSoundParam;
	m_apSoundParam = NULL;


	// オーディオデータ数の開放
	delete[] m_apSizeAudio;
	m_apSizeAudio = NULL;

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	// XAudio2オブジェクトの開放
	DIRECT_RELEASE(m_pXAudio2)

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
//    セグメント再生(停止)
//=============================================================================
HRESULT CSound::PlaySound(int nIdx, bool bPause)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	if (m_apDataAudio != NULL && m_apSourceVoice != NULL && m_apSizeAudio != NULL && m_apSoundParam != NULL)
	{// サウンドデータが生成されている
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_apSizeAudio[nIdx];
		buffer.pAudioData = m_apDataAudio[nIdx];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_apSoundParam[nIdx].nCntLoop;

		// 状態取得
		m_apSourceVoice[nIdx]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
		    // 一時停止
			m_apSourceVoice[nIdx]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[nIdx]->FlushSourceBuffers();
		}

		if (bPause == true)
		{// 一時停止から復活するなら
		    // 前回の再生位置にサイズを合わせておく
			buffer.PlayBegin = m_apSoundParam[nIdx].nPlayPositioned;
		}

		// オーディオバッファの登録
		m_apSourceVoice[nIdx]->SubmitSourceBuffer(&buffer);

		// 再生
		m_apSourceVoice[nIdx]->Start(0);
	}

	return S_OK;
}

//=============================================================================
//    セグメント停止
//=============================================================================
void CSound::StopSound(int nIdx, bool bPause)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	if (m_apSourceVoice[nIdx] != NULL)
	{// ボイスデータが生成されている
		m_apSourceVoice[nIdx]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
		    // 停止
			m_apSourceVoice[nIdx]->Stop(0);

			if (bPause == true)
			{// 一時停止ならば
				// 再生位置を保存
				m_apSoundParam[nIdx].nPlayPositioned = (int)xa2state.SamplesPlayed;
			}

			// オーディオバッファの削除
			m_apSourceVoice[nIdx]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
//    セグメント停止
//=============================================================================
void CSound::StopSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
	{// サウンドデータの数だけ繰り返し
		if (m_apSourceVoice[nCntSound] != NULL)
		{// メモリが確保されている
		    // 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
//    ボリュームの設定
//=============================================================================
void CSound::SetVoluem(int nIdx, float fValue)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	if (m_apSourceVoice[nIdx] != NULL)
	{// ボイスデータが生成されている
		m_apSourceVoice[nIdx]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			m_apSourceVoice[nIdx]->SetVolume(fValue);
		}
	}
}

//=============================================================================
//    サウンドデータを読み込む
//=============================================================================
HRESULT CSound::LoadSoundData(void)
{
	// ファイルオープン
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL) return E_FAIL;

	// データ読み込み開始
	char aStr[256];
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// スクリプト読み込み開始の合図だった
		LoadSoundScript(pFileLoader, aStr);
	}


	// ファイルを閉じる
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	return S_OK;
}

//=============================================================================
//    サウンドデータスクリプトを読み込む
//=============================================================================
void CSound::LoadSoundScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntSound = 0;

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_SOUND) == 0)
		{// サウンドデータの数だった
			m_nNumSound = CFunctionLib::ReadInt(pStr, NUM_SOUND);
			if (m_nNumSound >= 1)
			{// サウンドデータの数が1つでもある
				CreatePointer();
			}
		}
		else if (CFunctionLib::Memcmp(pStr, SOUNDSET) == 0)
		{// 音情報読み込み開始の合図だった
			LoadSoundSet(pFileLoader, pStr, nCntSound);
			nCntSound++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    サウンドセットを読み込む
//=============================================================================
void CSound::LoadSoundSet(CFileLoader *pFileLoader, char *pStr, int nCntSound)
{
	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, FILENAME) == 0)
		{// ファイル名だった
			strcpy(m_apSoundParam[nCntSound].pFilename, CFunctionLib::ReadString(pStr, m_apSoundParam[nCntSound].pFilename, FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{// ループカウント情報だった
			m_apSoundParam[nCntSound].nCntLoop = CFunctionLib::ReadInt(pStr, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SOUNDSET) == 0)
		{// 音情報読み込み終了の合図だった
			nCntSound++;
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    サウンドデータのポインタを確保する
//=============================================================================
HRESULT CSound::CreatePointer(void)
{
	// ソースボイスへのポインタ
	if (m_apSourceVoice == NULL)
	{// メモリを確保できる状態である
		m_apSourceVoice = new IXAudio2SourceVoice*[m_nNumSound];
		if (m_apSourceVoice != NULL)
		{// メモリを確保できた
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// サウンドデータの数だけ繰り返し
				m_apSourceVoice[nCntSound] = NULL;
			}
		}
		else
		{// メモリを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// オーディオデータへのポインタ
	if (m_apDataAudio == NULL)
	{// メモリを確保できる状態である
		m_apDataAudio = new BYTE*[m_nNumSound];
		if (m_apDataAudio != NULL)
		{// メモリを確保できた
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// サウンドデータの数だけ繰り返し
				m_apDataAudio[nCntSound] = NULL;
			}
		}
		else
		{// メモリを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// オーディオデータサイズ
	if (m_apSizeAudio == NULL)
	{// メモリを確保できる状態である
		m_apSizeAudio = new DWORD[m_nNumSound];
		if (m_apSizeAudio != NULL)
		{// メモリを確保できた
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// サウンドデータの数だけ繰り返し
				m_apSizeAudio[nCntSound] = 0x00;
			}
		}
		else
		{// メモリを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// 音素材のパラメータ
	if (m_apSoundParam == NULL)
	{// メモリを確保できる状態である
		m_apSoundParam = new SOUNDPARAM[m_nNumSound];
		if (m_apSoundParam != NULL)
		{// メモリを確保できた
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// サウンドデータの数だけ繰り返し
				m_apSoundParam[nCntSound].pFilename = new char[256];
				strcpy(m_apSoundParam[nCntSound].pFilename, "\0");
				m_apSoundParam[nCntSound].nCntLoop = 0;
				m_apSoundParam[nCntSound].nPlayPositioned = 0;
			}
		}
		else
		{// メモリを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
//    チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}