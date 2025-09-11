#include "Common.hlsli"
#include "Params.hlsli"


  // カメラからの距離を計算（EyePositionはカメラのワールド位置）(カメラ距離で対策）
float3 CalculateDistanceToCamera(VS_Input input)
{
// 頂点座標をワールド空間に変換
    float3 positionWS = mul(input.Position, World).xyz;
    float3 normalWS = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    
    // カメラからの距離を計算（EyePositionはカメラのワールド位置）(カメラ距離で対策）
    float distanceToCamera = length(positionWS - EyePosition);

    // FOV補正スケーリング（視野角の半分を使う）
    float fovScale = tan(FOVTheta * 0.5f); // ←FOVはラジアン
    
    // 線の太さ = 距離 × tan(FOV / 2)
    float outlineScale = saturate(distanceToCamera) * fovScale;

    // 距離に応じてスケーリング（距離が近いほど縮小）
    // 近距離でもアウトラインが消えないように clamp する
    float scale = saturate(distanceToCamera + 1.5f); // 適当な係数（調整可能）

    // スケールしたアウトライン幅を使って拡大
    //positionWS += normalWS * (OutlineThickness * outlineScale);
    
    return normalWS * (OutlineThickness * outlineScale + OutlineThickness);
}

float3 NonCalculateDistanceToCamera(VS_Input input)
{ // 頂点座標をワールド空間に変換
    float3 positionWS = mul(input.Position, World).xyz;
    float3 normalWS = normalize(mul(input.Normal, WorldInverseTranspose).xyz);

    return normalWS * OutlineThickness;
}


PS_Input main(VS_Input input)
{
    PS_Input output;

    // 頂点座標をワールド空間に変換
    float3 positionWS = mul(input.Position, World).xyz;
    float3 normalWS = normalize(mul(input.Normal, WorldInverseTranspose).xyz);

    //  カメラからの距離を計算（EyePositionはカメラのワールド位置）(カメラ距離で対策）
    positionWS += CalculateDistanceToCamera(input);
    

    // ワールド空間からクリップ空間への変換
    output.Position = mul(float4(positionWS, 1.0), ViewProj);

    // 法線を裏返す（カリング設定で背面を描画するため）
    output.NormalWS = -normalWS;

    // ワールド座標をそのまま出力
    output.PositionWS = positionWS;
    
    // UV座標をそのまま出力
    output.TexCoord = input.TexCoord;

    return output;
}