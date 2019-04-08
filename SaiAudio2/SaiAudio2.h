#ifndef _SAIAUDIO2_H_
#define _SAIAUDIO2_H_

#include "libTool.h"
#include <xaudio2.h>
#pragma comment (lib, "xaudio2.lib")
#include "LoadSoundClass.h"

// マクロ定義
#define SAI_INIT_SUCCEEDED					(3)
#define SAI_DEFAULT_VOLUME					(1.0f)
#define SAI_LOOP_CNT_NOPE					(XAUDIO2_NO_LOOP_REGION)
#define SAI_LOOP_CNT_LOOP					(XAUDIO2_LOOP_INFINITE)

//構造体宣言
typedef struct			// SaiVoiceTool (SVT)
{
	IXAudio2SourceVoice *sourceVoice;
	WAV_FILE			wavFmt;
	int					loopCnt;
	bool				isPlay;
}SAI_VOICE_TOOL;

// クラス宣言
class SAI_AUDIO2_INTERFACE
{
public:
	SAI_AUDIO2_INTERFACE();		// 初期化
	~SAI_AUDIO2_INTERFACE();	// 終了処理

	// ロードサウンド関数
	IXAudio2SourceVoice *LoadXAudio2Voice(const char *path, SAI_VOICE_TOOL *svt);

	// 初期化の結果(True:成功 False:失敗)
	bool InitCheck();

	// 再生
	HRESULT PlayVoice(SAI_VOICE_TOOL *svt);

	// 再生
	HRESULT StopVoice(SAI_VOICE_TOOL *svt);

	// 一時停止
	HRESULT PauseVoice(SAI_VOICE_TOOL *svt);

private:
	SAI_LOAD_WAV			*wavFile;					// wavファイルの読み込み用クラス

	IXAudio2MasteringVoice	*masteringVoice = NULL;		// マスターボイス
	IXAudio2				*xAudio2		= NULL;		// XAudio2のインターフェイス
	
	int						initCheck		= 0;		// 初期化の結果
	float					*channelVolume	= NULL;		// ボリューム
};

#endif