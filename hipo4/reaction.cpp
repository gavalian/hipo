#include <iostream>
#include "reaction.h"

fizika::reaction *__reaction;

extern "C" {

  void reaction_get_vector_debug(double *vec){
    printf("c extern x-component = %f\n",vec[0]);
    vec[0] = 0.1; vec[1] = 0.2; vec[2] = 0.3;
  }

  void reaction_define(const char *file, double energy, int *pids, int *counts, int length){
    __reaction = new fizika::reaction(file,energy,pids,counts,length);
  }

  void reaction_get_vector(double *vec, int *signs, int *pids, int *skips, double *masses, int length){
    fizika::lorentz4 v = __reaction->get(signs, pids, skips, masses, length);
    //printf("vector from REACTIONH = %f %f %f %f\n",v.x(),v.y(),v.z(),v.e());
    vec[0] = v.x(); vec[1] = v.y(); vec[2] = v.z(); vec[3] = v.e();
  }

  void reaction_get_cm(double *vec){
    fizika::lorentz4 &v = __reaction->cm();
    vec[0] = v.x(); vec[1] = v.y(); vec[2] = v.z(); vec[3] = v.e();
  }

  bool reaction_is_valid(){
    bool status = __reaction->is_valid();
    return status==true?1:0;
  }

  bool reaction_next(){
    bool status = __reaction->next();
    return status==true?1:0;
    //return __reaction->next();
  }

  void get_format_string(char *data){
    printf("data from \"C\" : [%s]\n",data);
    strcpy(data,"troubles"); data[5] = 0;
  }
}
