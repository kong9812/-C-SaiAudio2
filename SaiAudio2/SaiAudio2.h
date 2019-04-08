#ifndef _SAIAUDIO2_H_
#define _SAIAUDIO2_H_

#include "libTool.h"
#include "LoadSoundClass.h"
#include <xaudio2.h>
#pragma comment (lib, "xaudio2.lib")

// マクロ定義
#define SAI_INIT_SUCCEEDED					(3)
#define SAI_DEFAULT_VOLUME					(1.0f)
#define SAI_LOOP_CNT_NOPE					(XAUDIO2_NO_LOOP_REGION)
#define SAI_LOOP_CNT_INFINITE				(XAUDIO2_LOOP_INFINITE)

//構造体宣言
typedef struct			// SaiVoiceTool (SVT)
{
	IXAudio2SourceVoice *sourceVoice;		// 変更しないでください! DONT CHANGE ANYTHING! 
	WAV_FILE			wavFmt;				// 変更しないでください! DONT CHANGE ANYTHING! 
	bool				isPlay;				// 変更しないでください! DONT CHANGE ANYTHING! 
	int					loopCnt;			// ループ用カウンター
}SAI_SVT_BASE;

typedef struct			// SaiVoiceTool (SVT)
{
	float				freqPich;			// ピーチ
	
}SAI_SVT_SECONDARY;

typedef struct			// SaiVoiceTool (SVT)
{
	SAI_SVT_BASE		svtBase;			// SVTのベース部分
	SAI_SVT_SECONDARY	svtSecondary;		// SVTの基本パラメータ

}SAI_VOICE_TOOL;

// クラス宣言
class SAI_AUDIO2_INTERFACE
{
public:
	SAI_AUDIO2_INTERFACE(void);		// 初期化
	~SAI_AUDIO2_INTERFACE(void);	// 終了処理

	// ロードサウンド関数
	IXAudio2SourceVoice *LoadXAudio2Voice(const char *path, SAI_VOICE_TOOL *svt);

	// 初期化の結果(True:成功 False:失敗)
	bool InitCheck();

	// マスターボイスを取得
	IXAudio2MasteringVoice *GetMasterVoice(void);

	// 再生
	HRESULT PlayVoice(SAI_VOICE_TOOL *svt);

	// 再生
	HRESULT StopVoice(SAI_VOICE_TOOL *svt);

	// 一時停止
	HRESULT PauseVoice(SAI_VOICE_TOOL *svt);

	// (frequency ratio)ピーチ
	HRESULT SetPitch(SAI_VOICE_TOOL *svt);
	float	GetPitch(SAI_VOICE_TOOL *svt);

private:
	SAI_LOAD_WAV			*wavFile;					// wavファイルの読み込み用クラス

	IXAudio2MasteringVoice	*masteringVoice = NULL;		// マスターボイス
	IXAudio2				*xAudio2		= NULL;		// XAudio2のインターフェイス
	
	int						initCheck		= 0;		// 初期化の結果
	float					*channelVolume	= NULL;		// ボリューム
};

#endif