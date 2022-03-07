#include <iostream>
#include <sstream>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

//task 1

class acout :
    public ostringstream
{
private:
    static mutex mCout;

public:
    ~acout()
    {
        lock_guard<mutex> guard(mCout);
        cout << this->str();
    }
};

mutex acout::mCout;

void Thread1()
{
    acout{} << "Многопоточность: поток 1" << endl;
}
void Thread2()
{
    acout{} << "Многопоточность: поток 2" << endl;
}
void Thread3(size_t num)
{
    acout{} << "Многопоточность: поток 3. Проход " << num << endl;
}

//task 2

bool isPrime(unsigned long long result)
{
	if (result == 1) return false;
	for (size_t i = 2; i < sqrt(result); i++)
	{
		if (result % i == 0)
		{
			return false;
		}
	}
	return true;
}

void Prime(long long index, unsigned long long& result, long long& counter, bool& ready)
{
	while (counter < index)
	{
		result++;
		if (isPrime(result))
		{
			counter++;
		}
	}
	ready = true;
}

long long primeNumber(long long index)
{
	unsigned long long result = 0;
	long long counter = 0;
	bool ready = false;
	thread t2{ Prime, ref(index), ref(result), ref(counter), ref(ready) };
	while (!ready)
	{
		system("cls");
		cout << "Прогресс: " << counter / (index / 100.0) << "%" << endl;
		this_thread::sleep_for(100ms);
	}
	t2.join();
	system("cls");
	cout << "Прогресс: " << counter / (index / 100.0) << "%" << endl;
	return result;
}

	//task 3

	mutex mutexHouse;

	void master(vector<int> &things)
	{
		const static auto seed = chrono::system_clock::now().time_since_epoch().count();
		static mt19937_64 generator(seed);
		uniform_int_distribution<int32_t> dis(1, 100);
		while (!things.empty())
		{
			mutexHouse.lock();
			int new_thing = dis(generator);
			things.push_back(new_thing);
			cout << "В доме появилась вещь стоимостью " << new_thing << endl;
			cout << "Стоимость вещей в доме: " << endl;
			for (const auto& t : things)
			{
				cout << t << " ";
			}
			cout << endl << endl;
			mutexHouse.unlock();
			this_thread::sleep_for(1000ms);
		}
	}

	void thief(vector<int>&things)
	{
		while (!things.empty())
		{
			mutexHouse.lock();
			vector<int>::iterator high_price = max_element(things.begin(), things.end());
			cout << "В дом проник вор и украл вещи стоимостью: " << *high_price << endl;
			things.erase(high_price);
			cout << "Остались вещи по цене: " << endl;
			for (const auto& n : things)
			{
				cout << n << " ";
			}
			cout << endl << endl;
			mutexHouse.unlock();
			this_thread::sleep_for(1s);
		}
	}

int main() {
	//task 1
	setlocale(LC_ALL, "Russian");
	for (size_t i = 0, length = 6; i < length; i++)
	{
		thread t1{ Thread1 };
		thread t2{ Thread2 };
		thread t3{ Thread3, ref(i) };
		t1.join();
		t2.join();
		t3.join();
		acout{} << endl;
	}

	//task 2

	long long primeNumber№ = 1000;
	unsigned long long pNumber = primeNumber(primeNumber№);
	cout << primeNumber№ << "-ое простое число = " << pNumber << endl;
	
	//task 3

	vector<int> things = { 95,126,42,84 };
	thread masterThread(master, ref(things));
	thread theifThread(thief, ref(things));
	masterThread.join();
	theifThread.join();
	cout << "Вор украл вещи в вашем доме!" << endl;
	return 0;
}