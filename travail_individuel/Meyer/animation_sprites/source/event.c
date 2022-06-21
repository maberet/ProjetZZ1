#include "event.h"

SDL_Event event;

//void *eventLoop(void *arg)
void eventLoop()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT:
            printf("window event\n");
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                printf("appui sur la croix\n");
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                width = event.window.data1;
                height = event.window.data2;
                printf("Size : %d%d\n", width, height);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            printf("Appui :%d %d\n", event.button.x, event.button.y);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_RIGHT:
                break;
            case SDLK_c:
                printf("charge niveau.txt...\n");
                break;
            default:
                printf("une touche est tapee\n");
                break;
            }
            break;
        case SDL_QUIT:
            printf("on quitte\n");
            running = 0;
            break;
        }
    }
    SDL_Delay(10);
    //return NULL;
}