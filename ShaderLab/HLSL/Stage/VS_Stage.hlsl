#include "Stage.hlsli"
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
	
	//	渡された座標に、渡されたWorld、View、Projをクロス積していく。
	//	もし、C++側で既に計算された結果を使いたい場合、以下は邪魔となるため注意する
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    output.Pos = mul(output.Pos, matView);
    output.Pos = mul(output.Pos, matProj);
    output.Tex = input.Tex;
    return output;
}