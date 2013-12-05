/*  
*   Members: Garrett Young, Nick Tallents, Oliver Spryn, Zac Stahl
*   Course: 322, Obj. Oriented / Adv. Programming
*   Date: 04 Dec 2013 
*   Description: This file implements the Khet class which is the main engine for our game  
*/

#include "Khet.h"

Khet::Khet() : activeSelected(false), step(1), turn('g') {
    clickedThisFrame=false;
    gamestate = PIECES;
}

Khet::~Khet() {
	releaseAll();
}

void Khet::ai() {

}

void Khet::collisions() {
	D3DXVECTOR2 v;
	KhetPiece* t;

	for(int i = 0; i < 10; ++i) {
		for(int j = 0; j < 8; ++j) {
			t = (*grid)[i][j];
			if (t->collidesWith(*l, v)) {
				RESPONSE r;
				if (t->collidesWithLeft(*l, v))
					r = t->onCollision(0);
				else if (t->collidesWithTop(*l, v))
					r = t->onCollision(1);
				else if (t->collidesWithRight(*l, v))
					r = t->onCollision(2);
				else if (t->collidesWithBottom(*l, v))
					r = t->onCollision(3);

				switch(r) {
				case UP:
					l->changeDirDeg(90);
					break;
				case DOWN:
					l->changeDirDeg(270);
					break;
				case LEFT:
					l->changeDirDeg(180);
					break;
				case RIGHT:
					l->changeDirDeg(0);
					break;
				case DESTROY:
					t->setActive(false);
					t->setVisible(false);
				case NOTHING:
					this->l->destroy();
				}
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

	if (!instructionsTexture.initialize(graphics,INSTRUCTIONS_IMG))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing instructions texture"));
    if (!rules.initialize(graphics,0,0,0,&instructionsTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing rules"));

    if (!piecesTexture.initialize(graphics,PIECES_IMG))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing numGrid texture"));
    if (!pieceInfo.initialize(graphics,0,0,0,&piecesTexture))
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
	switch(gamestate) {
    case PIECES:
        pieceInfo.draw();
        break;
    case INSTRUCTIONS:
        rules.draw();
        break;
    case PLAY:
        this->g->draw();
	    l->draw();
        break;
    case END:
    default:
        break;
    }
    graphics->spriteEnd();   
}

void Khet::resetAll() {
	Game::resetAll();
}

void Khet::update() {
switch(gamestate) {
case PIECES:
    if(this->input->getMouseLButton() && clickedThisFrame == false) {
        gamestate = INSTRUCTIONS;
        clickedThisFrame = true;
    }
    break;
case INSTRUCTIONS:
    if (this->input->getMouseLButton() && clickedThisFrame == false) {
        gamestate = PLAY;
        clickedThisFrame = true;
    }
    break;
case PLAY:
    //Update stuff
	    l->update(frameTime);
	    this->g->update(frameTime);

    //User clicked
	    if (this->input->getMouseLButton() && clickedThisFrame == false) {
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
            clickedThisFrame = true;
	    }
        if (!this->input->getMouseLButton()) {
            clickedThisFrame = false;
        }

    //Move or rotate
        if (activeSelected) {
		    if (input->isKeyDown('1') && canSwap(-1, -1)) {
			    g->swap(active.x, active.y, active.x-1, active.y-1);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('2') && canSwap(0,-1)) {
			    g->swap(active.x, active.y, active.x, active.y-1);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('3') && canSwap(1,-1)) {
			    g->swap(active.x, active.y, active.x+1, active.y-1);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('4') && canSwap(-1,0)) {
			    g->swap(active.x, active.y, active.x-1, active.y);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('6') && canSwap(1,0)) {
			    g->swap(active.x, active.y, active.x+1, active.y);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('7') && canSwap(-1,1)) {
			    g->swap(active.x, active.y, active.x-1, active.y+1);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('8') && canSwap(0,1)) {
			    g->swap(active.x, active.y, active.x, active.y+1);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown('9') && canSwap(1,1)) {
			    g->swap(active.x, active.y, active.x+1, active.y+1);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown(VK_LEFT)) {
				active.tile->rotate(0);
			    activeSelected = false;
			    step = 3;
		    } else if (input->isKeyDown(VK_RIGHT)) {
				active.tile->rotate(1);
			    activeSelected = false;
			    step = 3;
		    }
	    }

    //Fire!!

	    if (!activeSelected && step == 3) {
		    int shootX, shootY;	
		    if (turn == 'g') {
			    shootX = X-1;
			    shootY = Y-1;
		    }
		    else {
			    shootX = 0;
			    shootY = 0;
		    }

		    switch((*grid)[shootX][shootY]->getOrientation())
		    {
		    case 0: //left
			    l->fireDeg((*grid)[shootX][shootY]->getCenterX()-32, (*grid)[shootX][shootY]->getCenterY(), 180.0f);
			    break;
		    case 1: //up
			    l->fireDeg((*grid)[shootX][shootY]->getCenterX(), (*grid)[shootX][shootY]->getCenterY()-40, 90.0f);
			    break;
		    case 2: //right
			    l->fireDeg((*grid)[shootX][shootY]->getCenterX()+32, (*grid)[shootX][shootY]->getCenterY(), 0.0f);
			    break;
		    case 3: //down
			    l->fireDeg((*grid)[shootX][shootY]->getCenterX(), (*grid)[shootX][shootY]->getCenterY()+32, 270.0f);
		    }
		    //change player
		    if(turn == 'g')
			    turn = 'r';
		    else
			    turn = 'g';

		    step = 1;
	    }
        break;
case END:
default:
    PostQuitMessage(0);
    break;
    }


if (!this->input->getMouseLButton()) {
        clickedThisFrame = false;
    }
}

bool Khet::canSwap(int x, int y) {
    if(active.tile->getName() == 'X') // pharoahs can't move
        return false;
    
    KhetPiece* current;    
    if(active.x + x >= 0 && active.x + x < X && active.y + y >= 0 && active.y + y < Y) {
       current = (*grid)[active.x+x][active.y+y];
       if(!current->getActive()) { //tile is empty and tile's color
            return true;
       }
       else if(active.tile->getName() == 'S') {
           switch(current->getName()) {
           case 'P': // Pyramid
           case 'A': //anubis
               return true;
               break;
           case 'X': // sphinx
           case 'H': //pharoah
           case 'S': //Scarab
           default:
               return false;
               break;
           }
       }
    }

    return false;
}     