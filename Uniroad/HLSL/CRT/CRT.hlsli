//	C++から受け取るデータ
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; //    ワールド行列
    matrix matView; //    ビュー行列
    matrix matProj; //    プロジェクション行列
    float4 color; //   色
    float4 time; //    時間
 
};

//	このシェーダが受け取る引数の内容
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

//	ピクセルシェーダへ渡す引数の内容。returnする値となる
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}

float3 GetRainbow(float cptime)
{
    float hue = fmod(cptime * 0.25, 1.0); // 時間に基づいてHueを変化
    return HSVtoRGB(float3(hue, 1.0, 1.0)); // HSVからRGBに変換
}

