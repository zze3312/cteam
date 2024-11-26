#include "../header/frdDfn.h"

void main(){
    User loginUser;
    srand(time(NULL));
    while(1)
    {
        mainMenu(&loginUser);
    }
}

#include "../header/func.h"