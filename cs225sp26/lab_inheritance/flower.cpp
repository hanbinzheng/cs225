/**
 * Copyright 2011 University of Illinois at Urbana-Champaign
 * CS 225 Lab 04 C++ Object Oriented Programming
 * Authors:
 *   Simon Krueger <skruege2@illinois.edu>
 *   Modified (FA17): Jordi Paris Ferrer <parisfe2@illinois.edu>
 */

#include "flower.h"

#include <cmath>
#include <cstdio>

#include "circle.h"
#include "color.h"
#include "rectangle.h"
#include "triangle.h"

const int STEM_HEIGHT = 32;
const int STEM_WIDTH = 2;
const HSLAPixel STEM_COLOR = color::GREEN;

const int LEAF_WIDTH = 16;
const int LEAF_HEIGHT = 16;
const HSLAPixel LEAF_COLOR = color::GREEN;

const int PISTIL_RADIUS = 12;
const HSLAPixel PISTIL_COLOR = color::YELLOW;

const int PETAL_RADIUS = 2;
const HSLAPixel PETAL_COLOR = color::RED;

Flower::Flower(const Vector2 &center)
    : Drawable() // stem(Vector2(center.x(), center.y() + STEM_HEIGHT / 2),
                 //                  STEM_COLOR, STEM_WIDTH, STEM_HEIGHT),
                 // pistil(Vector2(center.x(), center.y() - PISTIL_RADIUS / 2),
                 // PISTIL_COLOR,
                 //        PISTIL_RADIUS),
                 // leaf(
                 //     LEAF_COLOR,
                 //     Vector2(center.x() - STEM_WIDTH / 2 + 1,
                 //             center.y() + STEM_HEIGHT / 2),
                 //     Vector2(center.x() - STEM_WIDTH / 2 + 1 - LEAF_WIDTH,
                 //             center.y() + STEM_HEIGHT / 4 + STEM_HEIGHT / 2 -
                 //             LEAF_HEIGHT),
                 //     Vector2(center.x() - STEM_WIDTH / 2 + 1 - LEAF_WIDTH /
                 //     2,
                 //             center.y() + STEM_HEIGHT / 4 + STEM_HEIGHT / 2))
{
    // std::cout << "Address of leaf during initialization: " << &(this->leaf)
    //           << std::endl;

    const Vector2 stem_center(center.x(), center.y() + STEM_HEIGHT / 2);
    stem = new Rectangle(stem_center, STEM_COLOR, STEM_WIDTH, STEM_HEIGHT);

    pistil = new Circle(Vector2(center.x(), center.y() - PISTIL_RADIUS / 2),
                        PISTIL_COLOR, PISTIL_RADIUS);

    const Vector2 leaf_center(stem_center.x() - STEM_WIDTH / 2 + 1,
                              stem_center.y() + STEM_HEIGHT / 4);
    leaf = new Triangle(
        LEAF_COLOR, Vector2(leaf_center.x(), leaf_center.y() - STEM_HEIGHT / 4),
        Vector2(leaf_center.x() - LEAF_WIDTH, leaf_center.y() - LEAF_HEIGHT),
        Vector2(leaf_center.x() - LEAF_WIDTH / 2, leaf_center.y()));
    // leaf._set_center(my_leaf.center());

    // std::cout << "my: vertex 0: ( " << my_leaf.vertex(0).x() << " , "
    //           << my_leaf.vertex(0).y() << " ) " << std::endl;
    // std::cout << "my: vertex 1: ( " << my_leaf.vertex(1).x() << " , "
    //           << my_leaf.vertex(1).y() << " ) " << std::endl;
    // std::cout << "my: vertex 2: ( " << my_leaf.vertex(2).x() << " , "
    //           << my_leaf.vertex(2).y() << " ) " << std::endl;
    // std::cout << "my: center: ( " << my_leaf.center().x() << " , "
    //           << my_leaf.center().y() << " ) " << std::endl;

    // if (leaf.color() == my_leaf.color()) {
    //     std::cout << "Two colors are the same" << std::endl;
    // } else {
    //     std::cout << "Fucking Horrible Triangle Class! What a fucking stupid
    //     "
    //                  "assignment operator!!!!! "
    //               << std::endl;
    // }

    // // leaf.set_center(my_leaf.center());
    // for (int i = 0; i < 3; i++) {
    //     if (leaf.vertex(i).x() != leaf.vertex(i).x() ||
    //         leaf.vertex(i).y() != my_leaf.vertex(i).y())
    //         std::cout << "triangle different at vertex " << i << std::endl;
    // }
}

void Flower::drawPetals(PNG *canvas, const Vector2 &center, int x,
                        int y) const {
    Circle petal(center, PETAL_COLOR, PETAL_RADIUS);
    petal.set_center(Vector2(center.x() + x, center.y() + y));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() + x, center.y() - y));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() - x, center.y() + y));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() - x, center.y() - y));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() + y, center.y() + x));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() + y, center.y() - x));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() - y, center.y() + x));
    petal.draw(canvas);
    petal.set_center(Vector2(center.x() - y, center.y() - x));
    petal.draw(canvas);
}

void Flower::draw(PNG *canvas) const {
    stem->draw(canvas);
    pistil->draw(canvas);
    leaf->draw(canvas);

    // std::cout << "Address of leaf during drawing " << &(this->leaf) << std::endl;

    // std::cout << "real: vertex 0: ( " << leaf.vertex(0).x() << " , "
    //           << leaf.vertex(0).y() << " ) " << std::endl;
    // std::cout << "real: vertex 1: ( " << leaf.vertex(1).x() << " , "
    //           << leaf.vertex(1).y() << " ) " << std::endl;
    // std::cout << "real: vertex 2: ( " << leaf.vertex(2).x() << " , "
    //           << leaf.vertex(2).y() << " ) " << std::endl;
    // std::cout << "real: center: ( " << leaf.center().x() << " , "
    //           << leaf.center().y() << " ) " << std::endl;

    for (int x = 0; x < PISTIL_RADIUS; x++) {
        const double radius2 = PISTIL_RADIUS * PISTIL_RADIUS;
        const int y = static_cast<int>(sqrt(radius2 - (x * x)));
        drawPetals(canvas, pistil->center(), x, y);
    }
}

Flower::~Flower() {
    delete stem;
    stem = nullptr;
    delete pistil;
    pistil = nullptr;
    delete leaf;
    leaf = nullptr;
}
