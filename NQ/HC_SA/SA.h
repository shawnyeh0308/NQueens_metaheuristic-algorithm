#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

class SimulatedAnnealing
{
private:
    int N; //皇后數量
public:
    SimulatedAnnealing(int N)
    {
        this->N = N;
    } //默認構造函數

    //計算目前棋盤呈相互攻擊的皇后數目
    int getNumofConflicts(vector<int> *chessboard)
    {
        int numOfConflicts = 0;
        int width = this->N;//棋盤邊界，隨皇后N值不同 EX:100皇后  100x100
        for (int i = 0; i < width; i++)
        {
            for (int j = i + 1; j < width; j++)
            {
                if (abs(j - i) == abs((*chessboard)[i] - (*chessboard)[j]))
                {
                    numOfConflicts++;
                } // 兩個在對角線時，衝突+1
                if ((*chessboard)[i] == (*chessboard)[j])
                {
                    numOfConflicts++;
                } // 兩個在同一列時，衝突+1
            }
        }
        return numOfConflicts;
    }

    //使用模擬退火得到該行的合適位置
    int properPosition(const vector<int> *chessboard, int row)
    {
        double T = 10000.0;
        double Tmin = 8800.0;
        double decreaseRate = 0.88;
        //以上三個係數不解釋

        vector<int> *currentChessboard = new vector<int>(N);
        vector<int> *nextChessboard = new vector<int>(N);
        //目前和下一個狀態

        for (int i = 0; i < this->N; i++)
        {
            (*currentChessboard)[i] = ((*chessboard)[i]);
            (*nextChessboard)[i] = ((*chessboard)[i]);
        }
        // int queenPosition = 0 ;
        //退火開始
        while (T > Tmin)
        {
            //修改下一個狀態，改變皇后的位置
            for (int queenPosition = 0; queenPosition < this->N; queenPosition++)
            {
                (*nextChessboard)[row] = queenPosition;
                //得到前後狀態能量差
                double dE = getNumofConflicts(nextChessboard) - getNumofConflicts(currentChessboard);
                if (dE <= 0)
                {
                    //如果下一步更好則移動
                    (*currentChessboard)[row] = (*nextChessboard)[row];
                    //cout << "better" << "pos = " << queenPosition << std::endl;
                }
                //如果下一步更差，則一定接受移動
                else
                {
                    if (exp((-1) * dE / T) > random(10000) * 8800.0 / 10000)
                    {
                        //cout <<" worse " << " pos = " << queenPosition << std::endl;
                        (*currentChessboard)[row] = (*nextChessboard)[row];
                    }
                    else
                    {
                       // cout << "don't move pos =" << std::endl;
                    }
                }
            }
            T = decreaseRate * T; //降溫
        }
        // std::cout << "-------------------------" << std::endl;
        return (*currentChessboard)[row];
    }
    //使用模擬退火修改行，得到一個合適位置
    // (不一定是目前最佳，它以一定機率接受更差解)
    vector<int> *nextState(vector<int> *chessboard, int row)
    {
        (*chessboard)[row] = properPosition(chessboard, row);
        return chessboard;
    }

    //求解函數，在不同的行當中不斷尋找下一步最佳位置
    vector<int> *solve(vector<int> *chessboard)
    {
        int rowPosition = 0; //移動次數及列的位置為0
        int step = 0;
        while (getNumofConflicts(chessboard))//計算衝突數並開始尋找下一步最佳位置
        {
            if (rowPosition == this->N)
            {
                rowPosition %= rowPosition;
            }
            chessboard = nextState(chessboard, rowPosition++);
            step++;
        }
        cout << "Solved the problem, totally " << step << "steps." << endl;
        return chessboard;
    }
};

#endif