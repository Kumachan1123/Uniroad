/*
    CMO用: 環境マッピング(公式EnvironmentMapEffect風) + Shadow用データも維持
*/
#include "Material.hlsli"

PS_Input main(VS_Input input)
{
    PS_Input output;

    // Position
    output.PositionPS = mul(input.Position, WorldViewProj);
    output.PositionWS = mul(input.Position, World);

    // Normal (world)
    output.NormalWS = normalize(mul(input.Normal, WorldInverseTranspose));

    // UV
    output.TexCoord = input.TexCoord;

    // Light projection (影用に残す)
    output.LightPosPS = mul(output.PositionWS, lightViewProjection);

    // 仮タンジェント（本当はメッシュから来るべき）
    float3 Tws = mul(float3(1, 0, 0), (float3x3) World);
    output.TangentWS = float4(normalize(Tws), 1.0f);

    return output;
}