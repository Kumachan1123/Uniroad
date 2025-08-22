#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`���̐F���擾
    float4 output = tex.Sample(samLinear, input.Tex);

    // UV��[-1,1]�͈̔͂ɕϊ��i���S�� (0.5,0.5) �ɂ���j
    float2 uv = (input.Tex - 0.5f) * 2.0f;

    // ��]�̊�_
    float theta = atan2(uv.x, uv.y);

    // �p�x�� 0 ~ 1 �͈̔͂ɐ��K���i���v���j
    float normalizedTheta = (theta + 3.14159265359f) / (2.0f * 3.14159265359f);

    // ���߂̐i�s�x�ɉ����ăA���t�@��ύX�i���v���ɓ��߁j
    float smoothValue = renderRatio;
    output.a *= lerp(1.0f, 0.0f, smoothstep(smoothValue, smoothValue + 0.00000001f, normalizedTheta));

    // �Q�[�W�̐F��ύX
 // �F�̕�ԁi���^���Ȃ�A����ȊO�͐ԁj
    float fullGauge = saturate(renderRatio); // renderRatio��1�Ȃ�1.0�A����ȊO��0.0�ɋ߂Â�
    float4 fullColor = float4(0.0f, 0.5f, 1.0f, 1.0f); // �F
    float4 emptyColor = float4(1.5f, 0.0f, 0.0f, 1.0f); // �ԐF

    float4 gaugeColor = lerp(emptyColor, fullColor, fullGauge);
    float4 finalColor = float4(output * gaugeColor);
  
    return finalColor;
}