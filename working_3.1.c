/**Welcome to CommandPrompt Crush - by Sean Bourchier - 1806ICT Assignment 30% - due Fri 15 May 2020**/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define MAX_NAME_LEN 30
//17 functions not including main
int startUp();
int newGame();
int loadGame();
void endGame();
void createBoard();
void displayBoard();
void displayCommands();
void recieveCommand();
int commandSwap(int c1row, int c1col, int c2row, int c2col);
bool checkValidSwap(int c1row, int c1col, int c2row, int c2col);
int checkBoard();
bool findClumps();
void calculatePoints();
void cascade();
void displayTime();
void cleanBuffer();
void showChars();
//global variables
char CHARS[8] = {'!', '#', '$', '%', '&', '=', '?', '@'};
int s, u;
char *board = NULL;
int *header = NULL;
char *boardClumpsOnly = NULL;
int clumpCols[16] = {0};
int score = 0;
int pointsPerMove = 0;
clock_t gameStartTime;
bool loadedGame = false;
int loadedGameTime = 0;
FILE *inputFile = NULL;
FILE *outputFile = NULL;
char inputFileName[MAX_NAME_LEN];
char outputFileName[MAX_NAME_LEN];

int main()
{
startUp();
displayCommands();
displayBoard();
recieveCommand();
return 0;
}

int startUp()
{
int game = 0;
printf("                       Welcome to CommandPrompt Crush!\n\nPress 1 for New Game or Press 2 to Load Saved Game, or any other key to exit.\n");
while(1){
    scanf("%d", &game);
    if(game == 1){
        while( newGame() == 1){
            newGame();
        }
        return 1;
    }else if(game == 2){
        while( loadGame() == 1){
            loadGame();
        }
        return 1;
    }else{
        printf("You have decided to exit.\n");
        exit(1);
    }
}
return 0;
}

int loadGame()
{
printf("Enter filename: ");
scanf("%s", inputFileName);
//open save file and error checking options
inputFile = fopen(inputFileName, "r");
if(inputFile == NULL){
    printf("Unable to open file. Press 'y' to try another filename, 'n' to start a new game or any other key to quit.\n");
    cleanBuffer();
    char a;
    scanf("%c", &a);
    if(a == 'y'){
        return 1;
    }else if(a == 'n'){
        newGame();
        return 0;
    }else{
        printf("You have chosen to exit.\n");
        exit(1);
    }
}
//read s(size) u(unique chars) score and previous time from file and error checking options
if( fscanf(inputFile, "%d %d %d %d", &s, &u, &score, &loadedGameTime) != 4 ){
    printf("Unable to load game data. Press 'y' to try another filename, 'n' to start a new game or any other key to quit.\n");
    cleanBuffer();
    char a;
    scanf("%c", &a);
    if(a == 'y'){
        return 1;
    }else if(a == 'n'){
        newGame();
        return 0;
    }else{
        printf("You have chosen to exit.\n");
        exit(1);
    }
}
//allocate memory for board and header and error checking options
board = (char *)malloc(s * s * sizeof(char) + 1);
board[s * s] = '\0';
header = (int *)malloc((s + 1) * sizeof(int));
if (board == NULL || header == NULL){
printf("Memory allocation failure. Press 'y' to try another filename, 'n' to start a new game or any other key to quit.\n");
    cleanBuffer();
    char a;
    scanf("%c", &a);
    if(a == 'y'){
        return 1;
    }else if(a == 'n'){
        newGame();
        return 0;
    }else{
        printf("You have chosen to exit.\n");
        exit(1);
    }
}
//read board from save file and error checking options
if( fscanf(inputFile, "%s", board) != 1 ){
    printf("Unable to load game data. Press 'y' to try another filename, 'n' to start a new game or any other key to quit.\n");
    cleanBuffer();
    char a;
    scanf("%c", &a);
    if(a == 'y'){
        return 1;
    }else if(a == 'n'){
        newGame();
        return 0;
    }else{
        printf("You have chosen to exit.\n");
        exit(1);
    }
}
//create column header array
int col = 1;
for(int i = 1; i < s + 1; i++){
    *(header) = 0;
    *(header + i) = col;
    col++;
}
loadedGame = true;
return 0;
}

int newGame()
{
//read in size and number of unique chars and error checking options
while(1){
    printf("\nEnter board size (6-16) and unique symbols(4-8): ");
    scanf("%d %d", &s, &u);
    cleanBuffer();
    if(s < 6 || s > 16 || u < 4 || u > 8){
        printf("Invalid selection parameters. Make sure board size between 6-16 and unique symbols between 4-8.\n");
    }else{
    break;
    }
}
//allocate memory for board and header and error checking options
board = (char *)malloc(s * s * sizeof(char) + 1);
board[s*s] = '\0';
header = (int *)malloc((s + 1) * sizeof(int));
if (board == NULL || header == NULL) {
printf("Memory allocation failure. Press 'y' to try again or any other key to quit.\n");
    cleanBuffer();
    char a;
    scanf("%c", &a);
    if(a == 'y'){
        return 1;
    }else{
        printf("You have chosen to exit.\n");
        exit(1);
    }
}
createBoard();
gameStartTime = clock();
loadedGameTime = 0;
loadedGame = false;
return 0;
}

void cleanBuffer()
//force program to read the current char in input stream as long as its not EOF or end of line
{
int n;
while((n = getchar()) != EOF && n != '\n' );
}

void displayTime(){
int min, sec;
int currentTime = (clock() - gameStartTime);
sec = currentTime/1000;
min = sec/60;
sec = (currentTime/1000) - (min*60);
printf(" Time elapsed: %d min %d sec\n", min, sec);
}

void createBoard()
//only used in newGame() function
{
//fill board
for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
        *(board + i*s + j) = CHARS[rand()%u];
    }
}
//check board
//ensure rows don't have 3 symbols in a row
for(int i = 0; i < s; i++){
    for(int j = 1; j < s - 1; j++){
        while(*(board + i*s + j) == *(board + i*s + (j-1)) && *(board + i*s + j) == *(board + i*s + (j+1))){
            *(board + i*s + j) = CHARS[rand()%u];
        }
    }
}
//ensure cols don't have 3 symbols in a row and any col changes dont affect rows as well
for(int i = 1; i < s - 1; i++){
    for(int j = 0; j < s; j++){
        while(*(board + i*s + j) == *(board + (i-1)*s + j) && *(board + i*s + j) == *(board + (i+1)*s + j)){
            *(board + i*s + j) = CHARS[rand()%u];
            if(j == 0){
                while(*(board + i*s + j) == *(board + i*s + (j+1))){
                    *(board + i*s + j) = CHARS[rand()%u];
                }
            }else if(j == s-1){
                while(*(board + i*s + j) == *(board + i*s + (j-1))){
                    *(board + i*s + j) = CHARS[rand()%u];
                }
            }else{
                while(*(board + i*s + j) == *(board + i*s + (j-1)) || *(board + i*s + j) == *(board + i*s + (j+1))){
                    *(board + i*s + j) = CHARS[rand()%u];
                }
            }
        }
    }
}
//fill header
int col = 1;
for(int i = 1; i < s + 1; i++){
    *(header) = 0;
    *(header + i) = col;
    col++;
}
}

void displayBoard()
{
//display header
printf("    ");
for(int i = 0; i < s; i++){
    printf("%3d ", *(header + (i+1)));
}
printf("\n");
//display board
int row = 1;
for(int i = 0; i < s; i++){
    printf("%3d ", row);
    for(int j = 0; j < s; j++){
        printf("%3c ", *(board + i*s + j));
    };
    printf("\n");
    row++;
}
}

int commandSwap(int c1row, int c1col, int c2row, int c2col)
{
printf(" Characters for swap: %c with %c\n", *(board + (c1row-1)*s + (c1col-1)), *(board + (c2row-1)*s + (c2col-1)));
//check to see if the Cell 1 and Cell 2 were valid inputs in correct range of board
if((c1row-1) < 0 || (c1row-1) > (s-1) ||
    (c1col-1) < 0 || (c1col-1) > (s-1) ||
    (c2row-1) < 0 || (c2row-1) > (s-1) ||
    (c2col-1) < 0 || (c2col-1) > (s-1)){
    printf("Invalid cells, numbers must be between 1 and %d.\n", s);
    return 1;
    }else
//check to see if cells are next to each other
if((c1row - c2row) > 1 || (c1row - c2row) < -1 || //within 1 square of each other
    (c1col - c2col) > 1 || (c1col - c2col) < -1 ||
    ((c1row == c1col && c2row == c2col)) || //not diagonal
    ((c1row != c2row && c1col != c2col))){
    printf("Invalid swap, cells must be next to each other horizontally or vertically.\n");
    return 1;
    }else
//check to see if swap produces a row or column of three or more same symbols
if(checkValidSwap(c1row, c1col, c2row, c2col)==true){
    while(1){
        if( findClumps()== true ){          //keeps doing this until no clumps on board
            cascade();                      //only runs if findClumps true
            for(int i = 0; i < 16; i++){    //reset clumpCols array
                clumpCols[i] = 0;
            }
        }else{
            break;
        }
    }
    printf(" Points from move: %d\n", pointsPerMove*10);
    printf(" Score is: %d\n", score*10);
    pointsPerMove = 0; //reset points per move
    displayTime();
    displayBoard();
    return 0;
}else{
    printf("Swap does not produce a row or column of three or more of the same symbols. Try again.\n");
    return 1;
}
return 1;
}

bool findClumps()
{
bool clumps = false;
boardClumpsOnly = (char *)malloc(s * s * sizeof(char));
//fill boardClumpsOnly with character 0
for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
        *(boardClumpsOnly + i*s + j) = 48;
    }
}
//check rows for clumps, if there are then change corresponding values in boardClumpsOnly to character 1
for(int i = 0; i < s; i++){
    for(int j = 0; j < s - 2; j++){
        if(*(board + i*s + j) == *(board + i*s + (j+1)) && *(board + i*s + j) == *(board + i*s + (j+2))){
            *(boardClumpsOnly + i*s + j) = 49;
            *(boardClumpsOnly + i*s + (j+1)) = 49;
            *(boardClumpsOnly + i*s + (j+2)) = 49;
            clumps = true;
        }
    }
}
//check columns for clumps, if there are then change corresponding values in boardClumpsOnly to character 1
for(int i = 0; i < s - 2; i++){
    for(int j = 0; j < s; j++){
        if(*(board + i*s + j) == *(board + (i+1)*s + j) && *(board + i*s + j) == *(board + (i+2)*s + j)){
            *(boardClumpsOnly + i*s + j) = 49;
            *(boardClumpsOnly + (i+1)*s + j) = 49;
            *(boardClumpsOnly + (i+2)*s + j) = 49;
            clumps = true;
        }
    }
}
calculatePoints();
return clumps;
}

void calculatePoints()
{
int points = 0;
//count number of 1's in boardClumpsOnly to calculate points
//highlight columns that have clumps and will need to be cascaded
for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
        if( *(boardClumpsOnly + i*s + j) == 49 ){
            points++;
            clumpCols[j]+=1;
        }
    }
}
//points are calculated here before the cascade
pointsPerMove+=points;
score+=points;
}

void cascade()
{
for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
        //if 1's in first row of boardClumpsOnly, dont cascade, just change to 2's on boardClumpsOnly
        if( (*(boardClumpsOnly + i*s + j) == 49) && i == 0 ){
            for(int l = 0; l < s; l++){
                if(l < clumpCols[j]){
                    *(boardClumpsOnly + l*s + j) = 50;
                }else{
                    *(boardClumpsOnly + l*s + j) = 48;
                }
            }
        }
        //find 1's on boardClumpsOnly that are not in first row
        if( (*(boardClumpsOnly + i*s + j) == 49) && i != 0 ){
            //number of cells in column clump, used in below loop to fill from bottom of clump upwards
            int cellColToFill = clumpCols[j] - 1;
            //loop to cascade non-clump cells above clump down to bottom of clump
            for(int k = 1; k < i + 1; k++){
                *(board + (i + cellColToFill)*s + j) = *(board + (i-k)*s + j);
                //decremented so loop fills upwards
                cellColToFill--;
            }
            //change 1's on boardClumpsOnly to 2's, so that column won't be changed again and to indicate what will be randomized
            for(int l = 0; l < s; l++){
                if(l < clumpCols[j]){
                    *(boardClumpsOnly + l*s + j) = 50;
                }else{
                    *(boardClumpsOnly + l*s + j) = 48;
                }
            }
        }
    }
}
//refill top cells that were cascaded with random characters
for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
        if( (*(boardClumpsOnly + i*s + j) == 50) ){
            *(board + i*s + j) = CHARS[rand()%u];
            *(boardClumpsOnly + i*s + j) = 48;
        }
    }
}
}

bool checkValidSwap(int c1row, int c1col, int c2row, int c2col)
{
bool swap = false;
char temp;
//swap variables so check can be performed
temp = *(board + (c1row-1)*s + (c1col-1));
*(board + (c1row-1)*s + (c1col-1)) = *(board + (c2row-1)*s + (c2col-1));
*(board + (c2row-1)*s + (c2col-1)) = temp;
//check if swap produces col or row of 3 or more
if(checkBoard() > 0){
    swap = true;
}else{
//if swap doesn't produce col or row of 3 or more, swap back
    swap = false;
    temp = *(board + (c1row-1)*s + (c1col-1));
    *(board + (c1row-1)*s + (c1col-1)) = *(board + (c2row-1)*s + (c2col-1));
    *(board + (c2row-1)*s + (c2col-1)) = temp;
}
return swap;
}

int checkBoard()
{
bool clump = false;
int count = 0;
//check if rows have clump of 3 or more symbols in a row
for(int i = 0; i < s; i++){
    for(int j = 0; j < s - 2; j++){
        if(clump == true){
            if(*(board + i*s + j) != *(board + i*s + (j+1)) && *(board + i*s + j) != *(board + i*s + (j+2))){
            clump = false;
            }
        }else if(*(board + i*s + j) == *(board + i*s + (j+1)) && *(board + i*s + j) == *(board + i*s + (j+2))){
            clump = true;
            count++;
        }
    }
}
clump = false;
//check if columns have clump of 3 or more symbols in a row
for(int i = 0; i < s - 2; i++){
    for(int j = 0; j < s; j++){
        if(clump == true){
            if(*(board + i*s + j) != *(board + (i+1)*s + j) && *(board + i*s + j) != *(board + (i+2)*s + j)){
                clump = false;
            }
        }else if(*(board + i*s + j) == *(board + (i+1)*s + j) && *(board + i*s + j) == *(board + (i+2)*s + j)){
            if(i == 0){
                clump = true;
                count++;
            }else{
                //make sure it doesn't re-add same column to count
                if(*(board + i*s + j) != *(board + (i-1)*s + j)){
                    clump = true;
                    count++;
                }
            }
        }
    }
}
return count;
}

void showChars()
{
printf("\nThese are the %u unique characters: ", u);
for(int j = 0; j < u; j++){
    printf("%c ", CHARS[j]);
}
printf("\n");
}

void recieveCommand()
{
char command;
while(1){
    scanf("%c", &command);
    if(command == 's'){
        int c1row, c1col, c2row, c2col;
        scanf("%d %d %d %d", &c1row, &c1col, &c2row, &c2col);
        commandSwap(c1row, c1col, c2row, c2col);
    }else if(command == 'n'){
        newGame();
        gameStartTime = clock();
        score = 0;
        displayBoard();
    }else if(command == 'q'){
        endGame();
    }else if(command == 'd'){
        displayCommands();
    }else if(command == 't'){
        displayTime();
    }else if(command == 'p'){
        printf("Current score is: %d\n", score*10);
    }else if(command == 'c'){
        showChars();
    }
}
}

void endGame()
{
double finalScore = 0.0;
//change final score depending on loaded game or not
if(loadedGame){
    finalScore = (score*10)/((double)(clock() + loadedGameTime - gameStartTime)/1000);
}else{
    finalScore = (score*10)/((double)(clock() - gameStartTime)/1000);
}
printf("Your final score is (score/time): %.2f\n", finalScore);
printf("You have chosen to quit. Press 'y' to save game, any other key to exit without saving.\n");
while(1){
    cleanBuffer();
    char save;
    scanf("%c", &save);
    //save file and error checking options
    if(save == 'y'){
        while(outputFile == NULL){
            printf("Please enter output filename with .txt at the end: ");
            scanf("%s", outputFileName);
            outputFile = fopen(outputFileName, "w");
            if(outputFile == NULL){
                char tryAnother;
                printf("Error saving output file. Press 'y' to try another filename or any other key to exit.\n");
                cleanBuffer();
                scanf("%c", &tryAnother);
                if(tryAnother == 'y'){
                    continue;
                }else{
                    printf("You have decided to exit without saving.\n");
                    free(board);
                    free(header);
                    free(boardClumpsOnly);
                    exit(1);
                }
            }else{
                break;
            }
        }
        //loadedGameTime will equal zero if new game, if loaded game, previous time will be added
        int currentTime = (clock() - gameStartTime + loadedGameTime);
        char temp[s*s + 1];
        strcpy(temp, board);
        fprintf(outputFile, "%d %d %d %d %s", s, u, score, currentTime, temp);
        printf("File saved: %s\n Thanks for playing!\n", outputFileName);
        break;
    }else{
        printf("You have decided to exit without saving.\n");
        break;
    }
}
free(board);
free(header);
free(boardClumpsOnly);
exit(1);
}

void displayCommands()
{
printf("\nThe commands are:\n s for swap followed by row then column of both cells \n n for new game \n q for quit \n d for display commands \n t for time elapsed \n p for display score \n c to show unique characters for board \n\n");
}
