cbuffer Transform : register(b0)
{
    float4x4 World;
    float4x4 WorldInverseTranspose;
    float4x4 WorldViewProj;
    float4 DiffuseColor;
}

cbuffer Light : register(b1)
{
    float3 LightDirection : packoffset(c0); // ライトの方向
    float Padding : packoffset(c0.w); // パディング
    float3 LightColor : packoffset(c1); // ライトの色
    float Padding2 : packoffset(c1.w); // パディング
};