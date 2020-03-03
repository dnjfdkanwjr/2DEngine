#pragma once
#include <chrono>
#include "Singleton.h"

namespace rp {

	class Timer : public Singleton<Timer>
	{

	private:
		std::chrono::time_point<std::chrono::system_clock>	curTime;
		std::chrono::time_point<std::chrono::system_clock>	prevTime;
		std::chrono::duration<float>						deltaTime;


	public:
		void Init();
		void Update();
		float GetDelta() const;
		
	};

}

