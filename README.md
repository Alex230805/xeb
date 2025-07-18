# XEB Programming language 


> [!WARNING]
> This code is not meant to be anything serious in the first place, this is just a recreational programming section where I'm trying to implement nothing but just a programming language. 



This project was open in the first place because I want to explore not only how a compiler works but also how I can figure out an efficient way to process source code and translate it to something else. So this language has a structure, but it doesn't have a proper direction, it's just an experiment where I try to explore the ways that someone can follow to implement such a program and just have fun with it.


This project features his own lexer ( eventually dragged into a separate repo called "lxer" ) his own error handling system 
built to simplify the error tracking. This is not implemented on the level of the lexer but on the level of the compiler itself to abstract and define boundaries between different sections to minimize the surface level of errors in the code base ( and also to make it simpler to fix or upgrade ). 
The source code will be parsed by the compiler and translated with the tokenizer of the lexer in a global list of tokens, those tokens are then processed with the source code to generate contexts of each line. This compiler processes lines with tokens and context and this allows me to identify when an expression is written in a single line or in multiple lines, allowing multiple ways to define static expression ( like variable definition ) and multiline conditions ( like inside an "if" or a switch).


The compiler offers macros to automatically communicate with the lexer to switch tokens or to check for hit type and at the same time allowing an automatic context switch from one line to another. 


#### What does it mean? 


The tokenizer of Lxer flags out of bound expressions ( tokens that are not recognized or allowed ) with a separate, independent token. This token is not really used actively inside the Lxer, but is used by the compiler during the context generation where a bunch of tokens will be linked to a single line. Here to identify when a row of tokens ends this independent token will be placed to flag the end of the line like " there are no more tokens left here ". 

This might be useless at first, but with the context switch implemented and all the functions of Lxer wrapped inside the compiler this makes a lot of sense. Practically when the current token is required it returns the token pointed by the lexer tracker, inside the compiler this is translated into getting the token inside the current context.
This allows to identify if the current token is valid or not, and since this unique token is included inside the Lxer source code it allows a complete compatibility with all the functions of Lxer without adding too much code.


## Error handling 

I implement a simple system of error reporting that allows the compiler to proceed with a limited amount of errors to be able to produce a detailed report, when too many errors are generated it will automatically interrupt the compiler. 
For those two scenarios the full compilation will never start in any case, only the memory organization and all the instruction representations will continue to be generated as much as possible and only if no error is generated it will jump into the real translation layer. 


The error system is meant to be as detailed as possible and as solid as possible to be able to track effectively errors linked with the correct line. The error structure and error pushing macro include custom messages, allowing to describe as much as possible what goes wrong, and a tag reference. 
Each error is linked within a tag identifier, this tag is not only printed inside the final report generator but in future release it will be used to classify types of error and the level of danger that such practice may produce ( this for warnings and errors ).


## File structure 

The compiler for now is simple, all the instructions are located inside "xeb_inst.c", instructions like "ret()" or variable assignment will be processed by the designated function inside this file. 

"xeb_def.h" contains the main declarations used by the compiler source, things such the compiler structure, the instruction structure or may macro are declared inside this file. This to centralize all the main structure declarations and to clean the codebase.

The main compiler runtime is located inside "xeb.c" with his header "xeb.h", here the file is open and processed, where the generation of the contexts of each line is managed and where the main loop is located. Each function defined inside "xeb_inst.c" must be linked and used inside the main compilation loop inside the compiler. To be able to define the behaviour of the 
compiler the main loop must be modified accordingly. Functions like the error generation, error tracking, functions definition or variable definitions inside each function are managed here ( this always to minimize the surface of error ).


## Main dependencies 

This compiler uses for now libraries implemented by me and maintained by me, removing "granulosity" or unpredictability of third party dependencies that, for the majority, are bloated as hell. So to make things simple this project use my custom made 
general lexer called that eventually was forked to his separate repo: "Lxer". This library has all the things that I need 
to implement what I require, and since those two projects have an effect on each other, major updates may come to Lxer just because I require new things and decide to implement what I need.

The other major dependency that is a must is my Cbox library that functions as a toolbox for general things, from reading files to handling allocation. I use this specifically for my implementation of Arena allocator. This is required to dynamically allocate memory when needed, this is used by both Lxer and xeb.
