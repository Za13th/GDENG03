#pragma once
#include "Component.h"
#include "Texture.h"
#include "TextureManager.h"
class TextureComponent : public Component
{
public:
	TextureComponent(String name, GameObject* owner);
	~TextureComponent();
	void perform(float deltaTime) override;
	Texture* getTexture();
	void changeTexture(const wchar_t* filePath);


private:
	float mass = 1000; //in kilograms
	Texture* texture = nullptr;
};

