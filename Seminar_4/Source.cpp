#include <iostream>
using namespace std;

class Massiv_chisel {
protected:
	int dim;
	double* ptr;

public:
	Massiv_chisel(int n = 0) : dim(n) {
		if (dim > 0) {
			ptr = new double[dim]();
		}
		else {
			ptr = nullptr;
		}
	}

	Massiv_chisel(const Massiv_chisel& src) : dim(src.dim) {
		if (dim > 0) {
			ptr = new double[dim];
		}
		else {
			ptr = nullptr;
		}
		for (int i = 0; i < dim; i++) {
			ptr[i] = src.ptr[i];
		}
	}

	virtual ~Massiv_chisel() {
		delete[] ptr;
	}

	Massiv_chisel& operator=(const Massiv_chisel& src) {
		if (this == &src) return *this;
		delete[] ptr;
		dim = src.dim;
		if (dim > 0) {
			ptr = new double[dim];
		}
		else {
			ptr = nullptr;
		}
		for (int i = 0; i < dim; i++) {
			ptr[i] = src.ptr[i];
		}
		return *this;
	}

	int size() const { return dim; }

	double& operator[](int index) {
		static double dummy = 0.0;
		if (index < 0 || index >= dim) {
			cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			return dummy;
		}
		return ptr[index];
	}

	const double& operator[](int index) const {
		static double dummy = 0.0;
		if (index < 0 || index >= dim) {
			cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			return dummy;
		}
		return ptr[index];
	}

	friend ostream& operator<<(ostream& os, const Massiv_chisel& arr) {
		os << "[";
		for (int i = 0; i < arr.dim; i++) {
			os << arr.ptr[i];
			if (i < arr.dim - 1) os << ", ";
		}
		os << "]";
		return os;
	}
};

class Polynomial : public Massiv_chisel
{
public:
	Polynomial() : Massiv_chisel(5) {
		ptr[0] = 0.0;
	}

	Polynomial(double a1, double a2, double a3, double a4) : Massiv_chisel(5) {
		ptr[0] = 0.0;
		ptr[1] = a1;
		ptr[2] = a2;
		ptr[3] = a3;
		ptr[4] = a4;
	}

	Polynomial(const Polynomial& src) : Massiv_chisel(src) {}

	Polynomial& operator=(const Polynomial& src) {
		if (this == &src) return *this;
		Massiv_chisel::operator=(src);
		return *this;
	}

	~Polynomial() {}

	double evaluate(double x) const {
		double result = 0.0;
		double power = 1.0;
		for (int i = 0; i < dim; i++) {
			result += ptr[i] * power;
			power *= x;
		}
		return result;
	}

	Polynomial& operator+=(const Polynomial& other) {
		for (int i = 0; i < dim; i++) {
			ptr[i] += other.ptr[i];
		}
		return *this;
	}

	Polynomial& operator-=(const Polynomial& other) {
		for (int i = 0; i < dim; i++) {
			ptr[i] -= other.ptr[i];
		}
		return *this;
	}

	Polynomial& operator*=(double scalar) {
		for (int i = 0; i < dim; i++) {
			ptr[i] *= scalar;
		}
		return *this;
	}

	friend ostream& operator<<(ostream& os, const Polynomial& p) {
		bool first = true;
		for (int i = p.dim - 1; i >= 0; i--) {
			if (p.ptr[i] == 0.0) continue;

			if (!first) {
				if (p.ptr[i] > 0) {
					os << " + ";
				}
				else {
					os << " - ";
				}
			}
			else {
				if (p.ptr[i] < 0) {
					os << "-";
				}
			}

			double absVal;
			if (p.ptr[i] < 0) {
				absVal = -p.ptr[i];
			}
			else {
				absVal = p.ptr[i];
			}

			if (i == 0) {
				os << absVal;
			}
			else if (i == 1) {
				os << absVal << "x";
			}
			else {
				os << absVal << "x^" << i;
			}

			first = false;
		}
		if (first) os << "0";
		return os;
	}
};


class Massiv_Polynomov : public Massiv_chisel {
private:
	Polynomial* polyPtr;

public:
	Massiv_Polynomov(int n = 0) : Massiv_chisel(n), polyPtr(nullptr) {
		if (n > 0) {
			polyPtr = new Polynomial[n];
		}
	}

	Massiv_Polynomov(const Massiv_Polynomov& src) : Massiv_chisel(src.dim), polyPtr(nullptr) {
		if (src.dim > 0) {
			polyPtr = new Polynomial[src.dim];
			for (int i = 0; i < src.dim; i++) {
				polyPtr[i] = src.polyPtr[i];
			}
		}
	}

	~Massiv_Polynomov() override {
		delete[] polyPtr;
	}

	Massiv_Polynomov& operator=(const Massiv_Polynomov& src) {
		if (this == &src) return *this;
		delete[] polyPtr;
		dim = src.dim;
		if (dim > 0) {
			polyPtr = new Polynomial[dim];
		}
		else {
			polyPtr = nullptr;
		}
		for (int i = 0; i < dim; i++) {
			polyPtr[i] = src.polyPtr[i];
		}
		return *this;
	}

	Polynomial& operator[](int index) {
		static Polynomial dummy; // заглушка на случай ошибки
		if (index < 0 || index >= dim) {
			cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			return dummy;
		}
		return polyPtr[index];
	}

	const Polynomial& operator[](int index) const {
		static Polynomial dummy; // заглушка на случай ошибки
		if (index < 0 || index >= dim) {
			cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			return dummy;
		}
		return polyPtr[index];
	}

	friend ostream& operator<<(ostream& os, const Massiv_Polynomov& arr) {
		os << "Massiv_Polynomov [" << arr.dim << "]:\n";
		for (int i = 0; i < arr.dim; i++) {
			os << "  [" << i << "] " << arr.polyPtr[i] << "\n";
		}
		return os;
	}
};

int main() {
	setlocale(0, "russian");

	int n;
	do {
		cout << "Ввод массива полиномов 4 степени (P(0)=0)\n";
		cout << "Введите количество полиномов в массиве (число больше нуля): ";
		cin >> n;
		if (n <= 0) {
			cerr << "Ошибка: размерность должна быть больше нуля! Повторите ввод.\n\n";
		}
	} while (n <= 0);

	Massiv_Polynomov arr(n);

	for (int i = 0; i < n; i++) {
		cout << "\nПолином #" << i << ":\n";
		double a1, a2, a3, a4;
		cout << "  Введите коэффициент при x^1 (a1): ";
		cin >> a1;
		cout << "  Введите коэффициент при x^2 (a2): ";
		cin >> a2;
		cout << "  Введите коэффициент при x^3 (a3): ";
		cin >> a3;
		cout << "  Введите коэффициент при x^4 (a4): ";
		cin >> a4;

		arr[i] = Polynomial(a1, a2, a3, a4);
		cout << "  -> P" << i << "(x) = " << arr[i] << "\n";
		cout << "  -> P" << i << "(0) = " << arr[i].evaluate(0)
			<< " (должно быть 0)\n";
	}

	cout << "\nВведенный массив:\n";
	cout << arr << "\n";

	cout << "\nСумма полиномов:\n";
	int i1, i2;
	do {
		cout << "Введите индексы двух полиномов для сложения (0.." << (n - 1) << "): ";
		cin >> i1 >> i2;
		if (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n) {
			cerr << "Ошибка: индексы должны быть в диапазоне [0, " << (n - 1) << "]. Повторите ввод.\n";
		}
	} while (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n);

	arr[i1] += arr[i2];
	cout << "После arr[" << i1 << "] += arr[" << i2 << "]:\n";
	cout << "  arr[" << i1 << "] = " << arr[i1] << "\n";

	cout << "\nРазность полиномов:\n";
	do {
		cout << "Введите индексы двух полиномов для вычитания (0.." << (n - 1) << "): ";
		cin >> i1 >> i2;
		if (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n) {
			cerr << "Ошибка: индексы должны быть в диапазоне [0, " << (n - 1) << "]. Повторите ввод.\n";
		}
	} while (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n);

	arr[i1] -= arr[i2];
	cout << "После arr[" << i1 << "] -= arr[" << i2 << "]:\n";
	cout << "  arr[" << i1 << "] = " << arr[i1] << "\n";

	cout << "\nУмножение полинома на число:\n";
	int idx;
	double scalar;
	do {
		cout << "Введите индекс полинома для умножения (0.." << (n - 1) << "): ";
		cin >> idx;
		if (idx < 0 || idx >= n) {
			cerr << "Ошибка: индекс должен быть в диапазоне [0, " << (n - 1) << "]. Повторите ввод.\n";
		}
	} while (idx < 0 || idx >= n);

	cout << "Введите число, на которое умножаем arr[" << idx << "]: ";
	cin >> scalar;
	arr[idx] *= scalar;
	cout << "После arr[" << idx << "] *= " << scalar << ":\n";
	cout << "  arr[" << idx << "] = " << arr[idx] << "\n";

	cout << "\nИтоговый массив полиномов:\n";
	cout << arr << "\n";

	double x;
	cout << "\nВведите x для проверки значений полиномов в точке: ";
	cin >> x;
	cout << "Значения полиномов в точке x = " << x << ":\n";
	for (int i = 0; i < n; i++) {
		cout << "  P" << i << "(" << x << ") = " << arr[i].evaluate(x) << "\n";
	}

	return 0;
}
