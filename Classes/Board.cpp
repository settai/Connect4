#include <iostream>
#include <cstdlib>
#include <string.h>
#include "Board.h"


USING_NS_CC;
using namespace std;


Board::Board(cocos2d::Scene *layer, double rX, double rY)
{
    turn=1;
    for (int i=0;i<6;i++)for(int j=0;j<7;j++) T[i][j]=0;

	//Resolution parameters
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//Board Background
	Sprite* BoardBG = Sprite::create("res/Board.png");
	BoardBG->setPosition(Point(visibleSize.width / 2 + origin.x - visibleSize.width / 33, visibleSize.height / 2 + origin.y));
	BoardBG->setScale(rX, rY);
	layer->addChild(BoardBG,100);


	//Columns areas
	Rect BoardRect = BoardBG->getBoundingBox();
	Columns[0].setRect(BoardRect.origin.x, BoardRect.origin.y, BoardRect.size.width * 0.16F, BoardRect.size.height);
	for (int i = 1; i <= 5; i++)
	{
		Columns[i].setRect(Columns[i - 1].origin.x + Columns[i - 1].size.width, BoardRect.origin.y, BoardRect.size.width*0.132F, BoardRect.size.height);
	}
	Columns[6].setRect(Columns[5].origin.x + Columns[5].size.width, BoardRect.origin.y, BoardRect.size.width * 0.16F, BoardRect.size.height);

	//Origins of cercles
	Cercles[0][0].setPoint(BoardRect.origin.x + BoardRect.size.width * 0.105F, BoardRect.origin.y + BoardRect.size.height * 0.88F);
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 7; j++)
		{
			if (j != 0)
				Cercles[i][j].setPoint(Cercles[i][j - 1].x + BoardRect.size.width * 0.132F, Cercles[i][j - 1].y);
			else if (i != 0)
				Cercles[i][j].setPoint(Cercles[i - 1][j].x, Cercles[i][j - 1].y - BoardRect.size.height * 0.151F);
		}

				
	for (int i = 0; i < 7; i++)
		startPoints[i].setPoint(Cercles[0][i].x, Cercles[0][i].y - (Cercles[1][i].y - Cercles[0][i].y) / 2);




	
}

Board::Board(int Data[6][7])
{
    for (int i=0;i<6;i++)for(int j=0;j<7;j++) T[i][j]=Data[i][j];
}

Board::~Board()
{
    //dtor
}


int Board::emptyPlace(int j)
{
    int i;
    if(T[0][j]!=0) return -1; // return -1 when the column is FULL
    for(i=0;i<5;i++)
        if(T[i+1][j]!=0) // this means when i+1 is FULL, we break and return i (i is above i+1)
            break;
    return i;
}

bool Board::isWin(int i, int j, int pt)
{
	return (countHorizontal(i, j, pt) == 4 || countVertical(i, j, pt) == 4 || countDiagonal1(i, j, pt) == 4 || countDiagonal2(i, j, pt) == 4) ? true : false;
}

int Board::countHorizontal(int i, int j, int pt)
{
	int u, v;
	int counter = 0;
	u = i; v = j;
	while (u >= -1 && u<6 && v>-1 && v<7 && T[u][v++] == pt && counter<4)
		counter++;
	u = i; v = j - 1;
	while (u>-1 && u<6 && v>-1 && v<7 && T[u][v--] == pt && counter<4)
		counter++;
	return counter;
}

int Board::countVertical(int i, int j, int pt)
{
	int u, v;
	int counter = 0;

	u = i; v = j;
	while (u>-1 && u<6 && v>-1 && v<7 && T[u++][v] == pt && counter<4)
		counter++;
	u = i - 1; v = j;
	while (u>-1 && u<6 && v>-1 && v<7 && T[u--][v] == pt && counter<4)
		counter++;
	return counter;
}

int Board::countDiagonal1(int i, int j, int pt)
{
	int u, v;
	int counter = 0;

	u = i; v = j;
	while (u>-1 && u<6 && v>-1 && v<7 && T[u++][v++] == pt && counter<4)
		counter++;
	u = i - 1; v = j - 1;
	while (u>-1 && u<6 && v>-1 && v<7 && T[u--][v--] == pt && counter<4)
		counter++;

	return counter;
}

int Board::countDiagonal2(int i, int j, int pt)
{
	int u, v;
	int counter = 0; // on suppose que T[i][j]==pt

	u = i; v = j;
	while (u>-1 && u<6 && v>-1 && v<7 && T[u++][v--] == pt && counter<4)
		counter++;
	u = i - 1; v = j + 1;

	while (u>-1 && u<6 && v>-1 && v<7 && T[u--][v++] == pt && counter<4)
		counter++;

	return counter;
}

void Board::fill4inRowList(int i, int j, int pt)
{


	if (countHorizontal(i,j,pt) == 4)
	{
		int counter = 0;
		int u, v;
		u = i; v = j;

		while (u >= -1 && u<6 && v>-1 && v < 7 && T[u][v++] == pt && counter < 4)
		{
			fourinRow[counter][0] = u;
			fourinRow[counter][1] = v - 1;
			counter++;
		}
		u = i; v = j - 1;
		while (u>-1 && u<6 && v>-1 && v<7 && T[u][v--] == pt && counter<4)
		{
			fourinRow[counter][0] = u;
			fourinRow[counter][1] = v + 1;
			counter++;
		}

	}
	else if (countVertical(i,j,pt) == 4)
	{ 
		int counter = 0;
		int u, v;
		u = i; v = j;
		while (u>-1 && u<6 && v>-1 && v<7 && T[u++][v] == pt && counter<4)
		{
			fourinRow[counter][0] = u - 1;
			fourinRow[counter][1] = v;
			counter++;
		}
		u = i - 1; v = j;
		while (u>-1 && u<6 && v>-1 && v<7 && T[u--][v] == pt && counter<4)
		{
			fourinRow[counter][0] = u + 1;
			fourinRow[counter][1] = v;
			counter++;
		}

	}

	else if (countDiagonal1(i,j,pt) == 4)
	{ 
		int u, v;
		int counter = 0;

		u = i; v = j;
		while (u>-1 && u<6 && v>-1 && v<7 && T[u++][v++] == pt && counter<4)
		{
			fourinRow[counter][0] = u - 1;
			fourinRow[counter][1] = v - 1;
			counter++;
		}
		u = i - 1; v = j - 1;
		while (u>-1 && u<6 && v>-1 && v<7 && T[u--][v--] == pt && counter<4)
		{
			fourinRow[counter][0] = u + 1;
			fourinRow[counter][1] = v + 1;
			counter++;
		}
	}

	else if (countDiagonal2(i, j, pt) == 4)
	{

		int u, v;
		int counter = 0; 

		u = i; v = j;
		while (u>-1 && u<6 && v>-1 && v<7 && T[u++][v--] == pt && counter<4)
		{
			fourinRow[counter][0] = u - 1; 
			fourinRow[counter][1] = v + 1; 
			counter++;
		}
		u = i - 1; v = j + 1;

		while (u>-1 && u<6 && v>-1 && v<7 && T[u--][v++] == pt && counter<4)
		{
			fourinRow[counter][0] = u + 1;
			fourinRow[counter][1] = v - 1;
			counter++;
		}
	}
}


void Board::copyBoard(int tab[6][7]){

    for (int i=0;i<6;i++)for(int j=0;j<7;j++) tab[i][j] = T[i][j];

}



void Board::evalH(int i, int j, int &score, int pt)
{
    int p1t=pt%2+1, p2t=p1t%2+1;
    int cp1=0,cp2=0;
    for(int k=0;k<4;k++)
    {
        if(T[i][j+k]==p2t) cp2++;
        if(T[i][j+k]==p1t) cp1++;
    }
    editScore(score,cp1,cp2);
}

void Board::evalV(int i, int j, int &score, int pt)
{
    int p1t=pt%2+1, p2t=p1t%2+1;
    int cp1=0,cp2=0;
    for(int k=0;k<4;k++)
    {
        if(T[i+k][j]==p2t) cp2++;
        if(T[i+k][j]==p1t) cp1++;
    }
    editScore(score,cp1,cp2);
}

void Board::evalD1(int i, int j, int &score, int pt)
{
    int p1t=pt%2+1, p2t=p1t%2+1;
    int cp1=0,cp2=0;
    for(int k=0;k<4;k++)
    {
        if(T[i+k][j+k]==p2t) cp2++;
        if(T[i+k][j+k]==p1t) cp1++;
    }
    editScore(score,cp1,cp2);
}

void Board::evalD2(int i, int j, int &score, int pt)
{
    int p1t=pt%2+1, p2t=p1t%2+1;
    int cp1=0,cp2=0;
    for(int k=0;k<4;k++)
    {
        if(T[i-k][j+k]==p2t) cp2++;
        if(T[i-k][j+k]==p1t) cp1++;
    }
    editScore(score,cp1,cp2);
}


void Board::editScore(int&score, int cp1, int cp2)
{
    if(cp2==0)
    {
        switch(cp1)
        {
        case 1:
            score+=1;
            break;
        case 2:
            score+=10;
            break;
        case 3:
            score+=150;
        }
    }
    if(cp1==0)
    {
        switch(cp2)
        {
        case 1:
            score-=1;
            break;
        case 2:
            score-=10;
            break;
        case 3:
            score-=150;
        }
    }
}


int Board::heuristic(int pt)
{
    int score=0;
    for(int i=0; i<6;i++)
        for(int j=0; j<4;j++)
        {
            evalH(i,j, score, pt);
        }
    for(int i=0; i<3;i++)
        for(int j=0; j<7;j++)
        {
            evalV(i,j, score, pt);
        }
    for(int i=0; i<3;i++)
        for(int j=0; j<4;j++)
        {
            evalD1(i,j, score, pt);
        }
    for(int i=3; i<6;i++)
        for(int j=0; j<4;j++)
        {
            evalD2(i,j, score, pt);
        }
    //cout<<score<<endl;

	if (pt == 2) score += 16;
	else score -= 16;

    return score;
}
