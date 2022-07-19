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
void deleteNode(Node *head, int data){
    Node *cursor = head;
    while((cursor->next != NULL) && (cursor->next->data != data)){
        cursor = cursor->next;
    }
    if(cursor->next == NULL)
        return;
    cursor->next = cursor->next->next;
    head->data--;
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
int getData(Node *head, int index){
    Node *cursor = head;
    if(cursor->next == NULL)
        return -1;
    for(int i = 0; i < index; i++)
        cursor = cursor->next;
    if(index == 0)
        return cursor->next->data;
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


//given coordinates of a tile, this will eliminate the possibility that any tile in the same column row or box will share its value
void adjustPossibilities(square **tiles, int x, int y){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if((i == x | j == y) | (tiles[x][y].id == tiles[i][j].id))      
                deleteNode(tiles[i][j].possibilities, tiles[x][y].value);   //delete the node from the possibilities list that matches the value of the tile
        }
    }
}

//isPresent will take in a list and a value, and return true if any of the nodes in that list contain the value. currently unused.
bool isPresent(Node *head, int value){
    Node *cursor = head->next;
    while(cursor->next != NULL){
        if(cursor->data == value)
            return true;
        cursor = cursor->next;
    }
    return false;
}

//recursor is the engine that carries out one step of the creation process
//it first chooses a tile from the list of available tiles
//then if there are possibilities in that tile's list, it will insert a random possible value
//finally it will then adjust the possibilities of all other tiles on the board as such.
//if there are no possibilities, then it will output "failed"
void recursor(square **tiles, int counter, Node *openSpaces){
    int x, y, key, insert = 0;
    key = getData(openSpaces, (rand() % openSpaces->data));
    x = key / 9;
    y = key % 9;
    
    if(tiles[x][y].possibilities->data != 0)
        insert = getData(tiles[x][y].possibilities, (rand() % tiles[x][y].possibilities->data) + 1);
    else
        cout << "failed insertion\n";

    deleteNode(openSpaces, key);
    tiles[x][y].value = insert;
    adjustPossibilities(tiles, x, y);
}

//createGame is the driver that calls recursor
//currently will attempt a recursor until the board is determined to be unfillable 
//(i.e. there are no possibilities in the remaining squares)
void createGame(square **x, Node *openSpaces){
    int count = 1;
    while(openSpaces->data > 0) 
        recursor(x, count, openSpaces);
}

//debugger that allows the user to see the possibilities of a selected square
void debug(square **tiles){
    bool again;
    do{
        cout << "enter x coordinate of the tile you wanna check (0-8): ";
        int x;
        cin >> x;
        cout << "enter y coordinate of the tile you wanna check (0-8): ";
        int y;
        cin >> y;
        printList(tiles[y][x].possibilities);
        cout << "\n";
        cout << "another? (y/n): ";
        char input;
        cin >> input;
        if(input == 'y')
            again = true;
        else
            again = false;

    } while(again);
}

int main() {
    srand (time(NULL));                 //initialize randomizer
    square **tiles = new square*[9];    //initialize 2D array of squares
    for (int i = 0; i < 9; i++) 
        tiles[i] = new square[9];       
    Node *openSpaces = makeList();      //create openSpaces list
    for(int i = 0; i < 81; i++)
        addToList(openSpaces, i);       //fill openSpaces list

    initializeArray(tiles);             //fills each square with placeholder value and possibilities
    createGame(tiles, openSpaces);      //fills the board with values
    printBoard(tiles);                  //prints the board

    printList(openSpaces);              //for debugging, print out the openSpaces list
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