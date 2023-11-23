#include <iostream>
#include <unordered_map>
#include <SDL.h>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FPS 60

using namespace std;

unordered_map<SDL_Keycode, bool> keyStates;

bool initialize();
SDL_Point rotatePoint(SDL_Point, double, SDL_Point);

int main(void) {

    bool isInitialized = initialize();
    bool isRunning = true;
    Uint32 startTime = 0;
    Uint32 frameTime = 0;

    if (!isInitialized) {
        cout << "initialize failed with error: " << SDL_GetError() << endl;
        return EXIT_FAILURE; 
    }

    SDL_Window* window = SDL_CreateWindow(
        "Learn SDL", // title
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, // window width
        WINDOW_HEIGHT, // window height
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (renderer == nullptr) {
        cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    

    SDL_Rect rect;
    rect.x = 50;
    rect.y = 50;
    rect.w = 200;
    rect.h = 150;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    while (isRunning) {
        startTime = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) isRunning = false;
            if (event.type == SDL_KEYDOWN) keyStates[event.key.keysym.sym] = true;
            if (event.type == SDL_KEYUP) keyStates[event.key.keysym.sym] = false;

        }

        if (keyStates[SDLK_UP]) rect.y -= 2;
        if (keyStates[SDLK_DOWN]) rect.y += 2;
        if (keyStates[SDLK_RIGHT]) rect.x += 2;
        if (keyStates[SDLK_LEFT]) rect.x -= 2;

        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < (1000 / FPS)) {
            SDL_Delay((1000 / FPS) - frameTime);
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS; 
}

bool initialize() {
    return SDL_Init(SDL_INIT_EVERYTHING) <= 0;
}

SDL_Point rotatePoint(SDL_Point point, double angle, SDL_Point pivot) {
    double s = sin(angle);
    double c = cos(angle);

    // 회전 중심을 기준으로 좌표 변환
    point.x -= pivot.x;
    point.y -= pivot.y;

    // 회전 변환
    int new_x = static_cast<int>(point.x * c - point.y * s);
    int new_y = static_cast<int>(point.x * s + point.y * c);

    // 다시 원래 좌표계로 변환 후 반환
    return {new_x + pivot.x, new_y + pivot.y};
}