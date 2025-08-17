#include "Particle.hlsli"

PS_INPUT main(VS_INPUT input, uint vertexID : SV_VertexID)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = float4(input.Pos, 1);
    output.Color = input.Color;
    output.Tex = input.Tex;
    // インデックスをUVのz成分に詰めて渡す例（構造体にフィールドを追加しても良い）
    output.ParticleIndex = vertexID; // ここで詰める
    return output;
}