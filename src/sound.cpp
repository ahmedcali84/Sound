#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
using namespace std;

#define PI 3.14159265359
#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH  1600

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a):
            r(r), g(g), b(b), a(a) {}
};

#define RED Color(255, 0 , 0 , 255)

class Sound {
public:
    int SampleRate;
    float Frequency;
    float Duration;
    float Amplitude;

    Sound(int SampleRate, float Frequency, float Duration, float Amplitude):
          SampleRate(SampleRate), Frequency(Frequency), Duration(Duration), Amplitude(Amplitude) {}

    void GetDetails() {
        cout << "SampleRate: " << SampleRate << " , Frequency: " << Frequency << "Hz , Duration: " << Duration << "s , Amplitude: " << Amplitude << endl;
    }

    vector<float> SineWave() {
        int NumSamples = SampleRate*Duration;
        vector<float> samples;
        for (int i = 0; i < NumSamples; ++i) {
            float t = i / (float) SampleRate;
            float y = Amplitude * sin(2*PI*Frequency*t);
            samples.push_back(y);
        }
        return samples;
    }
    
    SDL_Window *InitWindow();
    SDL_Renderer *CreateRenderer(SDL_Window *window, Color color);
};

SDL_Window *Sound::InitWindow() {
    SDL_Window *window = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Error Initializing Video" << SDL_GetError() << endl;
        exit(1);
    }
    
    window = SDL_CreateWindow("Sine Wave", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Error Creating window" << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_UpdateWindowSurface(window);    
    return window;
}

SDL_Renderer *Sound::CreateRenderer(SDL_Window *window, Color color) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Failed to Create Renderer" << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b , color.a);
    return renderer;
}

void SDL_Cleanup(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

int main(int argc, const char **argv) {
    Sound s(44100, 440.0, 2.0, 1.0);

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " Flag " << endl;
        return 1;
    }
    
    if (strcmp(argv[1], "-d") == 0) {
        s.GetDetails();
    }
    
    vector<float> SineWaves = s.SineWave();
    for (int i = 0; i < (int)(s.Duration*s.SampleRate) % 1000; ++i) {
        cout << "Sample " << i << " : " << SineWaves[i] << endl;
    }
    
    SDL_Window *window = s.InitWindow();
    SDL_Renderer *renderer = s.CreateRenderer(window, RED);
    SDL_RenderDrawLine(renderer,0, SCREEN_HEIGHT/2, SCREEN_WIDTH-100, SCREEN_HEIGHT/2);
    SDL_RenderDrawLine(renderer, 100, 100, 100, SCREEN_HEIGHT-100);
    SDL_RenderPresent(renderer);

    // NOTE: DELAY WINDOW
    SDL_Delay(10000);
    
    SDL_Cleanup(window, renderer);
    return 0;
}
