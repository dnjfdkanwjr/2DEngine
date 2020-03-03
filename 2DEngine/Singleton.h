#pragma once
#include <memory>
namespace rp {

	template<class T>
	class Singleton
	{
	private:
		static std::shared_ptr<T> instance;
	public:
		static std::shared_ptr<T> GetInstance();
	};

	template<class T>
	std::shared_ptr<T> Singleton<T>::GetInstance()
	{
		if (!Singleton<T>::instance) {
			Singleton<T>::instance = std::make_shared<T>();
		}
		return Singleton<T>::instance;
	}

	template<class T>
	std::shared_ptr<T> Singleton<T>::instance = nullptr;
}

