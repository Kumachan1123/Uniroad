/*
	ライトwithシャドウマップ用ピクセルシェーダ
*/
#include "Common.hlsli"
#include "Params.hlsli"

// シャドウマップテクスチャ
Texture2D ShadowMapTexture : register(t1);

// シャドウマップ用テクスチャサンプラー
SamplerComparisonState ShadowMapSampler : register(s1);

// マッハバンド対策
static const float SHADOW_EPSILON = 0.0001f;


float4 main(PS_Input input) : SV_TARGET0
{
    // 法線を正規化する
    float3 normal = normalize(input.NormalWS);

    // ----------------------------------------------------------- //
    // ディフューズ
    // ----------------------------------------------------------- //
 
    // ライトを照らす方向を計算する
    float3 lightDir = normalize(lightDirirection);
    
    // ライトへ向かうベクトル
    float3 toLight = (-lightDir);

    // 光の強さを内積から算出する
    float intensity1 = max(dot(normal, toLight), 0.0f);

    // 正規化デバイス座標にする
    input.LightPosPS.xyz /= input.LightPosPS.w;

    // 現在のピクセルがシャドウマップ上でどの位置にあたるか計算する
    float2 uv = input.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;

    // 通常描画の深度値とライト空間の深度値を比較して影になるか調べる
    // ShadowMapTextureの該当する場所の深度値と現在のピクセルの深度値を比較して、影になるか調べる
    // shadow　0.0f：影がある、1.0f：影がない
    float shadow = ShadowMapTexture.SampleCmpLevelZero(ShadowMapSampler, uv, input.LightPosPS.z - SHADOW_EPSILON).x;

    // ライトと影による明るさを計算する
    float3 lightAmount = lerp(lightAmbient, float3(1.5f, 1.5f, 1.5f), shadow);
    lightAmount *= intensity1;

    // 拡散反射色を求める 
    float3 diffuse = DiffuseColor.rgb * lightAmount;

        
    // ----------------------------------------------------------- //
    // スペキュラ
    // ----------------------------------------------------------- //
    

    
    // 視線ベクトル
    float3 toEye = normalize(EyePosition - input.PositionWS.xyz);

    // ハーフベクトル＝カメラ座標＋ライト座標
    float3 halfVector = normalize(toEye + toLight);

    // 鏡面反射の影響割合を内積を使い計算する
    float intensity2 = max(dot(normal, halfVector), 0.0f);

    // スペキュラパワーを指数として使いハイライトのかかり具合を調整する
    float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
    
    // 影の影響を考慮する
    specular *= shadow;
 
    
    // ----------------------------------------------------------- //
    // 最終カラー
    // ----------------------------------------------------------- //

    // テクスチャカラーをサンプルする
    float4 textureColor = Texture.Sample(Sampler, input.TexCoord);
    
    // テクスチャの色に拡散反射と鏡面反射を反映する
    float3 finalColor = textureColor.rgb * diffuse + specular + lightAmbient;
    
    return float4(finalColor, 1.0f);
}
