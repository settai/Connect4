#include "cocos2d.h"

USING_NS_CC;

#include "Player.h"

class Board
{
    public:
        Board(cocos2d::Scene *layer, double rX, double rY);
        Board(int Data[6][7]);
        ~Board();

         void copyBoard(int tab[6][7]);

        int emptyPlace(int);
        bool isWin(int, int, int);
        int countHorizontal(int i, int j, int pt);
        int countVertical(int i, int j, int pt);
        int countDiagonal1(int i, int j, int pt);
        int countDiagonal2(int i, int j, int pt);
		void fill4inRowList(int i, int j, int pt);


        void editScore(int&score, int cp1, int cp2);
		void evalH (int i, int j, int &score, int pt);
		void evalV (int i, int j, int &score, int pt);
		void evalD1(int i, int j, int &score, int pt);
		void evalD2(int i, int j, int &score, int pt);
        int heuristic(int pt);

        void nextTurn(){
        turn++;
        }

    private:
        int T[6][7];		//Game table
		int fourinRow[4][2];//the connected line of the winner
        int turn;			// Total of played tours.
		Rect Columns[7];	//Columns areas
		Vec2 Cercles[6][7]; //Origins of cercles
		Vec2 startPoints[7]; //Strating points from which balls fall

		// Players 
		Player player;
		PlayerAI CPU;

		// Screen resolution parameters
		Size visibleSize;
		Point origin;

    friend class playerAI;
	friend class GameplayScene;

};


