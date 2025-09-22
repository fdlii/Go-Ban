#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Checker
{
public:
	int color;
	Sprite sprite;
	Checker(Sprite, int, int, int, float);
};