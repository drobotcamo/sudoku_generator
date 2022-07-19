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

//add to end will create a new node that is added to the end of the 
void addToEnd(Node *head, int data){
    Node* new_node = new Node();
    new_node->data = data;
    new_node->next = NULL;

    Node *cursor = head;
    while(cursor->next != NULL){
        cursor = cursor->next;
    }
    cursor->next = new_node;
    head->data++;
}

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

int getData(Node *head, int index){
    //if(index < head->data)
    //{
        Node *cursor = head;
        if(cursor->next == NULL)
            return -1;
        for(int i = 0; i < index; i++){
            cursor = cursor->next;
        }
        if(cursor != NULL)
            return cursor->data;
        else
            return -1;
    //}
    //else
    //    return -1;
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
           x[i][j].value = 0;          //initizlizes each square with an x, placeholder
           x[i][j].possibilities = makeList(); //gives each square a possibilities list
           for(int k = 0; k < 9; k++)
           {
                //x[i][j].possibilities[k] = k + 1;        //initializes each square to have all possibilities 1-9
                addToEnd(x[i][j].possibilities, k + 1);
                x[i][j].id = ((i / 3) + 1) + ((j / 3) * 3);  //assigns each square a box ID from 1-9 from left to right
           }
        }
    }
}


//given coordinates of a tile, this will eliminate the possibility that any tile in the same column row or box will share its value
void adjustPossibilities(square **tiles, int x, int y){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if((i == x | j == y) | (tiles[x][y].id == tiles[i][j].id))
                deleteNode(tiles[i][j].possibilities, tiles[x][y].value);
                //tiles[i][j].possibilities[tiles[x][y].value - 1] = 0;
        }
    }
}

bool isPresent(Node *head, int value){
    Node *cursor = head->next;
    while(cursor->next != NULL){
        if(cursor->data == value)
            return true;
        cursor = cursor->next;
    }
    return false;
}

void recursor(square **tiles, int counter, Node *openSpaces){
    int x, y, key, insert = 0;

    key = getData(openSpaces, (rand() % openSpaces->data));
    x = key / 9;
    y = key % 9;
    if(tiles[x][y].possibilities->data != 0){
        insert = getData(tiles[x][y].possibilities, (rand() % tiles[x][y].possibilities->data) + 1);
        deleteNode(openSpaces, key);
    }
    else{
        cout << "failed completion\n";
    }
    
    tiles[x][y].value = insert;
    
    adjustPossibilities(tiles, x, y);
}

void createGame(square **x, Node *openSpaces){
    int count = 1;
    for(int i = 0; i < 81; i++)
    {
        cout << "success #" << count << "\n";
        recursor(x, count, openSpaces);
        count++;
    }
}

void debug(square **tiles){
    bool again;
    do{
        cout << "enter x coordinate of the tile you wanna check: ";
        int x;
        cin >> x;
        x--;
        cout << "enter y coordinate of the tile you wanna check: ";
        int y;
        cin >> y;
        y--;
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

void buildList(Node *openSpaces){
    for(int i = 0; i < 81; i++)
    {
        addToEnd(openSpaces, i);
    }
}

int main() {
    srand (time(NULL));    //initialize randomizer

    square **tiles = new square*[9];
    for (int i = 0; i < 9; i++) {
        tiles[i] = new square[9];
    }
    Node *openSpaces = makeList();
    buildList(openSpaces);

    initializeArray(tiles);
    createGame(tiles, openSpaces);
    printBoard(tiles);

    printList(openSpaces);

    //debug(tiles);
    
    for(int i=0;i<9;i++)    //To delete the inner arrays
      delete [] tiles[i];   
      delete [] tiles;   
    return 0;
}
/*
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