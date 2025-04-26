#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;
 
// function to be used in callable
void func_sleep(int x)
 {
   //std::this_thread::sleep_for(N);
   //std::sleep(100);
   std::this_thread::sleep_for(std::chrono::milliseconds(x));
   cout << "Thread finished :: callable => function pointer\n";
 }
  
void func(int n){
  vector<thread*> threads;

  for(int i = 0; i < n; i++){
    threads.push_back(new thread(func_sleep,5000));    
  }
  for(int i = 0; i < n; i++){ threads[i]->join();}
}

int main()
{
// Define a Lambda Expression
//launch thread using function pointer as callable

//thread th1(func_sleep, 5000);  
//th1.join();
for(int k = 0; k < 10; k++){
 thread th2(func,5);
 th2.join();
 printf("--done iteration %d\n",k);
}

printf("--done\n");
  
return 0;
}
