//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFileLoader;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:    // 誰でもアクセス可能
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd, char *pFileName);
	void Uninit(void);
	HRESULT PlaySound(int nIdx, bool bPause = false);
	void StopSound(int nIdx, bool bPause = false);
	void StopSound(void);
	void SetVoluem(int nIdx, float fValue);

private:   // このクラスだけがアクセス可能
	HRESULT LoadSoundData(void);
	void LoadSoundScript(CFileLoader *pFileLoader, char *pStr);
	void LoadSoundSet(CFileLoader *pFileLoader, char *pStr, int nCntSound);
	HRESULT CreatePointer(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	char m_aFileName[256];                      // サウンドデータのスクリプトファイル名
	IXAudio2 *m_pXAudio2;						// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;	// マスターボイスへのポインタ
	IXAudio2SourceVoice **m_apSourceVoice;    	// ソースボイスへのポインタ
	BYTE **m_apDataAudio;					    // オーディオデータへのポインタ
	DWORD *m_apSizeAudio;					    // オーディオデータサイズへのポインタ
	int m_nNumSound;                            // サウンドデータの数

	//------------------------
	//  各音素材のパラメータ
	//------------------------
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;
	SOUNDPARAM *m_apSoundParam;   // 音素材のパラメータへのポインタ
};
#endif
