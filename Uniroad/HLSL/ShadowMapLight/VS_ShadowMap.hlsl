/*
	深度値描画用頂点シェーダ
*/
#include "Common.hlsli"
#include "Params.hlsli"


// メイン関数
PS_SM_Input main(VS_SM_Input input)
{
    /* 手順１ */

    PS_SM_Input output;
    
    output.Position = mul(input.Position, WorldViewProj);
    
    return output;
}