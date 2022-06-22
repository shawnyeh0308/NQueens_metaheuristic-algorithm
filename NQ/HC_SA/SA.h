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
    int N; //�ӦZ�ƶq
public:
    SimulatedAnnealing(int N)
    {
        this->N = N;
    } //�q�{�c�y���

    //�p��ثe�ѽL�e�ۤ��������ӦZ�ƥ�
    int getNumofConflicts(vector<int> *chessboard)
    {
        int numOfConflicts = 0;
        int width = this->N;//�ѽL��ɡA�H�ӦZN�Ȥ��P EX:100�ӦZ  100x100
        for (int i = 0; i < width; i++)
        {
            for (int j = i + 1; j < width; j++)
            {
                if (abs(j - i) == abs((*chessboard)[i] - (*chessboard)[j]))
                {
                    numOfConflicts++;
                } // ��Ӧb�﨤�u�ɡA�Ĭ�+1
                if ((*chessboard)[i] == (*chessboard)[j])
                {
                    numOfConflicts++;
                } // ��Ӧb�P�@�C�ɡA�Ĭ�+1
            }
        }
        return numOfConflicts;
    }

    //�ϥμ����h���o��Ӧ檺�X�A��m
    int properPosition(const vector<int> *chessboard, int row)
    {
        double T = 10000.0;
        double Tmin = 8800.0;
        double decreaseRate = 0.88;
        //�H�W�T�ӫY�Ƥ�����

        vector<int> *currentChessboard = new vector<int>(N);
        vector<int> *nextChessboard = new vector<int>(N);
        //�ثe�M�U�@�Ӫ��A

        for (int i = 0; i < this->N; i++)
        {
            (*currentChessboard)[i] = ((*chessboard)[i]);
            (*nextChessboard)[i] = ((*chessboard)[i]);
        }
        // int queenPosition = 0 ;
        //�h���}�l
        while (T > Tmin)
        {
            //�ק�U�@�Ӫ��A�A���ܬӦZ����m
            for (int queenPosition = 0; queenPosition < this->N; queenPosition++)
            {
                (*nextChessboard)[row] = queenPosition;
                //�o��e�᪬�A��q�t
                double dE = getNumofConflicts(nextChessboard) - getNumofConflicts(currentChessboard);
                if (dE <= 0)
                {
                    //�p�G�U�@�B��n�h����
                    (*currentChessboard)[row] = (*nextChessboard)[row];
                    //cout << "better" << "pos = " << queenPosition << std::endl;
                }
                //�p�G�U�@�B��t�A�h�@�w��������
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
            T = decreaseRate * T; //����
        }
        // std::cout << "-------------------------" << std::endl;
        return (*currentChessboard)[row];
    }
    //�ϥμ����h���ק��A�o��@�ӦX�A��m
    // (���@�w�O�ثe�̨ΡA���H�@�w���v������t��)
    vector<int> *nextState(vector<int> *chessboard, int row)
    {
        (*chessboard)[row] = properPosition(chessboard, row);
        return chessboard;
    }

    //�D�Ѩ�ơA�b���P��������_�M��U�@�B�̨Φ�m
    vector<int> *solve(vector<int> *chessboard)
    {
        int rowPosition = 0; //���ʦ��ƤΦC����m��0
        int step = 0;
        while (getNumofConflicts(chessboard))//�p��Ĭ�ƨö}�l�M��U�@�B�̨Φ�m
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