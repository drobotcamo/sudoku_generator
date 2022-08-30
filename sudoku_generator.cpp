#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

//linked lists constructed from these nodes. the data in the dummy node is the length.
class Node {
public:
    int data;
    Node* next;
};

//data structure, each square stores a value, possible numbers, and its box ID
struct square {
    int value;
    //int possibilities[9];
    Node *possibilities;
    int id;
};

//function that creates a list, returns the head node, which is the dummy
Node* makeList(){
    Node *head = new Node();
    head->data = 0;
    head->next = NULL;
    return head;
}

//addToList will add a node to a list, at the beginning, moving the list further down
void addToList(Node *head, int data){
    Node* new_node = new Node();
    new_node->data = data;
    new_node->next = head->next;
    head->next = new_node;
    head->data++;
}

//deleteNode will take in a list and a data point, then find and delete that data point from the list
//if the node is not present, then nothing will happen.
//will return a boolean depending whether or not deletion was successful
bool deleteNode(Node *head, int data){
    Node *cursor = head;
    while((cursor->next != NULL) && (cursor->next->data != data)){
        cursor = cursor->next;
    }
    if(cursor->next == NULL)
        return false;
    cursor->next = cursor->next->next;
    head->data--;
    return true;
}

//printList takes in a linked list as a parameter and prints it out in order to the terminal
void printList(Node *list){
    std::cout << "length: " << list->data << "  list: ";
    Node* cursor = list;
    while(cursor->next != NULL)
    {
        std::cout  << cursor->next->data << ", " ;
        cursor = cursor->next;
    }
    std::cout << "\n";
}

//getData takes in a list and an index, and will return the data of the node at that index
//index means that 
int getData(Node *head, int index){
    Node *cursor = head;
    if(cursor->next == NULL)
        return -1;
    cursor = cursor->next;     // testing
    for(int i = 0; i < index; i++)
        cursor = cursor->next;
    // if(index == 0)
    //     return cursor->next->data;
    if(cursor != NULL)
        return cursor->data;
    return -1;
}

//print the board out, parameter is the tiles array
void printBoard(square **board) {
    for(int i = 0; i < 9; i++){
        if(i % 3 == 0 && i != 0)
            cout << "- - - + - - - + - - -\n";
        for(int j = 0; j < 9; j++){
            if(j % 3 == 0 && j != 0)
                cout << "| "; 
            if(board[i][j].value != 0)
                cout << board[i][j].value << ' ';
            else
                cout << ". ";
        }
        cout << "\n";
    }   
}

//fill a new array of square structs
void initializeArray(square **x){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
           x[i][j].value = 0;                               //initizlizes each square with an 0, placeholder
           x[i][j].possibilities = makeList();              //gives each square a possibilities list
           for(int k = 0; k < 9; k++)
           {
                addToList(x[i][j].possibilities, k + 1);     //fills each square's possibilities list with 1-9
                x[i][j].id = ((i / 3) + 1) + ((j / 3) * 3); //assigns each square a box ID from 1-9 from left to right
           }
        }
    }
}

//debugger that allows the user to see the possibilities of a selected square
void debug(square **tiles){
    bool again;
    do{
        cout << "enter x coordinate of the tile you wanna check, or enter 82 to enter an ID (0-8): ";
        int x;
        cin >> x;
        cout << "enter y coordinate (or ID) of the tile you wanna check (0-8): ";
        int y;
        cin >> y;
        if(x > 81)
        {
            x = y % 9; 
            y = y / 9;                       
        }
        cout << x << "    " << y << "\n";
        printList(tiles[y][x].possibilities);
        cout << "\n";
        cout << "another? (y/n): ";
        char input;
        cin >> input;
        if(input == 'n')
            again = false;
        else if(input == 'q')
            return;
        else
            again = true;

    } while(again);
}

bool boardIsValid(square **tiles)
{
    Node *checker = makeList();
    for(int i = 1; i < 10; i++)
        addToList(checker, i);

    for(int column = 0; column < 9; column++){
        for(int row = 0; row < 9; row++){
            if(tiles[column][row].value != 0)
                deleteNode(checker, tiles[column][row].value);
            else{
                for(int index = 0; index < tiles[column][row].possibilities->data; index++)
                    deleteNode(checker, getData(tiles[column][row].possibilities, index));
            }
        }
        if(checker->data > 0){
            cout << "Row #" << column + 1 << " cannot be completed   |  ";
            cout << "The following value cannot be inserted: " << checker->next->data << "\n";
            return false;
        }
        for(int i = 1; i < 10; i++)
            addToList(checker, i);
    }
    for(int row = 0; row < 9; row++){
        for(int column = 0; column < 9; column++){
            if(tiles[column][row].value != 0)
                deleteNode(checker, tiles[column][row].value);
            else{
                for(int index = 0; index < tiles[column][row].possibilities->data; index++)
                    deleteNode(checker, getData(tiles[column][row].possibilities, index));
            }
        }
        if(checker->data > 0){
            cout << "Column #" << row + 1 << " cannot be completed   |  ";
            cout << "The following value cannot be inserted: " << checker->next->data << "\n";
            return false;
        }
        for(int i = 1; i < 10; i++)
            addToList(checker, i);
    }

    for(int boxID = 1; boxID < 9; boxID++){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(tiles[i][j].id == boxID){
                    if(tiles[i][j].value != 0)
                        deleteNode(checker, tiles[i][j].value);
                    else{
                        for(int index = 0; index < tiles[i][j].possibilities->data; index++)
                            deleteNode(checker, getData(tiles[i][j].possibilities, index));
                    }
                }
                if(tiles[i][j].possibilities->data == 0 && tiles[i][j].value == 0){
                    cout << "ERROR: tile at position x: " << j << ", y: " << i << " has 0 possibilities.\n";
                    return false;
                }
            }
        }
        if(checker->data > 0){
            cout << "Box #" << boxID << " cannot be completed   |  ";
            cout << "The following value cannot be inserted: " << checker->next->data << "\n";
            return false;
        }
        for(int i = 1; i < 10; i++)
            addToList(checker, i);
    }
    return true;
}

//given coordinates of a tile, this will eliminate the possibility that any tile in the same column row or box will share its value
//returns a boolean that will tell whether a box now has 0 possibilities.
//if a box now has 0 poss, then it will return false, and will restore all of the boxes that it had previously changed 
bool removePossibilites(square **tiles, int x, int y){
    bool succeeding = true;
    int val = tiles[x][y].value;
    Node *changed = makeList();
    Node *deletedPoss = makeList();
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if((i == x | j == y) | (tiles[x][y].id == tiles[i][j].id)){   
                if(deleteNode(tiles[i][j].possibilities, val))   //delete the node from the possibilities list that matches the value of the tile, assigns the bool to didChange
                    addToList(changed, (i * 9) + j);
            }
        }
    }
    for(int i = 1; i <= 9; i++){
        if(deleteNode(tiles[x][y].possibilities, i))
            addToList(deletedPoss, i);
    }

    if(boardIsValid(tiles) == false)
        succeeding = false;

    if(succeeding == false){
        int k, j, key;
        cout << "Clearing the changes made by inserting that " << val << "\n";
        while(changed->next != NULL){
            key = changed->next->data;
            k = key / 9;                        
            j = key % 9; 
            addToList(tiles[k][j].possibilities, val);
            deleteNode(changed, key);
        }
        while(deletedPoss->next != NULL){
            key = deletedPoss->next->data;
            addToList(tiles[x][y].possibilities, key);
            deleteNode(deletedPoss, key);
        }
    }
    
    return succeeding;
}

//recursor is the engine that carries out one step of the creation process
//it first chooses a tile from the list of available tiles
//then if there are possibilities in that tile's list, it will insert a random possible value
//finally it will then adjust the possibilities of all other tiles on the board as such.
//if there are no possibilities, then it will output "failed"
void recursor(square **tiles, int *counter, int openSpaces[81], bool *lastWasSuccessful, bool *complete){
    //debugger stepper \/
    // char placeholder;
    // cout << "press enter to take the next step \n";
    // cin >> placeholder;
    cout << "=====================================================================\n";
    cout << "recursor #" << *counter << " is now going to execute at position x:";
    
    int x, y, key, insert = 0;
    key = openSpaces[*counter];          //this code block gets the ID from the openspaces list, and translates it into x and y
    x = key / 9;                        //
    y = key % 9;                        //
    cout << y << ", y:" << x <<"\n";
    printBoard(tiles);

    if(*counter == 81)
    {                         
        *complete = true;
        return;
    }
    
    while(tiles[x][y].possibilities->data > 0  && *complete == false){
        tiles[x][y].value = getData(tiles[x][y].possibilities, (rand() % tiles[x][y].possibilities->data));
        cout << "attempting insertion of value " << tiles[x][y].value << " at position x: " << y << ", y: " << x << "\n";
        if(removePossibilites(tiles, x, y) != false && lastWasSuccessful){
            cout << "insertion successful \n";
            *counter = *counter + 1;
            *lastWasSuccessful = true;
            recursor(tiles, counter, openSpaces, lastWasSuccessful, complete);
        }
        else{
            deleteNode(tiles[x][y].possibilities, tiles[x][y].value);
            tiles[x][y].value = 0;
        }
    }
    //debug(tiles);
    cout << "mission failed. returning to base.\n";
    return;
}


    // if(lastWasSuccessful){       //there can be an insertion, and the last was successful
    //     tiles[x][y].value = getData(tiles[x][y].possibilities, (rand() % tiles[x][y].possibilities->data) + 1);
    //         //the line above will insert a random possibility, and then will adjust the other possibilities accordingly.
    //     cout << "attempt #" << counter << "\n";
    //         //now, if that adjustment left any empty tiles with 0 remaining possibilities, then we have a problem.

    //     if(removePossibilites(tiles, x, y) == false){
    //         cout << "and we just inserted a value at x:" << y << ", y:" << x << ", whose value is: " << tiles[x][y].value << "\n";
    //         lastWasSuccessful = false;
    //         tiles[x][y].value = 0;
            
    //     }
    //     else{     
    //         counter++;
    //         recursor(tiles, counter, openSpaces, lastWasSuccessful, complete);
    //     }
    // }

//createGame is the driver that calls recursor
//currently will attempt a recursor until the board is determined to be unfillable 
//(i.e. there are no possibilities in the remaining squares)
void createGame(square **x, int openSpaces[81]){
    int count = 0;
    bool lastWasSuccessful = true, complete = false;
    recursor(x, &count, openSpaces, &lastWasSuccessful, &complete);
}

void randomSequence(int openSpaces[81]){
    Node *available = makeList();
    int key;
    for(int i = 0; i < 81; i++){
        addToList(available, i);
    }
    for(int i = 0; i < 81; i++){
        key = (rand() % available->data);
        openSpaces[i] = getData(available, key);
        deleteNode(available, openSpaces[i]);
    }
}



int main() {
    srand (time(NULL));                 //initialize randomizer
    square **tiles = new square*[9];    //initialize 2D array of squares
    for (int i = 0; i < 9; i++) 
        tiles[i] = new square[9];      

    int openSpaces[81];
    randomSequence(openSpaces);

    initializeArray(tiles);             //fills each square with placeholder value and possibilities
    createGame(tiles, openSpaces);      //fills the board with values
    //printBoard(tiles);                   //prints the board
    //boardIsValid(tiles);
   
    //debug(tiles);
    
    for(int i=0;i<9;i++)                //delete the inner arrays
      delete [] tiles[i];   
    delete [] tiles;   
    return 0;
}

/* board reference
x x x | x x x | x x x 
x x x | x x x | x x x
x x x | x x x | x x x
- - - + - - - + - - -
x x x | x x x | x x x
x x x | x x x | x x x
x x x | x x x | x x x
- - - + - - - + - - -
x x x | x x x | x x x
x x x | x x x | x x x
x x x | x x x | x x x 
*/