#include <iostream>
#include <conio.h> // For _kbhit() and _getch()
#include <windows.h> // For Sleep() and console manipulation
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

bool Gameover;
int width= 10;
int height= 10;
int level= 300;
int dir, length;


class position{
	public:
	int x;
	int y;
	bool operator == (position p);
};

class Snake{
	public:
		vector<position> body;
};

class Food{
	public:
		position fd;
		void generation(Snake snake);
};

class Game: public position{
	public:
		position p;
		Snake snake;
		Food food;
		int mode;
		
		Game();
		void setDefault();
		void draw();
		void Input();
		void Update();
		void IsGameOver();
		void Catch();
		void run();
		void option();
		void ShowMenu();
		void AutoPlay();
		int AutoInput();
		void ShowInstructions();
};

void Food::generation(Snake snake){
	bool onSnake;
	do{
		onSnake= false;
		
		fd.x= rand()% width;
		fd.y= rand()% height;
		
		for(int i= 0; i< length; i++){
			if( snake.body[i]== fd) {
				onSnake= true;
				break;
			}
		}
	}while(onSnake);
}

bool position::operator == (position p){
	if(this->x==p.x && this->y== p.y) return true;
	else return false;
}

Game::Game(){
	srand(time(0));
	Gameover= false;
	food.generation(snake);
	p.x= width/ 2;
	p.y= height/ 2;
	snake.body.push_back(p);
	dir= 1;
	length= 1;
}

void Game::setDefault(){
	Gameover= false;
	snake.body.clear();
	p.x= width/ 2;
	p.y= height/ 2;
	snake.body.push_back(p);
	food.generation(snake);
	dir= 1;
	length= 1;
}

void Game::option(){
	int n;
	cout<<endl<<endl<<"Options";
	cout<<endl<<"Please Input Width(10~50):"; cin>>width;
	cout<<"Please Input Height(10~25):"; cin>>height;
	cout<<"Please Input Level(1~3):"; cin>>n;
	if(n== 1) level= 500;
	else if(n== 2) level= 300;
	else level= 100;
	cout<<"Do You Want To Play Or Automode?(Y/A):";
	char ch= _getch();
	if( ch== 'y') run();
	else AutoPlay();
}


/*int Game::AutoInput(){
	position head = snake.body[0];
    position pos = food.fd;
	
	int best, ax, ay;
	int pd[4]= {1, 2, 3, 4};
	int pm[4][2]= {{ head.x+ 1, head.y} ,{ head.x- 1, head.y} ,{ head.x, head.y - 1} ,{ head.x, head.y+ 1} };
	int vd[4], index= 0;
    for (int i= 0; i< 4; i ++) {
        if (pm[i][0] < 0 || pm[i][0] >= width || 
            pm[i][1] < 0 || pm[i][1] >= height) {
            continue;
        }
        
        // Check body collision (skip head)
        bool collision = false;
        for (size_t j = 1; j < length; j++) {
            if ( pm[i][0] == snake.body[j].x && pm[i][1] == snake.body[j].y) {
                collision = true;
            }
        }
        if (!collision) {
            vd[index]= pd[i];
            index++;
        }
    }
    
    if(index== 0) return dir;
    
    if( head.x > pos.x ) best= 2;
    else if( head.x < pos.x ) best= 1;
    else {
    	if( head.y > pos.y ) best= 3;
    	else best= 4;
	}
	
	for(int i= 0; i< index; i++){
		if( best== vd[i])  return best;
	}
	
	for(int i= 0; i< length; i++){
		ax += pm[i][0];
        ay += pm[i][1];
	}
	
	ax /= length;
    ay /= length;
    
    cout<<endl<<ax<<" "<<ay;
    
	cout<<endl<<"valid direction:"<<index<<endl;
	return vd[index- 1];
}*/

int Game::AutoInput() {
    position head = snake.body[0];
    position food_pos = food.fd;
    
    // Possible directions and their coordinates
    const int directions[4] = {1, 2, 3, 4}; // Right, Left, Up, Down
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    
    // Find all valid directions (no wall/body collision)
    vector<int> valid_directions;
    for (int i = 0; i < 4; i++) {
        int new_x = head.x + dx[i];
        int new_y = head.y + dy[i];
        
        // Check wall collision
        if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
            continue;
        }
        
        // Check body collision (skip head)
        bool collision = false;
        for (size_t j = 1; j < snake.body.size(); j++) {
            if (new_x == snake.body[j].x && new_y == snake.body[j].y) {
                collision = true;
                break;
            }
        }
        
        if (!collision) {
            valid_directions.push_back(directions[i]);
        }
    }
    
    // If no valid directions, return current direction
    if (valid_directions.empty()) {
        return dir;
    }
    
    // Determine best direction towards food
    int best_dir = -1;
    if (head.x > food_pos.x && find(valid_directions.begin(), valid_directions.end(), 2) != valid_directions.end()) {
        best_dir = 2; // Left
    } 
    else if (head.x < food_pos.x && find(valid_directions.begin(), valid_directions.end(), 1) != valid_directions.end()) {
        best_dir = 1; // Right
    } 
    else if (head.y > food_pos.y && find(valid_directions.begin(), valid_directions.end(), 3) != valid_directions.end()) {
        best_dir = 3; // Up
    } 
    else if (head.y < food_pos.y && find(valid_directions.begin(), valid_directions.end(), 4) != valid_directions.end()) {
        best_dir = 4; // Down
    }
    
    // If best direction found, use it
    if (best_dir != -1) {
        return best_dir;
    }
    
    // If no direct path to food, choose first valid direction (could be improved)
    return valid_directions[0];
}


void Game::AutoPlay(){
	while(!Gameover){
		dir= AutoInput();
		Update();
		draw();
		Sleep(level);
	}
}

void Game::Input(){
	if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                if (dir != 1) // Prevent moving directly opposite
                    dir = 2;
                break;
            case 'd':
                if (dir != 2)
                    dir = 1;
                break;
            case 'w':
                if (dir != 4)
                    dir = 3;
                break;
            case 's':
                if (dir != 3)
                    dir = 4;
                break;
            case 'q':
            	Gameover= true;
            	break;
        }
    }
}

void Game::Catch(){
	if( snake.body[0]== food.fd) {
		length++;
		food.generation(snake);
		snake.body.push_back(food.fd);
	}
}

void Game::Update(){
	for(int i= length- 1; i> 0; i--) snake.body[i]= snake.body[i-1];
	switch(dir){
		case 1:
			snake.body[0].x++;
			break;
		case 2:
			snake.body[0].x--;
			break;
		case 3:
			snake.body[0].y--;
			break;
		case 4:
			snake.body[0].y++;
			break;
	}
	this->Catch();
	this->IsGameOver();
}

void Game::draw(){
	COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	
	char h= 1, f= 3;
	
	
	for(int i= 0; i< width+ 2; i++) cout<<"*";
	cout<<endl;
	for(int i= 0; i< height; i++) {
		cout<<"*";
		for(int j= 0; j< width; j++) {
			int flag= 0;
			if( snake.body[0].x== j && snake.body[0].y== i) cout<<h;
			else if( i== food.fd.y && food.fd.x== j){
				cout<<f;
				flag++;
			}
			else{
				for(int num= 1; num< length; num++){
					if( snake.body[num].x== j && snake.body[num].y== i ) {
						cout<<"o";
						flag++;
					}	
				}
				if(flag== 0) cout<<" ";
			}
		}
		cout<<"*"<<endl;
	}
	for(int i= 0; i< width+ 2; i++) cout<<"*";
	
	cout<<endl<<"Directions"<<dir<<endl;
	cout<<"Your score is "<<length<<endl;
	cout<<"Food is on ( "<<food.fd.x<<", "<<food.fd.y<<" )"<<endl;
	cout<<"Current is ( "<<snake.body[0].x<<", "<<snake.body[0].y<<" )";
}

void Game::IsGameOver(){
	if( snake.body[0].x < 0 || snake.body[0].x >= width || snake.body[0].y < 0 || snake.body[0].y >= height) {
		Gameover= true;
		return;
	}
	for(int i= 1; i< length; i++) if( snake.body[0]== snake.body[i] ) {
		Gameover= true;
		return;
	}
}

void Game::run(){
	while(!Gameover){
		Input();
		Update();
		draw();
		Sleep(level);
	}
}

void Game::ShowInstructions(){
	cout<<endl<<"KeyBoard:"<<endl<<"	Up: W	Left: A 	 Right: D	Down: S 	Exit:X"<<endl;
	Sleep(3000);
}
void Game::ShowMenu() {
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow
    
	cout<<endl<<endl<<"       Enjoy With My"<<endl;
	cout<<"~-.,_,.-~-.,_,.-~ S N A K E ~-.,_,.-~-.,_,.-~";
	cout<< endl << endl;
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // Cyan
    cout << "1. Play Game" << endl;
    cout << "2. Auto Play" << endl;
    cout << "3. Instructions" << endl;
    cout << "4. Options"<< endl;
    cout << "5. Exit" << endl << endl;
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // White
    cout << "Select an option: ";
    
    mode= _getch() - '0'; // Convert char to int
}

int main(){
	Game g;
	while(1){
		g.ShowMenu();
		switch(g.mode){
			case 1:
				system("cls");
				g.run();
				break;
			case 3:
				g.ShowInstructions();
				break;
			case 2:
				system("cls");
				g.AutoPlay();
				break;
			case 4:
				g.option();
				break;
			case 5:
				cout<<endl<<endl<<"Thank You For Visiting Me!!!";
				Sleep(3000);
				exit(0);
		}
		g.setDefault();
	}
	return 0;
}
