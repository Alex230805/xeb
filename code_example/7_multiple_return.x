include test;

-- defining the structure info

def struct info{
  str: name;
  str: address;
}

-- creating a name function that return two string at the same time

fn get_name(info: main_info) -> (str, str){
  -- multiple return informations in a single line
  ret(main_info.name as str, main_info.address as str);
}
