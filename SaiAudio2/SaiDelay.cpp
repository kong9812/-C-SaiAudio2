#include "SaiDelay.h"

//=============================================================================
// APOクラスの登録
//=============================================================================
XAPO_REGISTRATION_PROPERTIES SAI_DELAY_APO::_regProps = {
	__uuidof(SAI_DELAY_APO),
	L"SAI_DELAY_APO",
	L"Copyright (C)2019 CHOI YAU KONG",
	1,
	0,
	XAPOBASE_DEFAULT_FLAG,
	1, 1, 1, 1 };

//=============================================================================
// クラスの初期化
//=============================================================================
SAI_DELAY_APO::SAI_DELAY_APO(SAI_VOICE_TOOL *svt) : CXAPOParametersBase(&_regProps, (BYTE *)apoParameter, sizeof(SAI_APO_DELAY), false)
{
	// パラメータの初期化
	for (int i = 0; i < 3; i++)
	{
		apoParameter[i].volume = DELAY_VOLUME_DEFAULT;
		apoParameter[i].delayTime = DELAY_TIME_DEFAULT;
	}

	// wavFileのコピー
	wavFmt = svt->svtBase.wavFmt;

	// メモリ確保(2秒のサンプリング)
	backupBuf = (float *)malloc(sizeof(float)*(wavFmt.fmt.fmtSampleRate * wavFmt.fmt.fmtChannel));

	// 遅延が発生できるサンプリング数(2秒の遅延)
	delaySample = (wavFmt.fmt.fmtSampleRate * wavFmt.fmt.fmtChannel);

	// 初期化
	memset(backupBuf, 0, sizeof(float)* delaySample);

	// 読み込み位置の初期化
	readPos = 0;

	// 書き出し位置の初期化
	writePos = 0;
}

//=============================================================================
// クラスの終了処理
//=============================================================================
SAI_DELAY_APO::~SAI_DELAY_APO()
{
	// メモリ解放
	free(backupBuf);
}

//=============================================================================
// LockForProcessのオーバーライド
//=============================================================================
HRESULT SAI_DELAY_APO::LockForProcess
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
void SAI_DELAY_APO::OnSetParameters
(const void* pParameters, UINT32 ParameterByteSize)
{
	SAI_APO_DELAY *tmpParameters = ((SAI_APO_DELAY *)pParameters);

	if (tmpParameters->volume > 1.0f)
	{
		tmpParameters->volume = 1.0f;
	}
	else if (tmpParameters->volume < 0.0f)
	{
		tmpParameters->volume = 0.0f;
	}
}

//=============================================================================
// Processのオーバーライド
//=============================================================================
void SAI_DELAY_APO::Process
(UINT32 inputProcessParameterCount,
	const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters,
	UINT32 outputProcessParameterCount,
	XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters,
	BOOL isEnabled)
{
	if (isEnabled)
	{
		if (pInputProcessParameters->BufferFlags != XAPO_BUFFER_SILENT)
		{
			// 仮のパラメータ構造体 = 使用するパラメータのポインたー
			SAI_APO_DELAY *tmpParmeter = (SAI_APO_DELAY *)BeginProcess();

			// 入出力のバッファ
			float *inputBuf = (float *)pInputProcessParameters->pBuffer;
			float *outputBuf = (float *)pOutputProcessParameters->pBuffer;

			for (int i = 0; i < ((int)pInputProcessParameters->ValidFrameCount * tmpParmeter->wavFile.fmt.fmtChannel); i++)
			{
				// バッファの保存
				backupBuf[writePos] = inputBuf[i] + backupBuf[readPos] * tmpParmeter->volume;
				writePos++;
				if (writePos >= delaySample)	// オーバー防止
				{
					// 0番目に戻る
					writePos = 0;
				}

				// 結果
				readPos = (int)(writePos - delaySample * tmpParmeter->delayTime);
				if (readPos < 0)	// オーバー防止
				{
					// 0番目に戻る
					readPos += delaySample;
				}
				outputBuf[i] = (backupBuf[readPos] * tmpParmeter->volume) + (inputBuf[i] * (1 - tmpParmeter->volume));
			}

			// エンドプロセス
			EndProcess();
		}
	}
}
