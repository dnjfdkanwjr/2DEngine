#pragma once
#include <string>
namespace rp {
	class Base
	{
	private:
		std::string name{};
		bool state{};


	public:
		Base(std::string&& name = "GameObject");
		virtual ~Base();

		void SetName(std::string&& name)noexcept;
		void SetName(std::string& name)noexcept;

		void SetState(bool state)noexcept;

		const std::string& GetName() const noexcept;
		bool GetState() const noexcept;
	};
}

