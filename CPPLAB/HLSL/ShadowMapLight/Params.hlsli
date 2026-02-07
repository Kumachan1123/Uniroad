/*
    共通で使用するパラメータ
*/

// ライト用定数バッファ
cbuffer LightParameters : register(b1)
{
    float4x4 lightViewProjection    : packoffset(c0);
    float4 lightPosition            : packoffset(c4);
    float3 lightDirirection         : packoffset(c5);
    float3 lightAmbient             : packoffset(c6);
};

// 頂点シェーダ入力用
struct VS_Input
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};

// ピクセルシェーダ入力用
struct PS_Input
{
    float2 TexCoord     : TEXCOORD0;
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float4 LightPosPS   : TEXCOORD3;
    float4 PositionPS   : SV_Position;
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
