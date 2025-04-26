/*****************************************************************************************
*   ██╗   ██╗███████╗ ██████╗████████╗ ██████╗ ██████╗  
*   ██║   ██║██╔════╝██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗ 
*   ██║   ██║█████╗  ██║        ██║   ██║   ██║██████╔╝ 
*   ╚██╗ ██╔╝██╔══╝  ██║        ██║   ██║   ██║██╔══██╗ 
*    ╚████╔╝ ███████╗╚██████╗   ██║   ╚██████╔╝██║  ██║ 
*     ╚═══╝  ╚══════╝ ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝ 
*                                                    
*    ██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗
*    ██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝
*    ██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝ 
*    ██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝  
*    ███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║   
*    ╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝                                                
*   DESCRIPTION: VECTOR LIBRARY for physics calcuations, includes a 3d vector class and
*                a Lorentz vector class. The library comes as an include file only. 
*                thre is no need to compile, just include this file into your code.
*   Author: G.Gavalian 
*   Date   : Wed Nov 18 22:43:50 EST 2009
*********************************************************************************************/

#ifndef __VECTOR_LIB__
#define __VECTOR_LIB__


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

namespace fizika {

class vector3 {
private:
  double cX;
  double cY;
  double cZ;
public:

vector3 (){}
vector3 (double x, double y, double z){setXYZ(x,y,z);}

~vector3 (){}

 void  setXYZ(double x, double y, double z){cX=x;cY=y;cZ=z;}
void    setMagThetaPhi(double mag, double theta, double phi)
{
  double amag = fabs(mag); cX = amag * sin(theta) * cos(phi);
  cY = amag * sin(theta) * sin(phi);cZ = amag * cos(theta);
}


double  theta() const
{ return cX == 0.0 && cY == 0.0 && cZ == 0.0 ? 0.0 : atan2(perp(),cZ);}

double  phi() const
{ return cX == 0.0 && cY == 0.0 ? 0.0 : atan2(cY,cX);}

double  perp() const
{ return sqrt(cX*cX + cY*cY);}

double  perp2() const 
{ return (cX*cX + cY*cY);}

double  mag2() const
{return (cX*cX + cY*cY + cZ*cZ);}

double  mag() const
{ return sqrt(mag2());}

 double x() const { return cX;};
 double y() const { return cY;};
 double z() const { return cZ;};

 void setX(double _x){cX=_x;};
 void setY(double _y){cY=_y;};
 void setZ(double _z){cZ=_z;};

 double  angle(const vector3 &q){
   double ptot2 = mag2()*q.mag2();
   if(ptot2 <= 0) {
      return 0.0;
   } else {
      double arg = dot(q)/sqrt(ptot2);
      if(arg >  1.0) arg =  1.0;
      if(arg < -1.0) arg = -1.0;
      return acos(arg);
   }
 }
void   rotateX(double angle)
{
  double s = sin(angle); double c = cos(angle); double yy = cY;
  cY = c*yy - s*cZ; cZ = s*yy + c*cZ;
}
 //------------------------------------------------------
void     rotateY(double angle)
{
  double s = sin(angle); double c = cos(angle); double zz = cZ;
  cZ = c*zz - s*cX; cX = s*zz + c*cX;
}
//------------------------------------------------------
void    rotateZ(double angle)
{
  double s = sin(angle); double c = cos(angle); double xx = cX;
  cX = c*xx - s*cY; cY = s*xx + c*cY;
}

vector3  unit() const;

vector3  cross(const vector3 &vec) const
{
  return vector3(cY*vec.z()-vec.y()*cZ, cZ*vec.x()-vec.z()*cX,
		    cX*vec.y()-vec.x()*cY);
}
double     dot(const vector3 &vec) const
{ return (cX*vec.x() + cY*vec.y() + cZ*vec.z());}


  void      print(const char *_line = "V3 ")
   {printf("%s : %9.5f %9.5f %9.5f (Mag = %9.5f )\n",
	   _line,x(),y(),z(),mag());};

 const  vector3 &operator=(const vector3 &vec){
  this->setXYZ(vec.x(),vec.y(),vec.z());
  return *this;
 }
 inline vector3 &operator += (const vector3 &);
 inline vector3 &operator -= (const vector3 &);
 
};


vector3  operator+(const vector3 &a, const vector3 &b)
{ return vector3(a.x() + b.x(),a.y() + b.y(),a.z() + b.z());}

vector3  operator-(const vector3 &a, const vector3 &b)
{ return vector3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());}

vector3  operator*(double a, const vector3 &b)
{ return vector3(a*b.x(),a*b.y(),a*b.z());}

vector3  operator*(const vector3 &b,double a)
{return vector3(a*b.x(),a*b.y(),a*b.z());}

//-----------------------------------------------------------
inline vector3& vector3::operator += (const vector3 & p) 
{  cX += p.cX;   cY += p.cY;  cZ += p.cZ;   return *this; }

inline vector3& vector3::operator -= (const vector3 & p) 
{  cX -= p.cX;   cY -= p.cY;  cZ -= p.cZ;   return *this; }


/*=================================================================================
 *   lorentz4 CLASS
 ==================================================================================*/

class lorentz4  {
private:

  vector3  fVect;
  double    fE;

public:

lorentz4 (){}
lorentz4 (double _px, double _py, double _pz, double _e)
{ fVect.setXYZ(_px,_py,_pz);fE  = _e;}
 lorentz4 (vector3 v, double energy) { fVect = v; fE = energy;};
~lorentz4 (){}

double    px() const {return fVect.x();};
 double    py() const {return fVect.y();};
 double    pz() const {return fVect.z();};
 double    x() const {return fVect.x();};
 double    y() const {return fVect.y();};
 double    z() const {return fVect.z();};
 double    theta() const {return fVect.theta();};
 double    phi() const {return fVect.phi();};

 double    m2() const {return (fE*fE-fVect.mag2());};
 double    p() const {return vect().mag();};
 //{ if(M2()>0) return sqrt(M2()); else return sqrt(-M2());};
 double    e() const {return fE;};
 double    t() const {return fE;};
 vector3  vect()  const {return fVect;};


 void      setX(double _p){fVect.setX(_p);};
 void      setY(double _p){fVect.setY(_p);};
 void      setZ(double _p){fVect.setZ(_p);};
 void      setT(double _eg){fE=_eg;};

 void      setXYZM(double _x, double _y, double _z, double _m)
{ if(_m>0)
    setXYZE(_x,_y,_z,sqrt(_x*_x+_y*_y+_z*_z+_m*_m));
  else 
    setXYZE(_x,_y,_z,sqrt(_x*_x+_y*_y+_z*_z));
}

void     setXYZE(double _x, double _y, double _z, double _e)
{
  fVect.setXYZ(_x,_y,_z);
  fE = _e;
}

void      setVectM(vector3 &_v, double _mass)
{
  setXYZM(_v.x(),_v.y(),_v.z(),_mass);
}

void      rotateX(double angle) 
   { fVect.rotateX(angle);};
   void      rotateY(double angle) 
   { fVect.rotateY(angle);};
 void      rotateZ(double angle) 
   { fVect.rotateZ(angle);};

double  m() const 
{
  double mm = m2();
   return mm < 0.0 ? -sqrt(-mm) : sqrt(mm);
}

vector3  boostVector()
{
  //  if(T()==0)
  return vector3(px()/t(),py()/t(),pz()/t());
}
void      boost(vector3 vb)
{
  boost(vb.x(),vb.y(),vb.z());
}

void      boost(double bx, double by, double bz)
{
   double b2 = bx*bx + by*by + bz*bz;
   //register 
   double gamma = 1.0 / sqrt(1.0 - b2);
   //register 
   double bp = bx*x() + by*y() + bz*z();
   //register 
   double gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

   setX(x() + gamma2*bp*bx + gamma*bx*t());
   setY(y() + gamma2*bp*by + gamma*by*t());
   setZ(z() + gamma2*bp*bz + gamma*bz*t());
   setT(gamma*(t() + bp));
}

 void      print(const char *_line = "LV ")
   {printf("%s : %9.5f %9.5f %9.5f (Mag = %9.5f , M = %9.5f)\n",
	   _line,vect().x(),vect().y(),vect().z(),vect().mag(),m());};

 inline lorentz4 & operator = (const lorentz4 &);
 inline lorentz4   operator +  (const lorentz4 &) const;
 inline lorentz4   operator -  (const lorentz4 &) const;
 inline lorentz4 & operator += (const lorentz4 &);
 inline lorentz4 & operator -= (const lorentz4 &);
};

inline lorentz4 &lorentz4::operator = (const lorentz4 & q) 
{ 
  fVect = q.vect();
  fE = q.t();
  return *this;
}
inline lorentz4 lorentz4::operator + (const lorentz4 & q) const {
  return lorentz4(fVect+q.vect(), fE+q.t());
}
inline lorentz4 &lorentz4::operator += (const lorentz4 & q) {   fVect += q.vect();  fE += q.t();   return *this;
}
inline lorentz4 lorentz4::operator - (const lorentz4 & q) const {  
return lorentz4(fVect-q.vect(), fE-q.t());
}
inline lorentz4 &lorentz4::operator -= (const lorentz4 & q) {   
  fVect -= q.vect();   fE -= q.t();  return *this;
}

}
#endif
