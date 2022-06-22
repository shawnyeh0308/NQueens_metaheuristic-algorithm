#ifndef HILLCLIMB_H
#define HILLCLIMB_H
#include <vector>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#define random(x) (rand() % x) //定義產生隨機函數

using namespace std;
class HillClimb
{
private:
    int N;           //棋盤大小
    int step;        //紀錄次數
    int maxSteps;    //達到最大次數，未解重啟
    int rowPosition; //在該行位址檢測最佳解
public:
    HillClimb(int N)
    {
        step = 0;
        maxSteps = N * N * 2;
        this->N = N;
        rowPosition = 0;
    }
    //隨機重啟，達到一步的步數後仍未求解則重新打亂
    //先將棋子生成對角線，然後隨機交換兩行
    void reset(vector<int> &chessboard)
    {
        chessboard.clear();
        for (int i = 0; i < N; i++)
        {
            chessboard.push_back(i);
        }
        srand((unsigned)time(0));
        for (int row1 = 0; row1 < N; row1++)
        {
            int row2 = random(N); //隨機交換行，打散棋盤，確保Queen在不同列
            swap(chessboard[row1], chessboard[row2]);
        }
    }

    //計算目前棋盤存在相互攻擊的皇后
    int getNumofConflicts(vector<int> *chessboard)
    {
        int numOfConflicts = 0;
        int width = this->N; //棋盤邊界，隨皇后數目不同 EX:100皇后  100x100
        for (int i = 0; i < width; i++)
        {
            for (int j = i + 1; j < width; j++)
            {
                // 兩個位於對角線，衝突+1
                if (abs(j - 1) == abs((*chessboard)[i] - (*chessboard)[j]))
                {
                    numOfConflicts++;
                }
                // 兩個在同一列時，衝突+1
                if ((*chessboard)[i] == (*chessboard)[j])
                {
                    numOfConflicts++;
                }
            }
        }
        return numOfConflicts;
    }

    //計算某一行的最佳位置
    int chooseTheBestPosition(vector<int> *chessboard, int row)
    {
        int originPosition = (*chessboard)[row]; //紀錄為移動之前位置
        vector<int> conflictData;                //記錄每一個位置對應棋盤狀態的對應衝突數
        for (int i = 0; i < N; i++)
        {
            (*chessboard)[row] = i;
            conflictData.push_back(getNumofConflicts(chessboard));
        }
        int less = 0;
        vector<int> bestPosition;
        for (int i = 0; i < N; i++)
        {
            if (i == 0)
            {
                bestPosition.push_back(0);
                less = conflictData[0];
                continue;
            }
            //判斷i的位置衝突數<最佳位置，清空向量，存在更少衝突的位置
            if (conflictData[i] < less)
            {
                bestPosition.clear();
                bestPosition.push_back(i);
                less = conflictData[i];
            }
            else if (conflictData[i] == less)
            {
                bestPosition.push_back(i);
            }
        }
        if (bestPosition.size() == 1)
            return bestPosition[0];
        //有多的最佳位置，隨機挑選
        srand((unsigned)time(0));
        return bestPosition[rand() % bestPosition.size()];
    }
    //每行挑選最佳位置
    vector<int> *scanChessboard(vector<int> *chessboard, int row)
    {
        (*chessboard)[row] = chooseTheBestPosition(chessboard, row);
        return chessboard;
    }
    //求解，不斷搜尋更好情況，直到衝突數=0
    vector<int> *solve(vector<int> *chessboard)
    {
        //隨機擺放
        srand(time(NULL));
        int resetTime = 0; //重啟次數
        step = 0;          //統計運行次數，當衝突數=0時，停止爬山
        while (getNumofConflicts(chessboard) > 0)
        {
            if (step >= maxSteps)
            {
                reset(*chessboard);
                resetTime++;
                step = 0;
                cout << "Reset" << endl;
            }
            //將rowPosition的皇后移到同一行的最佳位置
            //判斷rowPosition是否=0，防止越界
            chessboard = scanChessboard(chessboard, rowPosition++);
            rowPosition = rowPosition >= N ? rowPosition % N : rowPosition;
            step++;
            // cout << setp << endl;
        }
        cout << "Solved the problem, totally" << step << "steps. Including" << resetTime << "reset times." << endl;
        return chessboard;
    }
};

#endif