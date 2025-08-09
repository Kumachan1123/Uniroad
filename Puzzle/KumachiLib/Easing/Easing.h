#pragma once
#include <cmath>
#include "KumachiLib/Math/KumachiLib.h"
namespace Easing {

	// �T�C���C�[�Y�C��
	inline float EaseInSine(float t)
	{
		// t��0.0f�`1.0f�͈̔͂Ŏw��
		return 1.0f - std::cos((t * DirectX::XM_PI) / 2.0f);
	}

	// �T�C���C�[�Y�A�E�g
	inline float EaseOutSine(float t)
	{
		return std::sin((t * DirectX::XM_PI) / 2.0f);
	}

	// �T�C���C�[�Y�C���A�E�g
	inline float EaseInOutSine(float t)
	{
		return -(std::cos(DirectX::XM_PI * t) - 1.0f) / 2.0f;
	}

	// �񎟃C�[�Y�C��
	inline float EaseInQuad(float t)
	{
		return t * t;
	}

	// �񎟃C�[�Y�A�E�g
	inline float EaseOutQuad(float t)
	{
		return t * (2.0f - t);
	}

	// �񎟃C�[�Y�C���A�E�g
	inline float EaseInOutQuad(float t)
	{
		return t < 0.5f
			? 2.0f * t * t
			: 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

	// �O���C�[�Y�C��
	inline float EaseInCubic(float t)
	{
		// t��0.0f�`1.0f�͈̔͂Ŏw��
		return t * t * t;
	}

	// �O���C�[�Y�A�E�g
	inline float EaseOutCubic(float t)
	{
		float p = t - 1.0f;
		return p * p * p + 1.0f;
	}

	// �O���C�[�Y�C���A�E�g
	inline float EaseInOutCubic(float t)
	{
		return t < 0.5f
			? 4.0f * t * t * t
			: 1.0f + 4.0f * (t - 1.0f) * (t - 1.0f) * (t - 1.0f);
	}

	// �l���C�[�Y�C��
	inline float EaseInQuart(float t)
	{
		return t * t * t * t;
	}

	// �l���C�[�Y�A�E�g
	inline float EaseOutQuart(float t)
	{
		float p = t - 1.0f;
		return 1.0f - p * p * p * p;
	}

	// �l���C�[�Y�C���A�E�g
	inline float EaseInOutQuart(float t)
	{
		return t < 0.5f
			? 8.0f * t * t * t * t
			: 1.0f - 8.0f * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f);
	}

	// �܎��C�[�Y�C��
	inline float EaseInQuint(float t)
	{
		// t��0.0f�`1.0f�͈̔͂Ŏw��
		return t * t * t * t * t;
	}

	// �܎��C�[�Y�A�E�g
	inline float EaseOutQuint(float t)
	{
		float p = t - 1.0f;
		return p * p * p * p * p + 1.0f;
	}

	// �܎��C�[�Y�C���A�E�g
	inline float EaseInOutQuint(float t)
	{
		return t < 0.5f
			? 16.0f * t * t * t * t * t
			: 1.0f + 16.0f * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f);
	}

	// �w���C�[�Y�C��
	inline float EaseInExpo(float t)
	{
		return (t == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
	}

	// �w���C�[�Y�A�E�g
	inline float EaseOutExpo(float t)
	{
		return (t == 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
	}

	// �w���C�[�Y�C���A�E�g
	inline float EaseInOutExpo(float t)
	{
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		return t < 0.5f
			? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
			: (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

	// �~�`�C�[�Y�C��
	inline float EaseInCirc(float t)
	{
		// t��0.0f�`1.0f�͈̔͂Ŏw��
		return 1.0f - std::sqrt(1.0f - t * t);
	}

	// �~�`�C�[�Y�A�E�g
	inline float EaseOutCirc(float t)
	{
		float p = t - 1.0f;
		return std::sqrt(1.0f - p * p);
	}

	// �~�`�C�[�Y�C���A�E�g
	inline float EaseInOutCirc(float t)
	{
		if (t < 0.5f) return (1.0f - std::sqrt(1.0f - 4.0f * t * t)) / 2.0f;
		else
		{
			float p = 2.0f * t - 2.0f;
			return (std::sqrt(1.0f - p * p) + 1.0f) / 2.0f;
		}
	}

	// �o�b�N�C�[�Y�C��
	inline float EaseInBack(float t)
	{
		// s�̓I�[�o�[�V���[�g�ʁi��ʓI��1.70158f�j
		const float s = 1.70158f;
		return t * t * ((s + 1.0f) * t - s);
	}

	// �o�b�N�C�[�Y�A�E�g
	inline float EaseOutBack(float t)
	{
		const float s = 1.70158f;
		float p = t - 1.0f;
		return p * p * ((s + 1.0f) * p + s) + 1.0f;
	}

	// �o�b�N�C�[�Y�C���A�E�g
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

	// �G���X�e�B�b�N�C�[�Y�C��
	inline float EaseInElastic(float t)
	{
		// t��0.0f�`1.0f�͈̔͂Ŏw��
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		const float c4 = (2.0f * DirectX::XM_PI) / 3.0f;
		return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
	}

	// �G���X�e�B�b�N�C�[�Y�A�E�g
	inline float EaseOutElastic(float t)
	{
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		const float c4 = (2.0f * DirectX::XM_PI) / 3.0f;
		return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
	}

	// �G���X�e�B�b�N�C�[�Y�C���A�E�g
	inline float EaseInOutElastic(float t)
	{
		if (t == 0.0f) return 0.0f;
		if (t == 1.0f) return 1.0f;
		const float c5 = (2.0f * DirectX::XM_PI) / 4.5f;
		if (t < 0.5f) return -0.5f * std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5);
		else return std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5) * 0.5f + 1.0f;
	}

	// �o�E���X�C�[�Y�A�E�g�i�����֐��j
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

	// �o�E���X�C�[�Y�C��
	inline float EaseInBounce(float t)
	{
		// t��0.0f�`1.0f�͈̔͂Ŏw��
		return 1.0f - BounceOut(1.0f - t);
	}

	// �o�E���X�C�[�Y�A�E�g
	inline float EaseOutBounce(float t)
	{
		return BounceOut(t);
	}

	// �o�E���X�C�[�Y�C���A�E�g
	inline float EaseInOutBounce(float t)
	{
		if (t < 0.5f) return (1.0f - BounceOut(1.0f - 2.0f * t)) * 0.5f;
		else return BounceOut(2.0f * t - 1.0f) * 0.5f + 0.5f;
	}
	// �X���[�Y�X�e�b�v
	inline float Smoothstep(float t)
	{
		// t��0.0f�`1.0f�͈̔�
		return t * t * (3.0f - 2.0f * t);
	}

	// �X���[�Y�X�e�b�v�iSmootherstep�j
	inline float Smootherstep(float t)
	{
		return t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);
	}

	// �X�e�b�v�֐�
   // threshold�Ŏw�肵���l������0�܂���1��Ԃ�
	inline float Step(float t, float threshold = 0.5f)
	{
		// t��0.0f�`1.0f�͈̔�
		return t < threshold ? 0.0f : 1.0f;
	}

	// �C�ӂׂ̂���C�[�W���O�i�C�[�Y�C���j
	inline float EaseInPower(float t, float n)
	{
		// n�͎����i��:2=��, 3=�O���j
		return std::pow(t, n);
	}

	// �C�ӂׂ̂���C�[�W���O�i�C�[�Y�A�E�g�j
	inline float EaseOutPower(float t, float n)
	{
		return 1.0f - std::pow(1.0f - t, n);
	}

	// �C�ӂׂ̂���C�[�W���O�i�C�[�Y�C���A�E�g�j
	inline float EaseInOutPower(float t, float n)
	{
		if (t < 0.5f) return std::pow(2.0f * t, n) / 2.0f;
		else return 1.0f - std::pow(-2.0f * t + 2.0f, n) / 2.0f;
	}

	// ���W�X�e�B�b�N�֐��i�V�O���C�h�C�[�W���O�j
	// k�l��傫������ƌX�����}�ɂȂ�
	inline float Logistic(float t, float k = 10.0f)
	{
		// k: �T�^�I�ɂ�10�`20���x
		return 1.0f / (1.0f + std::exp(-k * (t - 0.5f)));
	}

	// �J�X�^���C�[�W���O�i���[�U�[�w���2���x�W�F�Ȑ��j
	// p0=�n�_(0,0), p1, p2=����_, p3=�I�_(1,1)
	// p1x,p1y,p2x,p2y��0�`1�͈̔͂Ŏw��
	inline float CustomBezier(float t, float p1x, float p1y, float p2x, float p2y)
	{
		// 1�����x�W�F��Ԃ�x(t)��t�𖞂���t'�����߂�y(t')��Ԃ�
		// �����ł͊ȈՓI�Ƀj���[�g���@�ŋߎ�
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
			// ���֐�
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

	// �����_��/�W�b�^�[�C�[�W���O�i�l�Ƀ����_���m�C�Y��������j
	// �U��amplitude���w��
	inline float RandomJitter(float t, float amplitude = 0.05f)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-amplitude, amplitude);
		return Clamp(t + dist(gen), 0.0f, 1.0f);
	}

	// �C�ӃC�[�W���O�֐��̋t�Đ��i���o�[�X�j
	// easingFunc�� t->float �̊֐�/�����_
	template<typename Func>
	inline float Reverse(Func easingFunc, float t)
	{
		return easingFunc(1.0f - t);
	}

	// �C�ӃC�[�W���O�֐��̉����i���t���N�g�j
	// �O���͒ʏ�A�㔼�͋t�Đ�
	template<typename Func>
	inline float Reflect(Func easingFunc, float t)
	{
		if (t < 0.5f) return easingFunc(t * 2.0f) * 0.5f;


		else return (1.0f - easingFunc((1.0f - t) * 2.0f)) * 0.5f + 0.5f;
	}
}