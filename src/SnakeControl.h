#ifndef __SNAKECONTROL__
#define __SNAKECONTROL__

#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include "Util.h"
#include "Map.h"
#include "Snake.h"

using namespace std;

extern bool _signal;
extern int snake_size_goal;
extern int plus_item_goal;
extern int minus_item_goal;
extern int gate_item_goal;

// 알람 신호를 처리하는 함수
void sig_alrm(int signum);

//[아이템 클래스]=========================================================

class Item
{
public:
    int x;         // 아이템의 x 좌표
    int y;         // 아이템의 y 좌표
    bool itemType; // 아이템의 종류: true는 플러스 아이템, false는 마이너스 아이템
    int lifeTime;  // 아이템이 맵에 남아 있는 시간
    Item(int x, int y, bool itemType, int lifeTime = 5) : x(x), y(y), itemType(itemType), lifeTime(lifeTime){};
};

//[Main,Mission,Score board관리 클래스]=========================================================
class Board
{
public:
    WINDOW *main_board, *score_board, *mission_board;
    Board()
    {
        // 색상 기능 초기화 (Ncurses)
        start_color();

        // 메인 보드 설정 (보드 배경과 출력 문자 설정)
        main_board = newwin(30, 60, 5, 10);
        wbkgd(main_board, COLOR_PAIR(2));

        // 점수 보드 설정 (보드 배경과 출력 문자 설정)
        score_board = newwin(15, 40, 5, 73);
        wbkgd(score_board, COLOR_PAIR(1));
        wattron(score_board, COLOR_PAIR(1));
        mvwprintw(score_board, 0, 14, "Score");

        // 미션 보드 설정 (보드 배경과 출력 문자 설정)
        mission_board = newwin(12, 40, 23, 73);
        wbkgd(mission_board, COLOR_PAIR(1));
        wattron(mission_board, COLOR_PAIR(1));
        mvwprintw(mission_board, 0, 14, "Mission");
    };
};

//[Gate관리 클래스]=========================================================
class Gate
{
public:
    Map &map_of_gate;         
    Snake &snake;            
    vector<pair<int, int> > gateList; 
    pair<int, int> &gate1;    
    pair<int, int> &gate2;   
    int &command;             
    int &gate_item_count;     
    Board &board;            
    int &gate_item_goal;     
    string gate_item_mission_str; // 게이트 아이템 미션 문자열
    
    // 생성자: 맵, 뱀, 명령, 게이트 아이템 수, 보드, 게이트 위치, 게이트 아이템 목표 초기화
    Gate(Map &map, Snake &snake, int &command, int &gate_item_count, Board &board, pair<int, int> &gate1, pair<int, int> &gate2, int &gate_item_goal) : map_of_gate(map), snake(snake), command(command), gate_item_count(gate_item_count), board(board), gate1(gate1), gate2(gate2), gate_item_goal(gate_item_goal), gate_item_mission_str("gate:" + to_string(gate_item_goal) + " (V)") {};

    // gate 좌표를 저장
    void syncMap();
    
    // gate좌표를 랜덤으로 선택
    void gateRefresh();

    // gate에서 snake를 감지해서 처리하는 함수들
    void updateCommandForGate(int fromGateX, int fromGateY, int toGateX, int toGateY);
    void handleGateTransition(int gateX, int gateY, int toGateX, int toGateY);
    void gateSensor();
};


//[게임 클래스]=========================================================
class Game
{
public:
    friend class Item;
    vector<Item> itemContainer; 
    Map map;                   
    int count;                  
    Snake snake;               
    Board board;                
    Gate gate;                  
    int speed;                  
    int snake_color;           
    int keyIn, command, gate_count, item_count; 
    pair<int, int> gate1;       
    pair<int, int> gate2;      
    int plus_item_count, minus_item_count, gate_item_count; 

    // 점수와 미션 문자열
    string snake_size_score;
    string plus_item_score;
    string minus_item_score;
    string score_gateCount;
    string snake_size_mission_str;
    string plus_item_mission_str;
    string minus_item_mission_str;
    string gate_item_mission_str;

    // Borad에 쓰이는 값
    Game() : board(), snake(), gate(map, snake, command, gate_item_count, board, gate1, gate2, gate_item_goal), count(0), speed(100000), snake_color(4), keyIn(0), command(2), gate_count(0), item_count(0), plus_item_count(0), minus_item_count(0), gate_item_count(0),
                     snake_size_score("size:" + to_string(snake.body.size())),
                     plus_item_score(":" + to_string(plus_item_count)),
                     minus_item_score("-:" + to_string(minus_item_count)),
                     score_gateCount("gate:" + to_string(gate_item_count)),
                     snake_size_mission_str("size:" + to_string(snake_size_goal) + " ( )"),
                     plus_item_mission_str(":" + to_string(plus_item_goal) + " ( )"),
                     minus_item_mission_str("-:" + to_string(minus_item_goal) + " ( )"),
                     gate_item_mission_str("gate:" + to_string(gate_item_goal) + " ( )") {};
//[인게임]=========================================================
    // 게임 단계별 설정
    void game_control(int arr[][60], int &i);

    // 게임 실행 함수
    void run_game(int m4[][60], int &i);

//[아이템]=========================================================
    // 아이템 먹기 
    void eat_item();

    // 아이템 배치 
    void set_item(Item item);

    // 아이템 제거 
    void remove_item(Item item);

    // board에 아이템을 생성
    void item_setting();

    // 아이템 관리 
    void manage_item(vector<Item> &v);


//[맵]=========================================================

    // 맵 렌더링 -> 맵 초기화 -> 일정간격으로 map초기화
    void render_map();

    // 맵 초기화 
    void clear_map();

//[뱀]=========================================================

    // 뱀 설정
    void set_snake();

//[부가적인 함수]=========================================================

    // 키보드 입력 처리 함수
    int keyboard_input();
    
    // 좌표가 비어있는지 확인하는 함수
    bool is_empty(int x, int y);
};

#endif 