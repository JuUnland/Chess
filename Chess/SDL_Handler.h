#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>


class SDL_Handler
{
public:

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 640;

	// window we'll be rendering to
	SDL_Window* m_window;

	// Surface contained by the window
	SDL_Surface* m_screenSurface;

	// Renderer
	SDL_Renderer* m_renderer;

	// Event (Mouseclick etc)
	SDL_Event m_event;
	
	// initialisiert das Feld
	bool init();

	// zerstört Renderer, window, surface
	void cleanUp();

	// destructor
	~SDL_Handler();

	// zeichnet ein Rechteck an einer bestimmten Stelle
	void DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text);

	// removes PieceRender
	void undoPieceRender(int x, int y);

	// lädt eine Textur
	SDL_Texture* loadImage(std::string filename);

	//renders the background of the field
	void renderBackground();

private:

};

