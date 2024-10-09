#include <stdio.h>
#include <stdlib.h>

#define LEFTSIZE 2
#define RIGHTSIZE 8
#define ROUNDS 31

long unsigned int RightKey = 0, LeftKey = 0;

FILE *fp;

char K[31][81];


void readKey()
{
    printf("Enter the rightmost 64 bit of the key:");
    scanf("%lu", &RightKey);

    printf("Enter the leftmost 64 bit of the key:");
    scanf("%lu", &LeftKey);
}


long unsigned int S(long unsigned int c)
{
    switch(c)
    {
        case 0x0:
            return 0xC;
        case 0x1:
            return 0x5;
        case 0x2:
            return 0x6;
        case 0x3:
            return 0xB;
        case 0x4:
            return 0x9;
        case 0x5:
            return 0x0;
        case 0x6:
            return 0xA;
        case 0x7:
            return 0xD;
        case 0x8:
            return 0x3;
        case 0x9:
            return 0xE;
        case 0xA:
            return 0xF;
        case 0xB:
            return 0x8;
        case 0xC:
            return 0x4;
        case 0xD:
            return 0x7;
        case 0xE:
            return 0x1;
        case 0xF:
            return 0x2;
        default:
            return 0;
    }
}

void printKey()
{

    fprintf(fp, "0x");

    fprintf(fp, "%04lX",LeftKey);

    fprintf(fp, "%016lX\n",RightKey);

    printf( "0x");

    printf( "%04lX",LeftKey);

    printf( "%016lX\n",RightKey);

}

void firstStep()
{
    long unsigned int right19[3] = {0,0,0}; // this is the rightmost 19 bits of the key

    right19[2] = RightKey & 0xFF; // get the last 8 bits
    right19[1] = (RightKey >> 8) & 0xFF; // get the bits 8-15
    right19[0] = (RightKey >> 16) & 0x07; // get the  bits 16-23

    RightKey = RightKey >> 19; // shift 19 times

    long unsigned int temp = (LeftKey << 45); // old left key becomes the values from 45 to 60 [ 16 bits]
    long unsigned int of3 = (right19[2] & 0x07) << 61; // previous last 3 bits becomes the bits 61-62-63


    temp = of3 | temp;

    RightKey = RightKey | temp;

    long unsigned int leftBytes[2] = {0,0};

    leftBytes[1] = (right19[2] >> 3);
    leftBytes[1] = leftBytes[1] | (right19[1] << 5 ) ;
    leftBytes[0] = (right19[1] >> 3);
    leftBytes[0] = leftBytes[0] | (right19[0] << 5 ) ;

    LeftKey = 0x0;

    LeftKey = leftBytes[0];
    LeftKey = LeftKey << 8;
    LeftKey = LeftKey | leftBytes[1];
}

void secondStep()
{
    long unsigned int c = (LeftKey >> 12);
    long unsigned int s = (S(c) << 12);
    LeftKey = (LeftKey & 0x0FFF) | s;
}

void thirdStep(unsigned int i)
{
    long unsigned int c = ((RightKey >> 15) & 0x1F), rightPart = RightKey & 0x7FFF;
    c = (c ^ i) << 15; 
    RightKey = RightKey & 0xFFFFFFFFFFF00000;
    RightKey = RightKey | c;
    RightKey = rightPart |  RightKey;
}

void updateKey(int i)
{
    
    firstStep();
    secondStep();
    thirdStep(i+1);
    return;
}

void  keySchedule()
{
    for(int i = 0 ; i < ROUNDS ; i++)
    {
        printKey();
        printf("**********************\n");
        updateKey(i);
    }
    printKey();
}

void openFile()
{
    fp = fopen("keys.txt", "w");
}

void closeFile()
{
    fclose(fp);
}

int main()
{

    openFile();

    readKey();

    keySchedule();

    closeFile();
    
    return  0;
}
