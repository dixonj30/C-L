#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>
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



int main() {
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
}




/************************
 * gridspace definitions*
 ************************/

gridspace::gridspace() // Default constructor
{
     next_space = NULL;
     is_chute = 0;
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
     while(temp->next_space != NULL) {
        cout << endl;
        do { // Traverses the list until it hits the end of the row
           cout << temp->contents;                  
           
           if (temp->next_space == NULL) // It should never hit this             
              break;
           
           temp = temp->next_space;
           } while((temp->board_position % rows) != 0);     
     }
}

gridspace *traverse(gridspace *start, int num)
{
     int i;
     for (i = 0; i < num; i++) {
         if (start->next_space == NULL)
            return start;
         start = start->next_space;
     }
     return start;
}

void generate_ladder(gridspace *start, int rows)
{
     int i, middle;
     gridspace *temp;
     temp = start;   
     
     temp->is_chute = 1;
     middle = (temp->contents.length()) / 2;
     temp->contents[middle] = 'v';
            
     for (i = 0; i <= ((rand() % 3)+ 1); i++) { // creating between 3 and 5 rungs
         temp->next_rung = traverse(temp, rows);
         // Pointing next_rung to the space above temp
         temp = temp->next_rung;
         
         if (temp == NULL) // shouldn't happen
            return;
         
         temp->contents[middle] = 'v';
         temp->is_chute = 1;
      }
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
         
         roll = rand() % (rows * 3);
         
         
         if (roll == 0)
            generate_ladder(curr_space, rows);
         
     }
}
                
         
