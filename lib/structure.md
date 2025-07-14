 Current compilation structure

The file is analized and when a function declaration is found it will be processed and a function scope begin
( if the function declaration is done right ).
Inside this scope we can have switch cases, if statement, variable definition and assignment, conditions ... we
can have basically anythings and those things will create what the xeb reference define as "code section". 
Inside a code section we have the function declaration ( which is also inserted in the global function table, it
doesn't need forward declarations ), the local variable declaration which create a local list of variable where 
the expression can access to by creating an "expression" tree.
Basically whats happen is this: when a variable is allocated it will be inserted inside this local list with the 
name and the type of variable, when an expression is required inside a if statement or a switch statement this list
is consulted to find the corresponding variable referencing and forming an intermediate representation of what the
expression is. For example if we have such code:

      module io.*;

      fn main() -> void {
        int: number = 0;

        if(number == 0){
          printf("Hello world");
        }
        ret();
      }

This code allocate a local variable named "number" of type "int" initializated with 0. Those are two differents 
expressions, we have a number allocation and a number assignment, so when the compiler find the definition it will 
push the variable inside the local list, then the assignment is composed as an expression which require a left hand 
argument "number" and with the assignment token "=" assign the implicit value "0" to the variable. This is important
because yes in this part the compilation process can start right away, but we prefer to first analize the code and
generate a local version defined in memory before require the intermediate representation layer to operate the actual
translation. Since this compiler use the QBE backend this memory-like intermediate representation allow a much higher
description level of what is processed without generating an assembly-like code that is still high level, bus sligly
lower than xebc approach. 
After the expression has beed pushed inside the memory the if statement is processed, here the condition is
threated outside of the compilation of the compilation process of the if statement with a function that is dedicated 
ONLY to expression parsing, in fact when the if is gonna be verified and compiled into the memory-like intermediate 
representation it will have the expression tree, the true branch and the false branch. Same things for the switch case 
which have a much complex structure:


    module io.*;

    fn main() -> void {
      int: name = 0;
      bool: allowed = true;

      switch(name){
        (0) -> {
          print("Still \{name}?");
        }
        (1),(2),( _ > 10 && allowed) ->{
          print("Wow, such a progess, keep going mate!");
        }
      }else{
        print("Not a valid number: \{name}");
      }

      ret();
    }

The xeb switch statemet is special because allow a similar if syntax, but with a much higher condition ergonomics rate.
This imply that the switch have a variable reference ( taken by the main local variable list ) which is NOT an
expression, but only a reference ( it can be function or whatever, just not a condition, so not an expression ),
then we have the switch condition that are wrapped in the "(..) ->{}" syntax, basically whatever is inserted in this
parentesis can directly be referred to the main variable used inside the switch statement, or it can be a condition
which allow the expression to have a local reference to the switch condition and external reference for external
condition like is shown in the example here. 
At the end the intermediate representation have here a switch declaration with the reference in the local variable 
list and a list of expression with a true ( or verified/switch taken ) branches, basically is a wrapper for if 
statement (with address-pointing optimized condition) with a single else branch which is the main else of the switc
statement
