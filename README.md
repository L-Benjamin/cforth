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

TODO