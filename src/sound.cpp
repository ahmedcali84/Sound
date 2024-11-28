#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
using namespace std;

//NOTE: MACROS FOR CONSTANT VALUES
#define PI 3.14159265359
#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH  1600

// NOTE: COLOR Struct to hold RGBA Values
struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a):
            r(r), g(g), b(b), a(a) {}
};

// NOTE: Macro Definition Of Red, Greem and Blue Colors
#define RED   Color(255, 0 , 0, 255)
#define GREEN Color(0, 255, 0, 255)
#define BLUE  Color(0, 0, 255, 255)

// NOTE: Actual Sound Class
class Sound {
public:
    int SampleRate;
    float Frequency;
    float Duration;
    float Amplitude;
    
    // NOTE: Contructor
    Sound(int SampleRate, float Frequency, float Duration, float Amplitude):
          SampleRate(SampleRate), Frequency(Frequency), Duration(Duration), Amplitude(Amplitude) {}
    
    // NOTE: Function to Get Sound Details
    void GetDetails() {
        cout << "SampleRate: " << SampleRate << " , Frequency: " << Frequency << "Hz , Duration: " << Duration << "s , Amplitude: " << Amplitude << endl;
    }
    
    // NOTE: Function to Calculate Sine Wave Values, Return Sample Array
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
    
    // NOTE: Function Declarations
    SDL_Window *InitWindow();
    SDL_Renderer *CreateRenderer(SDL_Window *window, Color color);
};

void SDL_CleanUp(SDL_Window *window, SDL_Renderer *renderer) {
    //NOTE: CleanUp After Visualization
    if (window)   SDL_DestroyWindow(window);
    if (renderer) SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

SDL_Window *Sound::InitWindow() {
    //NOTE: Initializes Window
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
    // NOTE: Initialize Renderer, Returns Error if Failed
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Failed to Create Renderer" << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    
    // NOTE: Set Rendering Color to RED
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b , color.a);
    return renderer;
}

void VisualizeSineWave(Sound s, Uint32 Delay) {
    //NOTE: Initialize an Array of SDL_Point
    vector<SDL_Point> Points;
    vector<float> SineWaves = s.SineWave();
    
    //NOTE: Number of Samples
    int NumSamples = SineWaves.size();

    // NOTE: Loop Through Number of Sample and Update X and Y for Each Points
    for (int i = 0; i < NumSamples; ++i) {
        // NOTE: Map One Cycle Across Screen Width
        int x = (i * SCREEN_WIDTH) / NumSamples;

        // NOTE: Scale Amplitude
        int y = (SCREEN_HEIGHT/2) - (SineWaves[i] * (SCREEN_HEIGHT / 4));
        
        //NOTE: Push x, y to Points Array
        SDL_Point point = {x, y};
        Points.push_back(point);
    }

    // NOTE: Initialize Window and Renderer
    SDL_Window *window = s.InitWindow();
    SDL_Renderer *renderer = s.CreateRenderer(window, RED);
    
    // NOTE: Draw Points
    SDL_RenderDrawLines(renderer, Points.data(), Points.size());
    SDL_RenderPresent(renderer);
 
    // NOTE: DELAY WINDOW
    SDL_Delay(Delay);
    SDL_CleanUp(window, renderer);
}

// TODO Define an Audio callback Function to populate the buffer with sine wave samples
void AudioCallBack(void *UserData, Uint8 *stream, int len) {
    Sound *sound = (Sound*)UserData; // Get Sound Object
    vector<float> SineWaves = sound->SineWave(); // Get Sine Wave Values

    static int SampleIndex = 0;
    for (int i = 0; i < len/2; ++i) {
        Sint16 sample = (Sint16)(SineWaves[SampleIndex] * 32767);
        ((Sint16*)stream)[i] = 0;

        // Move to next sample
        SampleIndex = (SampleIndex + 1) % SineWaves.size();
    }
}

int main(int argc, const char **argv) {
    //NOTE: Initialize Sound Variable
    Sound s(44100, 440.0, 0.1, 1);

    // NOTE: Return Error if command-line Args is not 3 or 2
    if (argc != 3 && argc != 2) {
        cout << "Usage: " << argv[0] << " Flag " << endl;
        return 1;
    }

    // NOTE: if Flag is -d , return Sound Details
    if (argc == 2) {
        if (strcmp(argv[1], "-d") == 0) {
            s.GetDetails();
        }
    }

    // NOTE: if Flag is -v, Visualize the Sine Wave
    if (argc == 3) {
        if (strcmp(argv[1], "-v") == 0) {
            VisualizeSineWave(s, stoul(argv[2]));
        }
    }
    return 0;
}
