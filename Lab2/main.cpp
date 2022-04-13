#include <bits/stdc++.h>
#include "windows.h"

#define ARRAY_SIZE 8

using namespace std;

static int array_list[ARRAY_SIZE][ARRAY_SIZE] = {{0, 1, 1, 0, 0, 0, 0, 0},
                                                 {0, 0, 0, 1, 1, 1, 0, 0},
                                                 {0, 0, 0, 1, 1, 0, 1, 0},
                                                 {0, 0, 0, 0, 1, 1, 1, 0},
                                                 {0, 0, 0, 0, 0, 1, 1, 0},
                                                 {0, 0, 0, 0, 0, 0, 0, 1},
                                                 {0, 0, 0, 0, 0, 0, 0, 1},
                                                 {0, 0, 0, 0, 0, 0, 0, 0}};
static double array_probs[] = {0.84, 0.84, 0.91, 0.6, 0.44, 0.74, 0.57, 0.79};
static int time_ = 10;

static void modified_dfs(int start, vector<int> endings, vector< vector<int> >& paths, vector<int>& current_path, vector<bool>& colored)
{
    if (colored.empty())
    {
        colored.resize(ARRAY_SIZE, true);
    }

    for (int i = 0; i < endings.size(); i++)
    {
        if (endings[i] == start)
        {
            paths.push_back(current_path);
            break;
        }
    }

    colored[start] = false;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (array_list[start][i] && colored[i])
        {
            current_path.push_back(i);
            modified_dfs(i, endings, paths, current_path, colored);
            for (int j = 0; j < current_path.size(); j++)
            {
                if (current_path[j] == i)
                {
                    current_path.erase(current_path.begin() + j);
                    break;
                }
            }
        }
    }
    colored[start] = true;
}

bool sort_vec_vec(vector<int> vec1, vector<int> vec2)
{
    return vec1.size() > vec2.size();
}

void dfs(int depth, int s, int i, vector<int>& c, const vector<int>& v, vector< vector<int> >& o)
{
    if (depth == s)
    {
        do
        {
            vector<int> tmp;
            for (int i=0;i<c.size();i++)
                tmp.push_back(c[i]);
            o.push_back(tmp);
        }
        while (next_permutation(c.begin(), c.end()));
    }
    else
    {
        for (int j = i + 1; j < (int)v.size(); ++j)
        {
            c.push_back(v[j]);
            dfs(depth + 1, s, j, c, v, o);
            c.pop_back();
        }
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Всі шляхи від 1 до " << ARRAY_SIZE << endl;

    vector<int> started, endings;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        bool flag_s = true, flag_e = true;
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            if (array_list[j][i] == 1)
            {
                flag_s = false;
            }
            if (array_list[i][j] == 1)
            {
                flag_e = false;
            }
        }
        if (flag_s)
        {
            started.push_back(i);
        }
        if (flag_e)
        {
            endings.push_back(i);
        }
    }

    vector< vector<int> > paths, paths_extended;
    for (int i = 0; i < started.size(); i++)
    {
        vector<int> current_path;
        vector<bool> colored;
        current_path.push_back(started[i]);
        modified_dfs(started[i], endings, paths, current_path, colored);
    }

    for (int i = 0; i < paths.size(); i++)
    {
        for (int j = 0; j < paths[i].size(); j++)
        {
            cout << paths[i][j] + 1 << " ";
        }
        cout << endl;
    }

    paths_extended = paths;
    vector<int> universal;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        universal.push_back(i);
    }

    paths_extended.push_back(universal);
    for (int i = 0; i < paths.size(); i++)
    {
        vector<int> tmp;
        set_difference (universal.begin(), universal.end(), paths[i].begin(), paths[i].end(), inserter(tmp, tmp.begin()));
        for (int j = 0; j < tmp.size(); j++)
        {
            vector< vector<int> > combs;
            vector<int> c;
            dfs(0, j, -1, c, tmp, combs);

            for (int k = 0; k < combs.size(); k++)
            {
                vector<int> tmp2(paths[i]);
                tmp2.insert(tmp2.end(), combs[k].begin(), combs[k].end());
                sort(tmp2.begin(), tmp2.end());

                bool flag = true;
                for (int m = 0; m < paths_extended.size(); m++)
                {
                    if (tmp2 == paths_extended[m])
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    paths_extended.push_back(tmp2);
                }
            }
        }
    }
    sort(paths_extended.begin(), paths_extended.end(), sort_vec_vec);

    vector<double> probs_counted;
    for (int i = 0; i < paths_extended.size(); i++)
    {
        vector<int> universal_copy;
        set_difference (universal.begin(), universal.end(),
                        paths_extended[i].begin(), paths_extended[i].end(),
                        inserter(universal_copy, universal_copy.begin()));

        double prob = 1.0;
        for (int j = 0; j < paths_extended[i].size(); j++)
        {
            prob *= array_probs[paths_extended[i][j]];
        }
        for (int j = 0; j < universal_copy.size(); j++)
        {
            prob *= 1.0 - array_probs[universal_copy[j]];
        }
        probs_counted.push_back(prob);
    }

    cout << "Таблиця працездатних станів системи:" << endl;

    double probs_sum = 0.0;
    for (int i = 0; i < paths_extended.size(); i++)
    {
        for (int j = 0; j < paths_extended[i].size(); j++)
        {
            cout << paths_extended[i][j] + 1 << " ";
        }
        probs_sum += probs_counted[i];
        cout << "\t= " << probs_counted[i] << endl;
    }

    cout    << "\nЙмовірність відмови P = " << probs_sum
            << "\nІнтенсивність відмов Lambda = " << -log(probs_sum) / time_
            << "\nСередній наробіток до відмови T = " << -time_ / log(probs_sum) << endl;

    return 0;
}
