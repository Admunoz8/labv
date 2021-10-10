#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();
bool areSame(char* str1, char* str2);
int initialCaseHandler(char* str);
int findRegNum(char* str);
void print_regs();

void print_regs(){
  int col_size = 10;
  for (int i = 0; i < 8; i++){
    printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
    printf("X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
    printf("X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
    printf("X%02i:%.*lld", i+24, col_size, (long long int) reg[i+24]);
  }
}

/*Receives register format strings and return the int value of the register.
For example, "X13" -> 13*/
int findRegNum(char* str){ 
  int count = 0;
  char num[2];
  while(*(str+count)!='\0'){
    count++;
  }
  if (count >= 3){ //accounting for the last token to have an extra NULL at the end.
    num[0] = *(str+1); num[1] = *(str+2);
  }
  else
    num[0] = *(str+1);

  return  atoi(num);
}

int initialCaseHandler(char* str){
  if (areSame(str, "LW")) {return 0;}
  else if (areSame(str, "SW")) {return 1;}
  else if (areSame(str, "ADD")) {return 2;}
  else if (areSame(str, "ADDI")) {return 3;}
  else if (areSame(str, "AND")) {return 4;}
  else if (areSame(str, "OR")) {return 5;}
  else if (areSame(str, "XOR")) {return 6;}
  else
    return -1;
}

bool areSame(char* str1, char* str2){
  char* temp1 = str1;
  char* temp2 = str2;
  int count1 = 0;
  int count2 = 0;

  while (*(temp1+count1) != '\0'){
    count1++;
  }
  while (*(temp2+count2) != '\0'){
    count2++;
  }
  if (count1 != count2){return false;}
  
  int i = 0;
  while (*(str1+i) != '\0' && *(str2+i) != '\0'){
    if (*(str1+i) != *(str2+i))
      return false;
    i++;
  }
  return true;
}

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}



/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
  char* textFile = "mem.txt";
  char **pointerStorage;
  pointerStorage = tokenize(instr, ' ');
  //print_all_tokens(pointerStorage); use for debugging purposes.
  int caseNum = initialCaseHandler(pointerStorage[0]); // decides what case the instruction is.
  switch(caseNum){
  case 0:
    printf("LW read");
    char* saveToLW = *(pointerStorage+1);
    int saveToLWint = findRegNum(saveToLW);
    char** memoryTokensLW = tokenize(*(pointerStorage+2), '('); // splits the main 3rd token in 2.
    char** cleanMemoryLW = tokenize(*(memoryTokensLW+1),')'); // cleans the mem token to only reg.

    char* memoryRegLW = *cleanMemoryLW;
    int memoryRegLWint = findRegNum(memoryRegLW);

    int32_t addressLW = atoi(*memoryTokensLW) + memoryRegLWint;
    int32_t readLW = read_address(addressLW, textFile);
    reg[saveToLWint] = readLW; // register is given whatever was read in addressLW
    break;
  case 1:
    printf("SW read");
    char* saveFromSW = *(pointerStorage+1);
    int saveFromSWint = findRegNum(saveFromSW);
    char** memoryTokensSW = tokenize(*(pointerStorage+2), '(');
    char** cleanMemorySW = tokenize(*(memoryTokensSW+1), ')');
    char* memoryRegSW = *cleanMemorySW;
    int memoryRegSWint = findRegNum(memoryRegSW);
    
    int32_t data_to_writeSW = reg[saveFromSWint];
    int32_t addressSW = atoi(*memoryTokensSW) + memoryRegSWint; 
    int32_t write = write_address(data_to_writeSW, addressSW, textFile);
    
    break;
  case 2:
    printf("ADD read");
    char* saveTo = *(pointerStorage+1);
    char* firstOperand = *(pointerStorage+2);
    char* secondOperand = *(pointerStorage+3);

    int saveToReg = findRegNum(saveTo);
    int firstReg = findRegNum(firstOperand);
    int secReg = findRegNum(secondOperand);

    reg[saveToReg] = reg[firstReg] + reg[secReg];
    printf("%d", reg[saveToReg]);
    break;
  case 3:
    printf("ADDI read");
    char* saveToADDI = *(pointerStorage+1);
    char* addTo = *(pointerStorage+2);
    int add = atoi(*(pointerStorage+3));

    int saveToADDIint = findRegNum(saveToADDI);
    int addToint = findRegNum(addTo);

    reg[saveToADDIint] = reg[addToint] + add;
    break;
  case 4:
    printf("AND read");
    char* saveToAnd = *(pointerStorage+1);
    char* firstOperandAnd = *(pointerStorage+2);
    char* secondOperandAnd = *(pointerStorage+3);

    int saveToRegAnd = findRegNum(saveToAnd);
    int firstRegAnd = findRegNum(firstOperandAnd);
    int secRegAnd = findRegNum(secondOperandAnd);

    reg[saveToRegAnd] = reg[firstRegAnd] & reg[secRegAnd];
    break;
  case 5:
    printf("OR read");
    char* saveToOr = *(pointerStorage+1);
    char* firstOperandOr = *(pointerStorage+2);
    char* secondOperandOr = *(pointerStorage+3);

    int saveToRegOr = findRegNum(saveToOr);
    int firstRegOr = findRegNum(firstOperandOr);
    int secRegOr = findRegNum(secondOperandOr);

    reg[saveToRegOr] = reg[firstRegOr] | reg[secRegOr];
    break;
  case 6:
    printf("XOR read");
    char* saveToXor = *(pointerStorage+1);
    char* firstOperandXor = *(pointerStorage+2);
    char* secondOperandXor = *(pointerStorage+3);

    int saveToRegXor = findRegNum(saveToXor);
    int firstRegXor = findRegNum(firstOperandXor);
    int secRegXor = findRegNum(secondOperandXor);

    reg[saveToRegXor] = reg[firstRegXor] ^ reg[secRegXor];
    break;
  default:
    printf("ERROR");
    return false;
  }
  return true;
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	char input[1000]; // User input can only be 100 chars long.
	printf("Enter 'x' to quit.\n");
	while(1){
	  printf("Input: ");
	  fgets(input, sizeof(input), stdin);
	  if (*input == 'x'){break;}
	  interpret(input);
	  printf("\n");
	}
	print_regs();

	return 0;
}
