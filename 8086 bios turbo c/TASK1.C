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

void main()
{
	clrscr();
	biosputs("Hello world\n\r");
	getch();
}