/*
�� �������� ������ �������� ������������ ����� �����, ��������� �� ������������������ ���������� �������������� �����,
����������� ����� ��� ����������� ��������� �/��� ������� ���������. ����� ������ ������ �����������. ����� ����� ������������ ������ �����.

��� ��������� ������ ������ ������������ ��� �����, �������� � ������ �����, ������������ �� ������ ���� �������� �����, 
� �� ������ - �� �������� ����. ����� � ������������ ������� ������ ���� ��������� ������ ����� ��������. ���������� ������ ������� �� �����.

���������� :
������ ������ ������������ �������� ��������, ������ � ��������� ������� �������������� � ������� ����������; 
��� ��������� ����� ������������ ������������ ������� (<string.h>)

���� ����� ������ ���� ����������� � ������� ������� scanf() �� �������������� ��� ����� �����.
������� getchar() ��� ������������ %� � scanf() �� ������������!

��������� ����������� ����� ��������� ������� ������ ������ ���� ��������� ���������� ��������� � �����������
(��������, ������� ������������, �������� ������, ����� ������ � ��).


������������� ��������� ����� �������� ��������������� ��������, ����� � ������� main ������ ���� ���� ������, 
����� ������� ���������, ����� ���������� - � �����, ���� �� ���������� ����; ����� �����-�� ���������� ����� �����.
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
    char* s = NULL; //������ ��������
    char* p = NULL; //������ �������
    int flag;
    int amount;
    int* m = NULL;
    //���� �� ����� ����� ����� �������� ����
    while (1)
    {
        puts("Enter string to make new or CTRL+Z to exit");
        //��� ����� ctrl+Z ������� scanf_s ������ �������� �������� -1 ��� ���� �������� ������ ���������. �� ������ �� �� ��������((


        flag = getStr(&s); //flag -  ��� ������
        if (flag == -1) //���� �� ����� -1 - �� ����� ����� �����
            return 0;
        if (flag == -2) //���� ����� -2 - �������� ������
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

//��������, ��� ������ ������� �� �������� �����
int check(char* s)
{
    int k = strspn(s, "0123456789. \t"); //���������� ������� �������� � ����� � �������������
    char c = *(s + k); //�������� ������ ����� ���, ���� ��� ����-����, �� ������ ��, ����� - ���
    return c == '\0';
}

int getStr(char** s) //�������� ������ �� �������� ������
{
    char buf[21];//��������� �� �������� ������ ������ � ������� ����� ������, ������� �� 20 ��������
    int n;//���� ����� ������������ ��������� scanf
    int len = 0;//���� ����� �������������� ������
    *s = (char*)malloc(1);//��������� �� �������������� ������
    **s = '\0';//���� ����, ���� ������ ����� ������ ����� ������

    do {
        n = scanf_s("%20[^\n]", buf, 21);//��������� �����
        if (n < 0)
        {//���� ����� ����� ����� (ctrl+Z), �� ����� -1
            free(*s);//������� ������, ���������� ������ ���������
            return -1;
        }
        if (n > 0) {//���� ����� �� ������
            len += strlen(buf);//����������� �������������� �����
            *s = (char*)realloc(*s, len + 1);//��������� ������
            if (*s)//���� ������ ����������
                strcat_s(*s, len + 1, buf);//�������� ������ �� ������ � ����� ����� ������
            else
            {//���� ������ �� ����������
                free(*s);//������� ������
                return -2;
            }
        }
        else
            scanf_s("%*c");//���� ������� ������, �� ������� ������� �����
    } while (n > 0);//���� �� ������� ������ ���� ���� ���� ������
    return 0;
}