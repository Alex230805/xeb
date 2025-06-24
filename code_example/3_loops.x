import io.*;



fn main() -> void{
  
  int a = 60;

  string hello = "Hello World\n";


  string list = {
    "Hello",
    "World"
  }

  for(a){         
    print(hello);
  }

  for(index, a){        -- for 'a' times with an index print the list, if the indexed list length is lower than the current index the lines is ignored  
    print(list[index]);
  }

  for(list as elem){          -- foreach type of loops
    print("We have \{elem} available, come here");
  }

  for(index, list as elem){             -- foreach type of loop with index
    print("Try also \{elem} with index \{index}");
  }

  int counter = 0;
  while(counter < 50){           -- while loop
    print("Goodbye");
    counter+=1;
  }

  counter = 0;
  do{
    print("Adios");
    counter += 1;
  }while(counter < 50);

  ret();
}
