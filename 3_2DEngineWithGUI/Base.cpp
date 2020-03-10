#include "Base.h"


rp::Base::Base(std::string&& name):
	name{std::move( name )},
	state{ true }
{
}

rp::Base::~Base()
{

}

void rp::Base::SetName(std::string&& name) noexcept
{
	this->name = std::move(name);
}

void rp::Base::SetName(std::string& name) noexcept
{
	this->name = name;
}

void rp::Base::SetState(bool state) noexcept
{
	this->state = state;
}

const std::string& rp::Base::GetName() const noexcept
{
	return name;
}

bool rp::Base::GetState() const noexcept
{
	return state;
}
