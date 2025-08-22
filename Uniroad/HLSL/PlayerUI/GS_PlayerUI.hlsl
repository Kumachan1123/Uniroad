#include "PlayerUI.hlsli"
static const unsigned int vnum = 4;
static const float4 offset_array[vnum] =
{
    float4(0.0f, 1.0f, 0.0f, 1.0f),
    float4(1.0f, 1.0f, 0.0f, 1.0f),
    float4(0.0f, 0.0f, 0.0f, 1.0f),
    float4(1.0f, 0.0f, 0.0f, 1.0f)
};

[maxvertexcount(vnum)]
void main(
    point PS_INPUT input[1],
    inout TriangleStream<PS_INPUT> output)
{
    for (uint i = 0; i < vnum; i++)
    {
		//ジオメトリ出力
        PS_INPUT element;
		
		//頂点に入れていた諸々を受け取る
        float2 window = windowSize.xy;
        float4 rect = input[0].Color;
        float2 pos = (rect.xy / window.xy) * 2.0f;
        float2 size = (rect.zw / window.xy);

        float2 scale = input[0].Pos.xy;
		//アンカータイプごとに補正する座標を決定する
        uint anc = input[0].Pos.z;
        float2 anchor;
        anchor.x = (size.x) * (anc % 3u) * scale.x;
        anchor.y = (size.y) * (anc / 3u) * scale.y;

		//頂点座標等から配置を決める
        element.Pos.xy = pos + scale * size * offset_array[i].xy * 2.0f - float2(1.0f, 1.0f);
        element.Pos.xy -= anchor;
        element.Pos.y *= -1;

		//XY平面なので zは0、wは1
        element.Pos.z = 0.0f;
        element.Pos.w = 1.0f;

		//サイズ変更等のUI化-----------------------------------
		//-----------------------------------------------------

		//色情報（貰った情報は使えないのでとりあえず白）
        element.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャのUV座標
        element.Tex = offset_array[i].xy;

        output.Append(element);
    }
    output.RestartStrip();

}