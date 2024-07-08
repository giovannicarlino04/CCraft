#include <SFML/Graphics.h>
#include <SFML/Window.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define TILE_SIZE 16
#define WORLD_WIDTH (WINDOW_WIDTH / TILE_SIZE)
#define WORLD_HEIGHT (WINDOW_HEIGHT / TILE_SIZE)
#define PLAYER_SPEED 5
#define ZOOM_LEVEL 2.5f  // Adjust zoom level as needed

int world[WORLD_WIDTH][WORLD_HEIGHT];
sfVector2f playerPosition = {5.0f * TILE_SIZE, 5.0f * TILE_SIZE};  // Starting position of the player

// Textures and texture rectangles for different tile types
sfTexture* terrain_texture;
sfIntRect tileRects[3];  // Adjust as per your tile types

// View and zoom related variables
sfView* view;

void initTextures() {
    terrain_texture = sfTexture_createFromFile("textures/terrain.png", NULL);

    // Define texture rectangles for each tile type
    tileRects[0] = (sfIntRect){16, 0, TILE_SIZE, TILE_SIZE};  // stone
    tileRects[1] = (sfIntRect){32, 0, TILE_SIZE, TILE_SIZE};  // dirt
}

void initWorld() {
    // Initialize the world with ground and other elements
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            // Example initialization based on your world logic
            world[x][y] = 2;  // Example: dirt or other tile types
        }
    }
}

void handleInput(sfRenderWindow* window) {
    sfEvent event;
    while (sfRenderWindow_pollEvent(window, &event)) {
        switch (event.type) {
            case sfEvtClosed:
                sfRenderWindow_close(window);
                break;
            case sfEvtKeyPressed:
                // Example interaction logic based on your needs
                if (event.key.code == sfKeyUp) {
                    // Move up logic with collision detection
                    if (playerPosition.y > 0)
                        playerPosition.y -= TILE_SIZE;
                } else if (event.key.code == sfKeyDown) {
                    // Move down logic with collision detection
                    if (playerPosition.y < WINDOW_HEIGHT - TILE_SIZE)
                        playerPosition.y += TILE_SIZE;
                } else if (event.key.code == sfKeyLeft) {
                    // Move left logic with collision detection
                    if (playerPosition.x > 0)
                        playerPosition.x -= TILE_SIZE;
                } else if (event.key.code == sfKeyRight) {
                    // Move right logic with collision detection
                    if (playerPosition.x < WINDOW_WIDTH - TILE_SIZE)
                        playerPosition.x += TILE_SIZE;
                }
                break;
            default:
                break;
        }
    }
}

void updateView(sfRenderWindow* window) {
    // Set the center of the view to follow the player
    sfVector2f center = {playerPosition.x + TILE_SIZE / 2.0f, playerPosition.y + TILE_SIZE / 2.0f};
    sfView_setCenter(view, center);

    // Adjust the zoom level of the view
    sfView_setSize(view, (sfVector2f){WINDOW_WIDTH / ZOOM_LEVEL, WINDOW_HEIGHT / ZOOM_LEVEL});

    // Apply the view to the window
    sfRenderWindow_setView(window, view);
}

void render(sfRenderWindow* window) {
    // Clear the window
    sfRenderWindow_clear(window, sfBlack);

    // Draw the world
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            int tileType = world[x][y];
            if (tileType != 0) {  // Draw only if not empty space
                sfIntRect tileRect = tileRects[tileType - 1];  // Adjust index since tileType starts from 1
                sfSprite* sprite = sfSprite_create();
                sfSprite_setTexture(sprite, terrain_texture, sfTrue);
                sfSprite_setTextureRect(sprite, tileRect);
                sfSprite_setPosition(sprite, (sfVector2f){x * TILE_SIZE, y * TILE_SIZE});
                sfRenderWindow_drawSprite(window, sprite, NULL);
                sfSprite_destroy(sprite);
            }
        }
    }

    // Draw the player
    sfCircleShape* player = sfCircleShape_create();
    sfCircleShape_setFillColor(player, sfGreen);
    sfCircleShape_setRadius(player, TILE_SIZE / 2.0f);  // Adjust size as needed
    sfCircleShape_setPosition(player, playerPosition);
    sfRenderWindow_drawCircleShape(window, player, NULL);
    sfCircleShape_destroy(player);

    // Display everything on screen
    sfRenderWindow_display(window);
}

int main() {
    // Create the main window
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "CCraft", sfClose, NULL);

    // Initialize textures and world
    initTextures();
    initWorld();

    // Initialize the view
    view = sfView_create();
    sfView_setCenter(view, (sfVector2f){playerPosition.x + TILE_SIZE / 2.0f, playerPosition.y + TILE_SIZE / 2.0f});
    sfView_setSize(view, (sfVector2f){WINDOW_WIDTH / ZOOM_LEVEL, WINDOW_HEIGHT / ZOOM_LEVEL});

    sfClock* clock = sfClock_create();

    // Game loop
    while (sfRenderWindow_isOpen(window)) {
        // Handle events and input
        handleInput(window);

        // Update the view to follow the player
        updateView(window);

        // Render the game
        render(window);

        // FPS limit
        sfTime elapsed = sfClock_getElapsedTime(clock);
        sfInt32 sleepTime = 16 - sfTime_asMilliseconds(elapsed);
        if (sleepTime > 0) {
            sfSleep(sfMilliseconds(sleepTime));
        }
        sfClock_restart(clock);
    }

    // Clean up resources
    sfTexture_destroy(terrain_texture);
    sfRenderWindow_destroy(window);
    sfView_destroy(view);
    sfClock_destroy(clock);

    return 0;
}
