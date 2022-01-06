#pragma once
#include "ResourceBase.h"

// DirectX 11 ResourceView Class
class ResourceView : public ResourceBase
{
public:
	ResourceView(RESOURCE_TYPE type);
	~ResourceView() = default;

public:
	void OnResize(UINT byteSize);

protected:
	UINT m_NumElements;
};

// DirectX 11 DepthStencilView Class
class DepthStencilView : public ResourceView
{
public:
	DepthStencilView(ID3D11DepthStencilView* dsv);
	~DepthStencilView();

public:
	void Reset() override;
	void Release() override;

public:
	ID3D11DepthStencilView* Get();
	ID3D11DepthStencilView** GetAddress();
	ID3D11DepthStencilView** ReleaseGetAddress();

	void GetDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* desc);

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};

// DirectX 11 RenderTargetView Class
class RenderTargetView : public ResourceView
{
public:
	RenderTargetView(ID3D11RenderTargetView* rtv);
	~RenderTargetView();

public:
	void Reset() override;
	void Release() override;

public:
	ID3D11RenderTargetView* Get();
	ID3D11RenderTargetView** GetAddress();
	ID3D11RenderTargetView** ReleaseGetAddress();

	void GetDesc(D3D11_RENDER_TARGET_VIEW_DESC* desc);

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
};

// DirectX 11 ShaderResourceView Class
class ShaderResourceView : public ResourceView
{
public:
	ShaderResourceView(ID3D11ShaderResourceView* srv);
	~ShaderResourceView();

public:
	void Reset() override;
	void Release() override;

public:
	ID3D11ShaderResourceView* Get();
	ID3D11ShaderResourceView** GetAddress();
	ID3D11ShaderResourceView** ReleaseGetAddress();

	void GetDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

// DirectX 11 UnorderedAccessView Class
class UnorderedAccessView : public ResourceView
{
public:
	UnorderedAccessView(ID3D11UnorderedAccessView* uav);
	~UnorderedAccessView();

public:
	void Reset() override;
	void Release() override;

public:
	ID3D11UnorderedAccessView* Get();
	ID3D11UnorderedAccessView** GetAddress();
	ID3D11UnorderedAccessView** ReleaseGetAddress();

	void GetDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC* desc);

private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
};

