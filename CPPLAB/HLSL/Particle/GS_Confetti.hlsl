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
	  // 時間を受け取れるようにグローバルから
    float speed = count.x; // C++から渡す時間を格納（colorsに流用）
    float time = count.y;
    for (int i = 0; i < vnum; i++)
    {
        PS_INPUT element;
        float4 res = offset_array[i];
        float randSeed = input[0].Tex.x + input[0].Tex.y * 21.0 + input[0].Pos.x * 0.05;
        float randX = Random(randSeed + 1.0);
        float randY = Random(randSeed + 2.0);
        float randZ = Random(randSeed + 3.0);

    // 回転方向をランダムに ±1 に決定
        float dirX = (randX > 0.5) ? 1.0 : -1.0;
        float dirY = (randY > 0.5) ? 1.0 : -1.0;
        float dirZ = (randZ > 0.5) ? 1.0 : -1.0;
    // ==== ひらひら回転 ====
        float angleX = frac(speed * input[0].Color.r * 2.5f) * 6.28318;
        float angleY = frac(speed * input[0].Color.g * 2.5f) * 6.28318;
        float angleZ = frac(speed * input[0].Tex.y * 2.5f) * 6.28318;
    // Z回転
        float2x2 rotZ = float2x2(cos(angleZ), -sin(angleZ), sin(angleZ), cos(angleZ));
        float2 rotated = mul(float2(res.x, res.y), rotZ);
        res.xy = rotated;

    // X回転
        res.y = res.y * cos(angleX) - res.z * sin(angleX);
        res.z = res.y * sin(angleX) + res.z * cos(angleX);

    // Y回転
        res.x = res.x * cos(angleY) + res.z * sin(angleY);
        res.z = -res.x * sin(angleY) + res.z * cos(angleY);

    // スケーリング
        res.xy *= input[0].Tex.x;

    // ==== 紙吹雪ごとに色ランダム ====
        float colorRandSeed = input[0].Tex.x + input[0].Tex.y * 37.0 + input[0].Pos.x * 0.1;
        float hue = frac(colorRandSeed) + frac(randSeed + time * 0.1); // 0～1の範囲で紙吹雪ごとに一意
        float3 rainbow = HSVtoRGB(float3(hue, 1.0, 1.0));
    // もしくは時間も混ぜて hue = frac(randSeed + time * 0.1);

    // ==== 座標変換 ====
        element.Pos = input[0].Pos + mul(res, matWorld);
        element.Pos = mul(element.Pos, matView);
        element.Pos = mul(element.Pos, matProj);

        element.Color = float4(rainbow, 1.0f);

        element.Tex.x = offset_array[i].x + 0.5f;
        element.Tex.y = -offset_array[i].y + 0.5f;

        output.Append(element);
    }
    output.RestartStrip();
}