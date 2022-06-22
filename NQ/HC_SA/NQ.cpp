/**********************************************************************************************
 * initChessboard:棋盤初始化
 * printChessboard:列印棋盤
 * 在兩個標頭檔Class裡定義資料結構:SA.h和HC.h
 * Vector<int>temp_chessboard = getNumofConflicts(vector<int> *chessboard) 
 * 
*************************************************************************************************/
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "HC.h"
#include "SA.h"
#define random(x) (rand() % x) //定義產生隨機函數

using namespace std;

int N;//定義N皇后問題，N=數目

void initChessboard(vector<int> &chessboard)
{
    for (int i = 0; i < N; i++)
    {
        chessboard.push_back(i);
    }
    srand((unsigned)time(0));
    for (int row1 = 0; row1 < N; row1++)
    {
        int row2 = random(N); //隨機交換，確保皇后在不同位置
        swap(chessboard[row1], chessboard[row2]);
    }
}

void printChessboard(vector<int> &chessboard) 
{
    for (int i = 0; i < N; i++)
    {
        int num = chessboard[i];
        for (int j = 0; j < num; j++)
        {
            cout << " = ";
        }
        cout << " Q ";
        for (int k = num + 1; k < N; k++)
        {
            cout << " = ";
        }
        cout << endl;
    }
}

int stringToInt(string choose)
{
    if (choose == "1")
    {
        return 1;
    }
    if (choose == "2")
    {
        return 2;
    }
    if (choose == "3")
    {
        return 3;
    }
    return 0;
}


int main()
{
    N = 0; //皇后為零，等輸入
    cout << "Enter the N value of the N-Queens(more than 8) " << endl;
    cin >> N;
    cout << N << "  Queens Problems =============== " << endl;
    cout << "Choose one algorithm to slove the problem " << endl;
    cout << "1. HillClimbing Algorithm " << endl;
    cout << "2. Simulated Annealing Alogrithm " << endl;
    cout << "3. Exit " << endl;
    string choose;
    //兩種棋盤
    vector<int> temp1_chessboard;
    vector<int> temp2_chessboard;
    //計時
    clock_t startTime;
    clock_t endTime;
    double totalTime;
    //兩種演算法
    HillClimb sol1(N);   
    SimulatedAnnealing sol2(N);

    while (1)
    {
        cin >> choose;
        int num = stringToInt(choose);
        int t = 0;  //執行次數
        double average = 0; //平均值
        int big = 0; //最長時間
        int small = 100; //最低時間
        int sum = 0; //總次數加總，求平均值
        switch (num)
        {
        case 1:
            cout << "Solution 1 -- Hill Climbing Algorithm " << endl;
            cout << "Enter the number of executions " << endl;
            cin >> t;

            for (int i = 0; i < t; i++)
            {
                cout << "Print chessbaord and initialize " << endl;
                initChessboard(temp1_chessboard);
                printChessboard(temp1_chessboard);
                startTime = clock();

                temp1_chessboard = *sol1.solve(&temp1_chessboard);//求解

                endTime = clock();
                totalTime = (double)(endTime - startTime);
                if (totalTime > big)
                {
                    big = totalTime;
                }
                if (totalTime < small)
                {
                    small = totalTime;
                }
                sum += totalTime;
                cout << "[ " << i << "] "
                     << "total time: " << totalTime << "ms." << endl;
                printChessboard(temp1_chessboard);
                cout << "-----------------------------------------------------------" << endl;
            }
            average = sum * 1.0 / t;
            cout << "average time: " << average << "ms" << endl;
            cout << "longest time: " << big << "ms" << endl;
            cout << "shortest time: " << small << "ms" << endl;

            break;
        case 2:
            cout << "Solution 2 -- Simulated Annealing Algorithm " << endl;
            cout << "Enter the number of executions " << endl;
            cin >> t;

            for (int i = 0; i < t; i++)
            {
                cout << "Print chessbaord and initialize " << endl;
                initChessboard(temp2_chessboard);
                printChessboard(temp2_chessboard);
                startTime = clock();
                temp2_chessboard = *sol2.solve(&temp2_chessboard);//求解
                endTime = clock();
                totalTime = (double)(endTime - startTime);
                if (totalTime > big)
                {
                    big = totalTime;
                }
                if (totalTime < small)
                {
                    small = totalTime;
                }
                sum += totalTime;
                cout << "[ " << i << "] "
                     << "total time: " << totalTime << "ms." << endl;
                printChessboard(temp2_chessboard);
                cout << "-----------------------------------------------------------" << endl;
            }
            average = sum * 1.0 / t;
            cout << "average time: " << average << "ms" << endl;
            cout << "longest time: " << big << "ms" << endl;
            cout << "shortest time: " << small << "ms" << endl;

            break;

        case 3:
            exit(0);
            break;

        default:
            break;
        }
    }
}