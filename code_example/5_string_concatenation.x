import io;



fn main() -> void {
  
  -- simple interpolation
  string: name = "Alex";
  print("Hello \{name}, how can I help you?");
  string scan_res = scan();
  print("Wow, this is strange I cannot serve you for \{scan_res} or whatever");
  
  string ability[] = {
    "Nothing",
    "Still nothing",
    "What about ... nothing?"
  }

  string time[] = {
    "01:00",
    "02:00",
    "03:00"
  }

  print("But I still can do that", ability); -- this must print the first line and the ability array

  print("And I can do that during this time stamp: ")
  for(index,ability.len){
    print(ability[index]);
  }

  string: name;
  print("If You don't mind what about my name? you still didn't give me a name, chose one: ");
  name = scan();
  print("I still can't do shit, but now my name is", name); -- this is a string concatenation
  ret();
}


