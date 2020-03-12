#include "Renderer.h"
#include "RpGameObject.h"

rp::RendererWrapper::RendererWrapper(RpGameObject* gameObject, std::string&& name):
	Component(gameObject, std::move(name))
{

}
rp::RendererWrapper::~RendererWrapper()
{

}

void rp::RendererWrapper::Start() noexcept
{
	
}

void rp::RendererWrapper::Update() noexcept
{

}

rp::IRenderer::IRenderer()
{

}

rp::IRenderer::~IRenderer()
{

}

void rp::IRenderer::Draw()
{

}

