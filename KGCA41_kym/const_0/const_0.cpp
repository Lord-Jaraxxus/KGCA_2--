#include <iostream>

int main()
{
    int DataA = 1;
    int DataB = 2;
    int DataC = 3;
    int DataD = 4;

    // 1
    const int* a = &DataA;  
    a = &DataB; // 가능, 별 왼쪽에 const가 있으니까 주소 변경가능 주소 안의 값 변경 불가
    //*a = 3; // 불가능

    // 1.5
    int const* a3 = &DataA; // 위에거랑 똑같은듯? 
    a3 = &DataC; // ㄱㄴ
    //*a3 = 3; // ㅂㄱㄴ

    // 2
    int* const b = &DataB; 
    //b = &DataC; // 불가능, 위에랑 반대
    *b = 3; // 가능성 탐구자 : 가능

    //3
    const int const* a1 = &DataC;
    a1 = &DataD; // 가능
    //*a1 = 3; // 불가능, 사실상 1번이랑 똑같음

    //4 
    int* const const b2 = &DataA;
    //b2 = &DataB; // 불가능
    *b2 = 3; // 거눙, 사실상 2번이랑 ㅇㅇ

    //5

    int const* const c = &DataD;
    //c = &DataC; // 불가능
    //*c = 3; // 불가능


    // 결론 : * 기준으로 const가 왼쪽 오른쪽 어딨느냐가 중요한듯
    // 왼쪾이면 주소 안에 있는 값만 변경 불가능, 오른쪽이면 주소만 변경 불가능 

    const int* ptr = nullptr;
    int num[2] = { 0, };
    ptr = num;
    ptr++;
    num[1] = 1;

}