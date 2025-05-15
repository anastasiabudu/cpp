#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1001
#define MAX_WORD 20
#define MAX_ARGS 10



int main(int argc, char* argv[]) {

    char** lines = (char**)malloc(MAX_LINE * sizeof(char*));
    char** copy_lines = (char**)malloc(MAX_LINE * sizeof(char*));
    if (lines == NULL || copy_lines == NULL) {
        printf("Memory allocation error\n");
        return 1;
    }
    char line[MAX_LINE];
    int line_count = 0;
    int max_word_length = 0;
    int read = 0;
    int remove_punctuation = 0;
    int remove_digits = 0;
    int register_mode_on = -1;    // 0 - R, 1 - r
    int output_mode_on = 0;
    int parameters_counter = 0;     // amount of parameters (-r, -R, -d, -p)

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && (strcmp("-p", argv[i]) != 0 &&
                                  strcmp("-s", argv[i]) != 0 && strcmp("-R", argv[i]) != 0 &&
                                  strcmp("-r", argv[i]) != 0 && strcmp("-d", argv[i]) != 0))
        {
            printf("E1\n");
            return 0;
        }

        if (strcmp(argv[i], "-p") == 0) {
            remove_punctuation = 1;
            ++parameters_counter;
        }
        else if (strcmp(argv[i], "-d") == 0) {
            remove_digits = 1;
            ++parameters_counter;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            register_mode_on = 1;
            ++parameters_counter;
        }
        else if (strcmp(argv[i], "-R") == 0)
        {
            register_mode_on = 0;
            ++parameters_counter;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            output_mode_on = 1;
            ++parameters_counter;
        }
    }

    if (register_mode_on != -1 && argc <= parameters_counter + 1)   // if -r | -R was included, replace string shoud be exist
    {
        printf("E2\n");
        return 0;
    }

    char replace_string[MAX_WORD] = { 0 };  // array for replace string
    if (register_mode_on == 0 || register_mode_on == 1)
    {
        strcpy(replace_string, argv[parameters_counter + 1]);
    }

    char setN[MAX_ARGS][MAX_WORD];  // array for set N
    parameters_counter += 2;
    int index = 0;  // index for word array
    while ((register_mode_on == 0 || register_mode_on == 1) && argc > parameters_counter)   // copy into array setN each prefix
    {
        strcpy(setN[index++], argv[parameters_counter++]);
    }
    int setSize = index;    // save size of Set of prefix



    if ((register_mode_on == 0 || register_mode_on == 1) && (strlen(replace_string) <= 0 || strlen(replace_string) > 20)) {
        printf("E3\n");
        return 0;
    }


    while (fgets(line, MAX_LINE, stdin) != NULL) {
        if (line[0] == '\n') {
            read = 1;
            break;
        }

        lines[line_count] = strdup(line);
        copy_lines[line_count] = strdup(line);
        if (lines[line_count] == NULL || copy_lines[line_count] == NULL) {
            printf("Memory allocation error\n");
            return 1;
        }

        int j = 0;
        for (int i = 0; line[i] != '\0'; ++i) {
            if (remove_digits && isdigit(lines[line_count][i]))
            {
                continue;
            }
            else if (remove_punctuation && ispunct(lines[line_count][i]))
            {
                continue;
            }
            line[j] = line[i];
            lines[line_count][j] = lines[line_count][i];
            copy_lines[line_count][j++] = lines[line_count][i];
        }
        line[j] = '\0';
        lines[line_count][j] = '\0';
        copy_lines[line_count][j] = '\0';

        if (output_mode_on)
        {
            char* token = strtok(line, " .,\t\n0123456789_=?!+*-;");
            while (token != NULL) {
                if (isalpha(token[0]) &&
                    strlen(token) == strspn(token, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
                    int current_word_length = strlen(token);
                    if (current_word_length > max_word_length) {
                        max_word_length = current_word_length;
                    }
                }
                token = strtok(NULL, " .,\t\n0123456789_=?!+*-;");
            }
        }

        line_count++;
    }

    if (read) {
        for (int l = 0; l < line_count; ++l) {
            char input[MAX_LINE] = { 0 };
            strcpy(input, lines[l]);

            if (register_mode_on != -1) // if -r | -R was included
            {
                char word[MAX_WORD] = { 0 };    // array for word

                if (register_mode_on == 0 &&
                    setSize != 0)  // if -R and set is not empty, convert each letter in Set into low register
                {
                    for (int i = 0; i < setSize; ++i) {
                        for (int j = 0; setN[i][j] != '\0'; ++j) {
                            setN[i][j] = tolower(setN[i][j]);
                        }
                    }
                }
                index = 0;
                int on_word = 0;    // flag, if we found word, it will save 1, else 0
                int start_word = 0; // word start index
                int end_word = 0;   // word end index
                for (int i = 0; input[i] != '\0'; ++i) {
                    if (isalpha(input[i]) &&
                        !on_word) // if we found alpha letter and were was searching word (on_word == 0), then save start word index
                    {
                        on_word = 1;
                        start_word = i;
                    }
                    else if ((ispunct(input[i]) || isspace(input[i]) || isdigit(input[i])) &&
                             on_word) // if we found non alpha letter and we were on word, then save end index and start to check prefix
                    {
                        on_word = 0;
                        end_word = i;
                        word[index] = '\0';
                        index = 0;

                        char* substr = NULL; // var to check prefix is exist in word
                        int is_prefix = 0;  // flag, 0 - prefix is not exist, else - 1

                        if (setSize != 0)   // if we had prefixes in set
                        {
                            if (register_mode_on == 0)// if -R, convert each register letter into small
                            {
                                for (int i = 0; word[i] != '\0'; ++i) {
                                    word[i] = tolower(word[i]);
                                }
                            }
                            char* substr = NULL;
                            for (int j = 0; j < setSize; ++j) {
                                if (strncmp(word, setN[j], strlen(setN[j])) == 0) // Сравниваем начало слова с префиксом
                                {
                                    is_prefix = 1;
                                    break;
                                }
                            }
                        }
                        else {
                            is_prefix = 1;  // if we dont have prefixes, also turn flag into 1
                        }

                        if (is_prefix)// if we should replace this word by replace string
                        {
                            for (int j = start_word, k = 0; j < end_word && replace_string[k] !=
                                                                            '\0'; ++j, ++k) // go through word use our indexes, and replace it by replace string, while we have letters in replace string an in word
                            {
                                input[j] = replace_string[k];
                            }
                        }
                    }
                    if (on_word)    // if we found word, fill word array
                    {
                        word[index++] = input[i];
                    }
                }
            }
            strcpy(lines[l], input);
        }

        if (output_mode_on)
        {
            int count_lexemes = 0;
            int count_old_lexemes = 0;
            char** lexemes = (char**)malloc(MAX_LINE * sizeof(char*));
            char** old_lexemes = (char**)malloc(MAX_LINE * sizeof(char*));
            int* line_numbers = (int*)calloc(MAX_LINE, sizeof(int));
            char** word_replace_string = (char**)malloc(MAX_LINE * sizeof(char*));
            int words_count = 0;

            char* word_token = strtok(replace_string, " ");
            while (word_token)
            {
                word_replace_string[words_count++] = strdup(word_token);
                word_token = strtok(NULL, " ");
            }

            for (int i = 0; i < line_count; i++) {
                int current_line_number = i + 1;
                char* token = strtok(lines[i], " .,\t\n0123456789_=?!+*-;");
                while (token != NULL) {
                    if (isalpha(token[0]) &&
                        strlen(token) == strspn(token, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
                        lexemes[count_lexemes] = strdup(token);
                        line_numbers[count_lexemes++] = current_line_number;
                    }
                    token = strtok(NULL, " .,\t\n0123456789_=?!+*-;");
                }
            }
            for (int i = 0; i < line_count; i++) {
                char* token = strtok(copy_lines[i], " .,\t\n0123456789_=?!+*-;");
                while (token != NULL) {
                    if (isalpha(token[0]) &&
                        strlen(token) == strspn(token, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
                        old_lexemes[count_old_lexemes++] = strdup(token);
                    }
                    token = strtok(NULL, " .,\t\n0123456789_=?!+*-;");
                }
            }

            int k = 0;
            for (int i = 0; i < count_lexemes && k < count_old_lexemes; i++) {
                if (words_count == 1)
                {
                    printf("%d. %-*s: %s\n", line_numbers[i], max_word_length, old_lexemes[k++], lexemes[i]);
                }
                else
                {
                    int ctr = 0;
                    int s = strlen(old_lexemes[k]);
                    int s_l = 0;
                    for (int j = 0; j < words_count && i + ctr < count_lexemes; ++j)
                    {
                        s_l += strlen(lexemes[i + ctr]);
                        if ((strcmp(word_replace_string[j], lexemes[i + ctr]) == 0 || word_replace_string[j][0] == lexemes[i + ctr][0]) &&
                            s >= s_l)
                        {
                            ++ctr;
                        }
                        else
                        {
                            break;
                        }
                        ++s_l;
                    }

                    printf("%d. %-*s:", line_numbers[i], max_word_length, old_lexemes[k++]);
                    if (ctr == 0)
                    {
                        printf("%s", lexemes[i]);
                    }
                    for (int j = 0; j < ctr && i + j < count_lexemes; ++j)
                    {
                        if (j == 0)
                        {
                            printf("%s", lexemes[i + j]);
                        }
                        else
                        {
                            printf(" %s", lexemes[i + j]);
                        }
                    }
                    printf("\n");
                    if (ctr != 0)
                    {
                        i += ctr - 1;
                    }
                }
            }

            free(line_numbers);
            for (int i = 0; i < count_lexemes; i++) {
                free(lexemes[i]);
            }
            for (int i = 0; i < count_old_lexemes; ++i)
            {
                free(old_lexemes[i]);
            }

            for (int i = 0; i < words_count; ++i)
            {
                free(word_replace_string[i]);
            }

            free(word_replace_string);
            free(old_lexemes);
            free(lexemes);
        }
        else
        {
            for (int i = 0; i < line_count; i++) {
                if (i == line_count - 1)
                {
                    lines[i][strlen(lines[i]) - 1] = '\0';
                }
                if (strlen(lines[i]) != 0)
                {
                    printf("%s", lines[i]);
                }
            }
        }

        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
            free(copy_lines[i]);
        }
        free(lines);
        free(copy_lines);

        return 0;
    }
}