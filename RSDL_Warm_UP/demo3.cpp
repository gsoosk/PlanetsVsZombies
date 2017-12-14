#include "rsdl.hpp"
#include <cstdlib>

using namespace std;

int main(void){
    srand(0);
    window win(640, 640);
    int i = 0,j = 0;
    int x = 0;
    int sgn = 1;
    while(true){
        win.draw_bg(string("./frontyard.png"),0,0);
        x += 5*sgn;
        if(x == 500)
            sgn = -1;
        if(x == 0)
            sgn = 1;
        // for (int j = 0; j < 200; j++)
        //     win.draw_bmp(string("./image.bmp"), rand()%600, rand()%600, 30, 30);
        win.draw_png(string("./pea.png"), 200-j,200-i,30,30);
        HANDLE({
            QUIT({return 0;})
            KEY_PRESS(w, {
                i += 5;
            })
            KEY_PRESS(s, {
                i -= 5;
            })
            KEY_PRESS(a, {
                j += 5;
            })
            KEY_PRESS(d, {
                j -= 5;
            })
            KEY_PRESS(q, {return 0;})
        })
        win.show_text("Salam");
        win.update_screen();
        DELAY(15);
    }
}
