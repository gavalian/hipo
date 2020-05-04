#include <iostream>
#include <XrdClient/XrdClient.hh>
#include <fstream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <math.h>

#include "XrdClient/XrdClientEnv.hh"
#include "XrdSys/XrdSysHeaders.hh"

kXR_unt16 open_mode = (kXR_ur);
kXR_unt16 open_opts = (1);

//To compile
//g++ mytest.cc -I/usr/include/xrootd -lXrdClient -o mytest
//g++ readFile.cc -I/usr/local/Cellar/xrootd/4.10.1/include/xrootd -L/usr/local/lib -lXrdClient -o readFile
int main(void)
{

  char mybuffer[256];

  //XrdClient *cli = new XrdClient("xroot://sci-xrootd.jlab.org//osgpool/hallb/text2.txt");
  XrdClient *cli = new XrdClient("xroot://sci-xrootd.jlab.org//osgpool/hallb/example_output.hipo");
  //XrdClient *cli = new XrdClient("xroot://sci-xrootd.jlab.org//home/gavalian/test4.txt");
  cli->Open(open_mode,open_opts);
  XrdClientStatInfo stats;
     cli->Stat(&stats);
     printf("File Size = %lld\n", stats.size);
  int nread = cli->Read(mybuffer,0,256);
  printf("number of bytes read = %d\n",nread);
  //std::cout << mybuffer ;
  for(int i = 0; i < 14; i++){
    printf("%3d : 0x%08X\n", i,  reinterpret_cast<int *> (mybuffer)[i]);
  }

  return 1;
}
