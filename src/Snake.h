#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

using namespace std;

class Snake
{
public:
    vector<vector<int> > body;
    vector<int> empty_body; 

    //snake생성 -> 초기 크기는 3
    Snake()
    {
        empty_body.push_back(10); 
        empty_body.push_back(35); 

        //snake의 위치
        vector<int> head;
        head.push_back(17);
        head.push_back(35);
        vector<int> head2;
        head2.push_back(17);
        head2.push_back(34);
        vector<int> head3;
        head3.push_back(17);
        head3.push_back(33);
        
        body.push_back(head);
        body.push_back(head2);
        body.push_back(head3);
    };

    //사이즈 키우기
    void grow();

    //사이즈 축소
    void shrink();

    //스네이크 이동시키기
    void move(int direction);
};

#endif 
