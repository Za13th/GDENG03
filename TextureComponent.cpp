#include "TextureComponent.h"

TextureComponent::TextureComponent(String name, GameObject* owner) :Component(name, ComponentType::Material, owner)
{
	this->texture = TextureManager::getInstance()->createTextureFromFile(L"Assets\\Textures\\default.jpg");
}

TextureComponent::~TextureComponent()
{

}

void TextureComponent::perform(float deltaTime)
{

}

Texture* TextureComponent::getTexture()
{
	return this->texture;
}

const wchar_t* TextureComponent::getTexturePath()
{
	return texPath;
}

void TextureComponent::changeTexture(const wchar_t* filePath)
{
	if (TextureManager::getInstance()->createTextureFromFile(filePath)) {
		this->texture = TextureManager::getInstance()->createTextureFromFile(filePath);
		this->texPath = filePath;
	}
}


