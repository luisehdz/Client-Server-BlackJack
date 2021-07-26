#pragma once
#include<string>

class BlackJack
{
public:

	std::string play();
	std::string logic();
	std::string score(int points);
	std::string deal();

	void shuffle();
	void setup();
	void dealS(std::string &output);
	void reset();
	const int getScore();

	bool hasWon = false;;
	bool hasLost = false;
	bool hasPlayed = false;

private:
	int points = 0;
	const int shuffleAmount = 666;
	const int totalCards = 52;
};

