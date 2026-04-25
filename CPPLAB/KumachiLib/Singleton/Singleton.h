#ifndef SINGLETON_H
#define SINGLETON_H
#include <mutex>
#include <memory>
template<typename T>
class Singleton
{
public:
	// インスタンスを取得する
	static T& Get()
	{
		static T instance;
		return instance;
	}

	// コピー・ムーブ禁止
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
protected:
	// コンストラクタとデストラクタを保護
	Singleton() {}
	~Singleton() {}
};
#endif // !SINGLETON_H
