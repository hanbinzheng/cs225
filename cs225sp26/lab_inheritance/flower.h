/**
 * Copyright 2011 University of Illinois at Urbana-Champaign
 * CS 225 Lab 04 C++ Object Oriented Programming
 * Authors:
 *   Simon Krueger <skruege2@illinois.edu>
 *   Modified (FA17): Jordi Paris Ferrer <parisfe2@illinois.edu>
 */

#ifndef FLOWER_H_
#define FLOWER_H_

#include "circle.h"
#include "cs225/HSLAPixel.h"
#include "rectangle.h"
#include "shape.h"
#include "triangle.h"
#include "vector2.h"

static const Vector2 fucking_initialization_list;
static const HSLAPixel a_color;

class Flower : public Drawable {
private:
  // Rectangle stem = Rectangle(fucking_initialization_list, a_color, 0, 0);
  // Circle pistil = Circle(fucking_initialization_list, a_color, 1);
  // Triangle leaf = Triangle(a_color, Vector2(), Vector2(1, 1), Vector2(3, 3));
  Shape *stem;
  Shape *pistil;
  Shape *leaf;

  void drawPetals(PNG *canvas, const Vector2 &center, int x, int y) const;

public:
  Flower(const Vector2 &center);
  ~Flower();
  void draw(PNG *canvas) const;
};

#endif // FLOWER_H_
