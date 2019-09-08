//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
	const char *Filename;	// ファイル名
	bool Loop;				// ループするかどうか
} PARAM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD Buffersize, DWORD Bufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *XAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *MasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *SourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *DataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD SizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

// 各音素材のパラメータ
PARAM Param[SOUND_LABEL_MAX] =
{
	{"data/BGM/bgm_GAME_MODE.wav", true},					// BGM0
	{"data/BGM/bgm_BOSS_MODE.wav", true},					// BGM1
	{"data/SE/shot000.wav", false},							// 弾発射音
	{"data/SE/explosion000.wav", false},					// 爆発音
	{"data/SE/se_maoudamashii_explosion05.wav", false},		// 爆発音２
	{"data/SE/se_Enterオン.wav", false},					// エンターオン
	{"data/SE/se_mission_false.wav", false},				// エンターオン
	{"data/SE/se_mission_true.wav", false},					// エンターオン
	{"data/SE/ロック音.wav", false},						// ロック音
	{"data/SE/キャンセル音.wav", false},					// キャンセル音
	{"data/SE/エネミー爆発音.wav", false},					// エネミーの爆発音
	
};

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&XAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = XAudio2->CreateMasteringVoice(&MasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(XAudio2)
		{
			// XAudio2オブジェクトの開放
			XAudio2->Release();
			XAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int CntSound = 0; CntSound < SOUND_LABEL_MAX; CntSound++)
	{
		HANDLE hFile;
		DWORD ChunkSize = 0;
		DWORD ChunkPosition = 0;
		DWORD Filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(Param[CntSound].Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &ChunkSize, &ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &Filetype, sizeof(DWORD), ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(Filetype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &ChunkSize, &ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, ChunkSize, ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &SizeAudio[CntSound], &ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		DataAudio[CntSound] = (BYTE*)malloc(SizeAudio[CntSound]);
		hr = ReadChunkData(hFile, DataAudio[CntSound], SizeAudio[CntSound], ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = XAudio2->CreateSourceVoice(&SourceVoice[CntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = SizeAudio[CntSound];
		buffer.pAudioData = DataAudio[CntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = 0;

		// オーディオバッファの登録
		SourceVoice[CntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int CntSound = 0; CntSound < SOUND_LABEL_MAX; CntSound++)
	{
		if(SourceVoice[CntSound])
		{
			// 一時停止
			SourceVoice[CntSound]->Stop(0);
	
			// ソースボイスの破棄
			SourceVoice[CntSound]->DestroyVoice();
			SourceVoice[CntSound] = NULL;
	
			// オーディオデータの開放
			free(DataAudio[CntSound]);
			DataAudio[CntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	MasteringVoice->DestroyVoice();
	MasteringVoice = NULL;
	
	if(XAudio2)
	{
		// XAudio2オブジェクトの開放
		XAudio2->Release();
		XAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = SizeAudio[label];
	buffer.pAudioData = DataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = 0;

	// 状態取得
	SourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		SourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		SourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	SourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	SourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント停止
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	SourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		SourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		SourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int CntSound = 0; CntSound < SOUND_LABEL_MAX; CntSound++)
	{
		if(SourceVoice[CntSound])
		{
			// 一時停止
			SourceVoice[CntSound]->Stop(0);
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD Read;
	DWORD ChunkType;
	DWORD ChunkDataSize;
	DWORD RIFFDataSize = 0;
	DWORD FileType;
	DWORD BytesRead = 0;
	DWORD Offset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &ChunkType, sizeof(DWORD), &Read, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &ChunkDataSize, sizeof(DWORD), &Read, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(ChunkType)
		{
		case 'FFIR':
			RIFFDataSize  = ChunkDataSize;
			ChunkDataSize = 4;
			if(ReadFile(hFile, &FileType, sizeof(DWORD), &Read, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, ChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		Offset += sizeof(DWORD) * 2;
		if(ChunkType == format)
		{
			*pChunkSize         = ChunkDataSize;
			*pChunkDataPosition = Offset;

			return S_OK;
		}

		Offset += ChunkDataSize;
		if(BytesRead >= RIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD Buffersize, DWORD Bufferoffset)
{
	DWORD Read;
	
	if(SetFilePointer(hFile, Bufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, Buffersize, &Read, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

