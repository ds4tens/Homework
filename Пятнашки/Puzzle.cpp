// Puzzle.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <utility>
#include <string>
#include <vector>

using namespace std;

enum direction {
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
};

class State
{
public:
	State(const string& str1, const string& str2, size_t d)
	{
		curState = str1;
		finalState = str2;
		deep = d;
		Manh();
		MakeW();
	}
	~State() {};

	size_t CountInverse(string str) const; //подсчет инверсий
	size_t getY(size_t index) const; //положение элемента по Y
	size_t getX(size_t index) const; //положение элемента по Х
	size_t GetIndex(size_t x, size_t y) const; //Получить индекс, зная Х и Y
	pair<bool, State> Move(direction dir); //возможные передвижения пустой клетки
	bool Compare(); //досгли ли конечного состояния
	void FullPrint()
	{
		for (size_t i = 0; i < curState.size() / 3; i++)
			cout << curState[i*3] << " " << curState[i*3 + 1] << " " << curState[i*3 + 2] << endl;
		cout << "Manhattan distance is: " << manh<<endl;
		cout << "Deep is:" << deep << endl;
		cout << "Function is: " << weight<<endl;
		cout << endl;
	} //печать всех данных
	void Print() {
		for (size_t i = 0; i < curState.size() / 3; i++)
			cout << curState[i * 3] << " " << curState[i * 3 + 1] << " " << curState[i * 3 + 2] << endl;
		cout << endl;
	} //распечатать состояние
	string getStr() const {
		return curState;
	} //получить строку
	size_t getmanh() const {
		return manh;
	} //получить Манхэттенское расстояние
	size_t Getdeep () const
	{
		return deep;
	} //получить гулбину
	size_t Getweight() const
	{
		return weight;
	} //получить вес
private:
	size_t weight;
	void MakeW();
	void Manh();
	string finalState;
	string curState;
	size_t manh;
	size_t deep;
};

bool IsSolvable(const State& state)
{
	//проверяет четность начальной и конечной строки
	bool startStateIsOdd = state.CountInverse("start") % 2 == 1;
	bool finishStateIsOdd = state.CountInverse("finish") % 2 == 1;
	return (startStateIsOdd == finishStateIsOdd);
}
//оператор для сравнения строки и вектора состояний
bool operator != (const string& lhs, const vector <State>& rhs)
{
	for (auto item : rhs)
		if (item.getStr() == lhs)
			return false;
	return true;
}
//поиск элемента с минимальным значением функции из всех возможных комбинаций
State GetMinM(const vector <State>& temp, vector<State>& Paths)
{
	size_t index=0;
	size_t min = temp[index].Getweight();
	for (size_t i = 0; i < temp.size(); i++)
	{
		if (temp[i].Getweight() < min)
		{
			min = temp[i].Getweight();
			index = i;
		}
	}
	//все остальные элементы добавляем в список посещенных вершин
	for (size_t i = 0; i < temp.size(); i++)
	{
		if (i == index)
			continue;
		Paths.push_back(temp[i]);
	}
	return temp[index];
}
//поиск минимального элемента из открытого списка
pair <size_t, size_t> MinPath(vector <State>& temp)
{
	size_t min = temp[0].Getweight();
	State state = temp[0];
	size_t item = 0;
	size_t j = 0;
	for (item = 0 ; item < temp.size(); item++)
	{
		if (temp[item].Getweight() <= min)
		{
			min = temp[item].Getweight();
			state = temp[item];
			j = item;
		}
	}
	return make_pair(min, j);
}

vector <State> Path(State state)
{
	vector <State> OpenPath; //открытый список
	vector <State> allPath; //посещенные пути
	vector <State> CorectPath; //конечный путь
	CorectPath.push_back(state);
	allPath.push_back(state);
	size_t iter = 1; //шаг
	while (!state.Compare())
	{
		cout << "Step " << iter<<endl;
		vector <State> temp_vec; //временный вектор
		cout << "Possible movement options" << endl;
		for (direction dir : {LEFT, RIGHT, UP, DOWN}) //проходимся по каждому движению
		{
			auto temp = state.Move(dir);
			if (temp.first && (temp.second.getStr() != allPath)) //если движение возможно и не совершалось до этого
			{
				temp_vec.push_back(temp.second);//добавляет его во временный вектор
				temp.second.FullPrint();
				allPath.push_back(temp.second); //добавляем в закрытый список
			}
		}
		state = GetMinM(temp_vec,OpenPath); //получаем состояние с минимальным значением функции
		pair <size_t, size_t> minPath = MinPath(OpenPath); //Получаем элемент с минимальным значением функции из открытого списка
		if (state.Getweight() > minPath.first) //если в открытом списке есть вершина с меньшим значением функции
		{
			cout << "Encountered the terminal node "<<endl;
			for (size_t k = 0; k < (state.Getdeep() - OpenPath[minPath.second].Getdeep()); k++)
			{
				CorectPath.pop_back(); //выкидываем посещенные вершины, возвращаясь к нужной вершине
			}
			cout << "Back to state: "<<endl;
			state = OpenPath[minPath.second]; //состояние из открытого списка с мин значением
			swap(OpenPath[OpenPath.size() - 1], OpenPath[minPath.second]); //меняем местами конечный элемент с элементом который удалим
			OpenPath.pop_back();
			state.Print();
		}
		else //иначе добавляем состояние к конечному пути
		{
			CorectPath.push_back(state); 
			cout << "Selected state: " << endl;
			state.FullPrint();
		}
		iter++;
	}
	return(CorectPath);
}

int main()
{	
	cout << "Only 3x3" << endl;
	string str1, str2;
	cout << "Enter start state: ";
	cin >> str1;
	cout << "Enter final state: ";
	cin >> str2;

	State state(str1, str2, 0);
	if (IsSolvable(state)) {
		auto ss = Path(state);
		cout << "Path is" << endl;
		for (auto item : ss)
		{
			item.Print();
			cout << endl;
		}
	}
	else
		cout << "Unsolvable";
	return 0;
}


size_t State::CountInverse(string str) const
{
	//сравниваем сколько элементов стоит не на своих местах для конечной и начальной строки
	if (str == "start") {
		size_t counter = 0;
		for (size_t i = 0; i < curState.size(); ++i)
		{
			for (size_t j = i; j < curState.size(); ++j)
			{
				auto ch1 = curState[i];
				auto ch2 = curState[j];
				if (ch1 == '0' or ch2 == '0')
					continue;
				else if (ch2 < ch1)
					counter++;
			}
		}
		return size_t(counter);
	}
	else if (str == "finish")
	{
		size_t counter = 0;
		for (size_t i = 0; i < finalState.size(); ++i)
		{
			for (size_t j = i; j < finalState.size(); ++j)
			{
				auto ch1 = finalState[i];
				auto ch2 = finalState[j];
				if (ch1 == '0' or ch2 == '0')
					continue;
				else if (ch2 < ch1)
					counter++;
			}
		}
		return size_t(counter);
	}
}

size_t State::getY(size_t index) const
{
	return (index / 3);
}

size_t State::getX(size_t index) const
{
	return (index % 3);
}

size_t State::GetIndex(size_t x, size_t y) const
{
	return ((y * 3) + x);
}

void State::MakeW()
{
	weight = manh + deep;
}
/*
void State::Manh()
{
	manh = 0;
	for (size_t i = 0; i < curState.size(); i++)
	{
		int index1 = i;
		int index2 = i;
		char ch = curState[index1];
		for (size_t j = 0; j < finalState.size(); j++)
		{
			if (ch == finalState[j])
				index2 = j;
		}
		manh += abs(index1 - index2);
	}
}
*/
void State::Manh()
{
	//формула dist = |X2-X1|+|Y2-Y1|
	manh = 0;
	for (size_t i = 0; i < curState.size(); i++)
	{
		int X1 = getX(i);
		int X2 = 0;
		int Y1 = getY(i);
		int Y2 = 0;
		for (size_t j = 0; j < finalState.size(); j++)
		{
			if (curState[i] == finalState[j])
			{
				X2 = getX(j);
				Y2 = getY(j);\
				manh += (abs(X2 - X1) + abs(Y2 - Y1));
			}
		}
	}
}


pair<bool, State> State::Move(direction dir)
{
	size_t zIndex = 0;
	for (size_t i = 0; i < curState.size(); i++)
	{
		if (curState[i] == '0')
			zIndex = i;
	}
	//получаем положение 0
	size_t zX = getX(zIndex);
	size_t zY = getY(zIndex);
	//исключаем все невозможнвые перемещения 0. Возвращая первым элементом false
	if (dir == LEFT and zX == 0
		or dir == RIGHT and zX >= 2
		or dir == DOWN and zY >= 2
		or dir == UP and zY == 0) 
	{
		return make_pair(false, *this);
	}
	size_t index = 0;
	//в зависимости от полученного направления получаем новое положение 0
	if (dir == LEFT)
		index = GetIndex(zX - 1, zY);
	else if (dir == UP)
		index = GetIndex(zX, zY - 1);
	else if (dir == DOWN)
		index = GetIndex(zX, zY + 1);
	else if (dir == RIGHT)
		index = GetIndex(zX +1, zY);
	string tempCur = curState;
	string tempfin = finalState;
	swap(tempCur[index], tempCur[zIndex]); //меняем 0 и элемент
	State state(tempCur, tempfin, deep + 1);//создаем новое состояние с новой позицией нуля
	return make_pair(true, state);
}

bool State::Compare()
{
	return curState == finalState;
}
