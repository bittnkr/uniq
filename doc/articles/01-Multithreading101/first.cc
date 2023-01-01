#include <thread>
#include <iostream>
using namespace std;

void deposit(int &balance, int amount) { 
  for(auto i = 0; i < amount; i++) 
    balance++; 
};

void withdrawal(int &balance, int amount) { 
  for(auto i = 0; i < amount; i++) 
    balance--; 
};

int main(){
  
  int A = 0;
  
  cout << "\nDepositing a bilion dollars"<< A << "\n";
  deposit(A, 1e9);
  cout << "\nBalance: $"<< A << "\n";

  cout << "\nWithdrawing a bilion dollars"<< A << "\n";
  withdrawal(A, 1e9);     

  cout << "Balance: $"<< A << "\n";
}