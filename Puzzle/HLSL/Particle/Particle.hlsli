//	共通で必要となる構造体を、ヘッダファイルとして抜き出し
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
    float4 colors;
    float4 count;
    float4 height;
    float4 width;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD0;
    uint ParticleIndex : TEXCOORD1; // 追加
};

float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}

float3 GetRainbow(float time)
{
    float hue = fmod(time * 0.5, 1.0); // 時間に基づいてHueを変化
    return HSVtoRGB(float3(hue, 1.0, 1.0)); // HSVからRGBに変換
}

// 構造体はC++と揃えて下さい
struct GPU_Particle
{
    float3 position;
    float life;
    float3 velocity;
    float type;
    float4 color;
    float3 rotation;
    float3 scale;
};