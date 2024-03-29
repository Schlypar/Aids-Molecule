#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "readline.h"
#include "sort.h"


#define PROMT "----> "

void proccess(List *list)
{
	if (!list)
		return;

	while (list && !list->head)
	{
		free(list);
		list = readline(PROMT);
		if (!list)
			return;
	}

	while (list->head->symbol == ' ')
	{
		Node *tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}

	Node *start = list->head, *end = list->head->next;
	Node *prev = start;

	while (end)
	{
		if (end->symbol == '\t')
			end->symbol = ' ';

		while (prev->symbol == ' ' && end->symbol == ' ')
		{
			prev->next = end->next;
			free(end);
			if (end == list->tail)
			{
				list->tail = prev;
				end = prev;
				break;
			}
			(prev->next != NULL) ? end = prev->next : prev;
		}

		if (end == list->tail)
		{
			sort(start, end, (int (*)(Node *, Node *)) compare);
			break;
		}
		if (end->next->symbol == ' ')
		{
			sort(start, end, (int (*)(Node *, Node *)) compare);
			if (end->next->next)
			{
				start = end->next;
				prev = end;
				end = start;
			}
			else
				break;
		}

		prev = end;
		end = end->next;
	}
	if (list->tail->symbol == ' ')
	{
		Node *ptr = list->head;
		while (ptr->next != list->tail)
			ptr = ptr->next;
		list->tail = ptr;
		free(list->tail->next);
		list->tail->next = NULL;
	}
}

int main()
{
	List *list = NULL;
	while (1)
	{
		list = readline(PROMT);
		if (!list)
		{
			printf("\n");
			break;
		}
		listPrint(list);
		proccess(list);
		listPrint(list);
		listDelete(list);
	}
	return 0;
}