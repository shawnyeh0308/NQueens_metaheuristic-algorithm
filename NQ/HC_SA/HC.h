#ifndef HILLCLIMB_H
#define HILLCLIMB_H
#include <vector>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#define random(x) (rand() % x) //�w�q�����H�����

using namespace std;
class HillClimb
{
private:
    int N;           //�ѽL�j�p
    int step;        //��������
    int maxSteps;    //�F��̤j���ơA���ѭ���
    int rowPosition; //�b�Ӧ��}�˴��̨θ�
public:
    HillClimb(int N)
    {
        step = 0;
        maxSteps = N * N * 2;
        this->N = N;
        rowPosition = 0;
    }
    //�H�����ҡA�F��@�B���B�ƫᤴ���D�ѫh���s����
    //���N�Ѥl�ͦ��﨤�u�A�M���H���洫���
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
            int row2 = random(N); //�H���洫��A�����ѽL�A�T�OQueen�b���P�C
            swap(chessboard[row1], chessboard[row2]);
        }
    }

    //�p��ثe�ѽL�s�b�ۤ��������ӦZ
    int getNumofConflicts(vector<int> *chessboard)
    {
        int numOfConflicts = 0;
        int width = this->N; //�ѽL��ɡA�H�ӦZ�ƥؤ��P EX:100�ӦZ  100x100
        for (int i = 0; i < width; i++)
        {
            for (int j = i + 1; j < width; j++)
            {
                // ��Ӧ��﨤�u�A�Ĭ�+1
                if (abs(j - 1) == abs((*chessboard)[i] - (*chessboard)[j]))
                {
                    numOfConflicts++;
                }
                // ��Ӧb�P�@�C�ɡA�Ĭ�+1
                if ((*chessboard)[i] == (*chessboard)[j])
                {
                    numOfConflicts++;
                }
            }
        }
        return numOfConflicts;
    }

    //�p��Y�@�檺�̨Φ�m
    int chooseTheBestPosition(vector<int> *chessboard, int row)
    {
        int originPosition = (*chessboard)[row]; //���������ʤ��e��m
        vector<int> conflictData;                //�O���C�@�Ӧ�m�����ѽL���A�������Ĭ��
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
            //�P�_i����m�Ĭ��<�̨Φ�m�A�M�ŦV�q�A�s�b��ֽĬ𪺦�m
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
        //���h���̨Φ�m�A�H���D��
        srand((unsigned)time(0));
        return bestPosition[rand() % bestPosition.size()];
    }
    //�C��D��̨Φ�m
    vector<int> *scanChessboard(vector<int> *chessboard, int row)
    {
        (*chessboard)[row] = chooseTheBestPosition(chessboard, row);
        return chessboard;
    }
    //�D�ѡA���_�j�M��n���p�A����Ĭ��=0
    vector<int> *solve(vector<int> *chessboard)
    {
        //�H���\��
        srand(time(NULL));
        int resetTime = 0; //���Ҧ���
        step = 0;          //�έp�B�榸�ơA��Ĭ��=0�ɡA����s
        while (getNumofConflicts(chessboard) > 0)
        {
            if (step >= maxSteps)
            {
                reset(*chessboard);
                resetTime++;
                step = 0;
                cout << "Reset" << endl;
            }
            //�NrowPosition���ӦZ����P�@�檺�̨Φ�m
            //�P�_rowPosition�O�_=0�A����V��
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