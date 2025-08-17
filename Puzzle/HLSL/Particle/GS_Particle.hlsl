#include "Particle.hlsli"


StructuredBuffer<GPU_Particle> particles : register(t0); // SRV
//	4つの頂点を作成する
static const int vnum = 4;

//	各頂点のベースとする情報
static const float4 offset_array[vnum] =
{
    float4(-0.5f, 0.5f, 0.0f, 0.0f), // 左上
	float4(0.5f, 0.5f, 0.0f, 0.0f), // 右上
	float4(-0.5f, -0.5f, 0.0f, 0.0f), // 左下
	float4(0.5f, -0.5f, 0.0f, 0.0f), // 右下

};

[maxvertexcount(4)]
void main(point PS_INPUT input[1], inout TriangleStream<PS_INPUT> output)
{
    uint idx = input[0].ParticleIndex;
    float3 center = input[0].Pos.xyz;
    float2 scale = float2(1.0f, 1.0f); // 必要なら input[0].scale.xy 等に変更
    // 必要なら input[0] に scale 情報を渡しておく

    for (int i = 0; i < 4; i++)
    {
        PS_INPUT element = input[0];

        // オフセットをスケール
        float3 offset = float3(offset_array[i].xy * scale, 0);

        // 中心+オフセットをワールド変換
        float4 worldPos = float4(center + offset, 1.0f);
        worldPos = mul(worldPos, matWorld);

        // ビュー・射影変換
        element.Pos = mul(worldPos, matView);
        element.Pos = mul(element.Pos, matProj);

        // UV
        element.Tex.x = offset_array[i].x + 0.5f;
        element.Tex.y = -offset_array[i].y + 0.5f;

        output.Append(element);
    }
    output.RestartStrip();
}