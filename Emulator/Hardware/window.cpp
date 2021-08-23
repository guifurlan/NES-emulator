#include "window.hpp"

#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>

using namespace std;

namespace Hardware
{

uint32_t getPixelColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  return a << 24 | r << 16 | g << 8 | b;
}

Window::Window(string name,
               size_t window_width,
               size_t window_height,
               size_t draw_width,
               size_t draw_height,
               bool enabled) :
  mWindow(),
  mWindowSurface(),
  mDrawSurface(),
  mEnabled(enabled),
  mShouldStop(false),
  mController{0, 0},
  mVerticalBlank(false)
{
  if (mEnabled) {
    // Init the SDL library
    ::SDL_Init(SDL_INIT_VIDEO);

    mWindow = ::SDL_CreateWindow(name.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                window_width,
                                window_height,
                                0);
    
    mWindowSurface = ::SDL_GetWindowSurface(mWindow);

    const auto* format = mWindowSurface->format;
    mDrawSurface = ::SDL_CreateRGBSurfaceWithFormat(
      0,
      draw_width,
      draw_height,
      format->BitsPerPixel,
      format->format);

    ::SDL_Log("Surface format: %s", SDL_GetPixelFormatName(mWindowSurface->format->format));

    ::SDL_SetSurfaceBlendMode(mWindowSurface, SDL_BLENDMODE_NONE);

    cout << "Creating window" << endl;
  }
}

Window::~Window() {
  if (mEnabled) {
    SDL_FreeSurface(mDrawSurface);

    SDL_DestroyWindow(mWindow);

    cout << "Destroying window" << endl;
  }

  SDL_Quit();
}

void Window::setPixel(uint32_t data, size_t x, size_t y)
{
  if (mEnabled) {
    assert(x >= 0);
    assert(x < mDrawSurface->w);
    assert(y >= 0);
    assert(y < mDrawSurface->h);
    auto* pixel = reinterpret_cast<uint32_t*>(mDrawSurface->pixels);
    pixel[y * mDrawSurface->w + x] = data & 0x00FFFFFF;
  }
}

void Window::showFrame()
{
  if (mEnabled) {
    // Render image
    ::SDL_BlitScaled(mDrawSurface, NULL, mWindowSurface, NULL);
    ::SDL_UpdateWindowSurface(mWindow);

    // Get input events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            mShouldStop = true;
            break;
        case SDL_KEYUP: 
        case SDL_KEYDOWN:
            setController(event);
            break;
        }
    }
  }

  mVerticalBlank = true;
}

bool Window::shouldStop() {
  return mShouldStop;
}

uint8_t Window::getController(int controllerNumber) {
  assert(controllerNumber >= 0);
  assert(controllerNumber <= 1);
  return mController[controllerNumber];
}

bool Window::clearVerticalBlank() {
  bool result = mVerticalBlank;
  mVerticalBlank = false;
  return result;
}

void Window::setController(SDL_Event& keyEvent) {
    int buttomBit = 0;
    int buttomNumber = -1;

    // Get the buttom bit and number
    switch (keyEvent.key.keysym.scancode) {
    case SDL_SCANCODE_RIGHT:    buttomBit++;
    case SDL_SCANCODE_LEFT:     buttomBit++;
    case SDL_SCANCODE_DOWN:     buttomBit++;
    case SDL_SCANCODE_UP:       buttomBit++;
    case SDL_SCANCODE_RETURN:   buttomBit++;
    case SDL_SCANCODE_RSHIFT:   buttomBit++;
    case SDL_SCANCODE_M:        buttomBit++;
    case SDL_SCANCODE_N:
        buttomNumber = 0;
        break;
    case SDL_SCANCODE_D:        buttomBit++;
    case SDL_SCANCODE_A:        buttomBit++;
    case SDL_SCANCODE_S:        buttomBit++;
    case SDL_SCANCODE_W:        buttomBit++;
    case SDL_SCANCODE_TAB:      buttomBit++;
    case SDL_SCANCODE_LSHIFT:   buttomBit++;
    case SDL_SCANCODE_2:        buttomBit++;
    case SDL_SCANCODE_1:
        buttomNumber = 1;
        break;
    }

    // Set or clear the buttom
    if (buttomNumber != -1) {
        if (keyEvent.type == SDL_KEYDOWN) {
            mController[buttomNumber] |= 1 << buttomBit;
        } else {
            mController[buttomNumber] &= ~(1 << buttomBit);
        }
    }
}

} // namespace sdl
