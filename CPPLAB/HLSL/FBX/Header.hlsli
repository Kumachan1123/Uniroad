cbuffer Transform : register(b0)
{
    float4x4 World;
    float4x4 WorldInverseTranspose;
    float4x4 WorldViewProj;
    float4 DiffuseColor;
}