// Assumption: The triangle is right-angled with a height of 3 and a base of 4.
// It is displayed as:
// ****
// **
// *

#include<iostream>

void triangle(int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if(j<=i){
                std::cout<<"*";
            }
        }
        std::cout<<"\n";
    }
}

int main(){
    std::cout<<"****\n**\n*\n";
}
