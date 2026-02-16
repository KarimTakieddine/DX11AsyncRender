#pragma once

#include <d3dcommon.h>
#include <wrl.h>

#include <string>

struct ID3D11Device;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace dx11_async_render
{
	class D3D11Shader
	{
	public:
		virtual ~D3D11Shader() = default;

		bool load(ID3D11Device* device, const std::string& file);

		Microsoft::WRL::ComPtr<ID3DBlob> getSource() const;

	protected:
		virtual bool create(ID3D11Device* device) = 0;

		Microsoft::WRL::ComPtr<ID3DBlob> m_source{ nullptr };

	private:
		HRESULT loadSource(const std::string& file);
	};

	class D3D11VertexShader : public D3D11Shader
	{
	public:
		D3D11VertexShader() = default;
		~D3D11VertexShader() final override = default;

		D3D11VertexShader(const D3D11VertexShader&) = delete;
		D3D11VertexShader& operator=(const D3D11VertexShader&) = delete;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> getHandle() const;

	protected:
		bool create(ID3D11Device* device) final override;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_handle{ nullptr };
	};

	class D3D11PixelShader : public D3D11Shader
	{
	public:
		D3D11PixelShader() = default;
		~D3D11PixelShader() final override = default;

		D3D11PixelShader(const D3D11PixelShader&) = delete;
		D3D11PixelShader& operator=(const D3D11PixelShader&) = delete;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> getHandle() const;

	protected:
		bool create(ID3D11Device* device) final override;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_handle { nullptr };
	};
}
