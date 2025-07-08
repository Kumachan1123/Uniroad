#pragma once
#include <cmath>
#include "KumachiLib/Math/KumachiLib.h"
namespace Easing {

	// サインイーズイン
	inline float EaseInSine(float t)
	{
		// tは0.0f～1.0fの範囲で指定
		return 1.0f - std::cos((t * DirectX::XM_PI) / 2.0f);
	}

	// サインイーズアウト
	inline float EaseOutSine(float t)
	{
		return std::sin((t * DirectX::XM_PI) / 2.0f);
	}

	// サインイーズインアウト
	inline float EaseInOutSine(float t)
	{
		return -(std::cos(DirectX::XM_PI * t) - 1.0f) / 2.0f;
	}

	// 二次イーズイン
	inline float EaseInQuad(float t)
	{
		return t * t;
	}

	// 二次イーズアウト
	inline float EaseOutQuad(float t)
	{
		return t * (2.0f - t);
	}

	// 二次イーズインアウト
	inline float EaseInOutQuad(float t)
	{
		return t < 0.5f
			? 2.0f * t * t
			: 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

	// 三次イーズイン
	inline float EaseInCubic(float t)
	{
		// tは0.0f～1.0fの範囲で指定
		return t * t * t;
	}

	// 三次イーズアウト
	inline float EaseOutCubic(float t)
	{
		float p = t - 1.0f;
		return p * p * p + 1.0f;
	}

	// 三次イーズインアウト
	inline float EaseInOutCubic(float t)
	{
		return t < 0.5f
			? 4.0f * t * t * t
			: 1.0f + 4.0f * (t - 1.0f) * (t - 1.0f) * (t - 1.0f);
	}

	// 四次イーズイン
	inline float EaseInQuart(float t)
	{
		return t * t * t * t;
	}

	// 四次イーズアウト
	inline float EaseOutQuart(float t)
	{
		float p = t - 1.0f;
		return 1.0f - p * p * p * p;
	}

	// 四次イーズインアウト
	inline float EaseInOutQuart(float t)
	{
		return t < 0.5f
			? 8.0f * t * t * t * t
			: 1.0f - 8.0f * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f);
	}

	// 五次イーズイン
	inline float EaseInQuint(float t)
	{
		// tは0.0f～1.0fの範囲で指定
		return t * t * t * t * t;
	}

	// 五次イーズアウト
	inline float EaseOutQuint(float t)
	{
		float p = t - 1.0f;
		return p * p * p * p * p + 1.0f;
	}

	// 五次イーズインアウト
	inline float EaseInOutQuint(float t)
	{
		return t < 0.5f
			? 16.0f * t * t * t * t * t
			: 1.0f + 16.0f * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f);
	}

	// 指数イーズイン
	inline float EaseInExpo(float t)
	{
		return (t == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
	}

	// 指数イーズアウト
	inline float EaseOutExpo(float t)
	{
		return (t == 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
	}

	// 指数イーズインアウト
	inline float EaseInOutExpo(float t)
	{
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		return t < 0.5f
			? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
			: (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

	// 円形イーズイン
	inline float EaseInCirc(float t)
	{
		// tは0.0f～1.0fの範囲で指定
		return 1.0f - std::sqrt(1.0f - t * t);
	}

	// 円形イーズアウト
	inline float EaseOutCirc(float t)
	{
		float p = t - 1.0f;
		return std::sqrt(1.0f - p * p);
	}

	// 円形イーズインアウト
	inline float EaseInOutCirc(float t)
	{
		if (t < 0.5f) return (1.0f - std::sqrt(1.0f - 4.0f * t * t)) / 2.0f;
		else
		{
			float p = 2.0f * t - 2.0f;
			return (std::sqrt(1.0f - p * p) + 1.0f) / 2.0f;
		}
	}

	// バックイーズイン
	inline float EaseInBack(float t)
	{
		// sはオーバーシュート量（一般的に1.70158f）
		const float s = 1.70158f;
		return t * t * ((s + 1.0f) * t - s);
	}

	// バックイーズアウト
	inline float EaseOutBack(float t)
	{
		const float s = 1.70158f;
		float p = t - 1.0f;
		return p * p * ((s + 1.0f) * p + s) + 1.0f;
	}

	// バックイーズインアウト
	inline float EaseInOutBack(float t)
	{
		const float s = 1.70158f * 1.525f;
		if (t < 0.5f) return (t * 2.0f) * (t * 2.0f) * ((s + 1.0f) * (t * 2.0f) - s) / 2.0f;
		else
		{
			float p = t * 2.0f - 2.0f;
			return (p * p * ((s + 1.0f) * p + s) + 2.0f) / 2.0f;
		}
	}

	// エラスティックイーズイン
	inline float EaseInElastic(float t)
	{
		// tは0.0f～1.0fの範囲で指定
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		const float c4 = (2.0f * DirectX::XM_PI) / 3.0f;
		return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
	}

	// エラスティックイーズアウト
	inline float EaseOutElastic(float t)
	{
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		const float c4 = (2.0f * DirectX::XM_PI) / 3.0f;
		return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
	}

	// エラスティックイーズインアウト
	inline float EaseInOutElastic(float t)
	{
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		const float c5 = (2.0f * DirectX::XM_PI) / 4.5f;
		if (t < 0.5f) return -0.5f * std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5);
		else return std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5) * 0.5f + 1.0f;
	}

	// バウンスイーズアウト（内部関数）
	inline float BounceOut(float t)
	{
		if (t < 1.0f / 2.75f)
		{
			return 7.5625f * t * t;
		}
		else if (t < 2.0f / 2.75f)
		{
			t -= 1.5f / 2.75f;
			return 7.5625f * t * t + 0.75f;
		}
		else if (t < 2.5f / 2.75f)
		{
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return 7.5625f * t * t + 0.984375f;
		}
	}

	// バウンスイーズイン
	inline float EaseInBounce(float t)
	{
		// tは0.0f～1.0fの範囲で指定
		return 1.0f - BounceOut(1.0f - t);
	}

	// バウンスイーズアウト
	inline float EaseOutBounce(float t)
	{
		return BounceOut(t);
	}

	// バウンスイーズインアウト
	inline float EaseInOutBounce(float t)
	{
		if (t < 0.5f) return (1.0f - BounceOut(1.0f - 2.0f * t)) * 0.5f;
		else return BounceOut(2.0f * t - 1.0f) * 0.5f + 0.5f;
	}
	// スムーズステップ
	inline float Smoothstep(float t)
	{
		// tは0.0f～1.0fの範囲
		return t * t * (3.0f - 2.0f * t);
	}

	// スムーズステップ（Smootherstep）
	inline float Smootherstep(float t)
	{
		return t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);
	}

	// ステップ関数
   // thresholdで指定した値を境に0または1を返す
	inline float Step(float t, float threshold = 0.5f)
	{
		// tは0.0f～1.0fの範囲
		return t < threshold ? 0.0f : 1.0f;
	}

	// 任意のべき乗イージング（イーズイン）
	inline float EaseInPower(float t, float n)
	{
		// nは次数（例:2=二次, 3=三次）
		return std::pow(t, n);
	}

	// 任意のべき乗イージング（イーズアウト）
	inline float EaseOutPower(float t, float n)
	{
		return 1.0f - std::pow(1.0f - t, n);
	}

	// 任意のべき乗イージング（イーズインアウト）
	inline float EaseInOutPower(float t, float n)
	{
		if (t < 0.5f) return std::pow(2.0f * t, n) / 2.0f;
		else return 1.0f - std::pow(-2.0f * t + 2.0f, n) / 2.0f;
	}

	// ロジスティック関数（シグモイドイージング）
	// k値を大きくすると傾きが急になる
	inline float Logistic(float t, float k = 10.0f)
	{
		// k: 典型的には10～20程度
		return 1.0f / (1.0f + std::exp(-k * (t - 0.5f)));
	}

	// カスタムイージング（ユーザー指定の2次ベジェ曲線）
	// p0=始点(0,0), p1, p2=制御点, p3=終点(1,1)
	// p1x,p1y,p2x,p2yは0～1の範囲で指定
	inline float CustomBezier(float t, float p1x, float p1y, float p2x, float p2y)
	{
		// 1次元ベジェ補間でx(t)→tを満たすt'を求めてy(t')を返す
		// ここでは簡易的にニュートン法で近似
		auto Bezier = [](float t, float a1, float a2)
			{
				float invT = 1.0f - t;
				return 3.0f * invT * invT * t * a1 +
					3.0f * invT * t * t * a2 +
					t * t * t;
			};
		float x = t;
		float guessT = t;
		for (int i = 0; i < 5; ++i)
		{
			float currentX = Bezier(guessT, p1x, p2x);
			float dx = currentX - x;
			if (std::abs(dx) < 1e-5f) break;
			// 導関数
			float dBezier = 3.0f * (1.0f - guessT) * (1.0f - guessT) * p1x
				+ 6.0f * (1.0f - guessT) * guessT * (p2x - p1x)
				+ 3.0f * guessT * guessT * (1.0f - p2x);
			if (dBezier == 0.0f) break;
			guessT -= dx / dBezier;
			if (guessT < 0.0f) guessT = 0.0f;
			if (guessT > 1.0f) guessT = 1.0f;
		}
		return Bezier(guessT, p1y, p2y);
	}

	// ランダム/ジッターイージング（値にランダムノイズを加える）
	// 振幅amplitudeを指定
	inline float RandomJitter(float t, float amplitude = 0.05f)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-amplitude, amplitude);
		return Clamp(t + dist(gen), 0.0f, 1.0f);
	}

	// 任意イージング関数の逆再生（リバース）
	// easingFuncは t->float の関数/ラムダ
	template<typename Func>
	inline float Reverse(Func easingFunc, float t)
	{
		return easingFunc(1.0f - t);
	}

	// 任意イージング関数の往復（リフレクト）
	// 前半は通常、後半は逆再生
	template<typename Func>
	inline float Reflect(Func easingFunc, float t)
	{
		if (t < 0.5f) return easingFunc(t * 2.0f) * 0.5f;


		else return (1.0f - easingFunc((1.0f - t) * 2.0f)) * 0.5f + 0.5f;
	}
}