#include "rsdl.hpp"
int main()
{
  window w(300,300);
  w.fill_rect(10,10,20,30,RGB(120,120,255));
  bool quit=false ;
  w.update_screen();
  while(!quit)
  {
    HANDLE(
      QUIT(quit=true);
    );
  }
  return 0;
}
