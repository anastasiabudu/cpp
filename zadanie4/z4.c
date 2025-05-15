
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>

#include "monopoly.h"

#define INVALID_INPUT	-1
#define BANKRUT			-2

PLAYER* createPlayersArr(int size)
{
    PLAYER* players = (PLAYER*)calloc(size, sizeof(PLAYER));
    if (players == NULL)
    {
        exit(-1);
    }

    int cash = 0;
    if (size == 2)
    {
        cash = 20;
    }
    else if (size == 3)
    {
        cash = 18;
    }
    else
    {
        cash = 16;
    }

    for (int i = 0; i < size; ++i)
    {
        players[i].number = i + 1;
        players[i].space_number = 1;
        players[i].num_properties = 0;
        players[i].num_jail_pass = 0;
        players[i].is_in_jail = 0;
        players[i].cash = cash;
    }

    return players;
}

int check_bankrut(PLAYER* players, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (players[i].cash < 0)
        {
            return 1;
        }
    }
    return 0;
}



int get_winner(PLAYER* players, int size, int looser)
{
    int max = INT_MIN;
    int index = 0;
    int equal = 0;

    int i1 = -1;
    int i2 = -1;
    int i3 = -1;

    for (int i = 0; i < size; ++i)
    {
        if (players[i].cash > max && i != looser)
        {
            max = players[i].cash;
            index = i;
        }
    }
    for (int i = 0; i < size; ++i)
    {
        if (index != i && players[index].cash == players[i].cash && i != looser)
        {
            equal = 1;
            if (i2 == -1)
            {
                i2 = i;
            }
            else if (i3 == -1)
            {
                i3 = i;
            }
        }
    }

    if (equal)
    {
        i1 = index;
        int* cash_sum = (int*)calloc(size, sizeof(int));
        for (int i = 0; i < size; ++i)
        {
            if (i == looser)
            {
                continue;
            }
            for (int j = 0; j < players[i].num_properties; ++j)
            {
                cash_sum[i] += players[i].owned_properties[j]->price;
            }
        }

        max = INT_MIN;
        for (int i = 0; i < size; ++i)
        {
            if (cash_sum[i] > max && i != looser && (i1 == i || i2 == i || i3 == i))
            {
                max = cash_sum[i];
                index = i;
            }
        }

        equal = 0;
        for (int i = 0; i < size; ++i)
        {
            if (index != i && cash_sum[index] == cash_sum[i] && i != looser)
            {
                equal = 1;
                break;
            }
        }
        free(cash_sum);

        if (equal)
        {
            return -5;
        }
    }

    return index;
}

void print_round_info(int turn, int r, int ind_player)
{
    printf("\nR: %d\nTurn: %d\nPlayer on turn: P%d\n", r, turn, ind_player);
}

void print_players_info(PLAYER* players, int size)
{
    printf("Players:\n");
    for (int i = 0; i < size; ++i)
    {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %s\n", players[i].number, players[i].space_number,
               players[i].cash, players[i].num_jail_pass, players[i].is_in_jail == 0 ? "no" : "yes");
        for (int j = 0; j < players[i].num_properties; ++j)
        {
            int pos = 0;
            for (int k = 0; k < NUM_SPACES; ++k)
            {
                if (game_board[k].property != NULL && strcmp(game_board[k].property->name, players[i].owned_properties[j]->name) == 0)
                {
                    pos = k;
                    break;
                }
            }
            char nums[10] = { 0 };
            //char num[5] = { 0 };
            snprintf(nums, 10, "S%d", pos + 1);
            //itoa(pos + 1, num, 10);
            //strcat(nums, "S");
            //strcat(nums, num);
            printf("      %-17s %d  %-9s %-4s\n", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price,
                   property_colors[players[i].owned_properties[j]->color], nums);
        }
    }
}

void print_board_info(PLAYER* players, int size)
{
    printf("Game board:\n");
    for (int i = 0; i < NUM_SPACES; ++i)
    {
        char num[4] = { 0 };
        snprintf(num, 4, "%d.", i + 1);
        //itoa(i + 1, num, 10);
        //strcat(num, ".");
        printf("%-5s", num);
        if (game_board[i].property == NULL)
        {
            printf("%-18s\n", space_types[game_board[i].type]);
        }
        else
        {
            PROPERTY* property = game_board[i].property;
            printf("%-17s %-2d %-10s", property->name, property->price, property_colors[property->color]);
            int flag1 = 0;
            int flag2 = 0;
            for (int h = 0; h < size; ++h)
            {
                for (int k = 0; k < players[h].num_properties; ++k)
                {
                    if (strcmp(property->name, players[h].owned_properties[k]->name) == 0)
                    {
                        char nums[10] = { 0 };
                        //char num[5] = { 0 };
                        snprintf(nums, 10, "P%d", players[h].number);
                        //itoa(players[h].number, num, 10);
                        //strcat(nums, "P");
                        //strcat(nums, num);
                        printf("%s", nums);
                        flag1 = 1;
                        break;
                    }

                }
                if (flag1)
                {
                    for (int k = 0; k < players[h].num_properties; ++k)
                    {
                        if (strcmp(property_colors[players[h].owned_properties[k]->color], property_colors[property->color]) == 0 &&
                            strcmp(property->name, players[h].owned_properties[k]->name) != 0)
                        {
                            printf(" yes\n");
                            flag2 = 1;
                            break;
                        }
                    }
                }
                if (flag1)
                {
                    break;
                }
            }
            if (!flag2 && flag1)
            {
                printf(" no\n");
            }
            if (!flag1)
            {
                printf("\n");
            }
        }
    }
}

void print_game_info(PLAYER* players, int size, PLAYER* winner, int is_draw)
{
    print_players_info(players, size);
    print_board_info(players, size);
    printf("WINNER: ");
    if (winner == NULL && !is_draw)
    {
        printf("-\n");
    }
    else if (is_draw)
    {
        printf("?\n");
    }
    else
    {
        printf("P%d\n", winner->number);
    }
}


int move_player(PLAYER* player, PLAYER* players, int size, int* rr)
{
    int r = 0;
    if (scanf("%d", &r) != 1)
    {
        *rr = r;
        return INVALID_INPUT;
    }
    *rr = r;

    if (player->is_in_jail)
    {
        if (player->num_jail_pass != 0)
        {
            player->num_jail_pass--;
        }
        else
        {
            if (player->cash - 1 < 0)
            {
                return BANKRUT;
            }
            player->cash -= 1;
        }
        player->is_in_jail = 0;
    }

    int player_position = player->space_number;
    if (player_position + r > NUM_SPACES)
    {
        player->cash += 2;
        player->space_number = (player_position + r) - NUM_SPACES;
    }
    else
    {
        player->space_number += r;
    }

    PROPERTY* prop = game_board[player->space_number - 1].property;
    int flag = 0;
    int index = 0;
    if (prop != NULL)
    {
        for (int i = 0; i < size; ++i)
        {
            if (i + 1 == player->number)
            {
                for (int j = 0; j < players[i].num_properties; ++j)
                {
                    if (strcmp(prop->name, players[i].owned_properties[j]->name) == 0)
                    {
                        flag = -1;
                        break;
                    }
                }
            }
            if (flag == -1)
            {
                break;
            }

            for (int j = 0; j < players[i].num_properties; ++j)
            {
                if (strcmp(prop->name, players[i].owned_properties[j]->name) == 0)
                {
                    index = i;
                    flag = 1;
                    break;
                }
            }

            if (flag)
            {
                for (int j = 0; j < players[i].num_properties; ++j)
                {
                    if (strcmp(property_colors[players[i].owned_properties[j]->color], property_colors[prop->color]) == 0
                        && strcmp(prop->name, players[i].owned_properties[j]->name) != 0)
                    {
                        flag = 2;
                        break;
                    }
                }
            }

            if (flag == 1 || flag == 2)
            {
                break;
            }
        }
        if (flag == 1)
        {
            if (player->cash - prop->price < 0)
            {
                return BANKRUT;
            }
            player->cash -= prop->price;
            players[index].cash += prop->price;
        }
        else if (flag == 2)
        {
            if (player->cash - 2 * prop->price < 0)
            {
                return BANKRUT;
            }
            player->cash -= 2 * prop->price;
            players[index].cash += 2 * prop->price;
        }

        if (!flag)
        {
            if (player->cash - prop->price < 0)
            {
                return BANKRUT;
            }
            player->cash -= prop->price;
            player->owned_properties[player->num_properties++] = prop;
        }
    }
    else
    {
        if (strcmp(space_types[game_board[player->space_number - 1].type], "JAIL PASS") == 0)
        {
            player->num_jail_pass++;
        }
        else if (strcmp(space_types[game_board[player->space_number - 1].type], "GO TO JAIL") == 0)
        {
            if (player->num_jail_pass > 0)
            {
                player->num_jail_pass--;
                return r;
            }

            player->space_number = 7;
            player->is_in_jail = 1;
        }
    }
    return r;
}



int main(int argc, char** argv)
{
    int num_players = 2;
    bool is_s = false;
    bool is_p = false;
    bool is_g = false;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            num_players = atoi(argv[i + 1]);
            ++i;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            is_s = true;
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            is_p = true;
        }
        else if (strcmp(argv[i], "-g") == 0)
        {
            is_g = true;
        }
    }

    PLAYER* players = createPlayersArr(num_players);

    int turn = 1;
    int player_turn = 0;

    print_game_info(players, num_players, NULL, 0);
    int res = 0;
    int r = 0;
    while (!check_bankrut(players, num_players))
    {
        res = move_player(&players[player_turn], players, num_players, &r);
        print_round_info(turn, r, player_turn + 1);
        if (res == BANKRUT || res == INVALID_INPUT)
        {
            break;
        }

        if (is_s && !check_bankrut(players, num_players))
        {
            printf("\n");
            print_board_info(players, num_players);
        }
        else if (is_p && !check_bankrut(players, num_players))
        {
            printf("\n");
            print_players_info(players, num_players);
        }
        else if (is_g && !check_bankrut(players, num_players))
        {
            printf("\n");
            print_game_info(players, num_players, NULL, 0);
        }

        ++player_turn;
        ++turn;
        if (player_turn >= num_players)
        {
            player_turn = 0;
        }
    }

    int ind_winner = get_winner(players, num_players, player_turn);
    printf("\n");
    print_game_info(players, num_players, ind_winner == -5 ? NULL : &players[ind_winner], ind_winner == -5 ? 1 : 0);

    free(players);

    return 0;
}
