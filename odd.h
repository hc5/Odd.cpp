#include <string.h>
#include <vector>
#include <ostream>
#define INVALID -1
#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define MAXSIZE 9
class Move{

	int player;
public:
	int color;
	int x;
	int y;
	Move(int,int,int,int);
	friend std::ostream& operator<<(std::ostream& out, const Move& m){
		out<<m.player<<" "<<(m.color==WHITE?"WHITE":"BLACK")<<" "<<m.x-4<<" "<<m.y-4<<"\n";
		return out;
	}
	static Move* fromString(std::string);
};

class Board{
	int * b;
public:
	Board();
	friend std::ostream& operator<<(std::ostream& out, const Board& board){
		std::string buf = std::string("");
		for(int i = 4 ;i>-5;i--){
			buf += std::string(4-i+(i>0?i:0), ' ');
			for(int j = -4;j<5;j++){
				switch(board.b[(j+4) * MAXSIZE+i+4]){
				case -1:
					buf += " ";
					break;
				case 0:
					buf += "+ ";
					break;
				case 1:
					buf += "O ";
					break;
				case 2:
					buf += "@ ";
					break;
				}
			}
			buf+= "\n";
		}
		out << buf;
		return out;
	}
	void set(int x, int y, int c){
		this->b[x*MAXSIZE + y] = c;
	}
	int get(int x, int y){
		return this->b[x*MAXSIZE + y];
	}
};

class Client{
	
	Board* board;
	int id;
	int over;
	int sockfd;
	void procMsg(std::string);
	void playMove();
	Move* lastMove;
	Move play(Move*, Board);
public:
	Client(const char *,int,const char*);
};


