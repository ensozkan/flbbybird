#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BIRD_WIDTH 40
#define BIRD_HEIGHT 40
#define PIPE_WIDTH 80
#define PIPE_GAP 200
#define GRAVITY 0.5
#define JUMP_STRENGTH -10

// Yapı tanımlamaları
typedef struct {
    float x, y;
    float velocity;
} Bird;

typedef struct {
    float x, y;
} Pipe;

// Fonksiyon deklarasyonları
void handleEvents(SDL_Event *event, bool *running, Bird *bird);
void updateBird(Bird *bird);
void updatePipes(Pipe pipes[], int *pipeIndex);
void drawBird(SDL_Renderer *renderer, Bird *bird);
void drawPipes(SDL_Renderer *renderer, Pipe pipes[], int pipeIndex);
void drawScore(SDL_Renderer *renderer, int score);

int main(int argc, char *argv[]) {
    // SDL Başlatma
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Ekran oluşturma
    SDL_Window *window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Rastgele sayı üretme için zamanlayıcı
    srand(time(NULL));

    // Oyunun başlangıç durumları
    Bird bird = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0};
    Pipe pipes[2] = {{SCREEN_WIDTH, 0}, {SCREEN_WIDTH + SCREEN_WIDTH / 2, 0}};
    bool running = true;
    int pipeIndex = 0;
    int score = 0;

    SDL_Event event;

    // Oyun döngüsü
    while (running) {
        // Olayları kontrol et
        while (SDL_PollEvent(&event)) {
            handleEvents(&event, &running, &bird);
        }

        // Kuşu güncelle
        updateBird(&bird);

        // Boruları güncelle
        updatePipes(pipes, &pipeIndex);

        // Çizim işlemleri
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Siyah arka plan
        SDL_RenderClear(renderer);

        // Kuşu çiz
        drawBird(renderer, &bird);

        // Boruları çiz
        drawPipes(renderer, pipes, pipeIndex);

        // Skoru çiz
        drawScore(renderer, score);

        // Render
        SDL_RenderPresent(renderer);

        // FPS sınırı
        SDL_Delay(16); // yaklaşık 60 FPS
    }

    // SDL kapama
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void handleEvents(SDL_Event *event, bool *running, Bird *bird) {
    if (event->type == SDL_QUIT) {
        *running = false;
    }

    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE) {
        bird->velocity = JUMP_STRENGTH; // Kuş zıplar
    }
}

void updateBird(Bird *bird) {
    bird->velocity += GRAVITY; // Yerçekimi
    bird->y += bird->velocity;

    // Kuşun ekran dışına çıkmasını engelle
    if (bird->y < 0) bird->y = 0;
    if (bird->y > SCREEN_HEIGHT - BIRD_HEIGHT) bird->y = SCREEN_HEIGHT - BIRD_HEIGHT;
}

void updatePipes(Pipe pipes[], int *pipeIndex) {
    pipes[*pipeIndex].x -= 5; // Borular sağdan sola hareket eder

    // Borular ekrandan çıktığında yenisini ekle
    if (pipes[*pipeIndex].x < -PIPE_WIDTH) {
        pipes[*pipeIndex].x = SCREEN_WIDTH;
        pipes[*pipeIndex].y = rand() % (SCREEN_HEIGHT - PIPE_GAP);
    }

    *pipeIndex = (*pipeIndex + 1) % 2; // Boru dizisi döngüsü
}

void drawBird(SDL_Renderer *renderer, Bird *bird) {
    SDL_Rect birdRect = {bird->x, bird->y, BIRD_WIDTH, BIRD_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Sarı kuş
    SDL_RenderFillRect(renderer, &birdRect);
}

void drawPipes(SDL_Renderer *renderer, Pipe pipes[], int pipeIndex) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Yeşil borular

    // Üst boru
    SDL_Rect topPipe = {pipes[pipeIndex].x, 0, PIPE_WIDTH, pipes[pipeIndex].y};
    SDL_RenderFillRect(renderer, &topPipe);

    // Alt boru
    SDL_Rect bottomPipe = {pipes[pipeIndex].x, pipes[pipeIndex].y + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipes[pipeIndex].y + PIPE_GAP)};
    SDL_RenderFillRect(renderer, &bottomPipe);
}

void drawScore(SDL_Renderer *renderer, int score) {
    // Burada skoru yazdırmak için bir yazı tipi kullanmak gerekebilir
    // Ancak SDL, metin rendering için ek kütüphaneler gerektirir.
    // Bu yüzden skor çizimi basitçe sayısal değeri ekranda göstermek için başka bir yöntem kullanılabilir.
}