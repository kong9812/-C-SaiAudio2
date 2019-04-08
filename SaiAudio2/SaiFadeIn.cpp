#include "SaiFadeIn.h"

//=============================================================================
// APOクラスの登録
//=============================================================================
XAPO_REGISTRATION_PROPERTIES SAI_FADE_IN_APO::_regProps = {
	__uuidof(SAI_FADE_IN_APO),
	L"SAI_FADE_IN_APO",
	L"Copyright (C)2019 CHOI YAU KONG",
	1,
	0,
	XAPOBASE_DEFAULT_FLAG,
	1, 1, 1, 1 };

//=============================================================================
// クラスの初期化
//=============================================================================
SAI_FADE_IN_APO::SAI_FADE_IN_APO(SAI_VOICE_TOOL *svt) : CXAPOParametersBase(&_regProps, (BYTE *)apoParameter, sizeof(SAI_APO_FADE_IN), false)
{
	// パラメータの初期化
	for (int i = 0; i < 3; i++)
	{
		apoParameter[i].fadeTime	= 10.0f;
	}

	// wavFileのコピー
	wavFmt = svt->svtBase.wavFmt;
}

//=============================================================================
// クラスの終了処理
//=============================================================================
SAI_FADE_IN_APO::~SAI_FADE_IN_APO()
{

}

//=============================================================================
// LockForProcessのオーバーライド
//=============================================================================
HRESULT SAI_FADE_IN_APO::LockForProcess
(UINT32 inputLockedParameterCount,
	const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters,
	UINT32 outputLockedParameterCount,
	const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters)
{
	const HRESULT hr = CXAPOParametersBase::LockForProcess(
		inputLockedParameterCount,
		pInputLockedParameters,
		outputLockedParameterCount,
		pOutputLockedParameters);

	if (SUCCEEDED(hr))
	{
		memcpy(&inFormat, pInputLockedParameters[0].pFormat, sizeof(inFormat));
		memcpy(&outFormat, pOutputLockedParameters[0].pFormat, sizeof(outFormat));
	}

	return hr;
}

//=============================================================================
// OnSetParametersのオーバーライド
//=============================================================================
void SAI_FADE_IN_APO::OnSetParameters
(const void* pParameters, UINT32 ParameterByteSize)
{
	SAI_APO_FADE_IN *tmpParameters = ((SAI_APO_FADE_IN *)pParameters);

	// フェイド可能の状態
	fadeSample		= (int)(tmpParameters->fadeTime * wavFmt.fmt.fmtChannel * wavFmt.fmt.fmtSampleRate);
	fadeSampleIndex = 0;
	isFade			= false;
}

//=============================================================================
// Processのオーバーライド
//=============================================================================
void SAI_FADE_IN_APO::Process
(UINT32 inputProcessParameterCount,
	const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters,
	UINT32 outputProcessParameterCount,
	XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters,
	BOOL isEnabled)
{
	if ((isEnabled) && (!isFade))
	{
		if (pInputProcessParameters->BufferFlags != XAPO_BUFFER_SILENT)
		{
			// 仮のパラメータ構造体 = 使用するパラメータのポインたー
			SAI_APO_FADE_IN *tmpParmeter = (SAI_APO_FADE_IN *)BeginProcess();

			// 入出力のバッファ
			float *inputBuf = (float *)pInputProcessParameters->pBuffer;
			float *outputBuf = (float *)pOutputProcessParameters->pBuffer;

			for (int i = 0; i < ((int)pInputProcessParameters->ValidFrameCount * wavFmt.fmt.fmtChannel); i++)
			{
				float tmp = (float)fadeSampleIndex / (float)fadeSample;
				outputBuf[i] = inputBuf[i] * tmp;

				fadeSampleIndex++;

				if (fadeSampleIndex >= fadeSample)
				{
					// フェイド完了
					isFade = true;
				}
			}

			// エンドプロセス
			EndProcess();
		}
	}
}