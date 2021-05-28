# cforth

Unix implementation of a [forth](https://en.wikipedia.org/wiki/Forth_(programming_language)) dialect interpreter in [ansi c](https://en.wikipedia.org/wiki/ANSI_C).
Lexing and parsing are done with [flex](https://en.wikipedia.org/wiki/Flex_(lexical_analyser_generator)) and [bison](https://en.wikipedia.org/wiki/GNU_Bison) respectively. Compilation uses [gcc](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) and [make](https://en.wikipedia.org/wiki/Make_(software)).

## Compile the interpreter

Place yourself at the root of the project, and do:
```
$ make build
```

## Run the interpreter

Place yourself at the root of the project, and do:
```
$ ./forth <source>
```
Where `source` is the name of a file containing forth code to be interpreted.

## Example

```
$ ./forth examples/hello_world.f
Hello, world!
```

## Syntax of the language

### Literals

There are three literals in the language:
- 64 bits signed integers: `[+-]?[0-9]+`
- double precision floating point numbers (double): `[+-]?[0-9]+\.[0-9]+(e[+-]?[0-9]+)?`
- character strings: `\"[^"]*\"`

The strings are pushed on the stack in reverse, with the first character of the string on top of the stack and a null character at the end.

### Words

`+`, `-`, `*`, `/`: arithmetic operations on the last two elements of the stack, interpreted as integers.

`+.`, `-.`, `*.`, `/.`: arithmetic operations on the last two elements of the stack, interpreted as doubles.

`and`, `or`, `xor`: logic operations on the last two elements of the stack.

`neg`, `inc`, `dec`: respectively negate, increment and decrement the last two elements of the stack, as integers.

`=`, `!=`, `<`, `>`, `<=`, `=>`: compare the last two elements of the stack as integers.

`neg.`: negate the last element of the stack as a double.

`dup`: duplicates the last element of the stack.

`pop`: removes the last element of the stack.

`swap`: swap the last two elements of the stack.

`strtoi`, `strtof`, `itostr`, `ftostr`: parse/write an integer/float to/from the stack.

`in`: reads a string from stdin.

`out`: writes a string to stdout.

`put`: prints the last element of the stack as an ascii character.

### Flow control

`if ... end`: pops the last value of the stack, if it is not 0, performs the words between `if` and `end`.

`if ... else ... end`: same as `if ... end` but performs the words between `else` and `end` if the value popped is 0.

`while ... end`: same as `if ... end` but when `end` is reached, loops back to the `while` and do it again.

Those three syntaxes may be nested into each other as wished.

`: <word> ... ;`: defines a new word, whose identifier is `<word>` (anything that is not a literal is a valid identifier). The action of that new word is given by the words in `...`. Definitions may not be nested and must be done top-level.