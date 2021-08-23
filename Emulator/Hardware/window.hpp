#ifndef HEADER_WINDOW_HPP
#define HEADER_WINDOW_HPP

#include <cstdint>

#include <string>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Surface SDL_Surface;
typedef union SDL_Event SDL_Event;

namespace Hardware
{

std::uint32_t getPixelColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0);

class Window {
public:
  Window(std::string name,
         std::size_t window_width,
         std::size_t window_height,
         std::size_t draw_width,
         std::size_t draw_height,
         bool enabled);
  ~Window();

  // Drawing methods
  void setPixel(uint32_t data, size_t x, size_t y);

  // Show the current surface state
  void showFrame();

  bool shouldStop();

  uint8_t getController(int controllerNumber);

  // Clear the vertical blank and get its last state
  bool clearVerticalBlank();

private:
  void setController(SDL_Event& keyEvent);

  // Window sdl components
  SDL_Window* mWindow;
  SDL_Surface* mWindowSurface;
  SDL_Surface* mDrawSurface;

  // Control variable
  bool mEnabled;

  // If the application should stop
  bool mShouldStop;

  // The controller state
  uint8_t mController[2];

  // Indicate if the a frame was displayed
  bool mVerticalBlank;
};

} // namespace sdl

#endif // HEADER_WINDOW_HPP