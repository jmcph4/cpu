#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define WORD_SIZE 8
#define NUM_REGS 8
#define MEM_SIZE 256
#define BUF_LEN 1000
#define STACK_SIZE 32

#define CODE_SEG_START 0x0
#define DATA_SEG_START 0x400

#define SPECIAL_CHAR '.'

int main(void)
{
  int i = 0;
  
  // registers
  uint8_t regs[NUM_REGS];
  uint8_t PC = 0; // program counter
  int flags[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t SR = 0; // stack pointer
  uint8_t MAR = 0;
  uint8_t MDR = 0;
  
  uint8_t mem[MEM_SIZE]; // memory
  uint8_t stack[STACK_SIZE]; // stack
  
  int stack_top = 0; // top of stack
  
  // for parsing instructions
  int val1 = 0;
  int op1 = 0;
  int op2 = 0;
  int op3 = 0;
  uint8_t temp = 0;
  
  char buf[BUF_LEN]; // input
  
  /***************************************************************************/
  
  buf[0] = '\0';
  
  srand(time(NULL));
  
  // initialise registers and memory to random values (indeterminate)
  for(i=0;i<NUM_REGS;i++)
  {
    regs[i] = rand();
  }
  
  i = 0;
  
  for(i=0;i<MEM_SIZE;i++)
  {
    mem[i] = rand();
  }
  
  i = 0;
  
  
  // main loop
  while(fgets(buf, sizeof(buf), stdin))
  {
    PC = MAR; // update program counter
    
    // reset flags
    for(i=0;i<8;i++)
    {
      flags[i] = 0;
    }
    
    i = 0;
    
    if(buf[0] == SPECIAL_CHAR) // shell commands
    {
      if(strncmp(buf, ".help", strlen(".help")) == 0) // help dialog
      {
        printf("Instructions:\n");
        printf("add a, b, dst\n");
        printf("sub a, b, dst\n");
        printf("mov src, dst\n");
        printf("mov dst, num\n");
        printf("xchg a, b\n");
        printf("push reg\n");
        printf("pop\n");
        printf("hlt\n");
        printf("load\n");
        printf("read dst\n");
        printf("rst\n\n");
	printf("and a, b, dst\n");
        
        printf("Commands:\n");
        printf(".help\n");
        printf(".show\n");
        printf(".quit\n");
      }
      else if(strncmp(buf, ".show", strlen(".show")) == 0) // show registers
      {
        for(i=0;i<NUM_REGS;i++)
        {
          printf("r%d: %d\n", i, regs[i]);
        }
        
        i = 0;
     
        printf("SR: %d\n", SR);
        printf("PC: %d\n", PC);
        printf("FLAGS: %d %d %d %d %d %d %d %d\n", flags[0], flags[1], flags[2], flags[3], flags[4], flags[5], flags[6], flags[7]);
        printf("MAR: %d\n", MAR);
        printf("MDR: %d\n", MDR);
        printf("--------------------------------------------------------------------------------\n");
      }
      else if(strncmp(buf, ".quit", strlen(".quit")) == 0) // exit
      {
        return EXIT_SUCCESS;
      }
      
      MAR--; // counteract
    }
    else // assembly mneumonic
    {
      if(sscanf(buf, "jmp %d", &val1) == 1) // jump to address
      {
        MAR = val1;
        MAR--; // counteract
      }
      else if(sscanf(buf, "add r%d, r%d, r%d", &op1, &op2, &op3) == 3) // add first two registers, store in third
      {
        regs[op3] = regs[op1] + regs[op2];
      }
      else if(sscanf(buf, "sub r%d, r%d, r%d", &op1, &op2, &op3) == 3) // subtract first two registers, store in third
      {
        regs[op3] = regs[op1] - regs[op2];
      
        if(regs[op2] > regs[op1]) // overflow
        {
          flags[0] = 1;
        }
      }
      else if(sscanf(buf, "mov r%d, r%d", &op1, &op2) == 2) // move second register to first
      {
        regs[op2] = regs[op1];
      }
      else if(sscanf(buf, "mov r%d, %d", &op1, &val1) == 2) // move constant to first register
      {
        regs[op1] = val1;
      
        if(val1 > 255) // overflow
        {
          flags[0] = 1;
        }
        else if(val1 < 0) // underflow
        {
          flags[1] = 1;
        }
      }
      else if(sscanf(buf, "push r%d", &op1) == 1) // push register contents onto stack
      {
        if(stack_top == STACK_SIZE)
        {
          // overflow
          flags[2] = 1;
          // should probably throw some form of interrupt
        }
        else
        {
          stack_top++;
          stack[stack_top] = regs[op1];
          SR = stack[stack_top];
        }
      }
      else if(sscanf(buf, "read r%d", &op1) == 1) // copy data in MDR to register
      {
        regs[op1] = MDR;
      }
      else if(sscanf(buf, "and r%d, r%d, r%d", &op1, &op2, &op3) == 3) // logical and of first two registers, store result in third
      {
        if(regs[op1] && regs[op2])
        {
          regs[op3] = 1;
        }
        else
        {
          regs[op3] = 0;
        }
      }
      else if(sscanf(buf, "or r%d, r%d, r%d", &op1, &op2, &op3) == 3) // logical or of first two registers, store result in third
      {
        if(regs[op1] || regs[op2])
        {
          regs[op3] = 1;
        }
        else
        {
          regs[op3] = 0;
        }
      }
      else if(sscanf(buf, "xchg r%d, r%d", &op1, &op2) == 2) // swap registers around
      {
        temp = regs[op1];
        regs[op1] = regs[op2];
        regs[op2] = temp;
      }
      else if(sscanf(buf, "write r%d", &op1) == 1) // copy contents of register to MDR
      {
        MDR = regs[op1];
      }
      else if(strncmp(buf, "save", strlen("save")) == 0) // push contents of MDR to memory bus
      {
        mem[MAR] = MDR;
      }
      else if(strncmp(buf, "pop", strlen("pop")) == 0) // pop an element off of the stack
      {
        if(stack_top == 0)
        {
          // underflow
          flags[3] = 1;
          // should probably throw some form of interrupt
        }
        else
        {
          stack[stack_top] = 0;
          stack_top--;
          SR = stack[stack_top];
        }
      }
      else if(strncmp(buf, "hlt", strlen("hlt")) == 0) // enter HALT state
      {
        break; // HALT
      }
      else if(strncmp(buf, "load", strlen("load")) == 0) // copy contents of memory at address stored in MAR to the MDR
      {
        MDR = mem[MAR];
      }
      else if(strncmp(buf, "nop", strlen("nop")) == 0) // NOP
      {
        // do nothing
      }
      else if(strncmp(buf, "rst", strlen("rst")) == 0) // set all registers to zero
      {
        for(i=0;i<NUM_REGS;i++)
        {
          regs[i] = 0;
        }
        
        i = 0;
      }
      else
      {
        fprintf(stderr, "Invalid input.\n");
        MAR--; // counteract
      }
      
      MAR++; // crawl forward in memory
      PC = MAR; // update program counter
    
      // display contents of registers
      for(i=0;i<NUM_REGS;i++)
      {
        printf("r%d: %d\n", i, regs[i]);
      }
        
      i = 0;
     
      printf("SR: %d\n", SR);
      printf("PC: %d\n", PC);
      printf("FLAGS: %d %d %d %d %d %d %d %d\n", flags[0], flags[1], flags[2], flags[3], flags[4], flags[5], flags[6], flags[7]);
      printf("MAR: %d\n", MAR);
      printf("MDR: %d\n", MDR);
      printf("--------------------------------------------------------------------------------\n");

    }
    
    // clean up
    op1 = 0;
    op2 = 0;
    op3 = 0;
    val1 = 0;
  }
  
  return EXIT_SUCCESS;
}
