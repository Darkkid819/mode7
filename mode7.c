#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define HALF_WIDTH (SCREEN_WIDTH / 2)
#define HALF_HEIGHT (SCREEN_HEIGHT / 2)
#define FOCAL_LENGTH 250
#define SCALE 100
#define SPEED 0.01

typedef struct Player {
    Vector2 position;
    float angle;
    float altitude;
    Camera2D camera;
} Player;

Player player;

Texture2D texSky;
Texture2D texMap;
RenderTexture2D target;

static void UpdateScreen(void);
static void RenderFrame(void);

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib");

    SetTargetFPS(60);

    texSky = LoadTexture("resources/sky1.png");
    texMap = LoadTexture("resources/map1.png");

    target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    player.position = (Vector2){0.0f, 0.0f};
    player.angle = 0.0f;
    player.altitude = 1.0f;
    player.camera = (Camera2D){0};

    while (!WindowShouldClose()) {
        UpdateScreen();

        BeginTextureMode(target);
        ClearBackground(BLACK);
        RenderFrame();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureRec(target.texture, (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT}, (Vector2){0, 0}, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(texSky);
    UnloadTexture(texMap);
    UnloadRenderTexture(target);

    CloseWindow();

    return 0;
}

static void UpdateScreen(void) {
    if (IsKeyDown(KEY_W)) {
        player.position.x += cosf(player.angle) * SPEED;
        player.position.y += sinf(player.angle) * SPEED;
    }
    if (IsKeyDown(KEY_S)) {
        player.position.x -= cosf(player.angle) * SPEED;
        player.position.y -= sinf(player.angle) * SPEED;
    }
    if (IsKeyDown(KEY_A)) {
        player.angle -= SPEED;
    }
    if (IsKeyDown(KEY_D)) {
        player.angle += SPEED;
    }
    if (IsKeyDown(KEY_Q)) {
        player.altitude += SPEED;
    }
    if (IsKeyDown(KEY_E)) {
        player.altitude -= SPEED;
    }

    player.altitude = Clamp(player.altitude, 0.3, 4.0);
}

static void RenderFrame(void) {

}