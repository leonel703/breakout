#pragma once

#include "raylib/raylib.h"

typedef struct Bolinha {
    Vector2 centro;
    Vector2 vel;
    float raio;
    Color cor;
} Bolinha;

void atualizarBolinha (Bolinha *bolinha, float delta);
void desenharBolinha (Bolinha *bolinha); 