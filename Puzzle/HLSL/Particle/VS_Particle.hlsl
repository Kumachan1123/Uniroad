#include "Particle.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	//	ピクセルシェーダに渡す座標は、入力そのまま
	output.Pos = float4(input.Pos,1);

	//	色も指定値を使う
	output.Color = input.Color;
	//	UV座標もそのまま使う
	output.Tex = input.Tex;
	//	次のシェーダへ情報を渡す
	return output;
}