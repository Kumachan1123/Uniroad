#include "Particle.hlsli"


cbuffer ParticleSystemParams : register(b1)
{
    float3 globalForce;
    float deltaTime;
    // 必要なら他にも
};


// パーティクルバッファ
RWStructuredBuffer<GPU_Particle> particles : register(u0);
[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint idx = DTid.x;

    GPU_Particle p = particles[idx];

    // 寿命が0以下なら何もしない（死んでる粒子）
    if (p.life <= 0.0f)
        return;

    // 基本挙動（例：重力適用、位置更新等）
    p.velocity += globalForce * deltaTime;
    p.position += p.velocity * deltaTime;

    // 寿命減少
    p.life -= deltaTime;

    // タイプごと分岐も可能
    // if (p.type == 1) { ... }

    // ここで色やスケール、回転の補間も可（必要なら）

    // 書き戻し
    particles[idx] = p;
}