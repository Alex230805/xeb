
-- testing the switch case compilation


fn main() -> void {

  int: test = 0;
  string: name = "Ezio Auditore";

-/

  This is an example of a probably good switch case syntax, nothing elaborated or 
  absurdly crazy implementation like the zig one.. but still clean and understandable
  at first glance without much effort.

/-

  bool: go_on = true;
  switch(test){
    (0) -> {
      -- do smth
    }
    (1) -> {
      -- do smth
    }

    (2) -> {
      -- do smth
    }
    (3),(_ > 10 || go_on) ->{
      -- do smth
    }
  }else{
    test = -1;
  }

  bool found = false;

  -- switch with multiple conditions

  switch(name){
    ("Ezio"),("Mario"),("Giuliano") -> {
      found = true;
    }
  }else{
    found = false;
  }


  ret();
}
