#pragma once

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[2] = { nullptr, nullptr };
};