#include <iostream>
#include <vector>
using namespace std;
struct a{
 int x;
};
#define BLOCK_WIDTH 62
#define BLOCK_HEIGHT 94
#define FRONTYARD_WIDTH 560
#define FRONTYARD_HEIGHT 470
#define FRONTYARD_X 200
#define FRONTYARD_Y 90
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define ASCII_OF_ZERO 48
#define NUMBER_OF_WIDTH_BLOCKS 9
#define NUMBER_OF_HEIGHT_BLOCKS 5
#define SECOND 50000
#define REAL_SECOND 1000000
#define USUAL_ZOMBIE_HEALTH 10
#define USUAL_ZOMBIE_SPEED 0.2
#define DIVISION_OF_SECOND 0.05
int main(){
  double temp = DIVISION_OF_SECOND;
  cout<<BLOCK_WIDTH*((DIVISION_OF_SECOND)*USUAL_ZOMBIE_SPEED)<<" ";


}
