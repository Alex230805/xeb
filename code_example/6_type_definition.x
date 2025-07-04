include io.*;

def struct TestStruct{
  str: name;
  str: surname;

  fn get_info() -> string {
    ret("We use to call you: \{local.name} \{local.surname}, welcome back");
  }

  fn _init() -> void {
    name = "";
    surname = "";
  }
}


fn main() -> void {
  TestStruct: st1 = {0}; -- initializing the structure with this synthax will call the _init() function in it
  TestStruct: st2 = {0};

  print(st1.get_info);
  print(st2.get_info);

  ret();
}
