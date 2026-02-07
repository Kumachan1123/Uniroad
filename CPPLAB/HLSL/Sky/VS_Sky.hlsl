#include "Header.hlsli"
PS_Input main(VS_Input input)
{
    PS_Input output;
    // 頂点座標をそのまま出力
    output.Position = mul(input.Position, WorldViewProj);
    // 法線をそのまま出力
    output.NormalWS = input.Normal;
      // ワールド座標をそのまま出力
    output.PositionWS = mul(input.Position, World).xyz;
    // UV座標をそのまま出力
    output.TexCoord = input.TexCoord;
    return output;
}