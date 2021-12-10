#pragma once
#include "ResourceBase.h"

// DirectX 11 ShaderResourceView Class
class ShaderResourceView : public ResourceBase
{
public:
	ShaderResourceView(ID3D11ShaderResourceView* srv);
	~ShaderResourceView();

public:
	void Reset() override;

public:
	ID3D11ShaderResourceView* Get();
	ID3D11ShaderResourceView** GetAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

// DirectX 11 UnorderedAccessView Class
class UnorderedAccessView : public ResourceBase
{
public:
	UnorderedAccessView(ID3D11UnorderedAccessView* uav);
	~UnorderedAccessView();

public:
	void Reset() override;

public:
	ID3D11UnorderedAccessView* Get();
	ID3D11UnorderedAccessView** GetAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
};

