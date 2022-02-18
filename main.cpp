#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include <algorithm>
#include<windows.h>

double MathTask(double Dval1, double Dval2, char ope)
{
    if (ope == '*' || ope == 'x') return Dval1 * Dval2;
    if (ope == ':' || ope == '/') return Dval1 / Dval2;
    if (ope == '+') return Dval1 + Dval2;
    if (ope == '-') return Dval1 - Dval2;
    if (ope == '^') return pow(Dval1, Dval2);
}

double stringToNumber(std::string Num)
{
    double result = 0;
    std::stringstream ss;
    ss << Num;
    ss >> result;

    return result;
}

bool FindChInStr(std::vector<char> vec, char chToFind)
{
    return std::find(vec.begin(), vec.end(), chToFind);
}

double stringToTask(std::string task)
{
    std::vector<double> Nums;
    std::vector<char> operators;
    {
        std::string stringNum = "";

        for (size_t i = 0; i < task.length(); i++)
            if (task[i] != ' ')
            {
                if ((task[i] >= '0' && task[i] <= '9') || task[i] == '.' || task[i] == ',')
                    stringNum.push_back(task[i]);

                else if (task[i] == '+' || task[i] == '-' || task[i] == '/' || task[i] == ':' || task[i] == '*' || task[i] == 'x' || task[i] == '^')
                {
                    if (i + 1 < task.length() && (task[i + 1] == '+' || task[i + 1] == '-' || task[i + 1] == '/' || task[i + 1] == ':'
                        || task[i + 1] == '*' || task[i + 1] == 'x' || task[i + 1] == '^'))
                        return 0;

                    operators.push_back(task[i]);
                    if (stringNum != "")
                    {
                        Nums.push_back(stringToNumber(stringNum));
                        stringNum = "";
                    }
                }

                else
                    return 0;
            }

        Nums.push_back(stringToNumber(stringNum));
    }


    std::cout << "\n\n";
    int solutionRow = 1;
    do
    {
        std::string MathSymbols = "";

        if (FindChInStr(operators), '*'))  || FindChInStr(operators), '/')) || FindChInStr(operators), ':')) ||
            FindChInStr(operators), 'x')) || FindChInStr(operators), '^')))
            MathSymbols = "*/:x^";

        else
            MathSymbols = "+-";

        for (size_t i = 0; i < operators.size(); i++)
        {
            for (size_t j = 0; j < MathSymbols.length(); j++)
                if (operators[i] == MathSymbols[j])
                {
                    double Val = MathTask(Nums[i], Nums[i + 1], operators[i]);
                    operators.erase(operators.begin() + i);
                    Nums.erase(Nums.begin() + i);
                    Nums.erase(Nums.begin() + i);
                    Nums.insert(Nums.begin() + i, Val);
                    break;
                }
        }

        if (!operators.empty())
        {
            std::cout << solutionRow << ") ";
            for (size_t i = 0; i < Nums.size(); i++)
            {
                std::cout << Nums[i] << " ";

                if (i < operators.size())
                    std::cout << operators[i] << " ";
            }
            std::cout << "\n\n";
            solutionRow++;
        }

    } while (!operators.empty());


    return Nums[0];
}

int main()
{
    while (1)
    {
        std::string Task = "";
        std::cout << "Math Task: ";

        std::getline(std::cin, Task);

        std::cout << "\n\nthe answer is: " << stringToTask(Task) << std::endl;
        system("pause>0");
        system("cls");
    }
}