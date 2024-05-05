#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

int bin2dec(char array[], int size){
    int sum = 0;
    for (int v=0 ; v<=(size-1) ; v++){
    sum += ((array[v]-'0')* pow(2,v));
}
    return sum;
}


int SignedBin2Dec(char array[], int size){
    if((array[size-1] - '0') == 0){
    int sum = 0;
    for (int v=0 ; v<=(size-1) ; v++){
    sum += ((array[v]-'0')* pow(2,v));
}
    return sum;
}
    else{
    int sum = 0;
    int v=0;
    for (; v<=(size-2) ; v++){
    sum += ((array[v]-'0')* pow(2,v));
}
    sum -= pow(2,v);
    return sum;
    }
}


char *SubArray(char array[],int EndIndex,int StartIndex){
    char *sub = malloc(32);
    int index = 0;
    for(int v=StartIndex ; v<=EndIndex ; v++,index++){
     sub[index] = array[v];
    }
    sub[index] = '\0';
    return sub;
}

char *dec2bin(int DecimalNumber){
char *BinaryResult = malloc(33);
int z = 0;
int x;
int y = 0;
//for positive numbers:
if(DecimalNumber>=0){
for(;DecimalNumber>0;z++)
{
BinaryResult[z]=DecimalNumber%2+'0';
DecimalNumber=DecimalNumber/2;
}
for (;z<=31;z++){
BinaryResult[z] = '0';
}
BinaryResult[z] = '\0';

return BinaryResult;
}

//for negative numbers:
if(DecimalNumber<0){
for(;(-DecimalNumber)>0;z++)
{
x = (-DecimalNumber)%2;
if(x==1) x=0;
else x=1;
BinaryResult[z]=x+'0';
DecimalNumber=DecimalNumber/2;
}

for (;z<=31;z++){
BinaryResult[z] = '1';
}

for(;y<=30;y++){
    if (BinaryResult[y] == '0'){
    BinaryResult[y] = '1';
    break;
    }
    else{
    BinaryResult[y] = '0';
    }
}
BinaryResult[z] = '\0';

return BinaryResult;
}

}


int main(){
//registers:
int Register[16];
Register[0] = 0;
for(int z=1;z<=15;z++){
    Register[z] = -31768;
}
//memory:
char Memory[16000][32];
//first, making memory blocks garbage
for(int ev = 0;ev<=15999;ev++){
    for(int ek = 0;ek<=31;ek++){
        Memory[ev][ek] = 'a';
    }
}
//this part reads machine code file
FILE *fp;
char buff[255];
fp = fopen("test 1 machine code.txt", "r");

for(int memoryline=0;!feof(fp);memoryline++){
fscanf(fp, "%s", buff);
for(int z=0;z<=31;z++){
  Memory[memoryline][z] = dec2bin(atoi(buff))[z];
}

//if you run this commented codes , it prints memory lines contents (in binary)
/*
for(int h=31;h>=0;h--){
printf("%c",Memory[memoryline][h]);
}
printf("%c",'\n');
*/



if (feof(fp)) break;




}
fclose(fp);



//from this part machine code begins to execute
int PC = 0;
int NumberOfExecutedInstructions = 0;

while(1==1){
printf("PC: %d   has been executed\nresults:\n",PC);
int opcode = bin2dec( (SubArray(Memory[PC],27,24)) , 4);
//R type instructions:
if( opcode <=4 ){
int SourceRegisterNumber = bin2dec( (SubArray(Memory[PC],23,20)) , 4);
int TargetRegisterNumber = bin2dec( (SubArray(Memory[PC],19,16)) , 4);
int DestinationRegisterNumber = bin2dec( (SubArray(Memory[PC],15,12)) , 4);

switch (opcode){
case 0 :
    Register[DestinationRegisterNumber] = Register[SourceRegisterNumber] + Register[TargetRegisterNumber];
    break;
case 1 :
    Register[DestinationRegisterNumber] = Register[SourceRegisterNumber] - Register[TargetRegisterNumber];
    break;
case 2 :
    if((Register[SourceRegisterNumber]) < (Register[TargetRegisterNumber])){ Register[DestinationRegisterNumber] = 1;}
    else    {Register[DestinationRegisterNumber] = 0;}
    break;
case 3 :
    Register[DestinationRegisterNumber] = (Register[SourceRegisterNumber] | Register[TargetRegisterNumber]);
    break;
case 4 :
    Register[DestinationRegisterNumber] = (~(Register[SourceRegisterNumber] & Register[TargetRegisterNumber]));
    break;
}
PC++;
}

//I type instructions and j instruction
if( (5<=opcode)&&(opcode<=13) ){
int SourceRegisterNumber = bin2dec( (SubArray(Memory[PC],23,20)) , 4);
int TargetRegisterNumber = bin2dec( (SubArray(Memory[PC],19,16)) , 4);
char Offset[16];
for(int z = 0;z<=15;z++){
    Offset[z] = SubArray(Memory[PC],15,0)[z];
}


switch (opcode){
case 5 :
    Register[TargetRegisterNumber] = Register[SourceRegisterNumber] + SignedBin2Dec(Offset,16);
    PC++;
    break;
case 6 :
    if(Register[SourceRegisterNumber]<SignedBin2Dec(Offset,16)){Register[TargetRegisterNumber] = 1;}
    else{Register[TargetRegisterNumber] = 0;}
    PC++;
    break;
case 7 :
    Register[TargetRegisterNumber] = Register[SourceRegisterNumber] | bin2dec(Offset,16);
    PC++;
    break;
case 8 :
    Register[TargetRegisterNumber] = SignedBin2Dec(Offset,16) * pow(2,16);
    PC++;
    break;
case 9 :
    Register[TargetRegisterNumber] = SignedBin2Dec(Memory[Register[SourceRegisterNumber]+SignedBin2Dec(Offset,16)]  ,   32);
    PC++;
    break;
case 10 :
    for(int z = 0;z<=31;z++){
    Memory[Register[SourceRegisterNumber]+SignedBin2Dec(Offset,16)][z] = dec2bin(Register[TargetRegisterNumber])[z];
    }
    PC++;
    break;
case 11 :
    if(Register[SourceRegisterNumber] == Register[TargetRegisterNumber]){
        PC = (PC + 1) + SignedBin2Dec(Offset,16);
    }
    else {PC++;}
    break;
case 12 :
    Register[TargetRegisterNumber] = (PC + 1);
    PC = Register[SourceRegisterNumber];
    break;
case 13 :
    PC = bin2dec(Offset,16);
}
}

//halt instruction
if(opcode==14){
PC++;
printf("this is halt, press any key to show statistics\n");
break;
}





NumberOfExecutedInstructions++;


for(int z=0;z<=15;z++){
if (Register[z] != -31768){ printf("Register [%d] : %d\n",z,Register [z]);}
else{printf("Register [%d] : garbage\n",z);}
}

system("pause");
system("cls");
}
system("pause");
system("cls");

printf("PC: %d\n",PC);
printf("Number of total executed instructions: %d instructions + 1 halt = %d instructions \n",NumberOfExecutedInstructions,NumberOfExecutedInstructions+1);

int UsedMemoryCount = 0;
for(int ev = 0;ev<=15999;ev++){
       if(Memory[ev][0] != 'a') {
        UsedMemoryCount++;
       }
}

int UsedMemoryByte = UsedMemoryCount * 4;
printf("Total used memory: %d Bytes\n",UsedMemoryByte);

int WrittenRegisters;
for(int z = 1;z<=15;z++){
    if(Register[z]!= -31768) {WrittenRegisters++;}
}
int PercentageOfWrittenRegisters = (WrittenRegisters*100)/16;
printf("Percentage of written registers: %d ",PercentageOfWrittenRegisters);
}













