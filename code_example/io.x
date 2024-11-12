module io;



@system def fn print(str line) -> void {}   -- define function 
def fn do_smth(int a, str b) -> void {} 

-- system keyword is used to tell if a methid is implemented inside the compiler or not

fn do_smth(int a, str b) -> void {

  str[] a = {
    "a",
    "b",
    "c"
  }

  int len = size(str);

  if(len != a){
    ret;
  }

  for(a){                     -- by default i is used as an index 
    print(b);                 -- print with for loop b for a times
    print(str[i]);  
  }
}


