#pragma once
#include "ResourceBase.h"

// DirectX 11 DepthStencilState Class
class DepthStencilState : public ResourceBase
{
public:
	DepthStencilState(ID3D11DepthStencilState** dss);
	~DepthStencilState();

public:
	void Reset();

public:
	ID3D11DepthStencilState* Get();
	ID3D11DepthStencilState** GetAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DSS;
};

// DirectX 11 RasterizerState Class
class RasterizerState : public ResourceBase
{
public:
	RasterizerState(ID3D11RasterizerState** rs);
	~RasterizerState();

public:
	void Reset();

public:
	ID3D11RasterizerState* Get();
	ID3D11RasterizerState** GetAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RS;
};

// DirectX 11 BlendState Class
class BlendState : public ResourceBase
{
public:
	BlendState(ID3D11BlendState** bs);
	~BlendState();

public:
	void Reset();

public:
	ID3D11BlendState* Get();
	ID3D11BlendState** GetAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BS;
};

// DirectX 11 BlendState Class
class SamplerState : public ResourceBase
{
public:
	SamplerState(ID3D11SamplerState** ss);
	~SamplerState();

public:
	void Reset();

public:
	ID3D11SamplerState* Get();
	ID3D11SamplerState** GetAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SS;
};