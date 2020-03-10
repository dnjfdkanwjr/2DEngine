#pragma once
#include <chrono>
#include "Singleton.h"

namespace rp {

	class RpTimer
	{

	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock>	curTime;
		static std::chrono::time_point<std::chrono::high_resolution_clock>	prevTime;
		static std::chrono::duration<float>									deltaTime;


	public:
		static void Init();
		static void Update();
		static float GetDelta();
	};

}

