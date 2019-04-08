#ifndef _SAI_CUSTOM_APO_TEST_H_
#define _SAI_CUSTOM_APO_TEST_H_

#define XAPOASSERT
#include <xapobase.h>
#pragma comment(lib,"xapobase.lib")

#include "xaudioPlayerObj.h"
#include "xaudioEffect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DELAY_TIME_DEFAULT		(1.0f)
#define DELAY_VOLUME_DEFAULT	(0.5f)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// SAI式I 遅延処理
{
	float delayTime;		// 遅延の秒数
	float volume;			// 残響の音量

}SAI_APO_DELAY;

//*****************************************************************************
// ディレイ用APOクラス
//*****************************************************************************
class __declspec(uuid("{80D4BED4-7605-4E4C-B29C-5479C375C599}"))SAI_DELAY_APO : public CXAPOParametersBase
{
public:
	SAI_DELAY_APO();		// 初期化用
	~SAI_DELAY_APO();	// 終了用

	// LockForProcessのオーバーライド
	STDMETHOD(LockForProcess)
	(UINT32 inputLockedParameterCount,
	const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters,
	UINT32 outputLockedParameterCount,
	const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters) 
	override;

	// Processのオーバーライド
	STDMETHOD_(void, Process)
	(UINT32 inputProcessParameterCount,
	const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters,
	UINT32 outputProcessParameterCount,
	XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters, 
	BOOL isEnabled) 
	override;

	// OnSetParametersのオーバーライト
	virtual void OnSetParameters
	(const void* pParameters, UINT32 ParameterByteSize);

private:
	static XAPO_REGISTRATION_PROPERTIES _regProps;

	WAVEFORMATEX inFormat;
	WAVEFORMATEX outFormat;

	// エコー用
	float	*backupBuf;
	int		delaySample;
	int		readPos;
	int		writePos;
	bool	play;

	SAI_APO_DELAY apoParameter[3];
};

#endif