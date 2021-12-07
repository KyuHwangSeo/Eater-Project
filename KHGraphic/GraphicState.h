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

class ViewPort : public ResourceBase
{
public:
	ViewPort(float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height);
	~ViewPort();

public:
	void OnResize(int width, int height);

public:
	D3D11_VIEWPORT* Get();

private:
	D3D11_VIEWPORT* m_ViewPort;

	float m_OffsetX;
	float m_OffsetY;
	float m_SizeX;
	float m_SizeY;
};