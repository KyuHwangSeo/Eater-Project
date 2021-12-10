#pragma once

// ResourceType Enum Class..
enum class eResourceType
{
	CB,			// Constant Buffer Resource
	SS,			// Sampler State Resource
	SRV,		// Shader Resource View Resource
	UAV,		// Unordered Access View Resource
	DSS,		// Depth Stencil State Resource
	RS,			// Rasterizer State Resource
	BS,			// Blend State Resource
	DS,			// Depth Stencil Resource
	RT,			// RenderTarget Resource
	VP,			// ViewPort Resource
	BD,			// Buffer Data Resource
};
