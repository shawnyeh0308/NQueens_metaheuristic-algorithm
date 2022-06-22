/**********************************************************************************************
 * initChessboard:�ѽL��l��
 * printChessboard:�C�L�ѽL
 * �b��Ӽ��Y��Class�̩w�q��Ƶ��c:SA.h�MHC.h
 * Vector<int>temp_chessboard = getNumofConflicts(vector<int> *chessboard) 
 * 
*************************************************************************************************/
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "HC.h"
#include "SA.h"
#define random(x) (rand() % x) //�w�q�����H�����

using namespace std;

int N;//�w�qN�ӦZ���D�AN=�ƥ�

void initChessboard(vector<int> &chessboard)
{
    for (int i = 0; i < N; i++)
    {
        chessboard.push_back(i);
    }
    srand((unsigned)time(0));
    for (int row1 = 0; row1 < N; row1++)
    {
        int row2 = random(N); //�H���洫�A�T�O�ӦZ�b���P��m
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
    N = 0; //�ӦZ���s�A����J
    cout << "Enter the N value of the N-Queens(more than 8) " << endl;
    cin >> N;
    cout << N << "  Queens Problems =============== " << endl;
    cout << "Choose one algorithm to slove the problem " << endl;
    cout << "1. HillClimbing Algorithm " << endl;
    cout << "2. Simulated Annealing Alogrithm " << endl;
    cout << "3. Exit " << endl;
    string choose;
    //��شѽL
    vector<int> temp1_chessboard;
    vector<int> temp2_chessboard;
    //�p��
    clock_t startTime;
    clock_t endTime;
    double totalTime;
    //��غt��k
    HillClimb sol1(N);   
    SimulatedAnnealing sol2(N);

    while (1)
    {
        cin >> choose;
        int num = stringToInt(choose);
        int t = 0;  //���榸��
        double average = 0; //������
        int big = 0; //�̪��ɶ�
        int small = 100; //�̧C�ɶ�
        int sum = 0; //�`���ƥ[�`�A�D������
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

                temp1_chessboard = *sol1.solve(&temp1_chessboard);//�D��

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
                temp2_chessboard = *sol2.solve(&temp2_chessboard);//�D��
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