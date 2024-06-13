//
//  FilmGenie.h
//
//  Author: Jordan Doyle
//

#define BUFFER_SIZE 33
#define DATABASE_SIZE 45
#define NUMBER_OF_TRIES 3
#define NUMBER_OF_GUESSES 5
#define NUMBER_OF_CHAR 27
#define INPUT_SIZE 7

enum boolean {
    False,
    True,
};

enum validationMode {
    GUESS,
    EXIT,
};

enum guessType {
    CHARACTER,
    TITLE,
    INVALID,
};

/*
 * Opens the database file for reading and saves each line in an array that is
 * passed as a parameter. Returns the number of lines in the file or -1 if the
 * file cannot be opened.
 */
int readDatabaseFile(char database[DATABASE_SIZE][BUFFER_SIZE]);

/*
 * Returns a random number between 1 and upperBound. 
 */
int getRandomNumber(int upperBound);

/*
 * This function prints a string in an anonymous fashion so that the user cannot
 * see the characters. When a character has been guessed correctly that
 * character will become visible. Returns the number of invisable characters.
 */
int printAnonymousString(char string[BUFFER_SIZE], char *used_char);

/*
 * Checks if char1 and char2 which are passed as function parameters are equal
 * to each other.
 */
enum boolean compareEquals(char char1, char char2);

/*
 * Asks the user what type of guess they would like to have, either guess a
 * character or guess the film title. The method also checks that the users
 * input is valid and returns INVALID if no valid input was given.
 */
enum guessType chooseGuess(void);

/*
 * Reads input from standard user input and returns the first character entered.
 * If the user enters more than one character then everything except the first
 * character is lost.
 */
char getCharacterInput(void);
    
/*
 * Checks if the character entered by the player is a valid character i.e. 'c',
 * 'C', 'f' or 'F' when in GUESS mode or 'Y', 'y', 'n' or 'N' in EXIT mode.
 */
enum boolean invalidCharacter(char character, enum validationMode);

/*
 * This function asks the user to enter there character guess. This function is
 * case insensitive and if the user has already guessed the character they will
 * be notified and asked to guess again. Once the user has guessed and the
 * appropriate message has been displayed they will be given the option to guess
 * again.
 */
char guessCharacter(char film[BUFFER_SIZE], char *used_char);

/*
 * Checks if the user has entered a guess already entered. previousGuesses
 * should contain all the users previous guesses and guess is the users current
 * guess. 
 */
enum boolean previousGuess(char * previousGuesses, char guess);

/*
 * This function asks the user to enter the film title guess. They are given
 * five chances to guess the title and when they do an appropriate message is
 * shown.
 */
void guessFilmTitle(char film[BUFFER_SIZE]);

/*
 * Asks the player if they want to continue an action and validates the users
 * input. Returns True if the player enters 'n' to exit the action and False if
 * 'y' is entered to continue the action. True is also returned if the player
 * fails to enter a valid option.
 */
enum boolean exitAction(char action[INPUT_SIZE]);
