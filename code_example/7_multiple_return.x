module test;

-- defining the structure info

def struct info{
  name: string;
  address: string;
}

-- creating a name function that return two string at the same time

fn get_name(info: info) -> (string, string){
  -- multiple return informations in a single line
  ret(info.name as string, info.address as string);
}
