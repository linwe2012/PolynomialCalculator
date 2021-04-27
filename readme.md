# Symbolic Polynomial Calculator

It's a symbolic polynomial calculator written for C Programming Language. Although the file extension is `.cpp`, it is written in C.
It supports:

- defining 52 different symbols

- reference previous expression

- naming the expression

- give value for certain symbol

- saving expression in disk, and reference then when reopen the application

## Example

### Defining Expressions

It is accepts only 52 kinds of variable (which is a-z and A-Z). You can write expression like:


```
x + 1+z
```

The result will be:
```
x+z+1 (ID: 1)
```

Then you can write another expression:
```
a+x
```
the result is:
```
a+x (ID: 2)
```

### Referencing Expression

And you can reference previous expression:
```
#19*#20+b
```
which gives:
```
xa+x^2+za+xz+a+x+b (ID: 3)
```

### Assigning Value to Symbol
Finally we give valye for symbols:
```
a=1,x=3
```

the results will be:
```
4z+b+16 (ID: 4)
```

### Saving Expression

Run command:
```
\sav exp1

```

and we can use:
```
#exp1*#2 + 12
```

the result will be:
```
4za+4zx+ba+bx+16a+16x+12 (ID: 5)
```