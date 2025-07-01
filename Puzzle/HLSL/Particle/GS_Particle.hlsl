#include "Particle.hlsli"

//	4つの頂点を作成する
static const int vnum = 4;

//	各頂点のベースとする情報
static const float4 offset_array[vnum] =
{
    float4(-0.5f, 0.5f, 0.0f, 0.0f), // 左上
	float4(0.5f, 0.5f, 0.0f, 0.0f), // 右上
	float4(-0.5f, -0.5f, 0.0f, 0.0f), // 左下
	float4(0.5f, -0.5f, 0.0f, 0.0f), // 右下

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream<PS_INPUT> output
)
{
	//	4つの頂点を生成していく
    for (int i = 0; i < vnum; i++)
    {
		//	新しい頂点情報用の変数
        PS_INPUT element;

		//	ベースの座標を取得
        float4 res = offset_array[i];

		//	Texに入っている情報で表示サイズを決定する。
		//	その計算結果を、表示したい位置まで移動。
        element.Pos = input[0].Pos + mul(offset_array[i] * input[0].Tex.x, matWorld);

		//	viewとprojを計算して、スクリーン座標へ変換
        element.Pos = mul(element.Pos, matView);
        element.Pos = mul(element.Pos, matProj);

		//	色情報はそのまま使う
        element.Color = input[0].Color;
		//	C++からもらっているTex情報はUV座標ではないため、
		//	実際に使用可能な画像のUV情報をここで算出する
        element.Tex.x = offset_array[i].x + 0.5f;
        element.Tex.y = -offset_array[i].y + 0.5f;

		//	次のシェーダへ情報を渡すため、頂点情報を追加する
        output.Append(element);
    }
	
	//	生成した頂点データから、ポリゴン情報を適切に判断してもらう
    output.RestartStrip();
}