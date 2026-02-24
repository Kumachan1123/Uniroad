/*
    複数のHLSLファイルで共通で使用するパラメータ
*/

// DirectXTKで標準的に定義されているパラメータ
// テクスチャ
Texture2D<float4> Texture : register(t0);
// ノーマルマップ
Texture2D<float4> NormalMap : register(t1);
// 発光マップ
Texture2D<float4> EmissiveMap : register(t2);
// アンビエントオクルージョンマップ
Texture2D<float4> AOMap : register(t3);
// ラフネスマップ
Texture2D<float4> RoughnessMap : register(t4);

sampler Sampler : register(s0);
// Material.hlsli、またはシェーダ内で追加
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

// 頂点シェーダ入力用
struct VS_Input
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Color : COLOR;
};

// ピクセルシェーダ入力用
struct PS_Input
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 LightPosPS : TEXCOORD3;
    float4 TangentWS : TEXCOORD4; // if available
    float4 PositionPS : SV_Position;
    float4 Diffuse : COLOR;
};

// シャドウマップの頂点シェーダ入力用
struct VS_SM_Input
{
    float4 Position : SV_Position;
};

// シャドウマップのピクセルシェーダ入力用
struct PS_SM_Input
{
    float4 Position : SV_Position;
};
