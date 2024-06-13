//
//  Film Genie.c
//
//  Author: Jordan Doyle
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "FilmGenie.h"

int readDatabaseFile(char database[DATABASE_SIZE][BUFFER_SIZE]);
int getRandomNumber(int upperBound);
int printAnonymousString(char film[BUFFER_SIZE], char *used_char);
enum boolean compareEquals(char char1, char char2);
enum guessType chooseGuess(void);
char getCharacterInput(void);
enum boolean invalidCharacter(char character, enum validationMode); 
char guessCharacter(char film[BUFFER_SIZE], char *used_char);
enum boolean previousGuess(char * previousGuesses, char guess);
void guessFilmTitle(char film[BUFFER_SIZE]);
enum boolean exitAction(char action[INPUT_SIZE]);


int main(void) {
    char filmDatabase[DATABASE_SIZE][BUFFER_SIZE];
    int filmCount = readDatabaseFile(filmDatabase);
    if (filmCount == -1) {
        printf("Error reading database file. Terminating program!\n");
        return 0;
    }

    srand((int) time(0));
    
	printf("====================Welcome to Film Genie=====================\n");
    
	while(True) {
        char randomFilm[BUFFER_SIZE];
        strcpy(randomFilm, filmDatabase[getRandomNumber(filmCount)]);
		
        char guessedCharacters[NUMBER_OF_CHAR];
        printf("Your film title to guess is: ");
		printAnonymousString(randomFilm, guessedCharacters);

        enum guessType choice = chooseGuess();
        if(choice == CHARACTER)
            guessCharacter(randomFilm, guessedCharacters);
        else if(choice == TITLE)
            guessFilmTitle(randomFilm);
        else if(choice == INVALID)
            break; 
        
        if(exitAction("play")) break;
	}
    
    printf("Game Over!!! Thank you for playing.\n");
    return 0;
}

int readDatabaseFile(char database[DATABASE_SIZE][BUFFER_SIZE]) {
    FILE *filePointer = fopen("database.txt", "r");
    if(filePointer == NULL) {
        printf("The file 'database.txt' could not be opened for reading. %s\n", strerror(errno));
        return -1;
    }
    
    int lineCount = 0;
    while (fgets(database[lineCount], BUFFER_SIZE, filePointer)) {
        // Remove the trailing newline character (\n).
        database[lineCount][strlen(database[lineCount]) - 1] = '\0';
        lineCount++;
    }
    
    fclose(filePointer);
    return lineCount;
}

int getRandomNumber(int upperBound) {
    return (rand() % upperBound) + 1;
}

int printAnonymousString(char string[BUFFER_SIZE], char visibleCharacters[NUMBER_OF_CHAR]) {
    
    int numberNotFound = 0;
	for(int i = 0; i < strlen(string); i++) {
        if(string[i] == ' ')
            printf(" ");
        else {
            enum boolean notFound = True;
            for(int j = 0; j < strlen(visibleCharacters); j++) {
                if(compareEquals(string[i], visibleCharacters[j])) {
                    printf("%c", string[i]);
                    notFound = False;
                    break;
                }
            }

            if(notFound) {
                printf("*");
                numberNotFound++;
            }
        }
	}
    printf("\n");
    
    return numberNotFound;
}

enum boolean compareEquals(char char1, char char2) {
    if(char1 >= 'A' && char1 <= 'Z') {
        if(char1 == char2 || char1 == (char2 + 32)) return True;
    } else if(char1 >= 'a' && char1 <= 'z') {
        if(char1 == char2 || char1 == (char2 - 32)) return True;
    }
    return False;
}

enum guessType chooseGuess(void) {
    int numberOfFails;
    char guessMode;
    
    for(numberOfFails = 0; numberOfFails < NUMBER_OF_TRIES; numberOfFails++) {
        printf("Would you like to guess a character (enter c) OR guess the film"
               " title (enter f): ");
        guessMode = getCharacterInput();
        
        if(invalidCharacter(guessMode, GUESS))
            printf("Input not recognised, plese enter a 'c' for character guess"
                   " or 'f' for title guess!\n");
        else {
            if(guessMode == 'c' || guessMode == 'C')
                return CHARACTER;
            else if(guessMode == 'f' || guessMode == 'F')
                return TITLE;
            
            break;
        }
    }
    
    if(numberOfFails == NUMBER_OF_TRIES) {
        printf("Input failed 3 times, please read instructions carefully and "
               "try again later!\n");
    }
    
    return INVALID;
}

char getCharacterInput(void) {
    char input[INPUT_SIZE];
    
    fgets(input, INPUT_SIZE, stdin);
    input[1] = '\0';
    
    return input[0];
}
    
enum boolean invalidCharacter(char character, enum validationMode mode) {
    if (mode == GUESS) {
        if(compareEquals(character, 'c') || compareEquals(character, 'f'))
            return False;
    } else if (mode == EXIT) {
        if(compareEquals(character, 'y') || compareEquals(character, 'n'))
            return False;
    }
    return True;
}

char guessCharacter(char filmTitle[BUFFER_SIZE], char *previousGuesses) {
    int charactersRemaining = (int) strlen(filmTitle);
    
    while (True) {
		printf("Please enter your character guess: ");
        char guess = getCharacterInput();

        if(previousGuess(previousGuesses, guess)) {
            printf("You've already guessed that character. Try again \n");
            continue;
        }
		
        previousGuesses[strlen(previousGuesses)] = guess;
        printf("Previous Guesses: %s \n", previousGuesses);
        
        enum boolean failed = True;
        for(int i = 0; i < strlen(filmTitle); i++) {
            if(compareEquals(guess, filmTitle[i])) {
                printf("Well done, Your character does exist in the title.\n");
                charactersRemaining = printAnonymousString(filmTitle,
                                      previousGuesses);
                failed = False;
                break;
            }
        }
        
        if(failed)
            printf("Hard luck, Your character does not exists in the title.\n");
        
        printf("Characters Remaining: %d \n", charactersRemaining);
        if (charactersRemaining == 0) {
            printf("Congradulations you guessed all the letters. You Won!!\n");
            break;
        }
            
        if(exitAction("guess")) break;
	}
    
	return *previousGuesses;
}

enum boolean previousGuess(char * previousGuesses, char guess) {
    for(int i = 0; i < strlen(previousGuesses); i++) {
        if(compareEquals(previousGuesses[i], guess)) {
            return True;
        }
    }
    return False;
}

void guessFilmTitle(char filmTitle[BUFFER_SIZE]) {
    int remaining;
	for(remaining = NUMBER_OF_GUESSES; remaining > 0; remaining--) {
		printf("You have %d tries left\n", remaining);
        char guess[BUFFER_SIZE];
		printf("Please enter your film title guess: ");
		fgets(guess, BUFFER_SIZE, stdin);
        guess[strlen(guess) - 1] = '\0';

		if (strcmp(filmTitle, guess) == 0) {
			switch(remaining) {
				case 5:
					printf("Wow!! You got it on your 1st guess, Well done\n");
					break;
				case 4:
					printf("Well done, you got it on your 2nd guess.\n");
					break;
				case 3:
					printf("Well done, you got it on your 3rd guess.\n");
					break;
				case 2:
					printf("Well done, you got it on your 4th guess.\n");
					break;
				case 1:
					printf("Phew! You got it on your last guess, Well done.\n");
					break;
				default:
					break;
			}
			break;
		} else printf("Sorry, you guessed wrong.\n");
	}

	if(remaining == 0) printf("Hard Luck, it was '%s', Maybe next time!!!\n",
                              filmTitle);
}

enum boolean exitAction(char action[INPUT_SIZE]) {
    int numberOfFails;
    char exitCharacter;
    
    for(numberOfFails = 0; numberOfFails < NUMBER_OF_TRIES; numberOfFails++) {
        printf("Would you like to %s again (y/n): ", action);
        exitCharacter = getCharacterInput();
        
        if(invalidCharacter(exitCharacter, EXIT))
            printf("Input not recognised, plese enter a 'Y' for Yes or 'N' for "
                   "No! \n");
        else {
            if(exitCharacter == 'y' || exitCharacter == 'Y')
                break;
            else if (exitCharacter == 'n' || exitCharacter == 'N')
                return True;
        }
    }
    
    if(numberOfFails == NUMBER_OF_TRIES) {
        printf("Input failed 3 times, please read instructions carefully and "
               "try again later!\n");
        return True;
    }
    
    return False;
}
