#pragma once
#include<SFML/Graphics.hpp>

class ShapeWithTex
{
private:

public:
	ShapeWithTex() { };

	ShapeWithTex(const std::string TexturePath)
	{
		ShapeTex.loadFromFile(TexturePath);
		Shape.setTexture(&ShapeTex);
	}
	ShapeWithTex(const std::string TexturePath, const sf::Vector2f ShapeSize)
	{
		ShapeTex.loadFromFile(TexturePath);
		Shape.setTexture(&ShapeTex);
		Shape.setSize(ShapeSize);
	}
	ShapeWithTex(const std::string TexturePath, const sf::Vector2f ShapeSize, const float Shape_pos_x, const float Shape_pos_y)
	{
		ShapeTex.loadFromFile(TexturePath);
		Shape.setTexture(&ShapeTex);
		Shape.setSize(ShapeSize);
		Shape.setPosition(Shape_pos_x, Shape_pos_y);
	}

	void setTexture(const std::string TexturePath)
	{
		ShapeTex.loadFromFile(TexturePath);
		Shape.setTexture(&ShapeTex);
	}

	void setTextureAndSize(const std::string TexturePath, const sf::Vector2f ShapeSize)
	{
		ShapeTex.loadFromFile(TexturePath);
		Shape.setTexture(&ShapeTex);
		Shape.setSize(ShapeSize);
	}

	~ShapeWithTex() { };

	sf::RectangleShape Shape;
	sf::Texture ShapeTex;
};

