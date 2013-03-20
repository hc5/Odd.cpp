#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "odd.h"

using namespace std;

string trim(const string &t, const string &ws=" \n\t")
{
    string str = t;
    size_t found;
    found = str.find_last_not_of(ws);
    if (found != string::npos)
    	str.erase(found+1);
    else
    	str.clear();          
    return str;
}

int stoi(string s){
	int i;
	istringstream(s) >> i;
	return i;
}

Move::Move(int player, int color, int x, int y){
	this->color = color;
	this->player = player;
	this->x = x;
	this->y = y;
}

Move* Move::fromString(string in){
	string buf;
	stringstream ss(in);
	vector<string> tok; 
	while(ss >> buf)
	tok.push_back(buf);
	return new Move(stoi(tok[0]) ,tok[1] == "WHITE"?1:2,stoi(tok[2])+4 ,stoi(tok[3])+4);
}


Board::Board(){
	b = new int[MAXSIZE * MAXSIZE];
	for(int i = 0;i<9;i++){
		for(int j = 0;j<9;j++){
			this->set(i,j, (i-j<-4||i-j>4) ? -1 : 0);
		}
	}
}

Client::Client(const char * serverIp,int port,const char* name){
	this->over = 0;
	this->lastMove = NULL;
	this->board = new Board();
	this->sockfd = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	this->sockfd=socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	server = gethostbyname(serverIp);
	bcopy(server->h_addr, &serv_addr.sin_addr, server->h_length);
    serv_addr.sin_port = htons(port);
	cout << "Connecting to server\n";
	if(connect(this->sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0){
		cout << "Error connecting to server\n";
		exit(EXIT_FAILURE);
	}
	string startmsg = "START ";
	startmsg += name;
	startmsg += "\n";
	write(this->sockfd, startmsg.data(), startmsg.length());
	char buf[100];
	memset(buf,0,100);
	while(!this->over){
		if(read(this->sockfd, buf, 100)){
			string msg = string(buf);
			stringstream ss(msg);
			string line;
			while(getline(ss,line))
			this->procMsg(line);
			memset(buf,0,100);
		}
	}
	close(sockfd);
}

void Client::procMsg(string msg){
	if(!msg.find("GAMEOVER")){
		cout << msg << "\n";
		this->over = 1;
	}
	else if(!msg.find("START")){
	this->id = (int)(msg[12]) - 48;
	}
	else if(!msg.find("PLAY")){
		this->playMove();
	}
	else if(!msg.find("1") || !msg.find("2")){
		this->lastMove = Move::fromString(msg);
		this->board->set(this->lastMove->x, this->lastMove->y,  this->lastMove->color);
		cout << "The opponent played " << *(this->lastMove) << "\n";
	}
	return;
}

void Client::playMove(){
	Move m = this->play(this->lastMove, *(this->board));
	stringstream ss;
	ss << m << "\n";
	this->board->set(m.x,m.y, m.color);
	write(this->sockfd, ss.str().data(), ss.str().length());
}

/**
Modify this method for your AI

In the playing board the center is (0,0), which makes the bottom left (-4,-4) and the 
top right (4,4), which is difficult to work with. The board of this python client is 
a 9x9 2d array, and it starts at (0,0) at the bottom left corner, and (8,8) at the top
 right corner which makes it a lot simpler to work with.

When the `last_move` param is given in the shifted coordinate system from (0,0) to 
(8,8), and the `move` tuple you return should also be in the shifted coordinate system.


last_move: Contains the last move that was played by the opponent, if you
are the first to play, last_move will be None

board: Contains get/set methods 0 if the cell is empty, 1 if white, 2 if black, and -1 if the cell
 is invalid. The current state of the board.


returns: a move object that is a valid move. If it's not valid then
you will lose the game and fail the course. 
**/


Move Client::play(Move* lastMove, Board board){
	cout << board << "\n";
	return Move(this->id,WHITE,0,0);
}

int main(int argc, char *argv[])
{
	Client("localhost", 8123, "cppbot");
	
}
