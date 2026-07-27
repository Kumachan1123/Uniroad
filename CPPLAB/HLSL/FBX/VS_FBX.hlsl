#include "Header.hlsli"

struct VS_Input
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct VS_Output
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

VS_Output main(VS_Input input)
{
    VS_Output output;

    /// 座標をクリップ座標へ変換
    output.Position =
        mul(
            float4(input.Position, 1.0f),
            WorldViewProj);

    /// 法線をワールド空間へ変換
    output.Normal =
        normalize(
            mul(
                float4(input.Normal, 0.0f),
                WorldInverseTranspose).xyz);

    /// UV
    output.TexCoord =
        input.TexCoord;

    return output;
}