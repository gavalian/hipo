/***************************************************************************
*     ████████╗██╗    ██╗██╗ ██████╗                      
*     ╚══██╔══╝██║    ██║██║██╔════╝                      
*        ██║   ██║ █╗ ██║██║██║  ███╗                     
*        ██║   ██║███╗██║██║██║   ██║                     
*        ██║   ╚███╔███╔╝██║╚██████╔╝                     
*        ╚═╝    ╚══╝╚══╝ ╚═╝ ╚═════╝                      
*                                                            
*        ██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗
*        ██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝
*        ██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝ 
*        ██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝  
*        ███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║   
*        ╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
* DESCRIPTION: 
* The light weight implementation of histogramming classes for hipo4 package.
* Provides simple 1D and 2D (soon to be implemented) histograms, that are 
* seriazible to HIPO files. The idea is to provide compatibility with TWIG
* Java library that provides visualization. 
* the java library can be found at : https://github.com/gavalian/twig
* Author: G.Gavalian 
* Date:   11/22/2023
****************************************************************************/

#ifndef __TWIG_LIBRARY__
#define __TWIG_LIBRARY__

#include "chart/ascii.h"
#include <cstdlib>
#include <iostream>

namespace twig {

  class axis {
  private:
    std::vector<double> bins;
  public: 
    axis(){}
    axis(int n, double min, double max){ init(n,min,max);}
    virtual ~axis(){}
    void init(int n, double min, double max){
      bins.resize(n+1); double step = (max-min)/n;
      for(int i = 0; i <=n; i++){ bins[i] = min + i*step;}
    }
    int find(double value){
      auto it = std::lower_bound(bins.begin(), bins.end(), value);
      return it-bins.begin()-1;
    } int nbins(){ return bins.size()-1;}
    double center(int bin){ return bins[bin] + 0.5*(bins[bin+1]-bins[bin]);}
    double  min(){ return bins[0];}
    double max(){return bins[bins.size()-1];}
  };

class h1d {

   private:
      int  hid;
      axis  x;
      std::vector<double> container;
   public:
      h1d(int n, double min, double max){ 
        hid = 0; x.init(n,min,max);container.resize(n+2);
      }
      h1d(int __id, int n, double min, double max){
         hid = __id; x.init(n,min,max);container.resize(n+2);
      }
      void id(int __id){ hid = __id;}
      int  id(){return hid;}
      void fill(double value){
         int bin = x.find(value); if(bin<0) { container[0] = container[0] + 1.0; return;}
         if(bin>=x.nbins()) { container[container.size()-1] = container[container.size()-1] + 1.0; return;}
         container[bin+1] = container[bin+1] + 1.0;
      }
      double content(int bin) { return container[bin+1]; }
      void show(){
         for(int b = 0; b < x.nbins(); b++) printf("%12.5f %12.5f\n",x.center(b), content(b));
      }
      void series(std::vector<double> &data){
         data.resize(container.size()-2);
         for(decltype(container)::size_type i = 1; i < container.size()-1;i++) data[i-1] = container[i];
      }
      void print(){
         std::vector<double> data;
         series(data);
         //ascii::Asciichart asciichart(std::vector<std::vector<double>>{data});
         ascii::Asciichart asciichart( {{"h1f",data}});
         
         std::string screen = asciichart.height(25).offset(4).Plot();
         int index = screen.find_first_of("┤");
         //std::cout << " AXIS x position = " << index << '\n';
         std::cout << '\n'
          << asciichart.height(25).offset(4).Plot(); // rescale to -3 ~ +3 lines
          //<< '\n';
	 std::cout << "\033[38;5;45m"; 
    for(int i = 0; i < (index-52); i++) std::cout << " ";
    std::cout << "\u2514\u252C";
	 for(int i = 0; i < x.nbins()/4; i++) std::cout << "\u2500\u2500\u2500\u252C";
	 std::cout << '\n';
    for(int i = 0; i < (index-51); i++) std::cout << " ";
	 std::cout << x.min();
	 for(int i = 0; i < x.nbins()-4; i++) std::cout << " ";
	 std::cout << x.max() << "\033[0m" << '\n';
          
      }
  };
}
#endif
