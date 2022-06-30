#include "gest_event.h"

int Keys[NB_KEYS];

void initKeys()
{
    int i;
    for (i = 0; i < NB_KEYS; i++)
    {
        Keys[i] = 1;
    }
}

void gestMenu()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = 0;
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                running = 0;
                continue;

            default:
                continue;
            }
            break;

        default:
            continue;
        }
    }
    SDL_Delay(5);
}

void gestGame()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = 0;
            break;

        case SDL_MOUSEMOTION:
            // mouvement de caméra
            if (event.motion.xrel > 0)
            {
                player.angle += 0.01;
                if (player.angle > 2 * pi)
                    player.angle -= 2 * pi;
            }
            if (event.motion.xrel < 0)
            {
                player.angle -= 0.01;
                if (player.angle < 0)
                    player.angle += 2 * pi;
            }
            if (event.motion.yrel > 0)
            {
                player.viewAngle -= 0.05 * RD;
            }
            if (event.motion.yrel < 0)
            {
                player.viewAngle += 0.05 * RD;
            }
            player.deltax = cos(player.angle);
            player.deltay = sin(player.angle);
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                running = 0;
                continue;

            case SDLK_z:
                // avance
                Keys[2] = 1;
                continue;

            case SDLK_d:
                // va à droite
                Keys[3] = 1;
                continue;

            case SDLK_s:
                // va en arrière
                Keys[0] = 1;
                continue;

            case SDLK_q:
                // va à gauche
                Keys[1] = 1;
                continue;

            case SDLK_h:
                if (showHub == 0)
                {
                    showHub = 1;
                }
                else
                {
                    showHub = 0;
                }
                break;

            case SDLK_b:
                initBall();
                landingPointPlayerIsFind = 0;
                landingPointEnnemyIsFind = 0;
                break;

            default:
                break;
            }

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_z:
                Keys[2] = 0;
                continue;

            case SDLK_d:
                Keys[3] = 0;
                continue;

            case SDLK_s:
                Keys[0] = 0;
                continue;

            case SDLK_q:
                Keys[1] = 0;
                continue;

            default:
                break;
            }

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                player.isHoldingClick = 1;
                player.startHitTimer = timer;
                player.startHitBool = 0;
            }
            break;

        case SDL_MOUSEBUTTONUP:

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                player.endHitTimer = timer;
                player.startHitBool = 1;
                player.hitIntensity = (player.endHitTimer - player.startHitTimer) / 200;
                player.isHoldingClick = 0;
            }
            break;

        default:
            break;
        }
    }
    SDL_Delay(5);
}

void *EventLoop(void *arg)
{
    while (running)
    {
        switch (game_state)
        {
        case MENU:
            gestMenu();
            break;
        case GAME:
            gestGame();
            break;
        default:
            printf("game state fault");
            break;
        }
    }
    return NULL;
}