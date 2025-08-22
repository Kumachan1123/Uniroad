/*
	ライトwithシャドウマップ用頂点シェーダ
*/
#include "Common.hlsli"
#include "Params.hlsli"


PS_Input main(VS_Input input)
{
	/* 手順８ */

    PS_Input output;

    // 頂点の位置を投影空間へ
    output.PositionPS = mul(input.Position, WorldViewProj);
    
    // 頂点の位置をワールド空間へ
    output.PositionWS = mul(input.Position, World);
    
    // 法線ベクトルをワールド空間へ（拡大縮小の影響を打ち消すため逆転置行列をかける）
    output.NormalWS = mul(input.Normal, WorldInverseTranspose);
    
    // テクスチャのUV座標
    output.TexCoord = input.TexCoord;
    
    // 頂点の位置をライトの投影空間へ
    output.LightPosPS = mul(output.PositionWS, lightViewProjection);
 
    return output;
}
