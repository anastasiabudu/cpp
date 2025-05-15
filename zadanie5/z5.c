#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "monopoly.h" // Nemenit !!!
// Nevkladat monopoly.c !!!

#define MAX_PROPERTIES 100
#define MAX_STRLEN 124

void readProperties(FILE* file, PROPERTY** properties, int* size);
int readBoard(FILE* file, PROPERTY* properties, SPACE** board, int max_size);
void printProperties(PROPERTY* properties, int num_properties);
void printBoard(SPACE* board, int num_properties);
FILE* openFile(char *filename);

COLOR parseColor(char *colorStr);
SPACE_TYPE parseSpaceType(char *typeStr);

char* rtrim(char* str);
int is_space(char c);

int main(int argc, char *argv[]) {
    int num_properties = NUM_PROPERTIES;
    int output_type = 0;
    int board_size = NUM_SPACES;
    PROPERTY* final_properties = properties;
    SPACE* final_board = game_board;

    char* board_file = NULL;
    char* properties_file = NULL;
    FILE* board_stream = NULL;
    FILE* properties_stream = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            properties_file = argv[i + 1];
            properties_stream = openFile(properties_file);
            i++;
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            board_file = argv[i + 1];
            board_stream = openFile(board_file);
            i++;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            exit(1);
        }
    }

    if (properties_file != NULL) {
        readProperties(properties_stream, &final_properties, &num_properties);
        output_type = 1;
    }
    if (board_file != NULL) {
        board_size = readBoard(board_stream, final_properties, &final_board, num_properties);
        output_type = 2;
    }

    switch (output_type) {
        case 1:
            printProperties(final_properties, num_properties);
            break;
        case 2:
            printBoard(final_board, board_size);
            break;
        default:
            break;
    }

    if (properties_file != NULL) {
        free(final_properties);
        fclose(properties_stream);
    }
    if (board_file != NULL) {
        free(final_board);
        fclose(board_stream);
    }

    return 0;
}

void readProperties(FILE* file, PROPERTY** properties, int* size) {
    *properties = malloc(sizeof(PROPERTY) * MAX_PROPERTIES);
    COLOR current_color;
    char line[MAX_STRLEN + 1];
    *size = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[') {
            rtrim(line);
            int len = strlen(line);
            line[len - 1] = '\0';
            current_color = parseColor(line + 1);
            continue;
        }

        char* sep = strchr(line, ':');
        *(sep - 1) = '\0';

        (*properties)[*size].color = current_color;
        strncpy((*properties)[*size].name, line+1, MAX_NAME);
        (*properties)[*size].price = atoi(sep + 1);

        (*size)++;
    }
}

int readBoard(FILE* file, PROPERTY* properties, SPACE** board, int max_props) {
    char line[MAX_STRLEN + 1];
    int size = atoi(fgets(line, sizeof(line), file));
    int prop_number = 0;
    *board = malloc(sizeof(SPACE) * size);

    for (int i = 0; i < size; i++) {
        fgets(line, sizeof(line), file);
        rtrim(line);
        (*board)[i].type = parseSpaceType(line);
        (*board)[i].property = (*board)[i].type == Property ? &properties[prop_number++] : NULL;

        if (prop_number > max_props) {
            fprintf(stdout, "E2");
            exit(1);
        }
    }

    return size;
}

void printProperties(PROPERTY* properties, int num_properties) {
    int max_name_length = 0;
    int max_price_length = 0;
    for (int i = 0; i < num_properties; i++) {
        int name_length = strlen(properties[i].name);
        if (name_length > max_name_length) {
            max_name_length = name_length;
        }
        char price_str[20];
        sprintf(price_str, "%d", properties[i].price);
        int price_length = strlen(price_str);
        if (price_length > max_price_length) {
            max_price_length = price_length;
        }
    }
    for (int i = 0; i < num_properties; i++) {
        printf("%-*s %-*d %s\n", max_name_length, properties[i].name, max_price_length, properties[i].price, property_colors[properties[i].color]);
    }
}


void printBoard(SPACE* board, int num_properties) {
    int max_index_length = snprintf(NULL, 0, "%d.", num_properties);
    int max_name_length = 0;
    int max_price_length = 0;
    for (int i = 0; i < num_properties; i++) {
        if (board[i].type == Property) {
            int name_length = strlen(board[i].property->name);
            if (name_length > max_name_length) {
                max_name_length = name_length;
            }
            int price_length = snprintf(NULL, 0, "%d", board[i].property->price);
            if (price_length > max_price_length) {
                max_price_length = price_length;
            }
        }
    }

    for (int i = 0; i < num_properties; i++) {
        char index[20];
        snprintf(index, sizeof(index), "%d.", i + 1);
        if (board[i].type == Property) {
            printf("%-*s %-*s %-*d %s\n", max_index_length, index, max_name_length, board[i].property->name, max_price_length, board[i].property->price, property_colors[board[i].property->color]);
            continue;
        }

        printf("%-*s %s\n", max_index_length, index, space_types[board[i].type]);
    }
}




FILE* openFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stdout, "E1");
        exit(1);
    }
    return file;
}

COLOR parseColor(char *colorStr) {
    if (strcmp(colorStr, property_colors[0]) == 0) {
        return Brown;
    } else if (strcmp(colorStr, property_colors[1]) == 0) {
        return Skyblue;
    } else if (strcmp(colorStr, property_colors[2]) == 0) {
        return Purple;
    } else if (strcmp(colorStr, property_colors[3]) == 0) {
        return Orange;
    } else if (strcmp(colorStr, property_colors[4]) == 0) {
        return Red;
    } else if (strcmp(colorStr, property_colors[5]) == 0) {
        return Yellow;
    } else if (strcmp(colorStr, property_colors[6]) == 0) {
        return Green;
    } else if (strcmp(colorStr, property_colors[7]) == 0) {
        return Blue;
    } else {
        fprintf(stderr, "Unknown color: %s\n", colorStr);
        exit(1);
    }
}

SPACE_TYPE parseSpaceType(char *typeStr) {
    if (strcmp(typeStr, space_types[0]) == 0) {
        return Property;
    } else if (strcmp(typeStr, space_types[1]) == 0) {
        return Start;
    } else if (strcmp(typeStr, space_types[2]) == 0) {
        return Free_parking;
    } else if (strcmp(typeStr, space_types[3]) == 0) {
        return In_jail;
    } else if (strcmp(typeStr, space_types[4]) == 0) {
        return Go_to_jail;
    } else if (strcmp(typeStr, space_types[5]) == 0) {
        return Jail_pass;
    } else {
        fprintf(stderr, "Unknown space type: %s\n", typeStr);
        exit(1);
    }
}

char* rtrim(char* str) {
    int len = strlen(str);
    while (len > 0 && is_space(str[len - 1])) {
        len--;
    }
    str[len] = '\0';
    return str;
}

int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

