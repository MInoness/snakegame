#include "SnakeControl.h"

using namespace std;

int main()
{
  srand((unsigned int)time(NULL));
  try
  {
    initscr();
  }
  catch(const std::exception& e)
  {
    cout << "게임을 시작할수 없습니다." << endl;
    std::cerr << e.what() << '\n';
  }

  keypad(stdscr, TRUE);

  curs_set(0);
  noecho();

  resize_term(200, 200);
  refresh();

  start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE); // 벽
    init_pair(2, COLOR_WHITE, COLOR_BLACK); // 게임화면
    init_pair(3, COLOR_BLACK, COLOR_CYAN); // 배경색
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);// snake색
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6,COLOR_CYAN,COLOR_BLACK);

  WINDOW *main_window;
  nodelay(stdscr, true);
  main_window = newwin(40, 125, 0, 0);
  wbkgd(main_window, COLOR_PAIR(3));
  wrefresh(main_window);

  Game start;

  Map m;
  int i=0;
  while(i!=-1){
    switch (i) {
      case 0:
          start.game_control(m.m1,i);
          break;
        case 1:
          start.game_control(m.m2,i);
          break;
        case 2:
          start.game_control(m.m3,i);
          break;
        case 3:
          start.game_control(m.m4,i);
          break;
        }


  }


  getch();
  endwin();

  return 0;
}