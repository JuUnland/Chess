#include "SDL_Handler.h"
#include <stdio.h>
#include <iostream>


SDL_Texture* SDL_Handler::loadImage(std::string filename)
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//Load the image
	loadedImage = IMG_Load(filename.c_str());

	//If the image loaded
	if (loadedImage == NULL)
	{
		std::cout << "Laden von " << filename << " verkackt" << std::endl;
	}

	SDL_Texture* text = SDL_CreateTextureFromSurface(m_renderer, loadedImage);

	//Return the optimized image
	return text;
}

void SDL_Handler::renderBackground()
{
	bool white = true;
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (white)
			{
				SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(m_renderer, 155, 103, 60, 255);
			}
			white = !white;
			SDL_Rect rectangle = { i * SCREEN_WIDTH / 8,
								  j * SCREEN_HEIGHT / 8,
								  SCREEN_WIDTH / 8,
								  SCREEN_HEIGHT / 8 };
			SDL_RenderFillRect(m_renderer, &rectangle);
		}
		white = !white;
	}
}

void SDL_Handler::undoPieceRender(int x, int y)
{
	if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1))
	{
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(m_renderer, 155, 103, 60, 255);
	}
	SDL_Rect rectangle = { x * SCREEN_WIDTH / 8,
						  y * SCREEN_HEIGHT / 8,
						  SCREEN_WIDTH / 8,
						  SCREEN_HEIGHT / 8 };
	SDL_RenderFillRect(m_renderer, &rectangle);
}

void SDL_Handler::cleanUp()
{
	//Free the Surface
	SDL_FreeSurface(m_screenSurface);

	//Destroy window
	SDL_DestroyWindow(m_window);

	SDL_DestroyRenderer(m_renderer);

	//Quit SDL subsystems
	SDL_Quit();
}


void SDL_Handler::DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text)
{
	if (text != nullptr)
	{
		SDL_RenderCopy(m_renderer, text, &source, &dest);
		SDL_RenderPresent(m_renderer);

		//Update the surface
		SDL_UpdateWindowSurface(m_window);
	}
	else
	{
		std::cout << "DrawRectangle: text was nullptr" << std::endl;
	}
}



SDL_Handler::~SDL_Handler()
{
}


bool SDL_Handler::init()
{
	//The window we'll be rendering to
	m_window = NULL;

	//The surface contained by the window
	m_screenSurface = NULL;

	bool quit = false;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		cleanUp();
		return false;
	}
	else
	{
		//Create window
		m_window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		}
	}
	return true;
}
