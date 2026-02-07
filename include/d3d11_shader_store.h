#pragma once

#include <memory>
#include <unordered_map>

#include "d3d11_shader.h"

namespace airful_engine
{
	class D3D11ShaderStore
	{
	public:
		bool addVertexShader(const std::string& name, std::unique_ptr<D3D11VertexShader> shader);
		bool addPixelShader(const std::string& name, std::unique_ptr<D3D11PixelShader> shader);

		D3D11VertexShader* getVertexShader(const std::string& name) const;
		D3D11PixelShader* getPixelShader(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<D3D11VertexShader>> m_vertexShaders;
		std::unordered_map<std::string, std::unique_ptr<D3D11PixelShader>> m_pixelShaders;
	};
}
