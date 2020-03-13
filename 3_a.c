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
char* copyEven(char** from, char* to);
char* copyOdd(char** from, char* to);
int check(char* s);
int getStr(char** s);

int main() {
    char* outEven, * outOdd;
    char* s = NULL; //строка источник
    char** p = &outOdd; //строка приёмник
    int flag;

    //пока не ввели конец файла работает цикл
    while (1)
    {
        puts("Enter string to make new or CTRL+Z to exit");
        flag = getStr(&s); //flag -  код ошибки
        if (flag == -1) //если он равен -1 - то ввели конец файла
        {
            puts("Goodbye!");
            return 0;
        }
        if (flag == -2) //если равен -2 - нехватка памяти
        {
            puts("Memory is out");
            return 0;
        }
        if (!check(s))
        {
            puts("Incorrect string. A string can only contain numbers and spaces.");
            free(s);
        }
        else
        {
            printf("Source string: \"%s\"\n", s);

            outOdd = (char*)calloc((strlen(s) + 2), sizeof(char));
            outEven = (char*)calloc((strlen(s) + 2), sizeof(char));
     
            form(s, outEven, outOdd);

            printf("Result string of even: \%s\ \n", outEven);
            printf("Result string of odd: \%s\ \n", outOdd);

            free(outEven);
            free(outOdd);
            free(s);
        }
    }
    return 0;
}
//Главная функция
void form(char* in, char* outEven, char* outOdd) {
    while (*(in = skip(in))) {
        if ((*in % 2)==0) 
                outEven = copyEven(&in, outEven);
        else
                outOdd = copyOdd(&in, outOdd);

    }
}
//Для четных чисел
char* copyEven(char** from, char* to) {
    int i, k;
        k = strspn(*from, "02468\t");
        strncat_s(to, k + 1 + strlen(to), *from, k);
        strncat_s(to, 2 + strlen(to), " ", 1);
        for (i = 0; i < k; ++i)
            *(*from)++;
    return to;
}
//Для нечетных чисел
char* copyOdd(char** from, char* to) {
    int i, k;
            k = strspn(*from, "13579\t");
            strncat_s(to, k + 1 + strlen(to), *from, k);
            strncat_s(to, 2 + strlen(to), " ", 1);
            for (i = 0; i < k; ++i)
                *(*from)++;
    return to;
}
//Пропуск пробелов
char* skip(char* str)
{
    while (*str && (*str < '0' || *str>'9'))
        str++;
    return str;
}
//Проверка, что строка состоит из чисел
int check(char* s)
{
    int k = strspn(s, "0123456789 \t"); //подсчитали сколько символов в ходит в перечисленные
    if (k == 0)
        return 0;
    char c = *(s + k); //получили символ после них, если это нуль-байт, то строка ок, иначе - нет
    return c == '\0';
}
//Ввод
int getStr(char** s) //получаем строку из входного потока
{
    char buf[81];
    int n;           //сюда будет записываться результат scanf
    int len = 0;     //сюда длина результирующей строки
    *s = (char*)malloc(1);//указатель на результирующую сткроу
    **s = '\0';//ноль байт, пока строка имеет только конец строки

    do {
        n = scanf_s("%80[^\n]", buf, 81);//считываем буфер
        
        if (n < 0)
        {                      //если ввели конец файла (ctrl+Z), то будет -1
            free(*s);          //очищаем память, возвращаем пустой указатель
            return -1;
        }
       
        if (n > 0) {           //если буфер не пустой
            len += strlen(buf);//увеличиваем результирующую длину
            *s = (char*)realloc(*s, len + 1);//добавляем память
            if (*s)            //если память выделилась
                strcat_s(*s, len + 1, buf);//копируем строку из буфера в конец нашей строки
            else
            {            //если память не выделилась
                free(*s);//очищаем память
                return -2;
            }
        }
        else
            scanf_s("%*c");//если перенос строки, то очищаем входной поток
    } while (n > 0);       //пока во входном потоке есть хоть один символ
    return 0;
}


//преобразования без функций строк
//char* copyEven(char** from, char* to, int* eflag) {
//    if (*eflag == 0) {
//        while (**from == '0' || **from == '2' || **from == '4' || **from == '6' || **from == '8') {
//            *to++ = *(*from)++;
//        }
//        *eflag = 1;
//    }
//    else if (*eflag == 1) {
//        *to++ = ' ';
//        while (**from == '0' || **from == '2' || **from == '4' || **from == '6' || **from == '8') {
//            *to++ = *(*from)++;
//        }
//    }
//    return to;
//}
