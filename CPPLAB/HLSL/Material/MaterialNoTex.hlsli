/*
    複数のHLSLファイルで共通で使用するパラメータ
    テクスチャなしバージョン（環境マップのみ使用）
*/

// 環境マップのみ定義（メタリック反射用）
TextureCube EnvMap : register(t10);
SamplerState EnvSampler : register(s10);


cbuffer Parameters : register(b0)
{
    float4 DiffuseColor : packoffset(c0);
    float3 EmissiveColor : packoffset(c1);
    float3 SpecularColor : packoffset(c2);
    float SpecularPower : packoffset(c2.w);

    float3 LightDirection[3] : packoffset(c3);
    float3 LightDiffuseColor[3] : packoffset(c6);
    float3 LightSpecularColor[3] : packoffset(c9);

    float3 EyePosition : packoffset(c12);

    float3 FogColor : packoffset(c13);
    float4 FogVector : packoffset(c14);

    float4x4 World : packoffset(c15);
    float3x3 WorldInverseTranspose : packoffset(c19);
    float4x4 WorldViewProj : packoffset(c22);
};

/*
    共通で使用するパラメータ
*/

// ライト用定数バッファ
cbuffer LightParameters : register(b1)
{
    float4x4 lightViewProjection : packoffset(c0);
    float4 lightPosition : packoffset(c4);
    float3 lightDirirection : packoffset(c5);
    float3 lightAmbient : packoffset(c6);
};

// 頂点入力用
struct VS_Input
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
};

// ピクセルライティング用構造体
struct PS_Input
{
    float4 Position : SV_Position;
    float3 NormalWS : NORMAL;
    float3 PositionWS : TEXCOORD0;
};