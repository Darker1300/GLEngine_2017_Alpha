#pragma once

#include <string>

class RenderTarget;

class Texture
{
public:
	friend class RenderTarget;

	Texture();
	Texture(const std::string& _path, const bool _flipY = true);
	~Texture();

	Texture(const Texture& _other) = delete;
	Texture(Texture&& _other);

	Texture& operator=(const Texture& _other) = delete;

#pragma region Getters
	unsigned int GetID() const { return m_id; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	int GetFormat() const { return m_format; }
	const unsigned char* const GetData() const { return m_data; }
#pragma endregion Getters

	void LoadTexture(const std::string& _path, const bool _flipY = true);
	/// <summary>	'internalFormat': eg: GL_RED, GL_RG, GL_RGB, GL_RGBA
	///	<para>		'format': eg: GL_DEPTH_COMPONENT, GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_STENCIL_INDEX, GL_DEPTH_STENCIL
	///</para>		'type' eg: GL_UNSIGNED_BYTE </summary>
	void CreateTexture(const int _width, const int _height, int _internalFormat, unsigned int _format, unsigned int _type);

private:
	unsigned int m_id;
	int m_width,
		m_height,
		m_format;

	unsigned char* m_data;
	bool m_flipY;
};
