/*******************************************
Statement - Print Hello World
Programmer - Vineet Choudhary
Written For - http://developerinsider.in
********************************************/

#include <stdio.h>
#include <conio.h>

void biosputs(char* str)
{
    int i=0;
    char holdc;

    while(str[i])
    {
        holdc = str[i++];
        _asm {
                mov ah, 0Eh
                mov al, holdc
                mov bx, 7
                int 10h
            } // end asm
    } // end while
} // end function

void biosgets(char* str)
{
    char c='0';
    int i=0;
    while(c!='\r'&& i<19)
    {
	_asm {
		mov ah, 1
		int 21h
		mov c, al
	    } // end asm
	str[i] = c;
	i++;
    } // end while
    str[i]='\0';
    return;
} // end function

void main()
{
    char str1[20];
    char str2[20];
    clrscr();

    biosputs("Enter something1: ");
    gets(str1);
    biosputs(str1);
    biosputs("\n\r");

    biosputs("Enter something2: ");
    biosgets(str2);
//    biosputs("\n\r");
    biosputs(str2);
    biosputs("\n\r");

    getch();
}