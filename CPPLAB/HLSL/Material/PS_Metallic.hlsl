#include "Material.hlsli"

// 上でグローバルに定義されている定数をそのまま使います。
float Metallic = 1.0;
float Roughness = 0.1;
float3 F0_default = float3(0.04, 0.04, 0.04);
float emissiveStrength = .5;
static const float kAmbientIBL = 0.15f; // 簡易環境拡散の強さ（微調整可）
static const float kMaxEnvMipLevel = 8.0f; // 環境マップの最大Mipレベル（256x256なら8）

// Schlick の近似フレネル
float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX Normal Distribution Function
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;

    return nom / max(denom, 0.0000001);
}

// Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / max(denom, 0.0000001);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float4 main(PS_Input input) : SV_TARGET
{
  
    float4 texColor = Texture.Sample(Sampler, input.TexCoord);
    float3 albedo = texColor.rgb;

    // ノーマルマップからタンジェント空間の法線を取得
    float3 normalMap = NormalMap.Sample(Sampler, input.TexCoord).rgb;
    float3 tangentSpaceNormal = normalize(normalMap * 2.0 - 1.0);

    // TBN行列を構築してワールド空間に変換
    float3 N = normalize(input.NormalWS);
    float3 T = normalize(input.TangentWS.xyz);
    float3 B = cross(N, T) * input.TangentWS.w; // タンジェントのw成分は符号（1 or -1）
    float3x3 TBN = float3x3(T, B, N);

    float3 normal = normalize(mul(tangentSpaceNormal, TBN));

    // 発光マップをロード
    float4 emissiveTex = EmissiveMap.Sample(Sampler, input.TexCoord);

    // アンビエントオクルージョンマップをロード（Rチャンネルに格納されることが多い）
    float ao = AOMap.Sample(Sampler, input.TexCoord).r;

    // ラフネスマップをロード（Rチャンネルに格納されることが多い）
    float roughness = RoughnessMap.Sample(Sampler, input.TexCoord).r;
    // マップ値が0に近いとき、最低限の値を保証（完全にゼロだと鏡面反射が極端になるため）
    roughness = max(roughness, 0.01);

    float3 viewDir = normalize(EyePosition - input.PositionWS.xyz);
    float3 reflectDir = reflect(-viewDir, normal);

    // ここでキューブ左右反転（XZ符号反転）
    float3 reflectDirForEnv = float3(-reflectDir.x, reflectDir.y, -reflectDir.z);

    // Roughnessに応じてMipレベルを計算（粗いほど高いMip = ぼやけた反射）
    float envMipLevel = roughness * kMaxEnvMipLevel;
    float3 envColor = EnvMap.SampleLevel(EnvSampler, reflectDirForEnv, envMipLevel).rgb;

    // 簡易ディレクショナルライト
    float3 lightDir = normalize(-lightDirirection); // ライトの方向
    float3 halfVec = normalize(viewDir + lightDir);

    // Metallic workflow: F0をMetallicに基づいて計算
    float3 F0 = lerp(F0_default, albedo, Metallic);

    // 拡散反射と鏡面反射の計算（Cook-Torrance BRDF）
    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0);

    // Specular BRDF
    float D = DistributionGGX(normal, halfVec, roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    float3 F = FresnelSchlick(max(dot(halfVec, viewDir), 0.0), F0);

    float3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL;
    float3 specular = numerator / max(denominator, 0.001);

    // エネルギー保存則
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - Metallic);

    // 拡散反射（Lambert）
    float3 diffuse = kD * albedo / 3.14159265359;

    // 最終的なライティング
    float3 directLight = (diffuse + specular) * NdotL * float3(1.0, 1.0, 1.0); // ライトカラーは白

    // 環境光（アンビエント）- AOで遮蔽を考慮
    float3 ambient = lightAmbient * albedo * kAmbientIBL * ao;

    // 環境マップからの反射（金属の場合は強く、非金属は弱く）
    float3 envReflection = envColor * F;
    
    // 発光マップの色に発光強度を乗算
    float3 emissive = emissiveTex.rgb * EmissiveColor * emissiveStrength;


    // 最終カラー = ディレクショナルライト + アンビエント + 環境反射 + 発光
    float3 color = texColor.rgb + emissive + ambient + envReflection;
    
    return float4(color, .05);
}

