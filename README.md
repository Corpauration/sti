# sti

simplest STack Interpreter
made for educational purpose

## Compilation

  $ make
  
## Quickstart

Open a file named test.st , and paste :

```
push 1
push 1
add
print
```

Run :

  $ ./sti test.st
  
  
## Instruction list

- push <num> : push a num at the top of the stack
- drop : drops the top element of the stack
- dup : duplicate an element on top of the stack
- swap : swap 2 elements on the top of the stack
- over : copy the element below the top of the stack and put it at the top
- not : doing the boolean not operation on the top of the stack element
- eq : test equality between the first two element, drop them, and push the result on the top
- lt : test less than between the first two element, drop them, and push the result on the top
- jump <num> : jump to line num+1
- jumpif <num> : if the top stack element is not 0, drop it, and jump to line num+1
- add : add the two first element of the stack, drop them, and push the result at the top
- print : print the top element as a num and drop it