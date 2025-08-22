#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	�w�肳�ꂽ�摜�̕\��
    float4 output = tex.Sample(samLinear, input.Tex);

	// �摜�̓��ߓx��ύX
    float smoothValue = renderRatio;
    output.a *= lerp(1.0f, 0.0f, smoothstep(smoothValue, smoothValue + 0.00000001f, input.Tex.x));
     // HP�̐F
    // �� �� ���F (0.5f�`1.0f)
    float greenToYellow = saturate((renderRatio - 0.5f) * 2.0f);

    // ���F �� ��F (0.25f�`0.5f)
    float yellowToOrange = saturate((renderRatio - 0.25f) * 4.0f) * (1.0f - greenToYellow);

    // ��F �� �ԐF (0.0f�`0.25f)
    float orangeToRed = saturate(renderRatio * 4.0f) * (1.0f - yellowToOrange - greenToYellow);

    // �e�F������
    float4 HPcolor =
        greenToYellow * float4(0.0f, 1.0f, 0.0f, 1.0f) + // �� �� ���F
        yellowToOrange * float4(1.0f, 1.0f, 0.0f, 1.0f) + // ���F �� ��F
        orangeToRed * float4(1.0f, 0.5f, 0.0f, 1.0f) + // ��F �� �ԐF
        (1.0f - greenToYellow - yellowToOrange - orangeToRed) * float4(1.0f, 0.0f, 0.0f, 1.0f); // ��
    
    return output * HPcolor;
}