#pragma once
#include <DirectXMath.h>
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

namespace cb {
	struct CameraBuffer {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	};


}