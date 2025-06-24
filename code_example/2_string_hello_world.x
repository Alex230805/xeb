
include io;    -- where the main io method are located

-- this is an example of what xeb may look like

fn main() -> {
  string empty_string = "";
  string world = "world";
  io::print("Hello"+world);     -- print is part of io module
  io::print("What's your name?: ");
  string name = io::scan();     -- NOTE

  -/  
 
    If scan have no argument the stdin is used as input buffer.
    By providing whatever buffer or source you want the scan function
    will read that and scan the incoming data

  /-

  io::print("Welcome", name); -- this is a valid string interpolation
  ret(0);
}
