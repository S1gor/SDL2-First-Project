#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
int win_width = 1000, win_height = 900;

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

	win = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

void mathCoordsToScreen(double x, double y, double scale, int centerx, int centery, int& sx, int& sy)
{
	sx = round(centerx + x * scale);
	sy = round(centery - y * scale);
}

int main(int args, char* argv[])
{
	system("chcp 1251"); system("cls");
	Init();

	SDL_Surface* surface = IMG_Load("diablo.jpg");
	if (surface == NULL)
	{
		printf("Не удалось загрузить картинку!");
		DeInit(1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);

	SDL_SetRenderDrawColor(ren, 205, 205, 205, 255);
	SDL_RenderClear(ren);
	
	double scale = 1.5;
	double x1, y1, x2, y2;
	int sx1, sy1, sx2, sy2;
	
	int want_point = 3;
	bool rising = true;

	SDL_Event ev;
	bool isRunning = true;
	int mousex = win_width / 2; int mousey = win_height / 2;
	bool isUpPressed = false; bool isDownPressed = false; bool isRightPressed = false; bool isLeftPressed = false;

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
				case SDL_SCANCODE_ESCAPE:
					isRunning = false;
					break;
				case SDL_SCANCODE_RIGHT:
					isRightPressed = true;
					break;
				case SDL_SCANCODE_LEFT:
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_UP:
					isUpPressed = true;
					break;
				case SDL_SCANCODE_DOWN:
					isDownPressed = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT:
					isRightPressed = false;
					break;
				case SDL_SCANCODE_LEFT:
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_UP:
					isUpPressed = false;
					break;
				case SDL_SCANCODE_DOWN:
					isDownPressed = false;
					break;
				}
				break;
			}
		}
		if (isUpPressed && !isDownPressed)		mousey -= 5;
		if (!isUpPressed && isDownPressed)		mousey += 5;
		if (isRightPressed && !isLeftPressed)	mousex += 5;
		if (!isRightPressed && isLeftPressed)	mousex -= 5;

		#pragma region DRAWING CIRCLE
		SDL_SetRenderDrawColor(ren, 205, 205, 205, 255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
		x1 = -175, x2 = 175;
		y1 = 0, y2 = 0;

		mathCoordsToScreen(x1, y1, scale, mousex, mousey, sx1, sy1);
		mathCoordsToScreen(x2, y2, scale, mousex, mousey, sx2, sy2);
		SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

		y1 = -175, y2 = 175;
		x1 = 0, x2 = 0;

		mathCoordsToScreen(x1, y1, scale, mousex, mousey, sx1, sy1);
		mathCoordsToScreen(x2, y2, scale, mousex, mousey, sx2, sy2);
		SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

		int point_count = want_point;
		SDL_Point* points = (SDL_Point*)malloc(sizeof(SDL_Point) * (point_count + 1));

		float alpha = 0;
		for (int i = 0; i < point_count; i++)
		{
			alpha += 2 * M_PI / point_count;
			mathCoordsToScreen(200 * cos(alpha), 200 * sin(alpha), 1.0, mousex, mousey, points[i].x, points[i].y);
		}
		points[point_count] = points[0];
		
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawLines(ren, points, point_count + 1);

		free(points);

		if (rising)
			want_point++;
		else
			want_point--;

		if (rising && want_point > 30 || !rising && want_point <= 3)
			rising = !rising;
		#pragma endregion

		SDL_RenderCopy(ren, texture, NULL, NULL);

		SDL_RenderPresent(ren);
		SDL_Delay(50);
	}
	
	SDL_DestroyTexture(texture);

	DeInit(0);
	return 0;
}