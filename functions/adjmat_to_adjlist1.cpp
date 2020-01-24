// A simple representation of graph using STL
#include <bits/stdc++.h>
#include <vector>

using namespace std;
typedef vector<vector<int>> dim2I;

dim2I AdjmatToAdjlist(const dim2I &A)
{
    int row = A.size();
    int col = A[0].size();
    dim2I adjlist;
    adjlist.resize(row);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (A[i][j] > 1e-5)
                adjlist[i].push_back(j);
        }
    }

    return adjlist;
}

int main(int argc, char *argv[])
{
    int N = 5;
    dim2I adjmat = {{0, 1, 0, 0, 1},
                    {1, 0, 1, 1, 1},
                    {0, 1, 0, 1, 0},
                    {0, 1, 1, 0, 1},
                    {1, 1, 0, 1, 0}};
    auto adjlist = AdjmatToAdjlist(adjmat);

    for (int i = 0; i < N; i++)
    {
        cout << i << " : ";
        for (int j = 0; j < adjlist[i].size(); j++)
        {
            cout << adjlist[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}