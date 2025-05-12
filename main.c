//---------------------------------------------------------------------------------------------------------------------
// A card game for two players which is loosely inspired by Spicy. All cards have two characteristics: a spice
// and a value. The game is played in multiple rounds. Each round consists of the players taking alternating turns.
// On their turn, a player plays a card from their hand face down and tells the other player what the card shows.
// The other player can believe what has been said and continue with their own turn, or challenge the first player
// on one of the two characteristics to see if it was a lie. Players score points for catching lies, but if a player
// challenges a card and the challenged characteristic was not a lie, the other player gets points. Challenging a card
// ends the current round and starts the next one. The game ends when the draw pile is empty, and the player
// with the most points wins.
//
// Tutor: Thomas Halbedl
//
// Author: 12329188
//---------------------------------------------------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

const int CARDS_PER_PLAYER = 6;
const int SIZE = 4;

typedef struct _Card_
{
    int value_;
    char spice_;
    struct _Card_ *next_;
} Card;

enum
{
    FINISH = -1,
    ONE = 1,
    TWO = 2,
    QUIT = 3,
    MEMORY = 4,
    RESTART = 5
};

void freeCardList(Card *head)
{
    Card *current = head;
    Card *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next_;
        free(temp);
    }
}

bool startOfStringCaseInsensitiveComparison(char *string, char *piece, int number_of_characters)
{
    // normal comparison just case insensitive

    if (number_of_characters == -1)
    {
        int length = strlen(string);
        for (int counter = 0; counter < length; counter++)
        {
            string[counter] = tolower(string[counter]);
        }
        return !strcmp(string, piece);
    }
    for (int counter = 0; counter < number_of_characters; counter++)
    {
        if (tolower(string[counter]) != tolower(piece[counter]))
            return false;
    }
    // if command has extra charactes
    if (strlen(string) > strlen(piece) && string[number_of_characters] != ' ')
        return false;
    return true;

    // play 3_w 3_w
}

int compare(Card *card1, Card *card2)
{
    if (card1->spice_ != card2->spice_)
        return card1->spice_ - card2->spice_;
    return card1->value_ - card2->value_;
}

void swapItems(Card *item1, Card *item2)
{
    Card aux = *item1;
    *item1 = *item2;
    *item2 = aux;
}

void mySort(Card **array_of_cards, int size_of_list)
{
    for (int primary = 0; primary < size_of_list; primary++)
        for (int secondary = primary; secondary < size_of_list; secondary++)
            if (compare(array_of_cards[secondary], array_of_cards[primary]) < 0)
                swapItems(array_of_cards[primary], array_of_cards[secondary]);
}

void sortCardSinglyLinkedList(Card **head)
{
    if (!*head)
        return;

    // count elements in list
    int size_of_list = 0;
    Card *current;

    for (current = *head; current != NULL; current = current->next_)
        ++size_of_list;

    Card *array_of_cards[size_of_list];
    Card **array_pointer = array_of_cards;
    for (current = *head; current != NULL; current = current->next_)
    {
        *array_pointer++ = current;
    }
    mySort(array_of_cards, size_of_list);
    for (int i = 0; i < size_of_list; i++)
    {
        if (i == size_of_list - 1)
            array_of_cards[i]->next_ = NULL;
        else
            array_of_cards[i]->next_ = array_of_cards[i + 1];
    }
    *head = array_of_cards[0];
}

int checkCommandLineArguments(int argc, char *argv[], char **file_name)
{
    printf("-%s", argv[1]);
    if (argc == 2)
        *file_name = argv[1];
    else
    {
        printf("Usage: ./a4-csf <config file>\n");
        return 1;
    }
    FILE *file = fopen(*file_name, "r");
    if (file == NULL)
    {
        printf("Error: Cannot open file: %s\n", *file_name);
        return 2;
    }
    char magic[5];
    fgets(magic, 5, file);
    if (strcmp(magic, "ESP\n"))
    {
        fclose(file);
        printf("Error: Invalid file: %s\n", *file_name);
        return 3;
    }
    fclose(file);
    return 0;
}

char *inputInfiniteString(void)
{
    int max_length = SIZE;
    char *string = (char *)malloc(max_length * sizeof(char));
    if (!string)
        return NULL;

    char input;
    long long counter = 0;
    while ((input = getchar()) != '\n')
    {
        string[counter++] = input;
        if (counter == max_length)
        {
            max_length *= 2;
            string = (char *)realloc(string, max_length * sizeof(char));
            if (!string)
                return NULL;
        }
    }
    string[counter] = '\0';
    return string;
}

void trimLeadingSpaces(char *string)
{
    int i = 0, j = 0;

    // skip spaces
    while (string[i] == ' ')
        i++;
    // shift characters back
    while ((string[j++] = string[i++]))
        ;
}

Card *createCard(int value, char spice)
{
    Card *newCard = (Card *)malloc(sizeof(Card));

    // needs return value and handling
    if (!newCard)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    newCard->value_ = value;
    newCard->spice_ = tolower(spice);

    newCard->next_ = NULL;
    return newCard;
}

void printCards(Card *head)
{
    Card *current = head;
    while (current != NULL)
    {
        printf(" %d_%c", current->value_, current->spice_);
        current = current->next_;
    }
    printf("\n");
}

int distributeCards(Card **player1_head, Card **player2_head, Card **deck_head, char *file_name)
{
    Card *player1_current = NULL;
    Card *player2_current = NULL;
    Card *deck_current = NULL;
    Card *new_card = NULL;

    FILE *
        file = fopen(file_name, "r");

    char line[6];
    fgets(line, 6, file); // skip magic line
    int player = 1;
    int value;
    char spice;
    for (int i = 0; i < 2 * CARDS_PER_PLAYER; i++)
    {
        fgets(line, 6, file);
        if (line[1] == '_')
        {
            value = line[0] - '0';
            spice = line[2];
        }
        else
        {
            value = 10;
            spice = line[3];
        }

        new_card = createCard(value, spice);
        if (!new_card)
            return 1;
        if (player == 1)
        {
            if (*player1_head == NULL)
                *player1_head = new_card;
            else
                player1_current->next_ = new_card;
            player1_current = new_card;
            player = 2;
        }
        else
        {
            if (*player2_head == NULL)
                *player2_head = new_card;
            else
                player2_current->next_ = new_card;
            player2_current = new_card;
            player = 1;
        }
    }

    // Distribute remaining cards to the deck
    while (fgets(line, 6, file))
    {
        if (line[1] == '_')
        {
            value = line[0] - '0';
            spice = line[2];
        }
        else
        {
            value = 10;
            spice = line[3];
        }
        new_card = createCard(value, spice);
        if (!new_card)
            return 1;
        if (*deck_head == NULL)
            *deck_head = new_card;
        else
            deck_current->next_ = new_card;
        deck_current = new_card;
    }
    deck_current->next_ = NULL;
    fclose(file);
    return 0;
}

bool isSpice(char spice)
{
    spice = tolower(spice);
    if (spice == 'c' || spice == 'p' || spice == 'w')
        return true;
    return false;
}

bool correctNumberOfParameters(char *command)
{
    // command
    int parameters = 0;
    char command_copy[strlen(command)];
    strcpy(command_copy, command);
    char *token = strtok(command_copy, " ");
    while ((token = strtok(NULL, " ")) != NULL)
    {
        parameters++;
        if (parameters > 2)
            return false;
    }
    if (parameters == 2)
        return true;
    return false;
}

bool testPlayCommandParameters(char *command, Card **hand_card, Card **played_as_card)
{
    // play
    char *token = strtok(command, " ");
    // first card
    token = strtok(NULL, " ");
    char value_as_string[2];
    int value, counter, parameters = 0;
    char spice;
    while (token != NULL)
    {
        if (strlen(token) < 3 || strlen(token) > 4)
            return false;
        // reset to base values
        value_as_string[0] = '0';
        value_as_string[1] = '\0';

        counter = 0;
        if (isdigit(token[counter]))
            value_as_string[0] = token[counter];
        else
            return false;
        counter++;
        if (isdigit(token[counter]))
        {
            value_as_string[1] = token[counter];
            counter++;
        }
        value = atoi(value_as_string);
        if (value < 1 || value > 10)
            return false;
        if (token[counter] != '_')
            return false;
        counter++;
        if (isSpice(token[counter]))
            spice = token[counter];
        else
            return false;
        if (strlen(token) == 4 && (token[counter + 1] == '!' || token[counter + 1] == ','))
            return false;
        parameters++;

        if (parameters == 1)
        {
            *hand_card = createCard(value, spice);
        }
        else if (parameters == 2)
        {
            *played_as_card = createCard(value, spice);
        }
        else if (parameters > 2)
        {
            return false;
        }

        token = strtok(NULL, " ");
    }
    if (parameters < 2)
        return false;
    return true;
}

int isChalengeParameterValid(char *command)
{
    // challenge
    char *token = strtok(command, " ");
    // type
    token = strtok(NULL, " ");
    if (!token)
        return 0;
    else if (strtok(NULL, " "))
        return 0;
    else if (startOfStringCaseInsensitiveComparison(token, "value", -1))
        return 1;
    else if (startOfStringCaseInsensitiveComparison(token, "spice", -1))
        return 2;
    return 3;
}

int printChallengeMessage(int parameter_code, Card *last_card, Card *last_played_as_card)
{
    bool win;
    // value
    if (parameter_code == 1)
    {
        if (last_card->value_ == last_played_as_card->value_)
            win = false;
        else
            win = true;
    }
    // spice
    else
    {
        if (last_card->spice_ == last_played_as_card->spice_)
            win = false;
        else
            win = true;
    }
    if (win)
    {
        printf("Challenge successful: %d_%c's %s does not match the real card %d_%c.\n", last_played_as_card->value_, last_played_as_card->spice_, parameter_code == 1 ? "value" : "spice", last_card->value_, last_card->spice_);
        return 1;
    }
    else
    {
        printf("Challenge failed: %d_%c's %s matches the real card %d_%c.\n", last_played_as_card->value_, last_played_as_card->spice_, parameter_code == 1 ? "value" : "spice", last_card->value_, last_card->spice_);
        return 0;
    }
}

bool drawCard(Card **deck_head, Card **current_player_head)
{
    if (!*deck_head)
        return false;
    Card *new_card;
    new_card = createCard((*deck_head)->value_, (*deck_head)->spice_);
    if (!new_card)
        ; // help
    new_card->next_ = *current_player_head;
    *current_player_head = new_card;
    Card *temp = *deck_head;
    (*deck_head) = (*deck_head)->next_;
    free(temp);
    return true;
}

int playingRound(Card **player1_head, Card **player2_head, Card **deck_head, int player, int *player1_points, int *player2_points)
{
    bool first_card = true, no_card_in_hand, spice_is_wrong, value_is_wrong, is_card_played = false, repeat = false, is_opponents_last_card;
    Card *last_card, *last_played_as_card, *hand_card, *played_as_card, **current_player_head = player == 1 ? player1_head : player2_head, *temp;

    int cards_played = 0, other_player, player_who_won;
    char *command;
    char round_spice = '.';

    printf("\n-------------------\nROUND START\n-------------------\n");
    while (1)
    {
        if (!repeat)
        {
            is_opponents_last_card = player == 1 ? !(*player2_head) : !(*player1_head);
            printf("\nPlayer %d:\n", player);
            if (first_card)
                printf("    latest played card:%s\n", is_opponents_last_card ? " LAST CARD" : "");
            else
                printf("    latest played card: %d_%c%s\n", last_played_as_card->value_, last_played_as_card->spice_, is_opponents_last_card ? " LAST CARD" : "");
            printf("    cards played this round: %d\n", cards_played);
            printf("    hand cards:");
            sortCardSinglyLinkedList(current_player_head);
            printCards(*current_player_head);
        }
        printf("P%d > ", player);
        // command input   <------------------
        command = inputInfiniteString();
        trimLeadingSpaces(command);
        repeat = true;
        if (!command)
            return MEMORY;
        if (startOfStringCaseInsensitiveComparison(command, "quit", 4))
        {
            strtok(command, " ");
            if (strtok(NULL, " "))
                printf("Please enter the correct number of parameters!\n");
            else
            {
                if (!first_card)
                {
                    free(last_card);
                    free(last_played_as_card);
                }
                free(command);
                return QUIT;
            }
        }
        else if (startOfStringCaseInsensitiveComparison(command, "play", 4))
        {
            if (!correctNumberOfParameters(command))
            {
                printf("Please enter the correct number of parameters!\n");
            }
            else if (testPlayCommandParameters(command, &hand_card, &played_as_card))
            {
                if (!hand_card || !played_as_card)
                {
                    free(command);
                    return MEMORY;
                }
                // test spice and value on fake card
                spice_is_wrong = false;
                if (!first_card && played_as_card->spice_ != round_spice)
                    spice_is_wrong = true;

                value_is_wrong = false;
                if (first_card || last_played_as_card->value_ == 10)
                {
                    if (played_as_card->value_ > 3)
                        value_is_wrong = true;
                }
                else if (played_as_card->value_ <= last_played_as_card->value_)
                    value_is_wrong = true;

                // test hand card
                no_card_in_hand = true;
                if ((*current_player_head)->value_ == hand_card->value_ && (*current_player_head)->spice_ == hand_card->spice_)
                {
                    if (!value_is_wrong && !spice_is_wrong && !is_opponents_last_card)
                    {
                        temp = *current_player_head;
                        *current_player_head = (*current_player_head)->next_;
                        free(temp);
                    }

                    no_card_in_hand = false;
                }
                else
                {
                    for (Card *current = *current_player_head; current->next_ != NULL; current = current->next_)
                    {
                        if (current->next_->value_ == hand_card->value_ && current->next_->spice_ == hand_card->spice_)
                        {
                            if (!value_is_wrong && !spice_is_wrong && !is_opponents_last_card)
                            {
                                temp = current->next_;
                                current->next_ = current->next_->next_;
                                free(temp);
                            }
                            no_card_in_hand = false;
                            break;
                        }
                    }
                }

                if (is_opponents_last_card || no_card_in_hand || value_is_wrong || spice_is_wrong)
                {
                    if (!first_card)
                    {
                        free(hand_card);
                        free(played_as_card);
                    }
                }

                if (is_opponents_last_card)
                {
                    printf("Please enter a command you can use at the moment!\n");
                }
                else if (no_card_in_hand)
                {
                    printf("Please enter a card in your hand cards!\n");
                }
                else if (value_is_wrong)
                {
                    printf("Please enter a valid VALUE!\n");
                }
                else if (spice_is_wrong)
                {
                    printf("Please enter a valid SPICE!\n");
                }
                else
                {
                    if (!first_card)
                    {
                        free(last_card);
                        free(last_played_as_card);
                    }
                    last_card = hand_card;
                    last_played_as_card = played_as_card;
                    round_spice = last_played_as_card->spice_;
                    cards_played++;
                    if (first_card)
                        first_card = false;

                    // Swap players
                    if (player == 1)
                    {
                        player = 2;
                        current_player_head = player2_head;
                    }
                    else
                    {
                        player = 1;
                        current_player_head = player1_head;
                    }
                    is_card_played = true;
                    repeat = false;
                }
            }
            else
            {
                printf("Please enter the cards in the correct format!\n");
            }
        }
        else if (startOfStringCaseInsensitiveComparison(command, "challenge", 9))
        {
            int parameter_code = isChalengeParameterValid(command);
            if (!parameter_code)
                printf("Please enter the correct number of parameters!\n");
            else if (!is_card_played)
                printf("Please enter a command you can use at the moment!\n");
            else if (parameter_code == 3)
                printf("Please choose SPICE or VALUE!\n");
            else
            {
                other_player = player == 1 ? 2 : 1;
                player_who_won = printChallengeMessage(parameter_code, last_card, last_played_as_card) ? player : other_player;
                printf("Player %d gets %d points.\n", player_who_won, cards_played);
                if (is_opponents_last_card && player_who_won != player)
                {
                    printf("Player %d gets 10 bonus points (last card).\n", player_who_won);
                    cards_played += 10;
                }
                if (player_who_won == 1)
                    *player1_points += cards_played;
                else
                    *player2_points += cards_played;

                for (int i = 0; i < 2; i++)
                {
                    if (!drawCard(deck_head, player_who_won == 1 ? player2_head : player1_head))
                    {
                        if (!first_card)
                        {
                            free(last_card);
                            free(last_played_as_card);
                        }
                        free(command);
                        return FINISH;
                    }
                };
                if (!(player_who_won == 1 ? *player1_head : *player2_head))
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if (!drawCard(deck_head, player_who_won == 1 ? player1_head : player2_head))
                        {
                            if (!first_card)
                            {
                                free(last_card);
                                free(last_played_as_card);
                            }
                            free(command);
                            return FINISH;
                        }
                    }
                }
                if (!first_card)
                {
                    free(last_card);
                    free(last_played_as_card);
                }
                free(command);
                return player_who_won;
            }
        }
        else if (startOfStringCaseInsensitiveComparison(command, "draw", 4))
        {
            strtok(command, " ");
            if (strtok(NULL, " "))
                printf("Please enter the correct number of parameters!\n");
            else if (is_opponents_last_card)
                printf("Please enter a command you can use at the moment!\n");
            else
            {

                if (!drawCard(deck_head, current_player_head))
                {
                    if (!first_card)
                    {
                        free(last_card);
                        free(last_played_as_card);
                    }
                    free(command);
                    return FINISH;
                }

                // Swap players
                if (player == 1)
                {
                    player = 2;
                    current_player_head = player2_head;
                }
                else
                {
                    player = 1;
                    current_player_head = player1_head;
                }
                is_card_played = false;
                repeat = false;
                if (!*deck_head)
                {
                    if (!first_card)
                    {
                        free(last_card);
                        free(last_played_as_card);
                    }
                    free(command);
                    return FINISH;
                }
            }
        }
        else if (startOfStringCaseInsensitiveComparison(command, "restart", 7))
        {
            strtok(command, " ");
            if (strtok(NULL, " "))
                printf("Please enter the correct number of parameters!\n");
            else
            {
                if (!first_card)
                {
                    free(last_card);
                    free(last_played_as_card);
                }
                free(command);
                return RESTART;
            }
        }
        else
        {
            printf("Please enter a valid command!\n");
        }
        free(command);
    }
    return 0;
}

void printAndAddScoresToFile(int player1_points, int player2_points, char *file_name)
{
    int first_player = 1, first_player_points = player1_points, second_player = 2, second_player_points = player2_points;
    if (player2_points > player1_points)
    {
        first_player = 2;
        second_player = 1;
        first_player_points = player2_points;
        second_player_points = player1_points;
    }

    char scoring[100];

    snprintf(scoring, 100, "\nPlayer %d: %d points\nPlayer %d: %d points\n", first_player, first_player_points, second_player, second_player_points);
    snprintf(scoring + strlen(scoring), 100, "\nCongratulations! Player %d wins the game!\n", first_player);
    if (player1_points == player2_points)
        snprintf(scoring + strlen(scoring), 100, "Congratulations! Player %d wins the game!\n", second_player);
    printf("%s", scoring);

    FILE *file = fopen(file_name, "a");
    if (file == NULL)
    {
        printf("Warning: Results not written to file!\n");
        return;
    }
    fprintf(file, "%s", scoring);
    fclose(file);
}

//---------------------------------------------------------------------------------------------------------------------
/// THE MAIN FUNCTION
///
/// @return
int main(int argc, char *argv[])
{
    int return_code;
    char *file_name;
    return_code = checkCommandLineArguments(argc, argv, &file_name);
    if (return_code)
        return return_code;

    Card *player1_head;
    Card *player2_head;
    Card *deck_head;

    int main_return_code, player, player1_points, player2_points;
    bool restart = false;
    do
    {
        player = 1;
        player1_points = 0;
        player2_points = 0;
        player1_head = NULL;
        player2_head = NULL;
        deck_head = NULL;
        printf("Welcome to Entertaining Spice Pretending!\n");

        if (distributeCards(&player1_head, &player2_head, &deck_head, file_name))
            return 4;
        while (1)
        {
            main_return_code = playingRound(&player1_head, &player2_head, &deck_head, player, &player1_points, &player2_points);
            if (main_return_code == FINISH)
            {
                break;
            }
            else if (main_return_code == ONE)
                player = 2;
            else if (main_return_code == TWO)
                player = 1;
            else if (main_return_code == QUIT)
            {
                freeCardList(player1_head);
                freeCardList(player2_head);
                freeCardList(deck_head);
                return 0;
            }
            else if (main_return_code == MEMORY)
            {
                freeCardList(player1_head);
                freeCardList(player2_head);
                freeCardList(deck_head);
                return 4;
            }
            else if (main_return_code == RESTART)
            {
                freeCardList(player1_head);
                freeCardList(player2_head);
                freeCardList(deck_head);
                restart = true;
                break;
            }
        }
    } while (restart);
    freeCardList(player1_head);
    freeCardList(player2_head);
    freeCardList(deck_head);
    printAndAddScoresToFile(player1_points, player2_points, file_name);
    return 0;
}
