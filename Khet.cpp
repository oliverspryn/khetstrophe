/*  
*   Members: Garrett Young, Nick Tallents, Oliver Spryn, Zac Stahl
*   Course: 322, Obj. Oriented / Adv. Programming
*   Date: 04 Dec 2013 
*   Description: This file implements the Khet class which is the main engine for our game  
*/

#include "Khet.h"

Khet::Khet() : activeSelected(false), step(1), turn('g') {
}

Khet::~Khet() {
	releaseAll();
}

void Khet::ai() {

}

void Khet::collisions() {
	D3DXVECTOR2 v;
	Tile* t;

	for(int i = 0; i < 10; ++i) {
		for(int j = 0; j < 8; ++j) {
			t = (*grid)[i][j];

			if (this->l->collidesWith(*t, v)) {
				t->setActive(false);
				t->setVisible(false);
				this->l->destroy();
			}
		}
	}
}

void Khet::callback(ClickData<KhetPiece> d) {
	char color = d.tile->getColor();

	if (color == turn) {

	} else {
		MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
	}
}

void Khet::initialize(HWND hwnd) {
	Game::initialize(hwnd);

	if (!numGridTexture.initialize(graphics,NUM_GRID))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing numGrid texture"));
    if (!numGrid.initialize(graphics,0,0,0,&numGridTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing numGrid"));

//Create the Laser
	l = new Laser(this, graphics, Laser::COLOR_RED);
	l->initialize();

//Create the GridParser
	this->kp = new KhetParser<KhetPiece>("config\\classic.txt");

//Create the Grid
	this->g = new Grid<KhetPiece, X, Y>(this, graphics);
	this->g->setBkg("pictures\\GameBoard.png");
	this->g->initialize(this->kp);

	this->grid = this->g->getT();
}

void Khet::releaseAll() {
	Game::releaseAll();
}

void Khet::render() {
	graphics->spriteBegin();
	this->g->draw();
	l->draw();	
    graphics->spriteEnd();   
}

void Khet::resetAll() {
	Game::resetAll();
}

void Khet::update() {
//Update stuff
	l->update(frameTime);
	this->g->update(frameTime);

//User clicked
	if (this->input->getMouseLButton()) {
		int mouseX = this->input->getMouseX();
		int mouseY = this->input->getMouseY();

		int height = Y * gridNS::HEIGHT;
		int width = X * gridNS::WIDTH;

	//What tile was clicked on?
		active.x = mouseX / gridNS::WIDTH;
		active.y = mouseY / gridNS::HEIGHT;
		active.tile = (*grid)[active.x][active.y];

		if (active.tile->getColor() == turn && active.tile->getActive()) {
			step = 2;
			activeSelected = true;
		} else {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		}

		
	}

//Move or rotate
	if (activeSelected) {
		if (input->isKeyDown('1')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('2')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('3')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('4')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('6')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('7')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('8')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown('9')) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown(VK_LEFT)) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		} else if (input->isKeyDown(VK_RIGHT)) {
			MessageBox(NULL, "This isn't your piece", "Error", MB_OK);
		}
	}
}