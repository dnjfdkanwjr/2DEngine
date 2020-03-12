#pragma once

using uint = unsigned int;
using uchar = unsigned char;

namespace global {
	extern uint gRtvDescriptorSize;
	extern uint gCbvSrvDescriptorSize;
}

namespace konstant {
	constexpr uint maxFrameCount = 2;
	constexpr uint kBackBufferCount = 2;
	constexpr uint kNumberOfAllocators = 3;
}

