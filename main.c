#include <stdio.h>
#include <stdlib.h>

#include <xeb.h>
#include <misc.h>

int main(){
  xeb_load_file("./code_example/0_hello_world.x");
  xeb_close_compiler();
  return 0;
}
