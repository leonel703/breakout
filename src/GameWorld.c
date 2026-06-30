/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Base template for game
 * development in C using Raylib (https://www.raylib.com/).
 * * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GameWorld.h"
#include "ResourceManager.h"
#include "Jogador.h"
#include "Bolinha.h"

#include "raylib/raylib.h"

int estd = 0;
int pontos = 0;
int hp = 3;
int espac = 0;

void resolverColisaoBolinhaAlvos (Bolinha *b, Alvo *alvos, int quantidade);
void resolverColisaoBolinhaJogador (Bolinha *b, Jogador *j);
void resolverRedef (Bolinha *b);

GameWorld *createGameWorld(void) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    int largura = 150;
    int altura = 20;

    gw->jogador = (Jogador){
        .ret = {
            GetScreenWidth() / 2 - largura / 2,
            GetScreenHeight() - 3 * altura,
            largura,
            altura},
        .velocidadeBase = 400,
        .velocidadeAtual = 0,
        .cor = WHITE
    };

    gw->bolinha = (Bolinha) {
        .centro = {
            GetScreenWidth() / 2,
            550
        },
        .raio = 10,
        .vel = {0, 0},
        .cor = WHITE,
    };

    gw->lin = 10;
    gw->col = 6;
    gw->alvos = (Alvo*) malloc (sizeof(Alvo) * gw->lin * gw->col);

    int larguraAlvo = 85;
    int alturaAlvo = 20;
    int espaco = 10;
    int larguraTotal = larguraAlvo * gw->col + espaco * (gw->col - 1);
    int xIni = GetScreenWidth() / 2 - larguraTotal / 2;
    int yIni = 150;

    for (int i = 0; i < gw->lin; i++) {
        for (int j = 0; j < gw->col; j++) {
            int p = i* gw->col + j;
            gw->alvos[p] = (Alvo) {
                .ret = {
                    .x = xIni + j * (larguraAlvo + espaco),
                    .y = yIni + i * (alturaAlvo + espaco),
                    .width = larguraAlvo,
                    .height = alturaAlvo,
                },
                .cor = WHITE,
                .hp = 1
            };
        }
    }

    return gw;

}

void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

void updateGameWorld( GameWorld *gw, float delta ) {

    if (pontos >= 20 && gw->jogador.ret.width == 150) {
        gw->jogador.ret.width *= 1.3f;
    }

    if (pontos >= 40 && gw->jogador.ret.width == 150) {
        gw->jogador.ret.width *= 1.3f;
    }

    if (pontos >= 60) {
        gw->bolinha.centro.x = GetScreenWidth() / 2;
        gw->bolinha.centro.y = 550;
        if (IsKeyPressed(KEY_SPACE)) {
            estd = 0;
            pontos = 0;
            espac++;
            hp = 3;
            gw->jogador.ret.width = 150;
            gw->jogador.ret.x = GetScreenWidth() / 2 - 150 / 2;
            for (int i = 0; i < gw->lin * gw->col; i++) {
                gw->alvos[i].hp = 1;
            }
        }
        gw->bolinha.vel.x = 0;
        gw->bolinha.vel.y = 0;
        return;
    }

    if (hp == 0) {
        if (IsKeyPressed(KEY_SPACE)) {
            estd = 0;
            pontos = 0;
            hp = 3;
            espac++;
            gw->jogador.ret.width = 150;
            gw->jogador.ret.x = GetScreenWidth() / 2 - 150 / 2;
            for (int i = 0; i < gw->lin * gw->col; i++) {
                gw->alvos[i].hp = 1;
            }
        }
        gw->bolinha.vel.x = 0;
        gw->bolinha.vel.y = 0;
        gw->bolinha.centro.y = 550;
        gw->bolinha.centro.x = GetScreenWidth() / 2;
        return;
    }

    if (estd == 0) {
        if (IsKeyPressed(KEY_SPACE)) {
            estd = 1;
        }
        atualizarJogador (&gw->jogador, delta);
        resolverRedef (&gw->bolinha);
        entradaJogador (&gw->jogador);
    }   

    if (estd == 1) {
        gw->bolinha.centro.y = 550;
        gw->bolinha.centro.x = GetScreenWidth() / 2;
        gw->bolinha.vel.x = 300;
        gw->bolinha.vel.y = 300;
        estd = -1;
        atualizarJogador (&gw->jogador, delta);
        resolverRedef (&gw->bolinha);
        entradaJogador (&gw->jogador);
    }

    if (estd == -1) {

        atualizarJogador (&gw->jogador, delta);

        entradaJogador (&gw->jogador);

        atualizarBolinha (&gw->bolinha, delta);

        resolverColisaoBolinhaAlvos (&gw->bolinha, gw->alvos, gw->lin * gw->col);

        resolverColisaoBolinhaJogador (&gw->bolinha, &gw->jogador);

        resolverRedef (&gw->bolinha);
    }
}

void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    desenharJogador (&gw->jogador);
    desenharBolinha (&gw->bolinha);
    desenharAlvos (gw->alvos, gw->lin * gw->col);

    if (espac == 0 && estd == 0 && hp > 0) {
        DrawText("APERTE ESPACO PARA COMEÇAR", 65, 450, 28, WHITE);
    }
 
    if (hp > 0 && pontos < 60) {
        DrawText(TextFormat("PONTOS: %d", pontos), 20, 20, 22, WHITE);
        for (int i = 0; i < 3; i++) {
            Color cor = (i < hp) ? RED : DARKGRAY;
            DrawCircle(600 - 130 + i * 30, 25, 10, cor);
        }
    }
 
    if (pontos >= 60) {
        DrawText("VOCE VENCEU, PARABENS!", 87, 460, 30, GREEN);
        DrawText("APERTE ESPACO PARA REINICIAR", 110, 500, 22, LIGHTGRAY);
    }
 
    if (hp == 0) {
        DrawText("FIM DE JOGO! VOCÊ PERDEU.", 80, 460, 30, RED);
        DrawText("APERTE ESPACO PARA TENTAR DE NOVO", 63, 500, 22, LIGHTGRAY);
    }

    EndDrawing();

}

void resolverColisaoBolinhaAlvos (Bolinha *b, Alvo *alvos, int quantidade) {

    for ( int i = 0; i < quantidade; i++ ) {

        Alvo *alvo = &alvos[i];

        if ( alvo->hp > 0 && CheckCollisionCircleRec( b->centro, b->raio, alvo->ret ) ) {

            alvo->hp--;
            pontos++;

            if (pontos % 10 == 0) {
                b->vel.x *= 1.2;
                b->vel.y *= 1.2;
            }

            float centroAlvoX = alvo->ret.x + alvo->ret.width  / 2.0f;
            float centroAlvoY = alvo->ret.y + alvo->ret.height / 2.0f;

            float overlapX = ( b->raio + alvo->ret.width  / 2.0f ) - fabs( b->centro.x - centroAlvoX );
            float overlapY = ( b->raio + alvo->ret.height / 2.0f ) - fabs( b->centro.y - centroAlvoY );

            if ( overlapX < overlapY ) {

                if ( b->centro.x < centroAlvoX ) {
                    b->centro.x = alvo->ret.x - b->raio;
                } else {
                    b->centro.x = alvo->ret.x + alvo->ret.width + b->raio;
                }
                b->vel.x = -b->vel.x;

            } else {

                if ( b->centro.y < centroAlvoY ) {
                    b->centro.y = alvo->ret.y - b->raio;
                } else {
                    b->centro.y = alvo->ret.y + alvo->ret.height + b->raio;
                }
                b->vel.y = -b->vel.y;

            }

        }
    }

}

void resolverColisaoBolinhaJogador (Bolinha *b, Jogador *j) {

    if (CheckCollisionCircleRec(b->centro, b->raio, j->ret)) {

        float centroJogadorX = j->ret.x + j->ret.width  / 2.0f;
        float centroJogadorY = j->ret.y + j->ret.height / 2.0f;

        float overlapX = (b->raio + j->ret.width  / 2.0f) - fabs(b->centro.x - centroJogadorX);
        float overlapY = (b->raio + j->ret.height / 2.0f) - fabs(b->centro.y - centroJogadorY);

        if (overlapX < overlapY) {
            if (b->centro.x < centroJogadorX) {
                b->centro.x = j->ret.x - b->raio;
            } else {
                b->centro.x = j->ret.x + j->ret.width + b->raio;
            }
            b->vel.x = -b->vel.x;
        } else {
            if (b->centro.y < centroJogadorY) {
                b->centro.y = j->ret.y - b->raio;
            } else {
                b->centro.y = j->ret.y + j->ret.height + b->raio;
            }
            b->vel.y = -b->vel.y;
        }
    }
}

void resolverRedef (Bolinha *b) {

    if (b->centro.y + b->raio >= GetScreenHeight()) {
        b->centro.y = 550;
        b->centro.x = GetScreenWidth() / 2;
        b->vel.x = 0;
        b->vel.y = 0;
        estd = 0;
        hp--;
    }

}