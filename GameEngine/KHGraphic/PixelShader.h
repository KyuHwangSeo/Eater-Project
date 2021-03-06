#pragma once

/// <summary>
/// PixelShader Class
/// </summary>
/// 
/// - 한개의 PixelShader를 관리하는 클래스

class PixelShader : public ShaderBase
{
public:
	PixelShader(const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines);
	~PixelShader();

public:
	void LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines) override;
	void Update() override;
	void Release() override;

private:
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
};