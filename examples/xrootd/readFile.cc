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

  char mybuffer[100];

  XrdClient *cli = new XrdClient("root://xrdsydsr.syd.coepp.org.au//coepp/local/antonio/eggs");
  cli->Open(open_mode,open_opts);
  cli->Read(mybuffer,0,30);
  std::cout << mybuffer ;

  return 1;
}
