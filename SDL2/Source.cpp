#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
int win_width = 1260, win_height = 630;

void DeInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Не удалось инициализировать SDL!");
		DeInit(1);
	}

	int res;
	if ((res = (IMG_INIT_PNG | IMG_INIT_JPG)) == 0)
	{
		printf("Не удалось инициализировать SDL_Image!");
		DeInit(1);
	}
	if (res & IMG_INIT_PNG) printf("Инициализирована библиотека PNG.\n"); else printf("Не удалось инициализировать библиотеку PNG.");
	if (res & IMG_INIT_JPG) printf("Инициализирована библиотека JPG.\n"); else printf("Не удалось инициализировать библиотеку JPG.");

	win = SDL_CreateWindow("s1mple", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (win == NULL)
	{
		printf("Не удалось создать окно!");
		DeInit(1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL)
	{
		printf("не удалось создать рисовальщик!");
		DeInit(1);
	}
}

SDL_Texture* loadTextureFromFile(const char* filename, SDL_Rect* rect)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL)
	{
		printf("Не удалось загрузить картинку!");
		DeInit(1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	*rect = { 0,0,surface->w,surface->h };
	SDL_FreeSurface(surface);
	return texture;
}

int main(int args, char* argv[])
{
	system("chcp 1251"); system("cls");
	Init();

	SDL_Rect back_rect;
	SDL_Texture* back_tex = loadTextureFromFile("back2.jpg", &back_rect);

	SDL_Event ev;
	bool isRunning = true;

	bool isup, isdown, isright, isleft;
	isup = isdown = isright = isleft = false;

	while (isRunning)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					win_width = ev.window.data1;
					win_height = ev.window.data2;
				}
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:	isRunning = false; break;
				case SDL_SCANCODE_RIGHT:	isright = true; break;
				case SDL_SCANCODE_LEFT:		isleft = true; break;
				case SDL_SCANCODE_UP:		isup = true; break;
				case SDL_SCANCODE_DOWN:		isdown = true; break;
				}
				break;
			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT:	isright = false; break;
				case SDL_SCANCODE_LEFT:		isleft = false; break;
				case SDL_SCANCODE_UP:		isup = false; break;
				case SDL_SCANCODE_DOWN:		isdown = false; break;
				}
				break;
			}
			
			SDL_SetRenderDrawColor(ren, 255, 255,255, 255);
			SDL_RenderClear(ren);

			SDL_RenderCopy(ren, back_tex, &back_rect, NULL);
		}
		SDL_RenderPresent(ren);
		SDL_Delay(50);
	}
	
	SDL_DestroyTexture(back_tex);

	DeInit(0);
	return 0;
}