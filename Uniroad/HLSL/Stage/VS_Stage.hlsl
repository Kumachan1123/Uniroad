#include "Stage.hlsli"
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
	
	//	�n���ꂽ���W�ɁA�n���ꂽWorld�AView�AProj���N���X�ς��Ă����B
	//	�����AC++���Ŋ��Ɍv�Z���ꂽ���ʂ��g�������ꍇ�A�ȉ��͎ז��ƂȂ邽�ߒ��ӂ���
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    output.Pos = mul(output.Pos, matView);
    output.Pos = mul(output.Pos, matProj);
    output.Tex = input.Tex;
    return output;
}