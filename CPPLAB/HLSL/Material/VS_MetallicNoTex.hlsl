/*
    CMO用: 環境マッピング(公式EnvironmentMapEffect風) + Shadow用データも維持
    テクスチャなしバージョン
*/
#include "MaterialNoTex.hlsli"

PS_Input main(VS_Input input)
{
    PS_Input output;

    // 頂点座標をそのまま出力
    output.Position = mul(input.Position, WorldViewProj);
    

    // 法線をそのまま出力
    output.NormalWS = mul(input.Normal, WorldInverseTranspose);
    
      // ワールド座標をそのまま出力
    output.PositionWS = mul(input.Position, World).xyz;

  
    return output;
}