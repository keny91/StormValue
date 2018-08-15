

#ifndef _Game_H_
#define _Game_H_


#include "du.h"
#include "logging.h"

/*
	GameClass is the skeleton of the program.

*/


/*	Remove these as they are implemented	*/
extern class PlayerClass;
typedef PlayerClass* Player;

extern class TeamClass;
typedef TeamClass* Team;

extern class HeroClass;
typedef HeroClass* Hero;


typedef class GameClass
{
public:
	GameClass();
	~GameClass();


	DuList PlayersTeamBlue;
	DuList PlayersTeamRed;

private:

}* Game;







#endif // _Game_H_