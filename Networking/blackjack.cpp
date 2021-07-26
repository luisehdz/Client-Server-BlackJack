//Modified from this forum: http://www.cplusplus.com/forum/beginner/162557/

#include<iostream>
#include<ctime>
#include "blackjack.h"

using namespace std;

struct Card
{
	string suit;
	string rank;
	int pointValue;
	bool inDeck;
}Deck[53];

void BlackJack::shuffle()
{
	srand(unsigned(time(0)));
	for (int i = 0; i < shuffleAmount; i++)
	{
		int a = rand() % totalCards;
		int b = rand() % totalCards;

		Deck[52].suit = Deck[a].suit;
		Deck[52].rank = Deck[a].rank;
		Deck[52].pointValue = Deck[a].pointValue;
		Deck[52].inDeck = Deck[a].inDeck;

		Deck[a].suit = Deck[b].suit;
		Deck[a].rank = Deck[b].rank;
		Deck[a].pointValue = Deck[b].pointValue;
		Deck[a].inDeck = Deck[b].inDeck;

		Deck[b].suit = Deck[52].suit;
		Deck[b].rank = Deck[52].rank;
		Deck[b].pointValue = Deck[52].pointValue;
		Deck[b].inDeck = Deck[52].inDeck;
	}
}

void BlackJack::setup()
{
	int suitNum = 0;
	string suits[4] = { "Clubs", "Hearts", "Spades", "Diamonds" };
	string ranks[13] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
	int pointsValue[13] = { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
	string status[3] = { "In Play", "Deck", "Discard" };

	for (int i = 0; i < totalCards; i++)
	{
		Deck[i].suit = suits[suitNum];
		Deck[i].rank = ranks[i % 13];
		Deck[i].pointValue = pointsValue[i % 13];
		Deck[i].inDeck = true;
		if ((i + 1) % 13 == 0)
		{
			suitNum++;
		}
	}

	shuffle();
}

string BlackJack::play()
{
	reset();
	string output = "shuffling...\n";
	if (!hasPlayed)
	{
		setup();
		hasPlayed = true;
	}
	dealS(output);
	dealS(output);

	return output;
}

string BlackJack::logic()
{
	points = 0;

	for (int i = 0; i < totalCards; i++)
	{
		if (Deck[i].inDeck == false)
		{
			points += Deck[i].pointValue;
		}
	}

	string output = score(points);
	cout << points << endl;

	if (points > 21)
	{
		output.append("You Lose, Replay? (Y/N) ");
		hasLost = true;
	}
	else if (points == 21)
	{
		output.append("You Win! Replay? (Y/N) ");
		hasWon = true;
	}
	else
	{
		output.append("Draw Another Card? (Y/N) ");
	}

	return output;
}

string BlackJack::score(int points)
{
	return "Your total score is " + to_string(points) + "\n";
}

string BlackJack::deal()
{
	int randCard = rand() % totalCards;
	while (Deck[randCard].inDeck == false)
	{
		randCard = rand() % totalCards;
	}
	Deck[randCard].inDeck = false;
	cout << "Card Dealt" << endl;
	return "You have been dealt a " + Deck[randCard].rank + " of " + Deck[randCard].suit + "\n";
}

void BlackJack::dealS(string &output)
{
	int randCard = rand() % totalCards;
	while (Deck[randCard].inDeck == false)
	{
		randCard = rand() % totalCards;
	}
	Deck[randCard].inDeck = false;
	cout << "Card Dealt" << endl;
	output.append("You have been dealt a " + Deck[randCard].rank + " of " + Deck[randCard].suit + "\n");
}

void BlackJack::reset()
{
	points = 0;
	hasLost = false;
	hasWon = false;
	for (int i = 0; i < totalCards; i++)
	{
		Deck[i].inDeck = 1;
	}
	shuffle();
}

const int BlackJack::getScore()
{
	return points;
}