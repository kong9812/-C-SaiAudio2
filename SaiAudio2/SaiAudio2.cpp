#include "SaiAudio2.h"

// 初期化
SAI_AUDIO2_INTERFACE::SAI_AUDIO2_INTERFACE()
{
	// COMライブラリの初期化
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		// 成功+1
		initCheck++;

		// XAudio2オブジェクトの作成
		if (SUCCEEDED(XAudio2Create(&xAudio2)))
		{
			// 成功+1
			initCheck++;

			// マスターボイスの作成
			if (SUCCEEDED(xAudio2->CreateMasteringVoice(&masteringVoice)))
			{
				// 成功+1
				initCheck++;

				// クラスnew
				wavFile = new SAI_LOAD_WAV;
			}
		}
	}
}


// 終了処理
SAI_AUDIO2_INTERFACE::~SAI_AUDIO2_INTERFACE()
{
	// COMライブラリの終了処理
	CoUninitialize();

	// マスターボイス
	if (masteringVoice != NULL)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = NULL;
	}

	// インターフェース
	if (xAudio2 != NULL)
	{
		xAudio2->Release();
		xAudio2 = NULL;
	}

	// ボリュームのメモリ解放
	free(channelVolume);

	// クラスdelete
	delete wavFile;
}

// 初期化の結果
bool SAI_AUDIO2_INTERFACE::InitCheck()
{
	// 初期化成功 = 三回のSUCCEEDED
	if (initCheck != SAI_INIT_SUCCEEDED)
	{
		return false;
	}

	return true;
}

// 再生
HRESULT SAI_AUDIO2_INTERFACE::PlayVoice(SAI_VOICE_TOOL *svt)
{
	// ボイスと再生状態のチェック
	if ((svt->sourceVoice != NULL)&&(!svt->isPlay))
	{
		// 再生
		svt->sourceVoice->Start();

		return S_OK;
	}

	return S_FALSE;
}

// 停止
HRESULT SAI_AUDIO2_INTERFACE::StopVoice(SAI_VOICE_TOOL *svt)
{
	// ボイスと再生状態のチェック
	if ((svt->sourceVoice != NULL) && (svt->isPlay))
	{
		// 停止
		svt->sourceVoice->Stop();

		return S_OK;
	}

	return S_FALSE;
}

// 一時停止
HRESULT SAI_AUDIO2_INTERFACE::PauseVoice(SAI_VOICE_TOOL *svt)
{
	// ボイスと再生状態のチェック
	if ((svt->sourceVoice != NULL) && (svt->isPlay))
	{
		// 一時停止
		svt->sourceVoice->Stop(XAUDIO2_PLAY_TAILS);

		return S_OK;
	}

	return S_FALSE;
}

//==============================================================
// ロードサウンド
//==============================================================
IXAudio2SourceVoice *SAI_AUDIO2_INTERFACE::LoadXAudio2Voice(const char *path, SAI_VOICE_TOOL *svt)
{
	IXAudio2SourceVoice *sourceVoice = NULL;	// ソースボイス
	WAVEFORMATEX		pcm;					// WAVEFORMATEX構造体
	XAUDIO2_BUFFER		buffer;					// バッファ

	{// ローカル変数の初期化

		// 構造体の初期化
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// wavファイルの読み込み(WAVEFORMATEX構造体)
		pcm = wavFile->LoadWavFile(path, &svt->wavFmt);

		// バッファの設定
		buffer.pAudioData	= (BYTE *)svt->wavFmt.data.waveData;
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes	= svt->wavFmt.data.waveSize;
		buffer.LoopCount	= svt->loopCnt;
	}

	// ソースボイスの作成
	if (FAILED(xAudio2->CreateSourceVoice(&sourceVoice, &pcm, NULL, XAUDIO2_MAX_FREQ_RATIO)))
	{
		return NULL;
	}

	// バッファを提出
	if (FAILED(sourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return NULL;
	}

	{// ボリュームの初期化
		// メモリ確保[チャンネル数]
		channelVolume = (float *)malloc(svt->wavFmt.fmt.fmtChannel * sizeof(float));
		for (int i = 0; i < svt->wavFmt.fmt.fmtChannel; i++)
		{
			channelVolume[i] = SAI_DEFAULT_VOLUME;
		}
	}

	return sourceVoice;
}
