#pragma once
#include "ShaderManagerBase.h"

class D3D11Graphic;

class ShaderManager : public IShaderManager
{
public:
	ShaderManager(D3D11Graphic* graphic);
	~ShaderManager();

public:
	friend class OriginalShader;

public:
	void Initialize() override;
	void Release() override;

public:
	void AddSampler(Hash_Code hash_code, ID3D11SamplerState** sampler) override;

public:
	ShaderBase* LoadShader(eShaderType shaderType, std::string shaderName);
	OriginalShader GetShader(std::string shaderName) override;

private:
	void CreateShader();

	VertexShader* GetVertexShader(std::string shaderName);
	PixelShader* GetPixelShader(std::string shaderName);
	ComputeShader* GetComputeShader(std::string shaderName);

private:
	// Shader List
	std::unordered_map<std::string, ShaderBase*> m_ShaderList;
};
