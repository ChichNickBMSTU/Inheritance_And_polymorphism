#include <iostream>
#include <cstdlib>

class Massiv_chisel {
protected:
	int dim;
	double* ptr;

public:
	Massiv_chisel(int n = 0) : dim(n) {
		ptr = (dim > 0) ? new double[dim]() : nullptr;
	}

	Massiv_chisel(const Massiv_chisel& src) : dim(src.dim) {
		ptr = (dim > 0) ? new double[dim] : nullptr;
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
		ptr = (dim > 0) ? new double[dim] : nullptr;
		for (int i = 0; i < dim; i++) {
			ptr[i] = src.ptr[i];
		}
		return *this;
	}

	int size() const { return dim; }

	double& operator[](int index) {
		if (index < 0 || index >= dim) {
			std::cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			exit(1);
		}
		return ptr[index];
	}

	const double& operator[](int index) const {
		if (index < 0 || index >= dim) {
			std::cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			exit(1);
		}
		return ptr[index];
	}

	friend std::ostream& operator<<(std::ostream& os, const Massiv_chisel& arr) {
		os << "[";
		for (int i = 0; i < arr.dim; i++) {
			os << arr.ptr[i];
			if (i < arr.dim - 1) os << ", ";
		}
		os << "]";
		return os;
	}
};

class Polynomial {
private:
	static const int ORDER = 5;
	double coeff[ORDER];

public:
	Polynomial() {
		for (int i = 0; i < ORDER; i++) coeff[i] = 0.0;
	}

	Polynomial(double a1, double a2, double a3, double a4) {
		coeff[0] = 0.0;
		coeff[1] = a1;
		coeff[2] = a2;
		coeff[3] = a3;
		coeff[4] = a4;
	}

	Polynomial(const Polynomial& src) {
		for (int i = 0; i < ORDER; i++) coeff[i] = src.coeff[i];
	}

	Polynomial& operator=(const Polynomial& src) {
		if (this == &src) return *this;
		for (int i = 0; i < ORDER; i++) coeff[i] = src.coeff[i];
		return *this;
	}

	~Polynomial() {}

	double& operator[](int index) {
		if (index < 0 || index >= ORDER) {
			std::cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (ORDER - 1) << "]\n";
			exit(1);
		}
		return coeff[index];
	}

	const double& operator[](int index) const {
		if (index < 0 || index >= ORDER) {
			std::cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (ORDER - 1) << "]\n";
			exit(1);
		}
		return coeff[index];
	}

	double evaluate(double x) const {
		double result = 0.0;
		double power = 1.0;
		for (int i = 0; i < ORDER; i++) {
			result += coeff[i] * power;
			power *= x;
		}
		return result;
	}

	Polynomial& operator+=(const Polynomial& other) {
		for (int i = 0; i < ORDER; i++) {
			coeff[i] += other.coeff[i];
		}
		return *this;
	}

	Polynomial& operator-=(const Polynomial& other) {
		for (int i = 0; i < ORDER; i++) {
			coeff[i] -= other.coeff[i];
		}
		return *this;
	}

	Polynomial& operator*=(double scalar) {
		for (int i = 0; i < ORDER; i++) {
			coeff[i] *= scalar;
		}
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
		bool first = true;
		for (int i = ORDER - 1; i >= 0; i--) {
			if (p.coeff[i] == 0.0) continue;

			if (!first) {
				if (p.coeff[i] > 0) {
					os << " + ";
				}
				else {
					os << " - ";
				}
			}
			else {
				if (p.coeff[i] < 0) {
					os << "-";
				}
			}

			double absVal;
			if (p.coeff[i] < 0) {
				absVal = -p.coeff[i];
			}
			else {
				absVal = p.coeff[i];
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
		polyPtr = (dim > 0) ? new Polynomial[dim] : nullptr;
		for (int i = 0; i < dim; i++) {
			polyPtr[i] = src.polyPtr[i];
		}
		return *this;
	}

	Polynomial& operator[](int index) {
		if (index < 0 || index >= dim) {
			std::cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			exit(1);
		}
		return polyPtr[index];
	}

	const Polynomial& operator[](int index) const {
		if (index < 0 || index >= dim) {
			std::cerr << "Ошибка: индекс " << index
				<< " вне границ [0, " << (dim - 1) << "]\n";
			exit(1);
		}
		return polyPtr[index];
	}

	friend std::ostream& operator<<(std::ostream& os, const Massiv_Polynomov& arr) {
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
		std::cout << "Ввод массива полиномов 4 степени (P(0)=0)\n";
		std::cout << "Введите количество полиномов в массиве (положительное число): ";
		std::cin >> n;
		if (n <= 0) {
			std::cerr << "Ошибка: размерность должна быть положительной! Повторите ввод.\n\n";
		}
	} while (n <= 0);

	Massiv_Polynomov arr(n);

	for (int i = 0; i < n; i++) {
		std::cout << "\nПолином #" << i << ":\n";
		double a1, a2, a3, a4;
		std::cout << "  Введите коэффициент при x^1 (a1): ";
		std::cin >> a1;
		std::cout << "  Введите коэффициент при x^2 (a2): ";
		std::cin >> a2;
		std::cout << "  Введите коэффициент при x^3 (a3): ";
		std::cin >> a3;
		std::cout << "  Введите коэффициент при x^4 (a4): ";
		std::cin >> a4;

		arr[i] = Polynomial(a1, a2, a3, a4);
		std::cout << "  -> P" << i << "(x) = " << arr[i] << "\n";
		std::cout << "  -> P" << i << "(0) = " << arr[i].evaluate(0)
			<< " (должно быть 0)\n";
	}

	std::cout << "\nВведенный массив:\n";
	std::cout << arr << "\n";

	std::cout << "\nСумма полиномов:\n";
	int i1, i2;
	do {
		std::cout << "Введите индексы двух полиномов для сложения (0.." << (n - 1) << "): ";
		std::cin >> i1 >> i2;
		if (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n) {
			std::cerr << "Ошибка: индексы должны быть в диапазоне [0, " << (n - 1) << "]. Повторите ввод.\n";
		}
	} while (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n);

	arr[i1] += arr[i2];
	std::cout << "После arr[" << i1 << "] += arr[" << i2 << "]:\n";
	std::cout << "  arr[" << i1 << "] = " << arr[i1] << "\n";

	std::cout << "\nРазность полиномов:\n";
	do {
		std::cout << "Введите индексы двух полиномов для вычитания (0.." << (n - 1) << "): ";
		std::cin >> i1 >> i2;
		if (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n) {
			std::cerr << "Ошибка: индексы должны быть в диапазоне [0, " << (n - 1) << "]. Повторите ввод.\n";
		}
	} while (i1 < 0 || i1 >= n || i2 < 0 || i2 >= n);

	arr[i1] -= arr[i2];
	std::cout << "После arr[" << i1 << "] -= arr[" << i2 << "]:\n";
	std::cout << "  arr[" << i1 << "] = " << arr[i1] << "\n";

	std::cout << "\nУмножение полинома на число:\n";
	int idx;
	double scalar;
	do {
		std::cout << "Введите индекс полинома для умножения (0.." << (n - 1) << "): ";
		std::cin >> idx;
		if (idx < 0 || idx >= n) {
			std::cerr << "Ошибка: индекс должен быть в диапазоне [0, " << (n - 1) << "]. Повторите ввод.\n";
		}
	} while (idx < 0 || idx >= n);

	std::cout << "Введите число, на которое умножаем arr[" << idx << "]: ";
	std::cin >> scalar;
	arr[idx] *= scalar;
	std::cout << "После arr[" << idx << "] *= " << scalar << ":\n";
	std::cout << "  arr[" << idx << "] = " << arr[idx] << "\n";

	std::cout << "\nИтоговый массив полиномов:\n";
	std::cout << arr << "\n";

	double x;
	std::cout << "\nВведите x для проверки значений полиномов в точке: ";
	std::cin >> x;
	std::cout << "Значения полиномов в точке x = " << x << ":\n";
	for (int i = 0; i < n; i++) {
		std::cout << "  P" << i << "(" << x << ") = " << arr[i].evaluate(x) << "\n";
	}

	return 0;
}