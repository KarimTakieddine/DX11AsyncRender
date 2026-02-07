#include <cstdlib>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "d3d11_shader.h"
#include "platform.h"

namespace airful_engine
{
	bool D3D11Shader::load(ID3D11Device* device, const std::string& file)
	{
		if (loadSource(file) != S_OK)
			return false;

		return create(device);
	}

	Microsoft::WRL::ComPtr<ID3DBlob> D3D11Shader::getSource() const
	{
		return m_source;
	}

	HRESULT D3D11Shader::loadSource(const std::string& file)
	{
		unsigned long long sourceSize = platformGetFileSize(file.c_str());
		if (sourceSize == 0)
			return D3D11_ERROR_FILE_NOT_FOUND;

		const HRESULT createStatus = D3DCreateBlob(sourceSize, m_source.GetAddressOf());
		if (createStatus != S_OK)
			return createStatus;

		// Includes NUL terminating character:

		const DWORD directoryPathSize = GetCurrentDirectory(0, nullptr);
		if (directoryPathSize == 0)
			return D3D11_ERROR_FILE_NOT_FOUND;

		char* currentDirectory = new char[directoryPathSize];
		GetCurrentDirectory(directoryPathSize, currentDirectory);

		const std::string fullPath = std::string(currentDirectory) + file;

		delete[] (currentDirectory);

		std::vector<wchar_t> fileWide(fullPath.length(), '\0');
		std::mbstowcs(fileWide.data(), file.c_str(), file.length());

		return D3DReadFileToBlob(fileWide.data(), m_source.GetAddressOf());
	}

	Microsoft::WRL::ComPtr<ID3D11VertexShader> D3D11VertexShader::getHandle() const
	{
		return m_handle;
	}

	bool D3D11VertexShader::create(ID3D11Device* device)
	{
		return device->CreateVertexShader(
			m_source->GetBufferPointer(),
			m_source->GetBufferSize(),
			nullptr,
			&m_handle
		) == S_OK;
	}

	Microsoft::WRL::ComPtr<ID3D11PixelShader> D3D11PixelShader::getHandle() const
	{
		return m_handle;
	}

	bool D3D11PixelShader::create(ID3D11Device* device)
	{
		return device->CreatePixelShader(
			m_source->GetBufferPointer(),
			m_source->GetBufferSize(),
			nullptr,
			&m_handle
		) == S_OK;
	}
}