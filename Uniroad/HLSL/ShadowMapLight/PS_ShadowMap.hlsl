/*
	深度値描画用ピクセルシェーダ
*/
#include "Common.hlsli"
#include "Params.hlsli"


// メイン関数
float4 main(PS_SM_Input input) : SV_Target0
{
    /* 手順２ */

    // Z値を出力する
    return input.Position.z;
    //return float4(input.Position.z, input.Position.z, input.Position.z, 1.0f);
}