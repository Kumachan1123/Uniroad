#include "Header.hlsli"
PS_Input main(VS_Input input)
{
    PS_Input output;
    // ���_���W�����̂܂܏o��
    output.Position = mul(input.Position, WorldViewProj);
    // �@�������̂܂܏o��
    output.NormalWS = input.Normal;
      // ���[���h���W�����̂܂܏o��
    output.PositionWS = mul(input.Position, World).xyz;
    // UV���W�����̂܂܏o��
    output.TexCoord = input.TexCoord;
    return output;
}