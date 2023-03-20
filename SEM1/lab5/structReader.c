#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "structReader.h"

#define MAX_LENGHT 100


char *fullNameGiver(char *string, int *end) 
{
    int spaces = 0, len = 0;
    while (spaces <= 2)
    {
        if (string[len] == ' ')
        {
            spaces++;
            if (spaces == 3)
            {
                break;
            }
            len++;
        }
        else
        {
            len++;
        }
    }
    if (spaces < 3) {
        fprintf(stderr, "\nWrong format: Family/Name/Patronymic\n");
        return NULL;
    }
    if (len < 0) 
    {
        fprintf(stderr, "\nCannot extract full name from file\n");
        return NULL;
    }
    char *result = (char *)malloc((len + 1) * sizeof(char));
    memcpy(result, string, (len + 1) * sizeof(char));
    if (!result) {
        fprintf(stderr, "\nError in writing full name in array\n");
        free(result);
        return NULL;
    }
    result[len] = '\0';
    if (atoi(result))
    {
        printf("%d\n", atoi(result));
        fprintf(stderr, "\nData is in wrong format\n");
        free(result);
        return NULL;
    }
    result = realloc(result, (strlen(result) + 1) * sizeof(char));
    result[strlen(result)] = '\0';
    *end = len;
    return result;
}

int votersPrint(Voter *voters, size_t quantity)
{
    if (!voters) 
    {
        fprintf(stderr, "\nNo stcructures was readed to print\n");
        return 1;
    }
    for (int i = 0; i < quantity; i++)
    {
        printf("%s %s %d\n", voters[i].fullName, voters[i].idPolling, voters[i].age);
    }
    return 0;
}

void structFree(Voter *array, size_t lines)
{
    if (!array)
    {
        return;
    }
    if (lines == 0)
    {
        free(array);
        return;
    }
    for (int i = 0; i < lines; i++)
    {
        free(array[i].fullName);
        //free(array[i].idPolling);
    }
    free(array);
}

Voter *structReader(size_t *numberOfVoters) 
{
    Voter voter;

    FILE *file;

    file = fopen("input.txt", "r");
    if (!file) 
    {
        fprintf(stderr, "\nError reading file Format: (Family/Name/Patronymic)\n");
        return NULL;
    }
    int end;
    *numberOfVoters = 0;
    char *tempString = malloc(MAX_LENGHT * sizeof(char));
    Voter *voterRegister = (Voter *)malloc(1 * sizeof(Voter ));
    while(fgets(tempString, MAX_LENGHT, file))
    {
        //voter.idPolling = malloc(8 * sizeof(char));
        voter.fullName = fullNameGiver(tempString, &end);
        if (!voter.fullName)
        {
            structFree(voterRegister, *numberOfVoters);
            return NULL;
        }
        end++;
        memcpy(voter.idPolling, tempString + end, 7);
        
        voter.idPolling[7] = '\0';
        if ((atoi(&voter.idPolling[3]) != 0) && (atoi(&voter.idPolling[3]) > atoi(&voter.idPolling[0]))) 
        {
            fprintf(stderr, "\nData is in wrong format. Format: (XXX-YYY) LINE: %d\n", *numberOfVoters);
            structFree(voterRegister, *numberOfVoters);
            return NULL;
        }
        end = end + strlen(voter.idPolling);
        voter.age = atoi(tempString + end);
        if (voter.age < 18)
        {
            fprintf(stderr, "\nYou are violating codex of Russian Federation, Dear Comrade\n");
            structFree(voterRegister, *numberOfVoters);
            return NULL;
        }
        if (voter.age > 110)
        {
            fprintf(stderr, "\nAren't you brought coffin with yourself? Is this person still alive?\n");
            structFree(voterRegister, *numberOfVoters);
            return NULL;
        }
        (*numberOfVoters)++;
        voterRegister = (Voter *)realloc(voterRegister, (*numberOfVoters) * sizeof(Voter ));
        voterRegister[(*numberOfVoters)-1] = voter;
        free(tempString);
        tempString = malloc(MAX_LENGHT * sizeof(char));
    }
    free(tempString);
    fclose(file);
    return voterRegister;
}
