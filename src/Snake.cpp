#include "Snake.h"

//몸 증가
void Snake::grow()
{
    body.push_back(empty_body);
}

//몸 감소
void Snake::shrink()
{
    if (!body.empty()) {
        empty_body = body.back();
        body.pop_back();
    }
}

void Snake::move(int command)
{
    if (body.empty()) {
        return;
    }

    // 현재 뱀 머리 위치
    vector<int> tmp_head = body[0];
    vector<int> tmp;

    // 명령이 들어오면 머리위치 변경
    switch (command)
    {
    case 1: // 위쪽 방향
        tmp_head[0] = (tmp_head[0] - 1 + 30) % 30;
        break;
    case 2: // 오른쪽 방향
        tmp_head[1] = (tmp_head[1] + 1) % 60;
        break;
    case 3: // 왼쪽 방향
        tmp_head[1] = (tmp_head[1] - 1 + 60) % 60;
        break;
    case 4: // 아래쪽 방향
        tmp_head[0] = (tmp_head[0] + 1) % 30;
        break;
    case 5: // 백스페이스(아무것도 안함)
        break;
    }

    // 몸통 이동
    for (size_t i = body.size() - 1; i > 0; --i)
    {
        body[i] = body[i - 1];
    }
    body[0] = tmp_head;

    // 위치 업데이트
    empty_body = body.back();
}
