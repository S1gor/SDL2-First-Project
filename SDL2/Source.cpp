#include <iostream>
#include <SDL.h>

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
int win_width = 800, win_height = 700;

void DeInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
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

	win = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

	SDL_SetRenderDrawColor(ren, 205, 205, 205, 255);
	SDL_RenderClear(ren);
	
	double scale = 1.5;
	double x1, y1, x2, y2;
	int sx1, sy1, sx2, sy2;
	int want_point = 3;
	bool rising = true;

	while (true)
	{
		SDL_SetRenderDrawColor(ren, 205, 205, 205, 255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
		x1 = -175, x2 = 175;
		y1 = 0, y2 = 0;

		mathCoordsToScreen(x1, y1, scale, win_width / 2, win_height / 2, sx1, sy1);
		mathCoordsToScreen(x2, y2, scale, win_width / 2, win_height / 2, sx2, sy2);
		SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

		y1 = -175, y2 = 175;
		x1 = 0, x2 = 0;

		mathCoordsToScreen(x1, y1, scale, win_width / 2, win_height / 2, sx1, sy1);
		mathCoordsToScreen(x2, y2, scale, win_width / 2, win_height / 2, sx2, sy2);
		SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

		int point_count = want_point;
		SDL_Point* points = (SDL_Point*)malloc(sizeof(SDL_Point) * (point_count + 1));

		float alpha = 0;
		for (int i = 0; i < point_count; i++)
		{
			alpha += 2 * M_PI / point_count;
			mathCoordsToScreen(200 * cos(alpha), 200 * sin(alpha), 1.0, win_width / 2, win_height / 2, points[i].x, points[i].y);
		}
		points[point_count] = points[0];
		
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawLines(ren, points, point_count + 1);

		SDL_RenderPresent(ren);
		SDL_Delay(50);
		free(points);

		if (rising)
			want_point++;
		else
			want_point--;

		if (rising && want_point > 30 || !rising && want_point <= 3)
			rising = !rising;
	}

	DeInit(0);
	return 0;
}