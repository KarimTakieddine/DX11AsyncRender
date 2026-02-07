#include "d3d11_shader_store.h"

namespace airful_engine
{
	bool D3D11ShaderStore::addVertexShader(const std::string& name, std::unique_ptr<D3D11VertexShader> shader)
	{
		const auto result = m_vertexShaders.emplace(name, std::move(shader));

		return result.second;
	}

	bool D3D11ShaderStore::addPixelShader(const std::string& name, std::unique_ptr<D3D11PixelShader> shader)
	{
		const auto result = m_pixelShaders.emplace(name, std::move(shader));

		return result.second;
	}

	D3D11VertexShader* D3D11ShaderStore::getVertexShader(const std::string& name) const
	{
		const auto it = m_vertexShaders.find(name);
		if (it == m_vertexShaders.cend())
			return nullptr;

		return it->second.get();
	}

	D3D11PixelShader* D3D11ShaderStore::getPixelShader(const std::string& name) const
	{
		const auto it = m_pixelShaders.find(name);
		if (it == m_pixelShaders.cend())
			return nullptr;

		return it->second.get();
	}
}