#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

Texture2D texSky;
Texture2D texMap;
Image imgMap;

float fWorldX = 1000.0f;
float fWorldY = 1000.0f;
float fWorldA = 0.1f;
float fNear = 0.005f;
float fFar = 0.03f; 
float fFoVHalf = PI / 4.0f;
float fSpeed = 50.0f; 

static void DrawMode7Line(int);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - Mode7 Demo");
    SetTargetFPS(60);

    texSky = LoadTexture("resources/sky1.png");
    texMap = LoadTexture("resources/map1.png");
    imgMap = LoadImageFromTexture(texMap);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_Q)) fNear += 1.0f * GetFrameTime();
        if (IsKeyDown(KEY_A)) fNear -= 1.0f * GetFrameTime();
        
        if (IsKeyDown(KEY_W)) fFar += 1.0f * GetFrameTime();
        if (IsKeyDown(KEY_S)) fFar -= 1.0f * GetFrameTime();        

        if (IsKeyDown(KEY_Z)) fFoVHalf += 1.0f * GetFrameTime();
        if (IsKeyDown(KEY_X)) fFoVHalf -= 1.0f * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) fWorldA -= 1.0f * GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) fWorldA += 1.0f * GetFrameTime();

        if (IsKeyDown(KEY_UP)) {
            fWorldX += cosf(fWorldA) * fSpeed * GetFrameTime();
            fWorldY -= sinf(fWorldA) * fSpeed * GetFrameTime();
        }

        if (IsKeyDown(KEY_DOWN)) {
            fWorldX -= cosf(fWorldA) * fSpeed * GetFrameTime();
            fWorldY += sinf(fWorldA) * fSpeed * GetFrameTime();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
            DrawMode7Line(y);
        }

        EndDrawing();
    }

    UnloadTexture(texSky);
    UnloadTexture(texMap);
    UnloadImage(imgMap);

    CloseWindow();

    return 0;
}

static void DrawMode7Line(int y) {
    float fDepth = 1.0f / ((float)(y) / (SCREEN_HEIGHT / 2.0f) * (fFar - fNear) + fNear);
    
    Vector2 lineStart = {
        fWorldX - cosf(fWorldA - fFoVHalf) * fDepth,
        fWorldY - sinf(fWorldA - fFoVHalf) * fDepth
    };

    Vector2 lineEnd = {
        fWorldX + cosf(fWorldA + fFoVHalf) * fDepth,
        fWorldY + sinf(fWorldA + fFoVHalf) * fDepth
    };

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float fSampleWidth = (float)x / (float)SCREEN_WIDTH;
        float fSampleX = (lineEnd.x - lineStart.x) * fSampleWidth + lineStart.x;
        float fSampleY = (lineEnd.y - lineStart.y) * fSampleWidth + lineStart.y;

        int texX = ((int)fSampleX % texMap.width + texMap.width) % texMap.width;
        int texY = ((int)fSampleY % texMap.height + texMap.height) % texMap.height;

        Color color = GetImageColor(imgMap, texX, texY);

        DrawPixel(x, (SCREEN_HEIGHT / 2) + y, color);
    }
}