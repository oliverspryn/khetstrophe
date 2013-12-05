#ifndef KHET_H             // Prevent multiple definitions if this 
#define KHET_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "Grid.h"
#include "KhetParser.h"
#include "Laser.h"

class Khet : public Game
{
private:


public:
    // Constructor
    Khet();
    // Destructor
    virtual ~Khet();
    // Initialize the game
    void initialize(HWND hwnd);

    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "

    void releaseAll();
    void resetAll();

	void restart();
};

#endif