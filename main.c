#include <stdio.h>
#include <stdlib.h>

#include <xeb.h>
#include <misc.h>

int main(){
  xeb_load_file("./code_example/7_multiple_return.x");
  xeb_error_init_handler();

  // get public buffer pointer to read incoming error messages during the compilation to handle it via build system for custom configuration.
  // This may require a bit more study
  // 
  //xeb_error_get_public_buffer_pointer();


  xeb_close_compiler();
  return 0;
}
