#include "rsdl.hpp"

using namespace std;

int main(void){
    window win(400,400);
    bool quit=false ;
    win.draw_png("./frontyard.png",0,0,400,400);
    win.update_screen();
    while (!quit)
    {
      HANDLE(
        QUIT(quit=true);
        LCLICK({
          quit=true;
        })
        KEY_PRESS(h,{
          quit=true;
        })
      );
    }
}
