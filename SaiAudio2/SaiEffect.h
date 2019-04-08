#ifndef _SAI_EFFECT_H_ 
#define _SAI_EFFECT_H_

#include "libTool.h"
#include "LoadSoundClass.h"
#include "SaiAudio2.h"
#include <xapobase.h>
#pragma comment(lib,"xapobase.lib")

// APO
#include "SaiDelay.h"

// �N���X
class SAI_EFFECT_INTERFACE
{
public:
	SAI_EFFECT_INTERFACE(SAI_VOICE_TOOL *svt);
	~SAI_EFFECT_INTERFACE();

private:

	XAUDIO2_EFFECT_CHAIN		chain;			// �G�t�F�N�g�`�F��
	XAUDIO2_EFFECT_DESCRIPTOR	descriptor;		// �G�t�F�N�g�f�B�X�N���v�^

	SAI_APO_DELAY				saiDelay;		// SAI_DELAY_APO�̃p�����[�^
};

#endif