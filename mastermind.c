#include <stdio.h>
// #include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* function for a secret number generation */
void selectRandomNumber(char codeLength[4]){             
    srand(time(0));                                         // the random number generator used by the rand() is seeded using srand() and NULL or 0 is used to set time seed to current
    // codeLength[4] = {2,2,2,2};                           //instead of int* codeLength = malloc(sizeof(int) *4);  allocating memory and string length to the secret code - dont forget to call the codeLength() whereever the selectRandomNumber() is used
    int position[128] = {0};                                //ASCII positions instead of
    int currPos = 0;
    while(currPos < 4){
        int ranNum = rand() % 8;                            // random numbers between 0-7 similar function as Math.floor(Maths.random (8) *1)
        if(position[ranNum] == 1){                          //to check if ranNum has already being assigned/used
            continue;
        }
        codeLength[currPos] = ranNum + '0';                 //-'0' converts the the ranNum to the character equuilavent in the ASCII positions... = for assigning a value and == for comparison
        currPos++;                                          //increment the currPos or mv on to the next position i.e from 0 to 1
        position[ranNum] = 1;                               //indicates that this position of the ranNum has been used
    }
}

/* function to authenticate or confirm if user input code string is of correct length and of unique numbers. */
int userGuessCodeAuth(char* code){
    int count = 0;
    while(*code != '\0'){                                   //*code(actual value of the code) since we dont know the length of the code
        count++;
        code++;
    }if (count == 4){
        return 0;
    }else{
        return 1;
    }
}

/* main function to run the program */
int main(int argc, char** argv){                            //argc is the total no of chr or argument entered in the cmd and argv is the value itself entered by the user
    printf("Will you find the secret code?\nPlease enter a valid guess\n");
    int attempts = 10;                                      // number of attempts by default when there is no input
    char code[4];                                           // secret code - 4 char string
    if(argc > 1){                                           //when there is an input from the user
       for (int u = 1; u < argc; u++){
            if (argv[u][0] == '-' && argv[u][1] == 't' && argv[u + 1] != NULL){      //argv[u] = current position of the user value
               attempts = atoi(argv[u + 1]);                //atoi() converts string to integer
           }  
            if (argv[u][0] == '-' && argv[u][1] == 'c' && argv[u + 1] != NULL){      //argv[u] = current position of the user value
                for(int v=0; v<4; v++){
                    code[v] = argv[u + 1][v];                   //value assigned to code
                }
           }          
       }
    }
    if(*code == 0){
        selectRandomNumber(code);                               //code here is same as the codeLength para in the void selectRandomNumber() above
    }

    /* cmd line user input cases for the game paramters -c CODE, -t ATTEMPTS, * and -d eachDigits */
    int u, v;                                                   // index for the loop
    char buf[5];
    int count = 1;
    int wellPlaced;
    int misPlaced;
    while (1){                                                  // game loops on infinite until its broken
        if (count <= attempts){                                 // remember that there are 10 attempts allowed, so this takes in the condition for when the count hasnt exceeded the no. of attempts allowed
            int totalRead = read(0, buf, sizeof(buf));          // takes in guesses entered by user
            if (totalRead == 0)                                 // Ctrl D to STOP GAME
                return 0;                                       //return 0 = no error, and return 1 = error
            buf[4] = '\0';                                      // terminates user guess string
            wellPlaced = 0;                                     // for each guess loop, re-initialise to 0
            misPlaced = 0;
            if (userGuessCodeAuth(buf) != 0){                   // test guess for correct input i.e 4 unique numbers
                    printf("Incorrect Input 👎\n");             // dont register it as an attempt if invalid, and notify player
            }else{                                              // else test the guesses entered by the user
                for (u = 0; u < 4; u++){                        // nested loops to compare user guess and secret code
                    for (v = 0; v < 4; v++){
                        if (buf[u] == code[v] && u == v){       // if number is matched and in place
                            wellPlaced++;                       // increment wellPlaced digits when occur
                        }else if (buf[u] == code[v] && u != v){  // if number is matched but not in place
                            misPlaced++;                        // increment misPlaced pieces when occur
                        }
                        }
                    }
                    if (wellPlaced == 4){                       // if correct guess, send the winner a message & exit the game
                        printf("Congratz! You did it!\n");
                        break;
                    }else{
                        count++;                                // increment attempts, give hint and continue game
                        printf("o---------------------------o\n");
                        printf("Well placed pieces: %d\n", wellPlaced);
                        printf("Misplaced pieces: %d\n", misPlaced);
                        printf("o---------------------------o\n");
                    }
                }
            }else{                                              // Attempts exhausted, show failure msg and exit game
                printf("⛔  No more attemps remaining, YOU LOST!\n");
                break;
            }
        }
}
