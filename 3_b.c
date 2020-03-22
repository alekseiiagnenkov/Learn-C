/*Из входного потока вводится произвольное число строк. Длина каждой строки произваольна.
Конец ввода определяется концом ввода файла.

Сжать каждую строку, удалив из нее все символы, совпадающие с первым отличным от пробела
символом исходной строки, а так же лишние пробелы и символы тамбуляции. В новой строке все
слова должны быть распределены только одним пробелом. Полученную строку вывести на экран.

Примечания:
1. Каждая строка представлена списком; первое поле элемента списка- символ строки;
второе- указатель на следующий элемент списка или NULL в конце списка; список результата
формировать, модифицируя исходный список.

2. Ввод строк должен быть организован с помощью функции scanf() со спецификациями для ввода
строк. Функцию getchar() или спецификацию %c в scanf() не использовать!

3. Логически законченные части алгоритма решения задачи должны быть оформлены отдельными
функциями с параметрами (например, пропуск разделителей, создание списка, вывод списка и др ).

4. Целесообразно обработку строк оформить соответствующей функцией, тогда в функции main
должны быть ввод строки, вызов функции обработки, вывод результата - в цикле, пока не
закончится ввод; конец цикла - по достижении конца файла.

5. Все функции должны работать со списком; при вводе строки следует сразу создавать список.*/

#include <stdio.h>
#include <malloc.h>
typedef struct Item {
	char c;
	struct Item* next;
} Item;

int getList(Item**);
void putList(char*, Item*);
Item* reorg(Item*);
Item* deleteList(Item*);
Item* skip(Item*,int*);
char* skip_str(char*);
Item* DeletS(Item*,char*,int*);

//Главная функция
int main()
{
	Item* st;

	while (puts("Enter string:"), getList(&st)) {
		putList("Put string:", st);
		if (st == NULL)
			continue;
		st = reorg(st);
		putList("Out string:", st);
		st = deleteList(st);
	}
	return 0;
}

//Преобразования
Item* reorg(Item* ptr)
{
	Item* head = NULL;
	Item* tmp = NULL;
	char symbol = ' '; // Первый символ
	//head = ptr;
	int flag=1, flagS=1, flagT=0, flagHead=0;

	while (ptr != NULL) {
		if (ptr->next != NULL) {
			flag = 0;
			flagS = 0;
			ptr = skip(ptr, &flag);
			if (ptr->c != ' ' && flagT == 0) {
				symbol = ptr->c;
				flagT = 1;
				tmp = ptr;
			}
			ptr = DeletS(ptr, symbol, &flagS);
			if (flag == 0 && flagS == 0)
				ptr = ptr->next;
			if (tmp != ptr && flagHead == 0) {
				head = ptr;
				free(tmp);
				flagHead = 1;
			}
		}
		else {
			printf("[%c] was deleted.\n", symbol);
			ptr = head;
			return head;
		}


	}
	printf("[%c] was deleted.\n", symbol);
	ptr = head;
	return head;
}

Item* DeletS(Item* ptr, char symbol, int* flagS)
{
	Item* tmp = NULL, *tmp1 = NULL, *tmp2=NULL;
	int a;

	while (ptr->next->c == symbol) {
		if (ptr->next->next != NULL) {
			tmp = ptr->next;
			ptr->next = ptr->next->next;
			if (ptr->next->next != NULL) {
				if (ptr->next->c == symbol && ptr->next->next->c == ' ') {
					tmp1 = ptr->next;
					ptr->next = ptr->next->next;
					free(tmp1);
				}
			}
			ptr = skip(ptr, &a);
			if (ptr->next->next != NULL) {
				if (ptr->c == ' ' && ptr->next->c == ' ' && ptr->next->next != NULL) {
					tmp2 = ptr->next;
					ptr->next = ptr->next->next;
					free(tmp2);
				}
			}

			free(tmp);
			*flagS = 1;
			if (ptr->next == NULL) {
				tmp = ptr->next;
				ptr = NULL;
				free(tmp);
				return ptr;
			}
		}
		else {
			tmp = ptr->next->next;
			ptr->next = NULL;
			free (tmp);
			return ptr;
		}
	}
	return ptr;
}

//Пропуск пробелов
Item* skip(Item* ptr, int* flag)
{
	Item* tmp = NULL;

	if (ptr->next == NULL)
		return ptr;
	if (ptr->next->next == NULL)
		return ptr;
	while (((ptr->next->c < '0' || ptr->next->c > '9') && (ptr->next->c < 'A' || ptr->next->c > 'Z') && (ptr->next->c < 'a' || ptr->next->c > 'z') && (ptr->next != NULL)) && ((ptr->next->next->c < '0' || ptr->next->next->c > '9') && (ptr->next->next->c < 'A' || ptr->next->next->c > 'Z') && (ptr->next->next->c < 'a' || ptr->next->next->c > 'z') && (ptr->next->next != NULL))) {
		tmp = ptr->next;
		ptr->next = ptr->next->next;
		free(tmp);
		if (ptr->next == NULL)
			return ptr;
		if (ptr->next->next == NULL)
			return ptr;
		*flag = 1;
	}
	return ptr;
}

// Вывод строчки(Спика).
void putList(char* msg, Item* ptr)
{
	printf("%s:", msg);
	for (; ptr != NULL; ptr = ptr->next)
			printf("%c", ptr->c);
	printf("\n");
}

//Очищение списка.
Item* deleteList(Item* ptr)
{
	Item* tmp = NULL;

	while (ptr != NULL) {
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return ptr;
}

//Пропуск пробелов
char* skip_str(char* str)
{
	while (*str && (*str < '0' || *str>'9') && (*str < 'A' || *str > 'Z') && (*str < 'a' || *str > 'z'))
		str++;
	return str;
}

//Ввод.Заполнение списка.
int getList(Item** pptr)
{
	char buf[81], * str;
	Item head = { '*', NULL };
	Item* last = &head;
	int n, rc = 1;

	do {
		n = scanf_s("%80[^\n]", buf, 81);
		if (n == 0) {
			scanf_s("%*c");
			continue;
		}
		if (n < 0) {
			deleteList(head.next);
			head.next = NULL;
			rc = 0;
			continue;
		}
		str = buf;
		str = skip_str(str);
		if (n > 0) {
			for (str; *str != '\0'; ++str) {
				last->next = (Item*)malloc(sizeof(Item));
				last = last->next;
				last->c = *str;
			}
			last->next = NULL;
		}
		else
			scanf_s("%*c");
	} while (n > 0);
	*pptr = head.next;
	return rc;
}