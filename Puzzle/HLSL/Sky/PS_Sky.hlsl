#include "Header.hlsli"
float4 main(PS_Input input) : SV_Target0
{
    // ���ʍ��W�i�@���x�N�g���j�̐��K��
    float3 n = normalize(input.NormalWS);
    // ���ʁ�UV
    // �o�x�A�ܓx��UV��
    float u = atan2(n.z, n.x) / (2.0 * 3.14159265) + 0.5;
    float v = 1.0 - (acos(n.y) / 3.14159265);
    // �e�N�X�`���T���v�����O
    float4 color = Texture.Sample(Sampler, float2(u, v));
    return color;
}