#ifndef _SAI_EFFECT_H_ 
#define _SAI_EFFECT_H_

#include "libTool.h"
#include "LoadSoundClass.h"
#include "SaiAudio2.h"
#include <xapobase.h>
#pragma comment(lib,"xapobase.lib")

// APO
#include "SaiDelay.h"

// クラス
class SAI_EFFECT_INTERFACE
{
public:
	SAI_EFFECT_INTERFACE(SAI_VOICE_TOOL *svt);
	~SAI_EFFECT_INTERFACE();

private:

	XAUDIO2_EFFECT_CHAIN		chain;			// エフェクトチェン
	XAUDIO2_EFFECT_DESCRIPTOR	descriptor;		// エフェクトディスクリプタ

	SAI_APO_DELAY				saiDelay;		// SAI_DELAY_APOのパラメータ
};

#endif