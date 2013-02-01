#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <limits>
#include <ios>
using namespace std;

/*********************
 * gridspace section *
 *********************/
 
struct gridspace 
{      
     std::string contents; // A string containing what the space looks like
     gridspace *next_space; // Pointer to the next space on the board
     bool is_chute; // Flag for marking a chute/ladder
     int board_position; // Numeric position in the board
     gridspace *next_rung; // Pointer to next rung of a chute/ladder
     char chute_symbol; // Predefined output symbol for chutes
     char ladder_symbol; // Predefined output symbol for ladders
     void insert_player(char symbol); // Puts a player in this space
     void remove_player(char symbol); // Removes a player in this space
     gridspace();
};


/***********************
 * player_data section *
 ***********************/

class player_data {
      public:
             void do_move(int num_moves); // Moves a player num_move spaces
             gridspace *get_space(); // Returns curr
             char get_symbol(); // Return the symbol
             void set_space(gridspace *setter); // Sets players space
             void set_symbol(char setter); // Sets the players symbol
             player_data();
             player_data(gridspace *head, char choice);
             
      private:
              char symbol;
              gridspace *curr; // Pointer to the space the player exists in
};

/***********************
 * Function Prototypes *
 ***********************/

void play_game(player_data *players[], int num_players);
gridspace *create_board(int rows, int cols); // Creates the board
void delete_board(gridspace *head); // Recursive cleanup function
void print_board(gridspace *head, int rows); // Prints the board
gridspace *traverse(gridspace *start, int num_times); //Linked list traversal
void generate_ladder(gridspace *start, int rows); // Creates a ladder
void generate_chute(gridspace *start, int rows); // Creates a chute
void populate_board(gridspace *head, int rows, int cols); /* Puts chutes and
                                                             ladders on board */
void play_game(player_data *players[], int num_players, gridspace *head, 
               int row_size);                     // Main game function

int main() {      // Begin Main
  /*******************
   * Initializations *
   *******************/
  std::string symbol_choices = "!@#$%&*";
  const int num_players = 2;
  const int row_size = 8;
  const int col_size = 12;
  char choice;
  int i;
  bool stop = 0;
  player_data *players[num_players];
  gridspace *head;
  srand(time(NULL));
  
  head = create_board(row_size, col_size);
  populate_board(head, row_size, col_size);
 /* 
  for (i = 0; i < num_players; i++)
  {
     players[i] = new player_data(head, (char) i); // Generating the players
  }    
 */
 players[0] = new player_data(head, '@');
 players[1] = new player_data(head, '#');
  while (!stop)
  {
     play_game(players, num_players, head, row_size);
     cout << endl << "Y to play another game or any key to exit: ";
     cin.clear();
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
     choice = cin.get();
     switch(choice)
     {
        case 'y':
        case 'Y':
             break;
        default:
             stop = 1;
     }
  } 
  
  cout << endl << "Exiting game... " << endl;           
  for (i = 0; i < num_players; i++)
      delete players[i];
  delete_board(head);
  system("PAUSE");
  return 0;
}                 // End Main




/*************************
 * gridspace definitions *
 *************************/

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

void gridspace::insert_player(char symbol)
{
    int x;        
    
    x = contents.find(' '); // Finding first empty space in contents
    
    if (x != string::npos)
       contents[x] = symbol; // Replacing with symbol
     
}

void gridspace::remove_player(char symbol)
{
     int x;
     
     x = contents.find(symbol); // Finding the player's symbol in contents
     
     if (x != string::npos)
       contents[x] = ' '; // Replacing with a space
}

/***************************
 * player_data definitions *
 ***************************/


player_data::player_data()
{
     curr = NULL;
     symbol = '@';
}

player_data::player_data(gridspace *head, char choice) // Primary constructor
{
     curr = head;
     symbol = choice;
     curr->insert_player(symbol);
}


void player_data::do_move(int num_moves) // Movement function
{
     int i;
     
     curr->remove_player(symbol);
     
     for (i = 0; i < num_moves; i++)
     {
         if (curr == NULL)
            return;
         curr = curr->next_space;
     }
     
     if (curr == NULL)
        return;
     curr->insert_player(symbol);

}
 
gridspace *player_data::get_space()
{
     return curr;
}

char player_data::get_symbol()
{
     return symbol;
}

void player_data::set_space(gridspace *setter)
{
     curr = setter;
}

void player_data::set_symbol(char setter)
{
     symbol = setter;
}

/************************
 * function definitions *
 ************************/


gridspace *create_board(int rows, int cols)
{
     gridspace *temp, *head;
     int i;
     head = new gridspace;
  
     temp = head;
 
     for (i = 0; i < (rows * cols); i++) 
     {
        temp->board_position = i;
        temp->next_space = new gridspace;
        temp = temp->next_space;
     }
  
     temp->board_position = i;
     return head;
}    

void delete_board(gridspace *head)
{
     if (head->next_space != NULL)
        delete_board(head->next_space);
     delete head;
}


void print_board(gridspace *head, int rows)
{
     gridspace *temp;
     temp = head;
     while(temp->next_space != NULL) 
     { // Traverse until the end of the list
        cout << endl;
        
        do { // Traverses the list until it hits the end of the row
           cout << temp->contents;                  
           
           if (temp->next_space == NULL) // It should never hit this             
              break;
           
           temp = temp->next_space;
           } while((temp->board_position % rows) != 0);     
     }
}

gridspace *traverse(gridspace *start, int num_times)
{
     int i;
     
     for (i = 0; i < num_times; i++) 
     {
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
            
     for (i = 0; i <= ((rand() % 3)+ 1); i++) 
     { // creating between 2 and 5 rungs
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
     
     for (i = 0; i <= ((rand() % 3) + 1); i++) 
     { // Creating 2 to 5 chutes
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
     
     for (i = 1; (i) < (board_size - rows); i++) 
     {
         curr_space = curr_space->next_space;
         
         if(curr_space == NULL) // Shouldn't happen
            return;
         
         if (curr_space->is_chute)  // If the space we're looking at is already
            continue;              // a part of a chute or ladder
         
         roll = rand() % (rows * 5);
         // 2/5ths of the rows will have a chute or a ladder, on average
         
         if (roll == 0)
            generate_ladder(curr_space, rows);
         
         if (roll == 1)
            generate_chute(curr_space, rows);
     }
}

void play_game(player_data *players[], int num_players, gridspace *head, 
               int row_size)
{
   bool quit = 0, error = 0;
   int curr_player, roll, turn = 0;
   char input;
   gridspace *curr_space;
   
   print_board(head, row_size);
   cout << endl;
   while(!quit) // Primary input loop
  { 
     curr_player = turn % num_players;
     turn++;
     cout << endl << "Player " << curr_player + 1
          << " ('R' for roll, 'Q' to quit): ";
     
     do { // Error checking loop
         error = 0;
         cin >> input;
         switch (input)
         { // Begin input checking switch
            case 'r':
            case 'R': 
                 roll = (rand() % 6) + 1; // Roll a D6
                 break;
            case 'q':
            case 'Q':
                 cout << "Exiting..." << endl;
                 quit = 1;
                 break;
            default:
                 error = 1;
                 cout << "Bad input. Try again." << endl;
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 // Clears input and ignores everything on the line
         } // End switch
        } while (error);
     
     if (quit)
        continue;
        
     cout << endl << "Your roll is: " << roll << endl;
     players[curr_player]->do_move(roll);
     print_board(head, row_size);
     cout << endl;
     
     // Checking if on a chute or ladder
     curr_space = players[curr_player]->get_space();
     if (curr_space->is_chute)
     {
        if (curr_space->next_rung == NULL)
           break;
        
        cout << endl << "You've landed on a special space!" << endl;
        system("PAUSE"); 
        curr_space->remove_player(players[curr_player]->get_symbol());
        while (curr_space->next_rung != NULL)
        {
              players[curr_player]->set_space(curr_space->next_rung);
              curr_space = curr_space->next_rung;
        }
        curr_space->insert_player(players[curr_player]->get_symbol());
        print_board(head, row_size);
        cout << endl;
     }
     
     //Checking for game over
     if (curr_space->next_space == NULL)
     {
        cout << endl << "Congratulations, Player " << curr_player
             << " has won the game!" << endl << "The game lasted "
             << turn << " turns.";
        quit = 1;
     }
  
  }
}
         
