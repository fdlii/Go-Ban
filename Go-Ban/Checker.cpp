#include "Checker.h"

Checker::Checker(Sprite spr, int clr, int x, int y, float scale) {
	sprite = spr;
	color = clr;
	sprite.setPosition(x, y);
	sprite.setScale(scale, scale);
}