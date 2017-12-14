#include "rsdl.hpp"
#include <vector>
#include <fstream>
#include <unistd.h> // for usleep
#include <string> // to string func

using namespace std;
struct Creature{
  float X;
  char TypeOfCreature;
  int RemainingHealth;
  int CreateTime;
};
struct Level{
  int NumberOfWaves;
  vector<int> ZombiesSequence;
  vector<int> WaveLenght;
};
struct Sun{
  float X;
  float Y;
  int RemainingTime;
  float SunFallHeight;
};
struct Pea{
  float X;
  int Y;
};
#define PEA 'a'
#define SUN_FLOWER 'b'
#define PEA_SHOOTER 'c'
#define WALL_NUT 'd'
#define USUAL_ZOMBIE 'u'
#define SUN 's'
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
#define USUAL_ZOMBIE_SPEED 0.62
#define FIRST_CARD_X 200
#define SUN_FLOWER_COST 50
#define NUT_COST 50
#define PEA_SHOOTER_COST 100
#define NO_CARD_CLICKED 0
#define SUN_FLOWER_CLICKED 1
#define PEA_SHOOTER_CLICKED 2
#define NUT_CLICKED 3
#define SHOVEL_CLICKED 4
#define NOT_IN_FRONTYARD 10
#define NUT_HEALTH 72
#define SUN_FLOWER_HEALTH 5
#define PEA_SHOOTER_HEALTH 5
#define SUN3_WIDTH 116
#define SUN3_HEIGHT 116
#define PROBABLITY_OF_SUN 20
#define SUN_TIME 5
#define SUN_FALL_SPEED 2.5
#define SUN_VALUE 25
#define SUN1_WIDTH 36
#define SUN1_HEIGHT 36
#define SUN2_WIDTH 77
#define SUN2_HEIGHT 79
#define SUN_FLOWER_MAKING_SUN_TIME 25
#define PEA_SHOOTER_SHOOTING_TIME 30
#define PEA_SIZE 30
#define PEA_SPEED 6
#define NUMBER_OF_SUN_AT_THE_BEGINING 100
#define USUAL_ZOMMBIE_SPEED_OF_EATING 1
typedef vector<vector<vector<Creature> > >  MapType;
typedef vector<Sun> SunType;
typedef vector<Pea> PeaType;

struct Level GetLevelInfoFromFile();
void MakeMap(MapType &Map);
vector<int> MakeAVectorFromZombiesTime(Level LevelInfo);
void MakeZombie(MapType &Map,int Time,vector<int>ZombiesTime);
void UpdateScreen(MapType &Map,window &win,int &Sun,int tick,SunType &SunsInfo,PeaType &PeasInfo,int Time);
void UpdateZombiesOnScreen(MapType &Map,window &win,int Time);
void UpdatePlantsOnScreen(MapType &Map,window &win,int Time);
void UpdateSunsOnScreen(SunType &SunsInfo,int tick,window &win);
void UpdateCards(MapType &Map,int Sun, window &win,int tick);
void UpdatePeasOnScreen(MapType &Map,PeaType &PeasInfo,window &win);
int CheckCardsClicked(int mouse_x,int mouse_y,int &Sun);
void PlantSomthing(MapType &Map,int ClickedCard,int mouse_x,int mouse_y,int &Sun,int Time);
void CheckWhichBlockCliked(int &block_x,int &block_y,int mouse_x,int mouse_y);
void MakeARandomSun(MapType &Map,SunType &SunsInfo,int Time);
void CheckSunClickedOrNot(SunType &SunsInfo,int mouse_x,int mouse_y,int &Sun);
void MakeANewSunBySunFlower(SunType &SunsInfo,int Time,MapType &Map);
void MakeANewPeaByPeashooter(PeaType &PeasInfo,MapType &Map,int Time);
bool ZombiesWonOrNot(MapType &Map);
bool PlayerWonOrNot(MapType &Map,int Time,vector<int> &ZombiesTime);
void EndOFTheGame(bool ZombiesWon,window &win,bool PlayerWon);
void StartScreen();
void DeletingSomthing(MapType &Map,int ClickedCard,int mouse_x,int mouse_y);

int main(void){
  bool QuitFromScreen=false;
  StartScreen();
  while(!QuitFromScreen){
    window win(WIN_WIDTH,WIN_HEIGHT);
    MapType Map;
    MakeMap(Map);
    Level LevelInfo;
    vector<int> ZombiesTime;
    bool quit=false ;
    int Sun=NUMBER_OF_SUN_AT_THE_BEGINING;
    int tick=0;
    int ClickedCard=0;
    SunType SunsInfo;
    PeaType PeasInfo;
    int Time=0;

    LevelInfo=GetLevelInfoFromFile();
    ZombiesTime=MakeAVectorFromZombiesTime(LevelInfo);
   
      while (!quit)
      {
        bool ZombiesWon=ZombiesWonOrNot(Map);
        bool PlayerWon=PlayerWonOrNot(Map,Time,ZombiesTime);
        if(ZombiesWon==false && PlayerWon==false)
        {
          MakeZombie(Map,Time,ZombiesTime);
          UpdateScreen(Map,win,Sun,tick,SunsInfo,PeasInfo,Time);
          MakeARandomSun(Map,SunsInfo,Time);
          MakeANewSunBySunFlower(SunsInfo,Time,Map);
          MakeANewPeaByPeashooter(PeasInfo,Map,Time);
          HANDLE(
            QUIT(quit=true);
            LCLICK({
              PlantSomthing(Map,ClickedCard,mouse_x,mouse_y,Sun,Time);
              DeletingSomthing(Map,ClickedCard,mouse_x,mouse_y);
              ClickedCard=CheckCardsClicked(mouse_x,mouse_y,Sun);
              CheckSunClickedOrNot(SunsInfo,mouse_x,mouse_y,Sun);
            })
            KEY_PRESS(q,{
              QuitFromScreen=true;
              quit=true;
            })
          );
        }
        else
        {
          EndOFTheGame(ZombiesWon,win,PlayerWon);
          quit=true;
        }
        usleep(SECOND);
        tick++;
        Time++;
    }
  }
}

void MakeMap(MapType &Map){
  for(int i=0;i<NUMBER_OF_WIDTH_BLOCKS;i++){
    vector<vector<struct Creature> > tempRow;
    for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++){
      vector<struct Creature> tempVectorOfStruct;
      tempRow.push_back(tempVectorOfStruct);
    }
    Map.push_back(tempRow);
  }
}


struct Level GetLevelInfoFromFile(){
  Level LevelDataFromFile ;
  std::ifstream LastLevelFile("savedata.txt");
  if (!LastLevelFile)
  {
      std::cerr << "Error opening file:\n";
      return LevelDataFromFile;
  }
  char NumberOfLevel;
  LastLevelFile>>NumberOfLevel;
  string FileName="x.level";
  FileName[0]=NumberOfLevel;
  std::ifstream FileOfLevel(FileName.c_str());
  if (!LastLevelFile)
  {
      std::cerr << "Error opening file:\n";
      return LevelDataFromFile;
  }
  //We don't need some data from file:
  string temp;
  FileOfLevel>>temp>>LevelDataFromFile.NumberOfWaves;
  FileOfLevel>>temp>>temp>>temp;
  for(int i=0;i<LevelDataFromFile.NumberOfWaves;i++)
  {
    int iZombieSequence;
    FileOfLevel>>iZombieSequence;
    LevelDataFromFile.ZombiesSequence.push_back(iZombieSequence);
  }
  FileOfLevel>>temp>>temp;
  for(int i=0;i<LevelDataFromFile.NumberOfWaves;i++)
  {
    int iWaveLenght;
    FileOfLevel>>iWaveLenght;
    LevelDataFromFile.WaveLenght.push_back(iWaveLenght);
  }
  return LevelDataFromFile;
}

vector<int> MakeAVectorFromZombiesTime(Level LevelInfo){
  //here we make a vector of times that zombies come
  vector<int> ZombiesTime;
  int temp=0;
  for(int j=0;j<LevelInfo.NumberOfWaves;j++)
  {
    for(int i=0;i<LevelInfo.ZombiesSequence[j];i++){
      int RandomNum;
      RandomNum=(rand()%(LevelInfo.WaveLenght[j]*(REAL_SECOND/SECOND))+temp);
      ZombiesTime.push_back(RandomNum);
    }
    temp+=(LevelInfo.WaveLenght[j]*(REAL_SECOND/SECOND));
  }
  return ZombiesTime;
}

void MakeZombie(MapType &Map,int Time,vector<int>ZombiesTime){
  for(int i=0;i<ZombiesTime.size();i++){
    if(Time==ZombiesTime[i])
    {
      int RandomY;
      RandomY=rand()%NUMBER_OF_HEIGHT_BLOCKS;
      Creature newZombie;
      newZombie.RemainingHealth=USUAL_ZOMBIE_HEALTH;
      newZombie.TypeOfCreature=USUAL_ZOMBIE;
      newZombie.X=FRONTYARD_WIDTH-2;
      newZombie.CreateTime=Time;
      Map[NUMBER_OF_WIDTH_BLOCKS-1][RandomY].push_back(newZombie);
    }
  }
}
void UpdateScreen(MapType &Map,window &win,int &Sun,int tick,SunType &SunsInfo,PeaType &PeasInfo,int Time){
  win.draw_png("./frontyard.png",0,0,WIN_WIDTH,WIN_HEIGHT);
  UpdateZombiesOnScreen(Map,win,Time);
  UpdateCards(Map,Sun,win,tick);
  UpdatePlantsOnScreen(Map,win,Time);
  UpdateSunsOnScreen(SunsInfo,tick,win);
  UpdatePeasOnScreen(Map,PeasInfo,win);
  win.update_screen();
}
void UpdateZombiesOnScreen(MapType &Map,window &win,int Time){
  bool ZombieMove=true;
  for(int i=0;i<NUMBER_OF_WIDTH_BLOCKS;i++){
    for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++)
    {
      for(int k=0;k<Map[i][j].size();k++)
      {
        
       if (Map[i][j][k].TypeOfCreature==USUAL_ZOMBIE){
          int X_blockOfPlant=Map[i][j][k].X/BLOCK_WIDTH-1;
          if(X_blockOfPlant>=0)
          {
            for(int l=0;l<Map[X_blockOfPlant][j].size();l++)
            {
              if(Map[X_blockOfPlant][j][l].TypeOfCreature!=USUAL_ZOMBIE){
                if(Map[i][j][k].X-((Map[X_blockOfPlant][j][l].X+1)*BLOCK_WIDTH)<1)
                {
                   ZombieMove=false;
                  if(Map[X_blockOfPlant][j][l].RemainingHealth==1)
                  {
                    if(Time%(USUAL_ZOMMBIE_SPEED_OF_EATING*(REAL_SECOND/SECOND))==0)
                    {
                      Map[X_blockOfPlant][j].erase(Map[X_blockOfPlant][j].begin()+l);
                    }
                  }
                  else{
                    if(Time%(USUAL_ZOMMBIE_SPEED_OF_EATING*(REAL_SECOND/SECOND))==0)
                    {
                      Map[X_blockOfPlant][j][l].RemainingHealth--; 
                    }
                  }
                }
              }
            }
          }
          if(ZombieMove==true){
            Map[i][j][k].X=Map[i][j][k].X-USUAL_ZOMBIE_SPEED;
          } else{
            ZombieMove=true;
          }
          string pngName="";
          pngName.append("./zombie/zombie-");
          int NumberOfFrame=(Time-Map[i][j][k].CreateTime)%6;
          string FrameString=to_string(NumberOfFrame);
          pngName.append(FrameString);
          pngName.append(".png");
           win.draw_png(pngName,FRONTYARD_X+Map[i][j][k].X,FRONTYARD_Y
          +BLOCK_HEIGHT*j,BLOCK_WIDTH,BLOCK_HEIGHT);
        }      
      }
    }
  }
}

void UpdateCards(MapType &Map,int Sun, window &win,int tick){
  if(Sun>=SUN_FLOWER_COST)
    win.draw_png("./card_SunFlower.png",FIRST_CARD_X,0,BLOCK_WIDTH,BLOCK_HEIGHT);
  if(Sun>=PEA_SHOOTER_COST)
    win.draw_png("./card_PeaShooter.png",FIRST_CARD_X+BLOCK_WIDTH,0,BLOCK_WIDTH,BLOCK_HEIGHT);
  if(Sun>=NUT_COST)
    win.draw_png("./card_Nut.png",FIRST_CARD_X+2*BLOCK_WIDTH,0,BLOCK_WIDTH,BLOCK_HEIGHT);
  win.draw_png("./shovel.png",FIRST_CARD_X-BLOCK_WIDTH,0,BLOCK_WIDTH,BLOCK_HEIGHT);
}

int CheckCardsClicked(int mouse_x,int mouse_y,int &Sun){
  if(Sun>=SUN_FLOWER_COST)
    if(mouse_x<FIRST_CARD_X+BLOCK_WIDTH && mouse_x>FIRST_CARD_X && mouse_y<BLOCK_HEIGHT)
      return SUN_FLOWER_CLICKED;
  if(Sun>=PEA_SHOOTER_COST)
    if(mouse_x<FIRST_CARD_X+2*BLOCK_WIDTH && mouse_x>FIRST_CARD_X+BLOCK_WIDTH && mouse_y<BLOCK_HEIGHT)
      return PEA_SHOOTER_CLICKED;
  if(Sun>=NUT_COST)
    if(mouse_x<FIRST_CARD_X+3*BLOCK_WIDTH && mouse_x>FIRST_CARD_X+2*BLOCK_WIDTH && mouse_y<BLOCK_HEIGHT)
      return NUT_CLICKED;
  if(mouse_x>FIRST_CARD_X-BLOCK_WIDTH && mouse_x<FIRST_CARD_X && mouse_y<BLOCK_HEIGHT)
    return SHOVEL_CLICKED;
  return NO_CARD_CLICKED;
}

void PlantSomthing(MapType &Map,int ClickedCard,int mouse_x,int mouse_y,int &Sun,int Time){
  if(ClickedCard!=0)
  {
    int block_x=NOT_IN_FRONTYARD;
    int block_y=NOT_IN_FRONTYARD;
    CheckWhichBlockCliked(block_x,block_y,mouse_x,mouse_y);
    if(block_x!=NOT_IN_FRONTYARD && block_y!=NOT_IN_FRONTYARD)
    {
      bool FlagOFPlantExistence=false;
      for(int i=0;i<Map[block_x][block_y].size();i++)
      {
        if(Map[block_x][block_y][i].TypeOfCreature==PEA_SHOOTER || Map[block_x][block_y][i].TypeOfCreature==WALL_NUT ||
            Map[block_x][block_y][i].TypeOfCreature==SUN_FLOWER)
          {
            FlagOFPlantExistence=true;
          }
      }
      if(FlagOFPlantExistence==false){
        Creature tempPlant;
        tempPlant.X=block_x;
        tempPlant.CreateTime=Time;
        if(ClickedCard==SUN_FLOWER_CLICKED){
          tempPlant.TypeOfCreature=SUN_FLOWER;
          tempPlant.RemainingHealth=SUN_FLOWER_HEALTH;
          Sun-=SUN_FLOWER_COST;
        }
        else if(ClickedCard==PEA_SHOOTER_CLICKED){
          tempPlant.TypeOfCreature=PEA_SHOOTER;
          tempPlant.RemainingHealth=PEA_SHOOTER_HEALTH;
          Sun-=PEA_SHOOTER_COST;
        }
        else if(ClickedCard==NUT_CLICKED){
          tempPlant.TypeOfCreature=WALL_NUT;
          tempPlant.RemainingHealth=NUT_HEALTH;
          Sun-=NUT_COST;
        }
        Map[block_x][block_y].push_back(tempPlant);
      }
    }
  }
}
void CheckWhichBlockCliked(int &block_x,int &block_y,int mouse_x,int mouse_y){
  //check he click front yard
  if(mouse_x>FRONTYARD_X && mouse_x<FRONTYARD_X+FRONTYARD_WIDTH && mouse_y>FRONTYARD_Y && mouse_y<FRONTYARD_Y+FRONTYARD_HEIGHT)
  {
    block_x=(mouse_x-FRONTYARD_X)/BLOCK_WIDTH;
    block_y=(mouse_y-FRONTYARD_Y)/BLOCK_HEIGHT;
  }else
  {
    block_x=NOT_IN_FRONTYARD;
    block_y=NOT_IN_FRONTYARD;
  }

}
void UpdatePlantsOnScreen(MapType &Map,window &win,int Time){
  for(int i=0;i<NUMBER_OF_WIDTH_BLOCKS;i++){
    for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++)
    {
      for(int k=0;k<Map[i][j].size();k++)
      {
        if (Map[i][j][k].TypeOfCreature==SUN_FLOWER){
          string pngName="";
          pngName.append("./sunflower/sun-");
          int NumberOfFrame=(Time-Map[i][j][k].CreateTime)%60;
          string FrameString=to_string(NumberOfFrame);
          pngName.append(FrameString);
          pngName.append(".png");
          win.draw_png(pngName,FRONTYARD_X+Map[i][j][k].X*BLOCK_WIDTH,FRONTYARD_Y
          +BLOCK_HEIGHT*j,BLOCK_WIDTH,BLOCK_HEIGHT);
        }else  if (Map[i][j][k].TypeOfCreature==PEA_SHOOTER){
          string pngName="";
          pngName.append("./pea/peashooter-");
          int NumberOfFrame=(Time-Map[i][j][k].CreateTime)%60;
          string FrameString=to_string(NumberOfFrame);
          pngName.append(FrameString);
          pngName.append(".png");
          win.draw_png(pngName,FRONTYARD_X+Map[i][j][k].X*BLOCK_WIDTH,FRONTYARD_Y
          +BLOCK_HEIGHT*j,BLOCK_WIDTH,BLOCK_HEIGHT);
        }
        else if (Map[i][j][k].TypeOfCreature==WALL_NUT){
          string PngName;
          if(Map[i][j][k].RemainingHealth<=72 && Map[i][j][k].RemainingHealth>48)
            PngName="./Wallnut_body.png";
          else if(Map[i][j][k].RemainingHealth<=48 && Map[i][j][k].RemainingHealth>24)
            PngName="./Wallnut_cracked1.png";
          else
            PngName="./Wallnut_cracked2.png";
          win.draw_png(PngName,FRONTYARD_X+Map[i][j][k].X*BLOCK_WIDTH,FRONTYARD_Y
          +BLOCK_HEIGHT*j,BLOCK_WIDTH,BLOCK_HEIGHT);
        }
      }
    }
  }
}

void MakeARandomSun(MapType &Map,SunType &SunsInfo,int Time){
  if (Time%(REAL_SECOND/SECOND)==0)
  {
    if(rand()%100<PROBABLITY_OF_SUN)
    {
      Sun tempSun;
      tempSun.RemainingTime=SUN_TIME*(REAL_SECOND/SECOND);
      tempSun.SunFallHeight=(rand()%FRONTYARD_HEIGHT);
      tempSun.X=(rand()%FRONTYARD_WIDTH)+FRONTYARD_X;
      tempSun.Y=0;
      SunsInfo.push_back(tempSun);
    }
  }
}

void UpdateSunsOnScreen(SunType &SunsInfo,int tick,window &win){
  for (int i=0;i<SunsInfo.size();i++){
    if(SunsInfo[i].SunFallHeight > SUN_FALL_SPEED){
      SunsInfo[i].SunFallHeight-=SUN_FALL_SPEED;
      SunsInfo[i].Y+=SUN_FALL_SPEED;
    }
    if(SunsInfo[i].RemainingTime==0)
    {
      SunsInfo.erase(SunsInfo.begin()+i);
    }
    else if(SunsInfo[i].SunFallHeight<SUN_FALL_SPEED){
      SunsInfo[i].RemainingTime--;
    }
    win.draw_png("./Sun3.png",SunsInfo[i].X,SunsInfo[i].Y,SUN3_WIDTH,SUN3_HEIGHT,tick);
    win.draw_png("./Sun2.png",SunsInfo[i].X+(SUN3_WIDTH-SUN2_WIDTH)/2,SunsInfo[i].Y+(SUN3_HEIGHT-SUN2_HEIGHT)/2,SUN2_WIDTH,SUN2_HEIGHT,-1*tick);
    win.draw_png("./Sun1.png",SunsInfo[i].X+(SUN3_WIDTH-SUN1_WIDTH)/2,SunsInfo[i].Y+(SUN3_HEIGHT-SUN1_HEIGHT)/2,SUN1_WIDTH,SUN1_HEIGHT);
  }
}

void CheckSunClickedOrNot(SunType &SunsInfo,int mouse_x,int mouse_y,int &Sun){
  for(int i=0;i<SunsInfo.size();i++)
  {
    if(mouse_x>SunsInfo[i].X && mouse_x<SunsInfo[i].X+SUN3_WIDTH && mouse_y>SunsInfo[i].Y && mouse_y<SunsInfo[i].Y+SUN3_HEIGHT)
    {
      SunsInfo.erase(SunsInfo.begin()+i);
      Sun+=SUN_VALUE;
    }
  }
}

void MakeANewSunBySunFlower(SunType &SunsInfo,int Time,MapType &Map){
  for(int i=0;i<NUMBER_OF_WIDTH_BLOCKS;i++){
    for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++)
      {
      for(int k=0;k<Map[i][j].size();k++)
        {
          if(Map[i][j][k].TypeOfCreature==SUN_FLOWER){
            if((Time-Map[i][j][k].CreateTime)%(SUN_FLOWER_MAKING_SUN_TIME*(REAL_SECOND/SECOND)) == 0)
            {
              Sun tempSun;
              tempSun.RemainingTime=SUN_TIME*(REAL_SECOND/SECOND);  
              tempSun.SunFallHeight=0;
              tempSun.X=Map[i][j][k].X*BLOCK_WIDTH+FRONTYARD_X;
              tempSun.Y=j*BLOCK_HEIGHT+FRONTYARD_Y;
              SunsInfo.push_back(tempSun);
            }
          }
        }
      }
    }
}

void MakeANewPeaByPeashooter(PeaType &PeasInfo,MapType &Map,int Time){
  for(int i=0;i<NUMBER_OF_WIDTH_BLOCKS;i++)
  {
    for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++)
    {
      for(int k=0;k<Map[i][j].size();k++)
      {
        if(Map[i][j][k].TypeOfCreature==PEA_SHOOTER)
        {
          bool ZombieExistInLine=false;
          for(int l=0;l<Map[NUMBER_OF_WIDTH_BLOCKS-1][j].size();l++)
          {
            if(Map[NUMBER_OF_WIDTH_BLOCKS-1][j][l].TypeOfCreature==USUAL_ZOMBIE && Map[NUMBER_OF_WIDTH_BLOCKS-1][j][l].X+BLOCK_WIDTH>=(i+1)*BLOCK_WIDTH)
            {
              ZombieExistInLine=true;
              break;
            }
          }
          if(ZombieExistInLine==true && (Time-Map[i][j][k].CreateTime)%(PEA_SHOOTER_SHOOTING_TIME) == 0){
            Pea tempPea;
            tempPea.X=(i+1)*BLOCK_WIDTH+FRONTYARD_X;
            tempPea.Y=(j)*BLOCK_HEIGHT+FRONTYARD_Y;
            PeasInfo.push_back(tempPea);
          }
        }
      }
    }    
  }
}

void UpdatePeasOnScreen(MapType &Map,PeaType &PeasInfo,window &win){
  int SizeOfBlock=PeasInfo.size();
  for(int l=0;l<SizeOfBlock;l++){
    PeasInfo[l].X+=PEA_SPEED;
    win.draw_png("./pea.png",PeasInfo[l].X,PeasInfo[l].Y,PEA_SIZE,PEA_SIZE);
    int i=NUMBER_OF_WIDTH_BLOCKS-1;
    int j=(PeasInfo[l].Y-FRONTYARD_Y)/BLOCK_HEIGHT;
   
   for(int k=0;k<Map[i][j].size();k++)
    {
      if(Map[i][j][k].TypeOfCreature==USUAL_ZOMBIE)
      {
        if((Map[i][j][k].X+FRONTYARD_X) - (PeasInfo[l].X+PEA_SIZE)<5 && (Map[i][j][k].X+FRONTYARD_X) - (PeasInfo[l].X+PEA_SIZE)>-2*PEA_SIZE)
        {
          if(Map[i][j][k].RemainingHealth ==1){
            Map[i][j].erase(Map[i][j].begin()+k);
            PeasInfo.erase(PeasInfo.begin()+l);
            l--;
            SizeOfBlock=PeasInfo.size();
            break;
          }
          else{
            Map[i][j][k].RemainingHealth--;
            PeasInfo.erase(PeasInfo.begin()+l);
            l--;
            SizeOfBlock=PeasInfo.size();
            break;
          }
        }
      }
    }
  }
}

bool ZombiesWonOrNot(MapType &Map){
  bool ZombiesWon=false;
  for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++)
  {
    for(int k=0;k<Map[NUMBER_OF_WIDTH_BLOCKS-1][j].size();k++)
      if(Map[NUMBER_OF_WIDTH_BLOCKS-1][j][k].X<-BLOCK_WIDTH && Map[NUMBER_OF_WIDTH_BLOCKS-1][j][k].TypeOfCreature==USUAL_ZOMBIE)
        ZombiesWon=true;
  }
  return ZombiesWon;
}

void EndOFTheGame(bool ZombiesWon,window &win,bool PlayerWon){
  if(ZombiesWon==true)
  {
    win.draw_png("./game_over.png",0,0,WIN_WIDTH,WIN_HEIGHT);
    win.update_screen();
    usleep(3000000);
  }
  else if(PlayerWon==true){
    win.draw_png("./win1.png",0,0,WIN_WIDTH,WIN_HEIGHT);
    win.draw_png("./win2.png",WIN_WIDTH/4,WIN_HEIGHT/4,WIN_WIDTH/2,WIN_HEIGHT/2);
    win.update_screen();
    usleep(3000000);
  }
}

bool PlayerWonOrNot(MapType &Map,int Time,vector<int> &ZombiesTime){
  bool ThereIsZombie=false;
  bool TimeEnds=false;
  for(int j=0;j<NUMBER_OF_HEIGHT_BLOCKS;j++)
  {
    for(int k=0;k<Map[NUMBER_OF_WIDTH_BLOCKS-1][j].size();k++)
      if(Map[NUMBER_OF_WIDTH_BLOCKS-1][j][k].TypeOfCreature==USUAL_ZOMBIE)
        ThereIsZombie=true;
  } 
  int EndOfTheZombiesTime=ZombiesTime.size()-1;
  if(Time>ZombiesTime[EndOfTheZombiesTime])
    TimeEnds=true;
  if(TimeEnds==true && ThereIsZombie==false)
  {
    std::ifstream LastLevelFile("savedata.txt");
    if (!LastLevelFile)
    {
        std::cerr << "Error opening file:\n";
        return false;
    }
    int PerviousLevel=0;
    LastLevelFile>>PerviousLevel;
    cerr<<PerviousLevel;
    std::ofstream NewLastLevelFile("savedata.txt");
    if (!NewLastLevelFile)
    {
        std::cerr << "Error opening file:\n";
        return false;
    }
    
    PerviousLevel++;
    NewLastLevelFile<<PerviousLevel;
    return true ;
  }
  else 
    return false;

}

void StartScreen(){
  bool quit=false;
  window win(WIN_WIDTH,WIN_HEIGHT);
  win.draw_png("./start.png",0,0,WIN_WIDTH,WIN_HEIGHT);
  win.update_screen();
  while(!quit){
   HANDLE(
     LCLICK({    
      if(mouse_x>250 && mouse_x<552 && mouse_y<577 && mouse_y>545)
      {
        quit=true;
      }     
        });
     QUIT(quit=true);
     );
  }
}
void DeletingSomthing(MapType &Map,int ClickedCard,int mouse_x,int mouse_y){
  if(ClickedCard==SHOVEL_CLICKED)
    {
      int block_x=NOT_IN_FRONTYARD;
      int block_y=NOT_IN_FRONTYARD;
      CheckWhichBlockCliked(block_x,block_y,mouse_x,mouse_y);
      if(block_x!=NOT_IN_FRONTYARD && block_y!=NOT_IN_FRONTYARD)
      {
        if(Map[block_x][block_y].size()>0)
        {
          for(int k=0;Map[block_x][block_y].size();k++){
            if(Map[block_x][block_y][k].TypeOfCreature!=USUAL_ZOMBIE){
              Map[block_x][block_y].erase(Map[block_x][block_y].begin()+k);
              break;
            }
          }
        }
      }
    }
}