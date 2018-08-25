#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

typedef std::vector<std::vector<int>> ArrayMatrix;
typedef std::vector<bool> ArrayBool;
typedef std::stack<int> StackInt;

//Функция, открывающая файл
bool openFile(std::ifstream&, const std::string&);
//Функция, запрашивающая имя файла
std::string getFileName();
//Функция, считывающая матрицу с файла
void processInput(std::ifstream&, unsigned&, ArrayMatrix&);
//Функция, вычисляющая степень вершины
int getDeg(ArrayMatrix&, int, int);
//Функция, проверяющая на условие Дирака
bool conditionDirac(ArrayMatrix&, int);
//Рекурсивная функция поиска гамильтонового цикла
void findHamiltonianCycle(int, ArrayMatrix&, int&,
                          int, ArrayBool&, StackInt&);
//Проверка на смежность
bool checkAdjacency(int, int, ArrayMatrix&);
//Функция, проверяющая на теорему Оре
bool oreTheorem(ArrayMatrix&, int);

int main() {
    std::string fileName = getFileName();
    std::ifstream input;
    if (!openFile(input, fileName))
        return 1;
    std::string graphName;
    std::getline(input, graphName); // ввод названия
    unsigned vertexAmount = 0;
    ArrayMatrix vertexList;
    processInput(input, vertexAmount, vertexList);
    ArrayBool path(vertexAmount);
    StackInt vertexStack;
    int vertex = 0;
    int count = 0;
    short output = 0;

    //Вывод смежной матрицы
    for (int i = 0; i < vertexAmount; ++i) {

        for (int j = 0; j < vertexAmount; ++j) {
            std::cout << vertexList[i][j] << ' ';
        }
        std::cout << '\n';
    }
    //Определение гамильтонового цикла
    if (vertexAmount <= 3) {
        if (vertexAmount == 3) {
            if (checkAdjacency(0, 2, vertexList))
                output = 1;
        }
    } else if (conditionDirac(vertexList, vertexAmount)) {
        output = 2;
    } else if (oreTheorem(vertexList, vertexAmount)) {
        output = 3;
    } else {
        findHamiltonianCycle(vertexAmount, vertexList, count, vertex, path, vertexStack);
        if(count)
            output = 1;
    }
    //Вывод результата
    switch (output) {
        case 1: // Гамильтонов цикл существует
            std::cout << "Hamiltonian Cycle exists.\n";
            break;
        case 2: // условие Дирака выполнено
            std::cout << "Dirac's condition implemented. Hamiltonian Cycle exists.\n";
            break;
        case 3: // теорема Оре выполнена
            std::cout << "Ore's theorem implemented. Hamiltonian Cycle exists.\n";
            break;
        default:// Вывод сообщения об ошибке
            std::cout << "Hamiltonian Cycle is not exists.\n";
            break;
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
    for (int i = 0; i < vertexAmount; ++i)
    {
        //Взятие линии в строку
        std::getline(input, line);

        for (int j = 0; j < vertexAmount; ++j)
        {
            //Символы переопределяются в числа
            vertexList[i].push_back(line[j] == '1' ? 1 : 0);
        }

    }
}

int getDeg(ArrayMatrix& vertexList, int vertexAmount, int vertexTitle) {
    int result = 0;
    //Циклом подсчитывается количество смежных вершин
    for (int i = 0; i < vertexAmount; ++i)
    {
        result += vertexList[vertexTitle][i];
    }
    return result;
}

bool conditionDirac(ArrayMatrix& vertexList, int vertexAmount) {
    //Задаётся тестовое значение
    int check = vertexAmount / 2;
    //Циклом определяется больше ли тестового значения каждая степень вершины
    for (int i = 0; i < vertexAmount; ++i) {
        //Если нет - то условие Дирака не соблюдено
        if(getDeg(vertexList, vertexAmount, i) <= check)
            return false;
    }
    return true;
}

void findHamiltonianCycle(int vertexAmount, ArrayMatrix& vertexList, int& count,
                          int vertex, ArrayBool& path, StackInt& vertexStack) {
    //Добавление вершины в стек
    vertexStack.push(vertex);
    if(vertexStack.size() == vertexAmount && vertexList[vertex][0] == 1)
        count++;

    if (count)
        return;

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
    return vertexList[firstVertex][secondVertex] != 0;
}

bool oreTheorem(ArrayMatrix &vertexList, int vertexAmount) {
    int i;
    for (i = 1; i < vertexAmount; ++i) {
        if (!vertexList[0][i])
            break;
    }
    return getDeg(vertexList, vertexAmount, 0) + getDeg(vertexList, vertexAmount, i) >= vertexAmount;
}

bool openFile(std::ifstream &input, const std::string &fileName) {
    std::string filePath = "inputFiles/" + fileName;
    input.open(filePath, ios_base::in);
    if (!input) {
        std::cout << "Error: file isn't found" << '\n';
        return false;
    }
    return true;
}

std::string getFileName() {
    std::cout << "Enter the file name: ";
    std::string fileName;
    std::cin >> fileName;
    return fileName;
}
