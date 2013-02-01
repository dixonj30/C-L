#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <limits>
#include <ios>
using namespace std;

/********************
 * gridspace section*
 ********************/
 
struct gridspace 
{      
     std::string contents;
     gridspace *next_space; // Pointer to the next space on the board
     bool is_chute;
     int board_position; // Numeric position in the board
     gridspace *next_rung; // Pointer to next rung of a chute/ladder
     char chute_symbol;
     char ladder_symbol;
     gridspace();
};


/**********************
 * player_data section*
 **********************/

class player_data {
      public:
             void do_move();
             gridspace *get_space();
             player_data();
             player_data(gridspace *head, char choice);
             
      private:
              char symbol;
              gridspace *curr; // Pointer to the space the player exists in
};

/**********************
 * Function Prototypes*
 **********************/

void insert_player(gridspace *space, char symbol);
gridspace *create_board(int rows, int cols);
void delete_board(gridspace *head); // Recursive cleanup function
void print_board(gridspace *head, int rows);
gridspace *traverse(gridspace *start, int num);
void generate_ladder(gridspace *start, int rows);
void populate_board(gridspace *head, int rows, int cols);



int main() {      // Begin Main
  const int row_size = 8;
  const int col_size = 12;
  gridspace *head;
  srand(time(NULL));
  head = create_board(row_size, col_size);
  player_data p1(head, '@'), p2(head, '&');
  print_board(head, row_size);
  system("PAUSE");
  populate_board(head, row_size, col_size);
  print_board(head, row_size);
  delete_board(head);
  system("PAUSE");
  return 0;
}                 // End Main




/************************
 * gridspace definitions*
 ************************/

gridspace::gridspace() // Default constructor
{
     next_space = NULL;
     is_chute = 0;
     chute_symbol = '^';
     ladder_symbol = 'v';
     contents = "[   ]";
     board_position = 0;
     next_rung = NULL;
}

/**************************
 * player_data definitions*
 **************************/


player_data::player_data()
{
     curr = NULL;
     symbol = '@';
}

player_data::player_data(gridspace *head, char choice) // Primary constructor
{
     curr = head;
     symbol = choice;
     insert_player(curr, symbol);
}

void player_data::do_move() // Movement function
{
     int x;
     x = curr->contents.find(symbol); // Finding the player's symbol in contents
     if (x != string::npos)
       curr->contents[x] = ' '; // Replacing with a space
 
     curr = curr->next_space; // Moving player to the next space
     
     insert_player(curr, symbol);
}
 
gridspace *player_data::get_space()
{
     return curr;
}



/***********************
 * function definitions*
 ***********************/

void insert_player(gridspace *space, char symbol)
{
    int x;
    
    if (space == NULL)
       return;
    
    x = space->contents.find(' '); // Finding first empty space in contents
    
    if (x != string::npos)
       space->contents[x] = symbol; // Replacing with symbol
     
}

gridspace *create_board(int rows, int cols)
{
     gridspace *temp, *head;
     int i;
     head = new gridspace;
  
     temp = head;
 
     for (i = 0; i < (rows * cols); i++) {
        temp->board_position = i;
        temp->next_space = new gridspace;
        temp = temp->next_space;
     }
  
     temp->board_position = i;
  
     return head;
}    

void delete_board(gridspace *head) // Recursive cleanup function
{
     if (head->next_space != NULL)
        delete_board(head->next_space);
     delete head;
}


void print_board(gridspace *head, int rows)
{
     gridspace *temp;
     temp = head;
     while(temp->next_space != NULL) { // Traverse until the end of the list
        cout << endl;
        do { // Traverses the list until it hits the end of the row
           cout << temp->contents;                  
           
           if (temp->next_space == NULL) // It should never hit this             
              break;
           
           temp = temp->next_space;
           } while((temp->board_position % rows) != 0);     
     }
}

gridspace *traverse(gridspace *start, int rows)
{
     int i;
     for (i = 0; i < rows; i++) {
         if (start->next_space == NULL)
            return NULL;
         start = start->next_space;
     }
     return start;
}

void generate_ladder(gridspace *start, int rows)
{
     int i, middle;
     gridspace *temp;
     temp = start;   
     middle = (temp->contents.length()) / 2;
            
     for (i = 0; i <= ((rand() % 3)+ 1); i++) { // creating between 2 and 5 rungs
         temp->contents[middle] = temp->ladder_symbol;
         temp->is_chute = 1;
         temp->next_rung = traverse(temp, rows); // Pointing next_rung to the 
                                                 // space above temp
         if (temp->next_rung == NULL)
            break;
         temp = temp->next_rung;
     }
      
      temp->contents[middle] = ' '; /* Just like for Chutes, removing the bottom 
                                       symbol to make output more intutive */
}

void generate_chute(gridspace *start, int rows)
{
     int i, middle;
     gridspace *temp, *holder;
     temp = start;
     holder = NULL;
     middle = (temp->contents.length()) / 2;
     
     for (i = 0; i <= ((rand() % 3) + 1); i++) { // Creating 2 to 5 chutes
         temp->is_chute = 1;
         holder = traverse(temp, rows);
         if (holder == NULL)
            break;
         holder->next_rung = temp;
         temp = holder;
         temp->contents[middle] = temp->chute_symbol;
     }
     /* Note: The top of a chute will still be considered a chute, but will not
              have the chute symbol. This is for more intuitive output */
}                               

void populate_board(gridspace *head, int rows, int cols)
{
     int i, roll, num;
     int board_size = (rows * cols);
     gridspace *curr_space;
     curr_space = head;
     for (i = 1; (i) < (board_size - rows); i++) {
         curr_space = curr_space->next_space;
         
         if(curr_space == NULL) // Shouldn't happen
            return;
         
         if (curr_space->is_chute)  // If the space we're looking at is already
            continue;              // a part of a chute or ladder
         
         roll = rand() % (rows * 5);
         
         
         if (roll == 0)
            generate_ladder(curr_space, rows);
         if (roll == 1)
            generate_chute(curr_space, rows);
     }
}
                
         
