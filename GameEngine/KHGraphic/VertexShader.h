#pragma once

/// <summary>
/// VetexShader Class
/// </summary>
/// 
/// - 한개의 VertexShader를 관리하는 클래스

class VertexShader : public ShaderBase
{
public:
	VertexShader(const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines);
	~VertexShader();

public:
	void LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines) override;
	void Update() override;
	void Release() override;

private:
	// Vertex Shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// VertexShader Input Layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
};