#pragma once
#include <chrono>
#include "Singleton.h"

namespace rp {

	class Timer : public Singleton<Timer>
	{

	private:
		static std::chrono::time_point<std::chrono::system_clock>	curTime;
		static std::chrono::time_point<std::chrono::system_clock>	prevTime;
		static std::chrono::duration<float>							deltaTime;


	public:
		void Init();
		void Update();
		float GetDelta() const;
		
	};

}

