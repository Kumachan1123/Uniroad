#include "Counter.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;
    float w = width.x;
    float h = height.x;
    uv.x /= w;
    uv.y /= h;
    float count2 = floor(count.x);
    
    uv.x += (1.0f / w) * (count2 % w);
    uv.y += (1.0f / h) * (int) (count2 / w);
    
    float4 color = tex.Sample(samLinear, uv);
    
   
    // �A���t�@�l��0�̏ꍇ�̓s�N�Z�����\���ɂ���
    float alphaMask = step(0.0f, color.a); // �A���t�@�l��0�Ȃ�0�A����ȊO��1�ɂȂ�
    color *= alphaMask; // �A���t�@��0�̃s�N�Z�������S�ɖ�����
   // �A���t�@�l��0����1�͈̔͂ɐ������A�F�Ɋ|�����킹��
   
    
    return color;
}
