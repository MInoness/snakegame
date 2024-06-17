#include "SnakeControl.h"

bool _signal = true;

int snake_size_goal;    
int plus_item_goal;
int minus_item_goal;
int gate_item_goal = 1; // 게이트 미션 고정

void sig_alrm(int signum)
{
    _signal = false;
}


//[게임 클래스 멤버함수 구현]=================================================================================================================

//게임 단계별 설정 - 단계가 올라갈수록 속도 빨라짐
void Game::game_control(int arr[][60], int &i)
{
    if (i == 0) {
        snake_color = 3;
        speed = 50000;
        snake_size_goal = 4;
        plus_item_goal = 2;
        minus_item_goal = 2;
    } else if (i == 1) {
        snake_color = 6;
        speed = 40000;
        snake_size_goal = 4;
        plus_item_goal = 2;
        minus_item_goal = 2;
    } else if (i == 2) {
        snake_color = 5;
        speed = 30000;
        snake_size_goal = 5;
        plus_item_goal = 2;
        minus_item_goal = 2;
    } else if (i == 3) {
        snake_color = 4;
        speed = 25000;
        snake_size_goal = 5;
        plus_item_goal = 2;
        minus_item_goal = 2;
    }
    //초기값 설정
    snake_size_score = "size:3";
    plus_item_score = "+:0";
    minus_item_score = "-:0";

    plus_item_count = 0;
    minus_item_count = 0;
    gate_item_count = 0;

    snake = Snake();
    signal(SIGALRM, sig_alrm);
    command = 2;
    run_game(arr, i);
}

//게임 실행
void Game::run_game(int m4[][60], int &i)
{
    //보드에 문자를 출력하는 부분
    string level = "level " + to_string(i + 1);
    mvwprintw(board.score_board,3,5,level.c_str());
    mvwprintw(board.score_board, 5, 5, snake_size_score.c_str());
    mvwprintw(board.score_board, 6, 5, plus_item_score.c_str());
    mvwprintw(board.score_board, 7, 5, minus_item_score.c_str());
    mvwprintw(board.score_board, 8, 5, score_gateCount.c_str());

    mvwprintw(board.mission_board, 5, 5, snake_size_mission_str.c_str());
    mvwprintw(board.mission_board, 6, 5, plus_item_mission_str.c_str());
    mvwprintw(board.mission_board, 7, 5, minus_item_mission_str.c_str());
    mvwprintw(board.mission_board, 8, 5, gate_item_mission_str.c_str());

    //map복사
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            map.m[i][j] = m4[i][j];
        }
    }

    //gate좌표들 pair로 저장하고 그 중에서 두개 뽑아서 생성
    gate.syncMap();
    gate.gateRefresh(); 

    while (true)
    {
        //일정 주기(GATE_CYCLE)마다 게이트 갱신
        if (gate_count == GATE_CYCLE)
        {
            gate_count = -50;
            gate.gateRefresh();
        }

        item_setting();

        _signal = true;

        ualarm(speed, 0); //타이머를 설정해서 speed값을 조정, 게임 속도를 제어한다.
        
        //signal = true면 키보드입력을 받는다
        while (_signal) 
        { 
            keyIn = keyboard_input();
            command = keyIn != 0 ? keyIn : command; //키보드 입력이 없으면 상태유지
        }
        //snake움직이기
        snake.move(command);

        //snake 머리가 몸에 부딪힌 경우
        if (map.m[snake.body[0][0]][snake.body[0][1]] == SNAKE_BODY)
        {
            break;
        }

        //snake 머리가 벽에 부딪힌 경우
        if (map.m[snake.body[0][0]][snake.body[0][1]] == WALL)
        {
            break;
        }
        
        //아이템을 먹은 경우
        eat_item();

        //gate에 들어간경우
        gate.gateSensor();

        //몸 크기 설정
        if (snake.body.size() < 3 || snake.body.size() > 10)
        {
            break;
        }

        render_map();

        //조건을 만족했을때 다음 level로 업데이트
        if (snake.body.size() >= snake_size_goal && plus_item_count >= plus_item_goal && minus_item_count >= minus_item_goal && gate_item_count >= gate_item_goal)
        {
            i++;
            return;
        }
    }
    int temp;

    refresh();
    WINDOW *ending_board3 = newwin(30, 60, 5, 10);
    wbkgd(ending_board3, COLOR_PAIR(1));
    wattron(ending_board3, COLOR_PAIR(1));
    mvwprintw(ending_board3, 14, 28, "Game Over");
    mvwprintw(ending_board3, 13, 16, "press any key for restart...");

    wrefresh(ending_board3);
    refresh();
    int tmp = command;
    command = 999;
    while (command == 999)
    {
        keyIn = keyboard_input();
        command = keyIn != 0 ? keyIn : command;
    }
    command = tmp;

    i = 0;
    count = 0;
}

void Game::eat_item()
{
    if (map.m[snake.body[0][0]][snake.body[0][1]] == PLUSITEM || map.m[snake.body[0][0]][snake.body[0][1]] == MINUSITEM)
    {
        //+먹었을때
        if (map.m[snake.body[0][0]][snake.body[0][1]] == PLUSITEM)
        {
            snake.grow();
            plus_item_count++;
        }
        //-먹었을때
        else
        {
            snake.shrink();
            minus_item_count++;
        }
        //Board에 적용
        map.m[snake.body[0][0]][snake.body[0][1]] = EMPTY_SPACE;
        snake_size_score = "size:" + to_string(snake.body.size());
        plus_item_score = "+:" + to_string(plus_item_count);
        minus_item_score = "-:" + to_string(minus_item_count);

        mvwprintw(board.score_board, 5, 5, snake_size_score.c_str());
        mvwprintw(board.score_board, 6, 5, plus_item_score.c_str());
        mvwprintw(board.score_board, 7, 5, minus_item_score.c_str());
    }

    if (snake.body.size() >= snake_size_goal)
    {
        snake_size_mission_str = "size:" + to_string(snake_size_goal) + " (V)";
        mvwprintw(board.mission_board, 5, 5, snake_size_mission_str.c_str());
    }
    else
    {
        snake_size_mission_str = "size:" + to_string(snake_size_goal) + " ( )";
        mvwprintw(board.mission_board, 5, 5, snake_size_mission_str.c_str());
    }

    if (plus_item_count >= plus_item_goal)
    {
        plus_item_mission_str = "+:" + to_string(plus_item_goal) + " (V)";
        mvwprintw(board.mission_board, 6, 5, plus_item_mission_str.c_str());
    }
    else
    {
        plus_item_mission_str = "+:" + to_string(plus_item_goal) + " ( )";
        mvwprintw(board.mission_board, 6, 5, plus_item_mission_str.c_str());
    }

    if (minus_item_count >= minus_item_goal)
    {
        minus_item_mission_str = "-:" + to_string(minus_item_goal) + " (V)";
        mvwprintw(board.mission_board, 7, 5, minus_item_mission_str.c_str());
    }
    else
    {
        minus_item_mission_str = "-:" + to_string(minus_item_goal) + " ( )";
        mvwprintw(board.mission_board, 7, 5, minus_item_mission_str.c_str());
    }
}

//아이템 생성
void Game::item_setting()
{
    if (item_count == ITEM_CYCLE)
    {
        bool randomItem = rand() % 2 ? true : false; //아이템type
        int rand_x = rand() % HEIGHT; //x좌표
        int rand_y = rand() % WIDTH; //y좌표

        //해당 좌표가 비어있을 때만 아이템 생성가능
        while (!is_empty(rand_x, rand_y))
        {
            rand_x = rand() % HEIGHT;
            rand_y = rand() % WIDTH;
        }
        Item item(rand_x, rand_y, randomItem);
        set_item(item);//아이템의 좌표와 종류 설정
        manage_item(itemContainer);
        itemContainer.push_back(item);
        item_count = 0;
    }
    gate_count++;
    item_count++;
}

//map의 변경사항을 1초마다 반영해서 나타냄
void Game::render_map()
{
    clear_map();
    set_snake();
    wrefresh(board.main_board);
    wrefresh(board.score_board);
    wrefresh(board.mission_board);

    //빈공간,벽,뱀,게이트,아이템 각각 보드에 어떻게 출력할지를 결정한다.
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            switch (map.m[i][j])
            {
            case EMPTY_SPACE:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, " ");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            case WALL:
                wattron(board.main_board, COLOR_PAIR(1));
                mvwprintw(board.main_board, i, j, " ");
                wattroff(board.main_board, COLOR_PAIR(1));
                break;
            case IMMUNE_WALL:
                wattron(board.main_board, COLOR_PAIR(1));
                mvwprintw(board.main_board, i, j, " ");
                wattroff(board.main_board, COLOR_PAIR(1));
                break;
            case SNAKE_BODY: 
                wattron(board.main_board, COLOR_PAIR(snake_color));
                mvwprintw(board.main_board, i, j, "X");
                wattroff(board.main_board, COLOR_PAIR(snake_color));
                break;
            case SNAKE_HEAD:
                wattron(board.main_board, COLOR_PAIR(snake_color));
                mvwprintw(board.main_board, i, j, "O");
                wattroff(board.main_board, COLOR_PAIR(snake_color));
                break;
            case GATE:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, "G");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            case PLUSITEM:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, "+");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            case MINUSITEM:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, "-");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            }
        }

        refresh();
        wrefresh(board.main_board);
        wrefresh(board.score_board);
        wrefresh(board.mission_board);
    };
}

//map초기화
void Game::clear_map()
{

    for (size_t i = 0; i < HEIGHT; i++)
        for (size_t j = 0; j < WIDTH; j++)
        {
            if (map.m[i][j] == SNAKE_BODY)
            {
                if (j == 0 || j == (WIDTH - 1) || i == 0 || i == (HEIGHT - 1) || (i == gate1.first && j == gate1.second) || (i == gate2.first && j == gate2.second))
                {
                    map.m[i][j] = WALL;
                }
                else
                {
                    map.m[i][j] = EMPTY_SPACE;
                }
            }
            if (gate_count == 10 && map.m[i][j] == GATE)
            {
                map.m[i][j] = WALL;
            }
        }
}

//snake 관리
void Game::set_snake()
{
    int tmp_x = snake.body[0][0];
    int tmp_y = snake.body[0][1];
    map.m[tmp_x][tmp_y] = SNAKE_HEAD;

    for (vector<vector<int> >::iterator i = snake.body.begin() + 1; i != snake.body.end(); i++)
    {
        tmp_x = (*i)[0];
        tmp_y = (*i)[1];
        map.m[tmp_x][tmp_y] = SNAKE_BODY;
    }
}

//입력에 따른 방향
int Game::keyboard_input()
{
    int input = getch();
    switch (input)
    {
    case UP:
        return 1;
    case RIGHT:
        return 2;
    case LEFT:
        return 3;
    case DOWN:
        return 4;
    case KEY_BACKSPACE:
        return 5;
    }
    return 0;
}

//아이템 종류설정
void Game::set_item(Item item)
{
    map.m[item.x][item.y] = item.itemType ? PLUSITEM : MINUSITEM;
}
//아이템 제거
void Game::remove_item(Item item)
{
    map.m[item.x][item.y] = EMPTY_SPACE;
}

//아이템 관리
void Game::manage_item(vector<Item> &v)
{
    //아이템의 지속시간 증가
    for (vector<Item>::iterator it = v.begin(); it != v.end(); it++)
    {
        it->lifeTime++; 
    }

    //지속시간이 다한 아이템 제거
    for (vector<Item>::iterator it = v.begin(); it != v.end(); it++)
    {
        if (it->lifeTime == ITEM_LIFE_TIME)
        {
            remove_item(*it);
            v.erase(it);
        }
    }
}

// 좌표가 비어있는지 확인하는 함수
bool Game::is_empty(int x, int y)
{
    return map.m[x][y] == EMPTY_SPACE;
}


//[게이트 클래스 멤버함수 구현]=================================================================================================================

// gate 좌표를 저장
void Gate::syncMap()
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (map_of_gate.m[i][j] == 1)
                {
                    gateList.push_back(pair<int, int>(i, j));
                }
            }
        }
    }

// gate좌표를 랜덤으로 선택
void Gate::gateRefresh()
    {
        int gateChoice1, gateChoice2;

        gateChoice1 = rand() % (gateList.size());
        gateChoice2 = rand() % (gateList.size());

        while (gateChoice1 == gateChoice2)
        {
            gateChoice2 = rand() % (gateList.size());
        }

        gate1 = make_pair(gateList[gateChoice1].first, gateList[gateChoice1].second);
        gate2 = make_pair(gateList[gateChoice2].first, gateList[gateChoice2].second);

        map_of_gate.m[gate1.first][gate1.second] = GATE_CYCLE;
        map_of_gate.m[gate2.first][gate2.second] = GATE_CYCLE;
    }

// gate에서 snake를 감지해서 처리하는 함수들
void Gate::updateCommandForGate(int fromGateX, int fromGateY, int toGateX, int toGateY)
    {
        if (toGateX == 0) {
            command = 4; 
        } else if (toGateX == 29) {
            command = 1; 
        } else if (toGateY == 0) {
            command = 2; 
        } else if (toGateY == 59) {
            command = 3; 
        } else {
            if (fromGateX != toGateX && fromGateY != toGateY) {
                switch (command) {
                    case 3: command = 1; break;
                    case 2: command = 4; break;
                    case 1: command = 2; break;
                    case 4: command = 3; break;
                }
            }
        }
    }

void Gate::handleGateTransition(int gateX, int gateY, int toGateX, int toGateY)
    {
        if (snake.body[0][0] == gateX && snake.body[0][1] == gateY) {
            gate_item_count++;
            updateCommandForGate(gateX, gateY, toGateX, toGateY);
            snake.body[0][0] = toGateX;
            snake.body[0][1] = toGateY;
            
            string state4 = "gate:" + to_string(gate_item_count);
            mvwprintw(board.score_board, 8, 5, state4.c_str());
            
            if (gate_item_count >= gate_item_goal) {
                mvwprintw(board.mission_board, 8, 5, gate_item_mission_str.c_str());
            }
        }
    }

void Gate::gateSensor()
    {
        handleGateTransition(gate1.first, gate1.second, gate2.first, gate2.second);
        handleGateTransition(gate2.first, gate2.second, gate1.first, gate1.second);
    }