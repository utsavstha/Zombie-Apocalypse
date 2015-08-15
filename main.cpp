#include "MainGame.h"
#include <iostream>
int main(int argc, char* argv[]) {

	std::cout<<" Welcome to Zombie Apocalypse\n Kill all zombies and save the earth\n Zombies can affect the humans and turn them into zombies\n";
	std::cout<<"\n\n Humans are purple.\n Zombies are green.\n\n";
	std::cout<<"\n\n pPess 1, 2 and 3 to switch the guns\n";
	std::cout<<"\n\n Shoot the zombies using left mouse button\n\n";
	int a;
	std::cout<<"\n\npress any key and hit enter to Start the game\n";
	std::cin>>a;
    MainGame mainGame;
    mainGame.run();

    return 0;
}