## Current compilation structure

The structure of this compiler is mostly based on a memory oriented intermediate representation, this means that 
as long as there is enough memory in the system the compiler will parse and organize the declaration statement, such 
function or variable, the instruction sequence and each code scope.
Currently the file is analized line by line, and with each line if validated with the proper syntax a statement is 
created inside the memory IR of xeb, each of this statement are linked together by the code scope where those are contained.
This means that for instance a function is been declared it not only pushes the definition inside the function definition
array, but it will also create what's called a "function scope". This scope, which is a structure in memory, it will contain 
declaration and instruction, so for each function you can have N amount of for example variable definition and N amount of 
general instruction, those instruction have what's called a "type" which is an identifier to define the type of operation 
associated with this specific operation specified. To keep things simple each instruction will be stored like an array of 
pointers, this array contain pointer to a linking structure that store the type of instruction and the pointer to the 
memory structure where the instruction is located. This is an example:


    ...
    ...
    if(n > 0){              // this is a check or condition instruction
        ret(n);             // this is a return instruction
    }
    n = 1;                  // this is a variable assignment instruction 
    ...
    ...

Those tree instruction can be vewed differently: the if instruction could open a separate scope by itself which define a 
different execution branch for the function scope based on a specific condition or as a separate instruction, but 
the condition is also a type of instruction because a condition check is indeed performed as an operation before a 
conditional jump will or will not been performed.
So to simplify this the first parsing process ( where the first IR is created ) condition implied by an 
if or a white it will be defined as a separate chain of operation, a some sort of "context" branch.
This might not be clear at first but it's super simple: each instruction is defined as an abstraction linked by a 
intermediate structure which store the type of instruction, when the code is been generated this type will be 
checked and based on it the correct type of instruction or instructions sequence will be generated, this means that 
internal conditional jump like while loops, if statement and so on will indeed been parsed like normal instruction, and 
then from what this instruction structure will contain it will proceed which the parsing of internal instructions related 
with this type of scope. The reason behind this decision is based on how the assembly representation work and how 
it can be interpreted. Conditions and loops are infact based on a conditional jump, this define a different execution 
branch inside the same function scope that can affect the behaviour of such function. This can be viewed as separate 
scope like for example in the assembly code representation or by defining a general scope and a general chain of 
instructions based on the operation required. This second solution is closer to a higher type of representation than the 
assembly or assembly-like one, but it can be infact organized better for optimizations. Essentially the first type is more 
appropriate if the IR used is an assembly-like while the second one can be indeed be better for a memory oriented IR, and 
for that you don't realy need such way to organize your code, I mean you could do it, but it can be at an higher level 
with a lower complexity, expecially if your code is not directly generated while the rource is been parsed, and at the 
end you will have a function structure which his definition and is instruction sequence, and it one of this instruction 
is a condition it will contain the chained instruction related to that execution branch, as simple as that.


