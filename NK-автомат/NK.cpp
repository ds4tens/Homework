

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <map>

using namespace std;

short const n = 6;
short const k = 2;

/*Векторы перехода*/
vector <bool> const n1 = { 0,1,1,0,0,0 };
vector <bool> const n2 = { 1,0,1,0,0,0 };
vector <bool> const n3 = { 0,0,1,1,0,0 };
vector <bool> const n4 = { 1,0,0,0,1,0 };
vector <bool> const n5 = { 0,0,0,0,1,1 };
vector <bool> const n6 = { 1,0,0,0,0,1 };

vector <vector<bool>> Matrix = { n1, n2, n3, n4, n5, n6 }; //матрица перехода

void _refil(vector <bool> & bvec, int iter);//пересоздание базовой матрицы
void _state(vector <bool> & bvec, int iter, vector <vector <string>>& atract);//создание новых состояний
void _find(vector <bool>const& vec, vector <int>& pos); //поиск позиций единичек
bool _func(bool x, bool y, int k); //булевые операции
void _vec_to_string(vector<bool>const& temp, string& str);//переход от вектора к строке
void _uniqe_atr(vector <vector <string >>& atr); //поиск уникальных атракторов
ostream& operator << (ostream& cc, vector <bool> const& bvec); //перегрузка оператора 
bool operator == (string str, vector <string> temp); // оператор сравнения вектора строк

int main()
{
    vector <bool> bvec(n,0); //базовый вектор
    vector <vector <string>> atract; //вектор атракторов
    for (int i = 0; i < pow(2, n); ++i) //перебираем все 64 варинта базового вектора
    {
        _refil(bvec, i); //функция заполнения базового вектора
        _state(bvec, i, atract); //функция перехода состояний
    }
    _uniqe_atr(atract); //функция поиска уник. атрактора
    int a;
    cin >> a;
}

void _refil(vector <bool> & bvec, int iter)
{
    int j = 0;
    bvec.clear();
    bvec.resize(n, 0);//обнуляем вектор
    while (true)
    {
        bvec[j] = iter % 2; //получаем остаток от деления 
        iter = iter / 2; //делим
        if (iter == 0) //если делить больше нечего, выъодим из функции
        {
            return;
        }
        j++; //переходит к след позиции элемента баз вектора
    }
}

void _state(vector <bool>& bvec, int iter, vector <vector <string>>& atract)
{
    cout << "Bvec " << bvec << endl;
    string str = "";
    vector <string> temp;
    temp.resize(0);
    _vec_to_string(bvec, str);//переход к типу строки
    temp.push_back(str); //заносим начальное состояние в атрактор
    vector <int> pos; //позиции элементов из матрицы связи
    for (int m = 0; m < pow(2, n); m++) { //проходимся по всем состояниям
        for (int k = 0; k < n; k++)
        {
            pos.resize(0);
            _find(Matrix[k], pos); //ищем позции связи
            for (int j = 0; j < n; ++j)
                bvec[k] = _func(bvec[pos[0]], bvec[pos[1]], j);// производим лог операции по позициям связи
        }
        cout << "State " << m + 1 << " " << bvec<<endl;
        _vec_to_string(bvec, str);//переводим в строку
        if (str == temp)
        {
            m = pow(2, n);//если нашлось повторение, то заканчиваем поиск атрактора
            atract.push_back(temp); //добавляем состояние к атрактору 
        }
        else
            temp.push_back(str);//добавляем строку и продолжаем поиск
    }
}

void _find(vector<bool> const& vec, vector <int>& pos)
{
    int k = 0;
    for (auto i : vec)
    {
        if (i == 1)
            pos.push_back(k);//нашли единичку и добавили к позиции
        k++;//продолжаем поиск
    }
}

bool _func(bool x, bool y, int k)
{
    /*Список лог операций*/
    if (k == 0)
        return x and y;
    else if (k == 1)
        return x or y;
    else if (k == 2)
        return x xor y;
    else if (k == 3)
        return not(x and y);
    else if (k == 4)
        return not(x or y);
    else if (k == 5)
        return not(x xor y);
    else if (k == 6)
        return x == y;
    return false;
}

void _vec_to_string(vector<bool>const& temp, string& str)
{
    /*Переход в строчный формат*/
    str = "";
    for (auto t : temp) {
        if (t)
            str += "1";
        else
            str += "0";
    }
}

void _uniqe_atr(vector<vector<string>>& atr)
{
    map <string, int> un_atr; //словарь уникальных атракторов
    /*Поиск уникального атрактора по след. принципу
    складываем элементы в столбцах
    101
    +++
    001
    +++
    111
    +++
    100
    ---
    313
    
    */
    for (int i = 0; i < atr.size() ; i++)//берем атрактор
    {
        string sum;
        for (int j = 0; j < n; j++)//проходимся по каждому столбцу
        {
            stringstream temp_sum;
            int temp = 0;
            for (int k = 0; k < atr[i].size(); k++)//о каждому элементу столбца
            {
                stringstream s;
                s << atr[i][k][j];//получаем эелмент столбца
                int t;
                s >> t;
                temp += t;//суммируем
            }
            temp_sum << temp;
            string s;
            temp_sum >>s;
            sum += s;//соеднияем сумму каждого столбца
        }
        un_atr[sum] = i; //добавляем номер атрактора в свою "группу"
    }
    int j = 1;
    for (auto x : un_atr)
    {
        cout << "atr "<<j<<endl;
        for (auto s : atr[x.second])
            cout << s << endl;
        j++;
    }
    cout << "Unique atr " << un_atr.size() << endl;// вывод кол-ва уникальных атр
    int len=0;
    //подсчет средней длины
    for (auto i : atr)
    {
        len += i.size();
    }
    cout << "Overage len " << len / atr.size();
}

ostream& operator<<(ostream& cc, vector<bool> const& bvec)
{
    for (auto item : bvec)
        cc << item;
    return cc;
}

bool operator==(string str, vector<string> temp)
{
    for (auto t : temp)
        if (t == str) //если эелмент нашелся возвращаем правду
            return true;
    return false;
}
