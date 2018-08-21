#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

typedef std::vector<std::vector<int>> ArrayMatrix;
typedef std::vector<int> ArrayInt;
typedef std::vector<bool> ArrayBool;
typedef std::stack<int> StackInt;

//Функция, считывающая матрицу с файла
void processInput(std::ifstream&, unsigned&, ArrayMatrix&);
//Функция, вычисляющая степень вершины
int getDeg(ArrayMatrix&, int, int);
//Функция, проверяющая на условие Дирака
bool conditionDirak(ArrayMatrix&, int);
//Рекурсивная функция поиска гамильтонового цикла
void findHamiltonianCycle(int, ArrayMatrix&, int&,
                          int, ArrayBool&, StackInt&);
//Проверка на смежность
bool checkAdjacency(int, int, ArrayMatrix&);

int main() {
    std::ifstream input;
    input.open("inputFiles/input.txt", ios_base::in);
    if (!input) {
        std::cout << "Error: file isn't found" << '\n';
        return 1;
    }
    std::string graphName;
    std::getline(input, graphName); // ввод названия
    unsigned vertexAmount = 0;
    ArrayMatrix vertexList;
    processInput(input, vertexAmount, vertexList);
    ArrayBool path(vertexAmount);
    StackInt vertexStack;
    int vertex = 0;
    int count = 0;


    //Вывод смежной матрицы
    for (size_t i = 0; i < vertexAmount; i++)
    {

        for (size_t j = 0; j < vertexAmount; j++)
        {
            std::cout << vertexList[i][j] << ' ';
        }
        std::cout << '\n';
    }
    //Определение гамильтонового цикла
    if (vertexAmount <= 3) {
        if (vertexAmount == 3) {
            if (checkAdjacency(1, 3, vertexList)) {
                std::cout << "Hamiltonian Cycle is real.\n";
            } else {
                std::cout << "Hamiltonian Cycle is not real.\n";
            }
        } else {
            std::cout << "Hamiltonian Cycle is not real.\n";
        }
        //} else if (conditionDirak(vertexList,vertexAmount)) {
        //    cout << "Условие Дирака выполнено. Hamiltonian Cycle is real.\n";
    } else  {
        findHamiltonianCycle(vertexAmount, vertexList, count, vertex, path, vertexStack);
        if(count)
            std::cout << "Hamiltonian Cycle is real.\n";
        else
            std::cout << "Hamiltonian Cycle is not real.\n";
    }

    input.close();
    system("pause");
    return 0;
}

// Функция, считывающая матрицу с файла
void processInput(std::ifstream& input, unsigned& vertexAmount, ArrayMatrix& vertexList) {
    //Считывается количество вершин
    input >> vertexAmount;
    //Двойному вектору задаётся размер равный количеству вершин
    vertexList.resize(vertexAmount);
    std::string line;
    //Пропускается линия
    std::getline(input, line);
    //Копирование смежной матрицы в двойной вектор
    for (size_t i = 0; i < vertexAmount; i++)
    {
        //Взятие линии в строку
        std::getline(input, line);

        for (size_t j = 0; j < vertexAmount; j++)
        {
            //Символы переопределяются в числа
            vertexList[i].push_back(line[j] == '1' ? 1 : 0);
        }

    }
}

int getDeg(ArrayMatrix& vertexList, int vertexAmount, int vertexTitle) {
    int result = 0;
    //Циклом подсчитывается количество смежных вершин
    for (size_t i = 0; i < vertexAmount; i++)
    {
        result += vertexList[vertexTitle][i];
    }
    return result;
}

bool conditionDirak(ArrayMatrix& vertexList, int vertexAmount) {
    //Условие Дирака работает только при наличии больше 3 вершин
    if (vertexAmount > 3) {
        //Задаётся тестовое значение
        int check = vertexAmount / 2;
        //Циклом определяется больше ли тестового значения каждая степень вершины 
        for (size_t i = 0; i < vertexAmount; i++)
        {
            //Если нет - то условие Дирака не соблюдено
            if(getDeg(vertexList, vertexAmount, i) < check)
                return false;
        }
        return true;
    }
    else {
        return false;
    }
}

void findHamiltonianCycle(int vertexAmount, ArrayMatrix& vertexList, int& count,
                          int vertex, ArrayBool& path, StackInt& vertexStack) {
    //Добавление вершины в стек
    vertexStack.push(vertex);
    if(vertexStack.size() == vertexAmount && vertexList[vertex][0] == 1)
        count++;


    for(int i = vertexAmount - 1; i >= 0; i--)
    {
        //Проверка смежных вершин на посещённость
        if (vertexList[vertex][i] == 1 && !path[i]) {
            path[vertex] = true;
            //Запуск рекурсивной функции для следующей вершины
            findHamiltonianCycle(vertexAmount, vertexList, count, i, path, vertexStack);
            path[vertex] = false;
            //Очистка стека
            vertexStack.pop();
        }
    }
}

bool checkAdjacency(int firstVertex, int secondVertex, ArrayMatrix& vertexList) {
    return vertexList[firstVertex - 1][secondVertex - 1] != 0;
}