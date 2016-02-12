# Instruction Set Architecture #
---

## Registers ##
The CPU has various registers that fulfill various arithmetic and memory operations.

 - General purpose registers
   - `r0`-`r7`
 - Status flags register
   - Status flag bits pertaining to various arithmetic operations
   - `FLAGS`
 - Stack register
   - Holds the contents of the top of the stack
   - `SR`
 - Program counter
   - Points to next instruction
   - `PC`
 - Memory address register
   - `MAR`
 - Memory data register
   - `MDR`

## Instructions ##

### add ###
#### Syntax ####
`add a, b, dst`

#### Description ####
Adds the registers `a` and `b` together and stores the result in register `dst`.

### sub ###
#### Syntax ####
`sub a, b, dst`

#### Description ####
Subtractss the registers `a` and `b` together and stores the result in register `dst`.

### mov ###
#### Syntax ####
`mov src, dst`
`mov dst, num`

#### Description ####
Either moves the contents of register `src` to register `dst`, or sets register `dst` to `num` (depending on which syntax is used).

#### Example ####
```
mov r0, 1 ; r0 = 1
mov r1, 0 ; r1 = 0
mov r0, r1 ; r1 = 1
```

### xchg ###
#### Syntax ####
`xchg a, b`

#### Description ####
Exchanges the contents of register `a` with contents of register `b`.

#### Example ####
```
mov r0, 0
mov r1, 1
xchg r0, r1 ; r0 = 1 and r1 = 0
```

### push ###
#### Syntax ####
`push reg`

#### Description ####
Pushes the contents of register `reg` onto the stack, subsequently populating `SR` with `reg`'s contents as well.

#### Example ####
```
mov r0, 1
push r0 ; SR = 1
```

### pop ###
#### Syntax ####
`pop`

#### Description ####
Pops the top element off of the stack, subsequently making `SR` equal to the next element.

#### Example ####
```
mov r0, 1
push r0 ; SR = 1
pop
```

### hlt ###
#### Syntax ####
`hlt`

#### Description ####
Halts the CPU.

#### Example ####
`hlt`

### load ###
#### Syntax ####
`load`

#### Description ####
Loads whatever data is at the current memory address into the memory data register (`MDR`).

#### Example ####
Assume memory is laid out like so:

|12|15|33|

```
rst ; zero initialise all general purpose registers
load ; MDR = 12
load ; MDR = 15
load ; MDR = 33
```

### read  ###
#### Syntax ####
`read dst`

#### Description ####
Reads the contents of the memory data register (`MDR`) into register `dst`.

#### Example ####
Assume memory is laid out like so:

|12|15|33|

```
load ; MDR = 12
read r0 ; r0 = 12
```
 
### rst ###
#### Syntax ####
`rst`

#### Description ####
Resets the CPU to a zero-initialised state.

#### Example ####
`rst`
