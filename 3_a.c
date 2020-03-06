/*
Из входного потока вводится произвольное число строк, состоящих из последовательности десятичных многоразрядных чисел,
разделенных одним или несколькими пробелами и/или знаками табуляции. Длина каждой строки произвольна. Конец ввода определяется концом файла.

При обработке каждой строки сформировать две новые, поместив в первую числа, образованные из четных цифр исходных чисел, 
а во вторую - из нечетных цифр. Числа в образованных строках должны быть разделены только одним пробелом. Полученные строки вывести на экран.

Примечания :
Каждая строка представлена массивом символов, доступ к элементам массива осуществляется с помощью указателей; 
при обработке строк использовать библиотечные функции (<string.h>)

Ввод строк должен быть организован с помощью функции scanf() со спецификациями для ввода строк.
Функцию getchar() или спецификацию %с в scanf() не использовать!

Логически законченные части алгоритма решения задачи должны быть оформлены отдельными функциями с параметрами
(например, пропуск разделителей, создание списка, вывод списка и др).


Целесообразно обработку строк оформить соответствующей функцией, тогда в функции main должны быть ввод строки, 
вызов функции обработки, вывод результата - в цикле, пока не закончится ввод; конец цикла-по достижении конца файла.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void form(char* in, char* outEven, char* outOdd);
char* skip(char* str);
char* copyEven(char** from, char* to, int* nflag);
char* copyOdd(char** from, char* to, int* cflag);
int check(char* s);
int getStr(char** s);

int main() {
    char* outEven, * outOdd;
    char* s = NULL; //строка источник
    char* p = NULL; //строка приёмник
    int flag;
    int amount;
    int* m = NULL;
    //пока не ввели конец файла работает цикл
    while (1)
    {
        puts("Enter string to make new or CTRL+Z to exit");
        //При вводе ctrl+Z функция scanf_s должна выводить значение -1 при этом рекращая работу программы. но почему то не работает((


        flag = getStr(&s); //flag -  код ошибки
        if (flag == -1) //если он равен -1 - то ввели конец файла
            return 0;
        if (flag == -2) //если равен -2 - нехватка памяти
        {
            puts("Memory is out");
            return 0;
        }
        if (!check(s))
        {
            puts("Incorrect string. String could contain inly digits.");
            free(s);
        }
        else
        {
            printf("Source string: \"%s\"\n", s);

            outOdd = (char*)malloc((strlen(s)));
            outEven = (char*)malloc((strlen(s)));

            form(s, outEven, outOdd);

            printf("Result string of nums: \"%s\" \n", outEven);
            printf("Result string of words: \"%s\" \n", outOdd);

            free(outEven);
            free(outOdd);
            free(s);
        }
    }
    return 0;
}

void form(char* in, char* outEven, char* outOdd) {
    int nflag = 0, cflag = 0;

    while (*(in = skip(in))) {
        if (*in == '0' || *in == '2' || *in == '4' || *in == '6' || *in == '8') {
            if (nflag) {
                outEven = copyEven(&in, outEven, &nflag);
                nflag = 1;
            }
            if (!nflag)
                outEven = copyEven(&in, outEven, &nflag);

        }
        else {
            if (cflag) {
                outOdd = copyOdd(&in, outOdd, &cflag);
                cflag = 1;
            }
            if (!cflag)
                outOdd = copyOdd(&in, outOdd, &cflag);
        }
    }
    *outEven = '\0';
    *outOdd = '\0';
}

char* copyEven(char** from, char* to, int* eflag) {
    if (*eflag == 0) {
        while (**from == '0' || **from == '2' || **from == '4' || **from == '6' || **from == '8') {
            *to++ = *(*from)++;
        }
        *eflag = 1;
    }
    else if (*eflag == 1) {
        *to++ = ' ';
        while ((**from == '0' || **from == '2' || **from == '4' || **from == '6' || **from == '8')) {
            *to++ = *(*from)++;
        }
    }
    return to;
}

char* copyOdd(char** from, char* to, int* oflag) {
    if (*oflag == 0) {
        while (**from == '1' || **from == '3' || **from == '5' || **from == '7' || **from == '9') {
            *to++ = *(*from)++;
        }
        *oflag = 1;
    }
    else if (*oflag == 1) {
        *to++ = ' ';
        while (**from == '1' || **from == '3' || **from == '5' || **from == '7' || **from == '9') {
            *to++ = *(*from)++;
        }
    }
    return to;
}

char* skip(char* str)
{
    while (*str && ((*str < '0' || *str>'9') && (*str < 'A' || *str>'Z') && (*str < 'a' || *str>'z'))) {
        str++;
    }
    return str;
}

//проверка, что строка состоит из двотчных чисел
int check(char* s)
{
    int k = strspn(s, "0123456789. \t"); //подсчитали сколько символов в ходит в перечисленные
    char c = *(s + k); //получили символ после них, если это нуль-байт, то строка ок, иначе - нет
    return c == '\0';
}

int getStr(char** s) //получаем строку из входного потока
{
    char buf[21];//считываем из входного потока строку с помощью этого буфера, кусками по 20 символов
    int n;//сюда будет записываться результат scanf
    int len = 0;//сюда длина результирующей строки
    *s = (char*)malloc(1);//указатель на результирующую сткроу
    **s = '\0';//ноль байт, пока строка имеет только конец строки

    do {
        n = scanf_s("%20[^\n]", buf, 21);//считываем буфер
        if (n < 0)
        {//если ввели конец файла (ctrl+Z), то будет -1
            free(*s);//очищаем память, возвращаем пустой указатель
            return -1;
        }
        if (n > 0) {//если буфер не пустой
            len += strlen(buf);//увеличиваем результирующую длину
            *s = (char*)realloc(*s, len + 1);//добавляем память
            if (*s)//если память выделилась
                strcat_s(*s, len + 1, buf);//копируем строку из буфера в конец нашей строки
            else
            {//если память не выделилась
                free(*s);//очищаем память
                return -2;
            }
        }
        else
            scanf_s("%*c");//если перенос строки, то очищаем входной поток
    } while (n > 0);//пока во входном потоке есть хоть один символ
    return 0;
}