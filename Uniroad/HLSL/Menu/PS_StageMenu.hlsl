#include "Menu.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}
float4 oldTV(float2 inUV)
{
    float2 uv = inUV;
	//	中心をずらす
    uv = uv - 0.5f; // uv - float2 (0.5f, 0.5f);

    float vignette = length(uv);
    uv /= 1 - vignette * 0.2f;
	//	中心を戻す
    float2 texUV = uv + 0.5f;
    
    
    // sin波で0.025fから0の間を往復する値を生成
    float warp = cos(time.x) * 0.25f;
    warp = lerp(warp, 0.000001f, step(1.5f, time.x)); // stepを使用して条件を切り替え
    warp = 0.00001f;
    texUV.x += (random(floor(texUV.y * 1500) + time.x * 0.001f) - 0.5f) * warp;
    texUV.y += (random(floor(texUV.x * 1500) + time.x * 0.001f) - 0.5f) * warp;

    
    float4 base = tex.Sample(samLinear, texUV);;
       // --- UV座標の調整（色収差用） ---
    float2 uvR = texUV + float2(0.00325, 0.0) * length(texUV - 0.0125); // 赤チャンネルを少し右にずらす
    float2 uvB = texUV - float2(0.00325, 0.0) * length(texUV - 0.0125); // 青チャンネルを少し左にずらす

    float3 col;

    col.r = tex.Sample(samLinear, uvR).r; // 赤チャンネルをずらしてサンプル
    col.g = tex.Sample(samLinear, texUV).g; // 緑チャンネルはそのまま
    col.b = tex.Sample(samLinear, uvB).b; // 青チャンネルをずらしてサンプル
 
    col = lerp(col, float3(
                random(uv + float2(123 + time.x, 0)),
                random(uv + float2(123 + time.x, 1)),
                random(uv + float2(123 + time.x, 2))),
                step(random(floor(texUV.y * 500) + time.x * 0.00025f), 0.00025f));
    

        
// --- スキャンラインの効果計算（不規則） ---
    float scanlineRandom = random(float2(floor(texUV.y * 5.0), time.x * 10.0));
    float scanline = sin(texUV.y * 100.0 + scanlineRandom * 100.0) * 0.0025f;
    col *= 1.0 - scanline;

// --- ノイズの追加 ---
    float noise = (random(texUV * time.x * 50.0f) - 0.05f) * 0.0000000006f;
    col += noise;

    base = float4(col, base.a);
  
    base = lerp(base, float4(0, 0, 0, 1), step(0, max(abs(uv.y) - 0.5f, abs(uv.x) - 0.5f)));
    return base;
}

float4 main(PS_INPUT input) : SV_TARGET
{
  
    // 結果を返す
    return oldTV(input.Tex);
}

