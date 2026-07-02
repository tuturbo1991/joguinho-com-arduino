#include <TVout.h>
#include <fontALL.h>

struct game {
    uint8_t screen_width;
    uint8_t screen_height;
    int score;
    int max_score;
    TVout screen;
};
typedef struct game Game;

struct player {
    float x;
    float y;
    float dir;
    float vel;
    float size;
};
typedef struct player Player;

struct spike {
    uint8_t num;
    uint8_t gap;
    uint8_t begin;
    uint8_t end;
};
typedef struct spike Spike;

struct box {
    float x;
    float y;
    float width;
    float height;
    float vel;
};
typedef struct box Box;

Game jogo;
Player boneco;
Spike top;
Spike botton;
Box caixa;

void setup() {
    // Inicializar corretamente o gerador de números aleatórios
    randomSeed(analogRead(0));

    // Inicializando comunicação serial
    Serial.begin(115200);

    // Inicializando estrutura do jogo
    jogo.screen_width = 128;
    jogo.screen_height = 96;
    jogo.score = 0;
    jogo.max_score = 0;
    jogo.screen.begin(NTSC, jogo.screen_width, jogo.screen_height);

    // Inicializando o boneco
    boneco.x = 4.0;
    boneco.y = 20.0;
    boneco.dir = 0.0;
    boneco.vel = 0.6;
    boneco.size = 6.0;

    // Inicializando os espetos
    top.num = 30;
    top.gap = 4;
    top.begin = 12;
    top.end = 15;

    botton.num = 30;
    botton.gap = 4;
    botton.begin = 90;
    botton.end = 93;

    // Inicializando a caixa
    caixa.x = 150.0;
    caixa.y = 32.0;
    caixa.width = 10.0;
    caixa.height = 20.0;
    caixa.vel = 0.8;
}

float box_newy() {
    return float(random(top.end + 1, botton.begin - caixa.height - 1));
}

void box_update() {
    caixa.x -= caixa.vel;

    if (caixa.x <= -1.0 * (caixa.width + 8.0)) {
        caixa.x = 180.0;
        caixa.y = box_newy();
        caixa.vel += 0.03;
    }
}

void box_render() {
    jogo.screen.draw_rect(caixa.x, caixa.y, caixa.width, caixa.height, WHITE);
}

void spike_render() {
    // espetos de cima
    for (int i = 0; i < top.num; i++) {
        uint8_t top_x = i * top.gap;
        jogo.screen.draw_line(top_x, top.begin, top_x, top.end, WHITE);
    }

    // espetos de baixo
    for (int i = 0; i < botton.num; i++) {
        uint8_t botton_x = i * botton.gap;
        jogo.screen.draw_line(botton_x, botton.begin, botton_x, botton.end, WHITE);
    }
}

void player_update() {
    boneco.y += boneco.dir * boneco.vel;
}

void player_render() {
    jogo.screen.draw_line(boneco.x, boneco.y, boneco.x, boneco.y+boneco.size, WHITE);
    jogo.screen.draw_line(boneco.x, boneco.y, boneco.x+boneco.size, boneco.y+boneco.size/2, WHITE);
    jogo.screen.draw_line(boneco.x, boneco.y+boneco.size, boneco.x+boneco.size, boneco.y+boneco.size/2, WHITE);
}

void game_reset() {
    // atualização dos scores
    if (jogo.score > jogo.max_score) {
        jogo.max_score = jogo.score;
    }
    jogo.score = 0;

    // valores iniciais do jogador
    boneco.x = 4.0;
    boneco.y = 20.0;
    boneco.dir = 0.0;

    // valores iniciais da caixa
    caixa.x = 180.0;
    caixa.y = box_newy();
    caixa.vel = 0.8;
}

void game_input() {
    while (Serial.available()) {
        Serial.read();
        if (boneco.dir == 0.0) {
            boneco.dir = 1.0;
        } else {
            boneco.dir = -1.0 * boneco.dir;
        }
    }
}

void game_update() {
    player_update();
    box_update();

    // colisão com os espetos
    if (boneco.y <= top.end || boneco.y + boneco.size >= botton.begin) {
        game_reset();
    }

    // colisão com a caixa
    if (boneco.x < caixa.x + caixa.width &&
        boneco.x + boneco.size > caixa.x &&
        boneco.y < caixa.y + caixa.height &&
        boneco.y + boneco.size > caixa.y) {
        game_reset();
    }

    jogo.score += 1;
}

void game_render() {
    jogo.screen.clear_screen();

    // scores (atual e max)
    jogo.screen.select_font(font4x6);
    jogo.screen.println(0, 0, "SCORE:");
    jogo.screen.println(30, 0, String(jogo.score).c_str());
    jogo.screen.println(60, 0, "MAX:");
    jogo.screen.println(90, 0, String(jogo.max_score).c_str());

    // jogador, caixa, espetos
    player_render();
    box_render();
    spike_render();
    
    jogo.screen.delay_frame(1);
}

void loop() {
    game_input();
    game_update();
    game_render();
}
