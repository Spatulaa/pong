#include <raylib.h>
#include <iostream>
#include <string>
#include <math.h>

// Window Preset
const char* title = "pong";
static int height = 900;
static int width = 900;

class Paddle
{
private:
    Rectangle rect;
    int height, width;
    float x, y;

public:
    int dir = 0;
    Paddle(int p_height, int p_width, float p_x)
    {
        height = p_height;
        width = p_width;
        y = GetScreenHeight() / 2 - height / 2;
        x = p_x;

        rect = Rectangle{x, y, (float) width, (float) height};
    }

    void Display()
    {
        rect = Rectangle{x, y, (float) width, (float) height};
        DrawRectangleRec(rect, WHITE);
    }

    void Reset()
    {
        y = GetScreenHeight() / 2 - height / 2;
    }

    void Clamp()
    {
        // Clamping paddles to screen
        if (y < 0)
        {
            y = 0;
        }
        else if (y > GetScreenHeight() - height)
        {
            y = GetScreenHeight() - height;
        }
    }

    bool Is_Colliding(float p_x, float p_y, float p_radius, float& p_speed)
    {
        if (CheckCollisionCircleRec(Vector2{p_x, p_y}, p_radius, rect))
        {
            return true;
        }
        return false;
    }

    Rectangle Get_Rect()
    {
        return rect;
    }

    void Set_Y(float p_new_y)
    {
        y = p_new_y;
    }

    float Get_X()
    {
        return x;
    }

    float Get_Y()
    {
        return y;
    }
};

class Ball
{
private:
    int radius;
    float original_speed;
    float x, y;

public:
    float speed_x, speed_y;
    Ball(int p_radius, float p_x, float p_y, float p_speed)
    {
        radius = p_radius;
        original_speed = p_speed;
        speed_x = p_speed;
        speed_y = 0;
        x = p_x;
        y = p_y;
    }

    void Display()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Reset(int last_scored)
    {
        speed_y = 0;
        if (last_scored == 1) speed_x = original_speed;
        else speed_x = -original_speed;
        x = GetScreenHeight() / 2;
        y = GetScreenHeight() / 2;
    }

    void Set_Y(float p_new_y)
    {
        y = p_new_y;
    }

    void Set_X(float p_new_x)
    {
        x = p_new_x;
    }

    int Get_Radius()
    {
        return radius;
    }

    float Get_Y()
    {
        return y;
    }

    float Get_X()
    {
        return x;
    }
};

const char* winner_text = NULL;
char score_text[5];
void Display_Text()
{
    int text_width;
    if (winner_text == NULL)
    {
        text_width = MeasureText(score_text, 80);
        DrawText(score_text, GetScreenWidth() / 2 - text_width / 2, GetScreenHeight() / 2 - 40, 80, GRAY);
    }
    else
    {
        text_width = MeasureText(winner_text, 80);
        DrawText(winner_text, GetScreenWidth() / 2 - text_width / 2, GetScreenHeight() / 2 - 40, 80, WHITE);
    }
}

int main(void)
{
    InitWindow(width, height, title);
    SetWindowState(FLAG_VSYNC_HINT); // Caps FPS at refresh rate

    // ------------------------------ MAIN LOOP ------------------------------ //

    // Objects
    Paddle player_one(120, 5, 50);
    Paddle player_two(120, 5, GetScreenWidth() - 60);
    Ball ball(7, GetScreenWidth() / 2, GetScreenHeight() / 2, 300);

    int player_one_score = 0;
    int player_two_score = 0;

    while (!WindowShouldClose())
    {
        // --------------- UPDATES --------------- //
        if (winner_text == NULL)
        {
            ball.Set_X(ball.Get_X() + ball.speed_x * GetFrameTime());
            ball.Set_Y(ball.Get_Y() + ball.speed_y * GetFrameTime());
        }

        // Wall Collisions
        if (ball.Get_X() < 0 + ball.Get_Radius()) // X
        {
            ball.Reset(2);
            player_one.Reset();
            player_two.Reset();
            player_two_score++;
            if (player_two_score >= 5) winner_text = "PLAYER 2 WINS!";
        }
        else if (ball.Get_X() > GetScreenWidth() - ball.Get_Radius()) // X
        {
            ball.Reset(1);
            player_one.Reset();
            player_two.Reset();
            player_one_score++;
            if (player_one_score >= 5) winner_text = "PLAYER 1 WINS!";
        }
        if (ball.Get_Y() < 0 + ball.Get_Radius() || ball.Get_Y() > GetScreenHeight() - ball.Get_Radius()) // Y
        {
            ball.speed_y *= -1;
        }

        // Paddle Collision
        if (player_one.Is_Colliding(ball.Get_X(), ball.Get_Y(), ball.Get_Radius(), ball.speed_x))
        {
            if (ball.speed_x < 0)
            {
                // Pong Physics :shrug:
                if (player_one.dir == 1)
                {
                    ball.speed_y = -abs(ball.Get_Y() - player_one.Get_Y()) * 5;
                }
                else if (player_one.dir == -1)
                {
                    ball.speed_y = abs(ball.Get_Y() - player_one.Get_Y()) * 5;
                }
                
                ball.speed_x *= -1.1f;
            }
        }
        else if (player_two.Is_Colliding(ball.Get_X(), ball.Get_Y(), ball.Get_Radius(), ball.speed_x))
        {
            if (ball.speed_x > 0)
            {
                // Pong Physics :shrug:
                if (player_two.dir == 1)
                {
                    ball.speed_y = -abs(ball.Get_Y() - player_two.Get_Y()) * 5;
                }
                else if (player_two.dir == -1)
                {
                    ball.speed_y = abs(ball.Get_Y() - player_two.Get_Y()) * 5;
                }

                ball.speed_x *= -1.1f;
            }
        }

        // Controlling Paddles
        if (IsKeyDown(KEY_W)) // Left Paddle
        {
            player_one.Set_Y(player_one.Get_Y() - (800 * GetFrameTime()));
            player_one.dir = 1;
        }
        else if (IsKeyDown(KEY_S))
        {
            player_one.Set_Y(player_one.Get_Y() + (800 * GetFrameTime()));
            player_one.dir = -1;
        }
        else player_one.dir = 0;

        if (IsKeyDown(KEY_UP)) // Left Paddle
        {
            player_two.Set_Y(player_two.Get_Y() - (800 * GetFrameTime()));
            player_two.dir = 1;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            player_two.Set_Y(player_two.Get_Y() + (800 * GetFrameTime()));
            player_two.dir = -1;
        }
        else player_two.dir = 0;

        player_one.Clamp();
        player_two.Clamp();

        // --------------- DRAWING SCREEN --------------- //
        BeginDrawing(); // Start Rendering Process
        ClearBackground(BLACK); // Changes Background

        // UI
        std::sprintf(score_text, "%i | %i", player_one_score, player_two_score);
        Display_Text();

        if (winner_text == NULL)
        {
            ball.Display();
        }

        player_one.Display();
        player_two.Display();

        EndDrawing(); // Handles Events
    }
    

    CloseWindow();
    return 0;
}