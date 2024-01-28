#define WORDLIST_FILENAME "words.txt"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define BUFFER_SIZE 100

void print_available_letters(const char available_letters[]);
bool is_letter_in_str(const char letter_guessed, const char secret[]);
int is_word_guessed(const char secret[], const char letters_guessed[]);
void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]);
void get_available_letters(const char letters_guessed[], char available_letters[]);
void hangman(const char secret[]);
int get_word(char secret[]);

int main() {
    srand((unsigned long)time(NULL));
    char secret[BUFFER_SIZE];
    get_word(secret);
    hangman(secret);
    return 0;
}

int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]) {
    size_t secret_len = strlen(secret);
    size_t letters_guessed_len = strlen(letters_guessed);
    int common_letters = 0;
    for (int i = 0; i < secret_len; i++) {
        for (int j = 0; j < letters_guessed_len; j++) {
            if (secret[i] == letters_guessed[j]) {
                common_letters++;
                break;
            }
        }
    }
    if (secret_len == common_letters) {
        return 1;
    }
    return 0;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {
    int secret_len = strlen(secret);
    if (strlen(letters_guessed) == 0) {
        int i;
        for (i = 0; i < secret_len; i++) {
            guessed_word[i] = '_';
        }
        guessed_word[i] = '\0';
        return;
    }
    int i;
    for (i = 0; i < secret_len; i++) {
        for (int j = 0, letters_guessed_len = strlen(letters_guessed); j < letters_guessed_len; j++) {
            if (secret[i] == letters_guessed[j]) {
                guessed_word[i] = secret[i];
                break;
            }
            else {
                guessed_word[i] = '_';
            }
        }
    }
    guessed_word[i] = '\0';
}

void get_available_letters(const char letters_guessed[], char available_letters[]) {
    int pos = 0;
    int letters_guessed_len = strlen(letters_guessed);
    bool is_in_str;
    for (int i = 0; i < 26; i++) {
        is_in_str = false;
        for (int j = 0; j < letters_guessed_len; j++) {
            if (i + 'a' == letters_guessed[j]) {
                is_in_str = true;
                break;
            }
        }
        if (!is_in_str) {
            available_letters[pos++] = i + 'a';
        }
    }
    available_letters[pos] = '\0';
}


void hangman(const char secret[]) {
    char available_letters[26];
    char letters_guessed[sizeof(available_letters)];
    size_t secret_len = strlen(secret);
    char result[BUFFER_SIZE];
    int tries = 8;
    int count = 0;
    bool bad_guess = false;

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %ld letters long.\n", secret_len);
    printf("-------------\n");
    while (tries > 0 && is_word_guessed(secret, letters_guessed) != 1) {
        printf("You have %d guesses left.\n", tries);
        printf("Available letters: ");
        get_available_letters(letters_guessed, available_letters);
        print_available_letters(available_letters);
        printf("Please guess a letter: ");
        char temp[BUFFER_SIZE];
        fgets(temp, BUFFER_SIZE, stdin);
        temp[strlen(temp) - 1] = '\0';
        size_t temp_len = strlen(temp);
        fflush(stdin);
        if (temp_len > 1) {
            if (strcmp(temp, secret) == 0) {
                strcpy(letters_guessed, temp);
            }
            else {
                bad_guess = true;
            }
            break;
        }
        else if (temp_len == 1) {
            if (isalpha(temp[0])) {
                char lower_temp = tolower(temp[0]);
                if (!is_letter_in_str(lower_temp, letters_guessed)) {
                    if (is_letter_in_str(lower_temp, secret)) {
                        printf("Good guess: ");
                    }
                    else if (!is_letter_in_str(lower_temp, secret)) {
                        printf("Oops! That letter is not in my word: ");
                        tries--;
                    }
                    letters_guessed[count++] = tolower(lower_temp);
                    letters_guessed[count] = '\0';
                }
                else {
                    printf("Oops! You've already guessed that letter: ");
                }
            }
            else {
                printf("Oops! '%c' is not a valid letter: ", temp[0]);
            }
        }
        get_guessed_word(secret, letters_guessed, result);
        for (int i = 0, result_len = strlen(result); i < result_len; i++) {
            if (i == result_len - 1) {
                printf("%c", result[i]);
                break;
            }
            printf("%c ", result[i]);
        }
        printf("\n");
        printf("-------------\n");
    }
    if (is_word_guessed(secret, letters_guessed)) {
        printf("Congratulations, you won!\n");
    }
    else if (bad_guess) {
        printf("Sorry, bad guess. The word was %s.\n", secret);
    }
    else {
        printf("Sorry, you ran out of guesses. The word was %s.\n", secret);
    }
}

void print_available_letters(const char available_letters[]) {
    int i;
    int available_letters_len = strlen(available_letters);
    for (i = 0; i < available_letters_len; i++) {
        if (available_letters[i] >= 'a' && available_letters[i] <= 'z' ) {
            printf("%c", available_letters[i]);
        }
    }
    printf("\n");
}

bool is_letter_in_str(const char letter_guessed, const char secret[]) {
    for (int i = 0, secret_len = strlen(secret); i < secret_len; i++) {
        if (letter_guessed == secret[i]) {
            return true;
        }
    }
    return false;
}
