/* 1.27 Из входного потока вводится непрямоугольная матрица целых чисел[aij], i = 1, …, m, j = 1, …, n.
Значения m и ni заранее не известны и вводятся из входного потока.
Сформировать новую матрицу, поместив в ее i - ую строку сначала те  элементы из i - ой строки исходной матрицы,
в записи которых цифры расположены строго по возрастанию, а затем те, в записи которых цифры расположены строго по убыванию.
Исходную и полученную матрицы вывести в выходной поток с необходимыми комментариями.*/

#include <stdio.h>
#include <malloc.h>


// структура для задания строки матрицы
typedef struct Line {

	int n; // количество элементов в строке матрицы
	int* a; // массив элементов

} Line;

// структура для задания самой матрицы
typedef struct Matrix {

	int lines; // количество строк матрицы
	Line* matr; // массив строк матрицы

} Matrix;

int getInt(int*); // ввод целого числа
int input(Matrix* a); // ввод матрицы
void output(const char* msg, Matrix a); // вывод матрицы
void erase(Matrix* a); // освобождение памяти

// функция ввода матрицы
int input(Matrix* rm)
{
	const char* pr = ""; // будущее сообщении об ошибке
	int m, i, j; // m - количество строк
	int* p;
	do {
		printf("%s\n", pr);
		printf("Enter number of lines: --> ");
		pr = "You are wrong; repeat, please!";
		if (getInt(&m) == 0)
			return 0;
	} while (m < 1);
	rm->lines = m;
	// выделение памяти под массив структур - строк матрицы
	rm->matr = (Line*)calloc(m, sizeof(Line));
	for (i = 0; i < rm->lines; i++) {
		// ввод количества столбцов для каждой строки
		pr = "";
		do {
			printf("%s\n", pr);
			printf("Enter number of items in line %d: --> ", i + 1);
			pr = "You are wrong; repeat, please!";
			if (getInt(&m) == 0) {
				rm->lines = i;
				erase(rm);
				return 0;
			}
		} while (m < 1);
		rm->matr[i].n = m;
		p = (int*)malloc(sizeof(int) * m);
		rm->matr[i].a = p;
		// ввод элементов строки матрицы
		printf("Enter items for matrix line #%d:\n", i + 1);
		for (j = 0; j < m; ++j, ++p)
			if (getInt(p) == 0) {
				rm->lines = i + 1;
				erase(rm);
				return 0;
			}
	}
	return 1;
}


// вывод
void output(const char* msg, Matrix a)
{
	int i, j;
	int* p;
	printf("%s:\n", msg);
	for (i = 0; i < a.lines; ++i) {
		p = a.matr[i].a;
		for (j = 0; j < a.matr[i].n; ++j, ++p)
			printf("%d ", *p);
		printf("\n");
	}
}

// осовбождение занятой памяти
void erase(Matrix* a)
{
	int i;
	for (i = 0; i < a->lines; ++i)
		free(a->matr[i].a);
	free(a->matr);
	a->lines = 0;
	a->matr = NULL;
}

// ввод целого числа 
// при обнаружении ошибки ввод повторяется
// функция возвращает 1, если ввод корректен,
// и 0 при обнаружении конца файла 
int getInt(int* a)
{
	int n;
	do {
		n = scanf("%d", a, sizeof(int));
		if (n < 0) // обнаружен конец файла
			return 0;
		if (n == 0) { // некорректный ввод - ошибка
			printf("%s\n", "Error! Repeat input");
			scanf("%*c", 0);
		}
	} while (n == 0);
	return 1;
}


//проверка на возрастание
int check_growing(int num) {
	int flag = 1;
	while (num > 0) {
		if ((num % 100) / 10 < (num % 10)) {
			num = num / 10;
		}
		else {
			flag = 0;
			break;
		}
	}
	return flag;
}

//проверка на убывание
int check_falling(int num) {
	int flag = 1;
	while (num > 0) {
		if ((num % 100) / 10 > (num % 10)) {
			num = num / 10;
		}
		else {
			flag = 0;
			break;
		}
	}
	return flag;
}

// формирование новой матрицы(результат)
void result(Matrix sm) {
	int res;
	int k = 0;
	int help;
	for (int i = 0; i < sm.lines; ++i) { // проход по всем строкам матрицы
		for (int j = 0; (sm.matr[i].n) - k != j; ++j) {
			if (check_growing(sm.matr[i].a[j])) {
				continue;
			}
			else if (check_falling(sm.matr[i].a[j])) {
				while (check_falling(sm.matr[i].a[(sm.matr[i].n) - k]))
					k++;
				if (check_growing(sm.matr[i].a[(sm.matr[i].n) - k])) {
					help = sm.matr[i].a[j];
					sm.matr[i].a[j] = sm.matr[i].a[(sm.matr[i].n) - k + 1];
					sm.matr[i].a[(sm.matr[i].n) - k] = help;
				}
			}
			else if ((check_growing(sm.matr[i].a[(sm.matr[i].n) - k] == 0)) && (check_falling(sm.matr[i].a[(sm.matr[i].n) - k] == 0))) {
				for (int t = j; t < (sm.matr[i].n) - 1; t++)
					sm.matr[i].a[t] = sm.matr[i].a[t + 1];
				(sm.matr[i].n)--;
			}
		}
	}
}

int main()
{
	Matrix matr = { 0, NULL }; // исходный массив
	Matrix res; // полученный результат
	int* p;

	if (input(&matr) == 0) { // ввод матрицы

		printf("%s\n", "End of file occured");
		return 1;

	}
	output("Source matrix", matr);
	result(matr);
	output("Result", matr);
	erase(&matr); // освобождение памяти
	return 0;

}
