#include "engine.hpp"



int sdl_init(SDL_Window **w);
void sdl_cleanup();



#ifdef _MSC_VER
#define snprintf _snprintf
#elif !defined(__MINGW32__)
#include <iconv.h>
#endif



int main(int argc, char *argv[])
{
    Engine e;
    return e.Run();
}
