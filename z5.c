#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "data2.h" 

char* readline(FILE* file)
{
    static char buffer[MAX_LINE+1];
    fgets(buffer, sizeof(buffer), file);
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

void print_items(ITEM* items, int n)
{
	for (int i = 0; i<n; i++)
	{
		printf("Name: %s\nAttack: %d\nDefense: %d\nSlots: %d\nRange: %d\nRadius: %d\n",
		items[i].name, items[i].att, items[i].def, 
		items[i].slots, items[i].range, items[i].radius);
		printf("\n");
	}
}

int position_in_database(char item[], ITEM* items, int n)
{
    for (int i = 0; i < n; i++)
    {
        
        if (strcmp(item, items[i].name) == 0)
        {
            return i;
        }  
    }
    return -1;
}

void print_army(UNIT army[],int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("    Unit: %d\n", i);
        printf("    Name: %s\n", army[i].name);
        printf("    HP: %d\n", army[i].hp);
        printf("    Item 1: %s,%d,%d,%d,%d,%d\n",
            army[i].item1->name,
            army[i].item1->att,
            army[i].item1->def,
            army[i].item1->slots,
            army[i].item1->range,
            army[i].item1->radius);
        
        if (army[i].item2 != NULL)
        {
            printf("    Item 2: %s,%d,%d,%d,%d,%d\n",
                army[i].item2->name,
                army[i].item2->att,
                army[i].item2->def,
                army[i].item2->slots,
                army[i].item2->range,
                army[i].item2->radius);
        }
		printf("\n");
    }
}

int main(const int argc, char *argv[]) {

	if (argc != 1 && argc != 3)
	{
		fprintf(stderr, ERR_CMD);
		return 0;
	}

	FILE* f = fopen(ITEMS_FILE, "r");
	if (f == NULL)
	{
		fprintf(stderr, ERR_FILE);
		return 0;
	}
	
	ITEM* items = (ITEM*) malloc(sizeof(ITEM));
	char* line;
	readline(f); //zbavim sa hranatej zatvorky

	int n = 1; //pociatocna velkost pola items
	while (1)
	{
		readline(f); //zbavim sa kuceravej zatvorky

		// nakopirovanie mena do prvku items
		line = readline(f);
		char* ptr = line;
		while (*ptr != ':')
		{
			ptr++;
		}
		while (*ptr != '"')
		{
			if (isdigit(*ptr))
			{
				fprintf(stderr, ERR_MISSING_ATTRIBUTE);
				return 0;
			}
			else if (*ptr == ',')
			{
				fprintf(stderr, ERR_MISSING_VALUE);
				return 0;
			}
			ptr++;
		}
		ptr++;
		char* pt = ptr;
		while (*pt != '"')
		{
			pt++;
		}
		*pt = '\0';
		strcpy(items[n-1].name, ptr);

		//ostatne atributy
		for (int i = 1; i<=4; i++)
		{
			line = readline(f);
			char* p = line;
			int comma = 0;
			while (*p != '\0')
			{
				if (*p == ',')
				{
					comma++;
				}
				p++;
			}
			if (comma != 1)
			{
				fprintf(stderr, ERR_MISSING_ATTRIBUTE);
				return 0;
			}

			p = line;
			while (*p != ':')
			{
				p++;
			}
			p++;
		
			if (*p == ',')
			{
				fprintf(stderr, ERR_MISSING_VALUE);
				return 0;
			}
			while (*p == ' ' || *p == '\t')
			{
				p++;
				if (*p == ',')
				{
					fprintf(stderr, ERR_MISSING_VALUE);
					return 0;
				}	
			}

			p = line;
			while (*p != ':')
			{
				p++;
			}
			p++;
			
			while (1)
			{

				if (*p == ' ' || *p == '\t')
				{
					p++;
					continue;
				}
				if (*p == ',')
				{
					break;
				}
				if (isdigit(*p) == 0)
				{
					fprintf(stderr, ERR_BAD_VALUE);
					return 0;
				}
				p++;
			}

			p = line;
			while (*p != ':')
			{
				p++;
			}
			p++;

			while (isdigit(*p) == 0)
			{
				p++;
			}

			char* p2 = p;
			while (isdigit(*p2))
			{
				p2++;
			}
			*p2 = '\0';
			
			int cislo = atoi(p);
		    if (i == 1)
			{
				items[n-1].att = cislo;
			}
			else if (i == 2)
			{
				items[n-1].def = cislo;
			}
			else if (i == 3)
			{
				items[n-1].slots = cislo;
			}
			else if (i == 4)
			{
				items[n-1].range = cislo;
			}
		}

		//posledny atribut
		line = readline(f);
		char* pointer = line;
		while (*pointer != ':')
		{
			pointer++;
		}
		pointer++;

		if (*pointer == '\0')
		{
			fprintf(stderr, ERR_MISSING_VALUE);
			return 0;
		}
		while (*pointer == ' ' || *pointer == '\t' || *pointer == '\r')
		{
			pointer++;
			if (*pointer == '\0')
			{
				fprintf(stderr, ERR_MISSING_VALUE);
				return 0;
			}	
		}
		
		pointer = line;
		while (*pointer != ':')
		{
			pointer++;
		}
		pointer++;
		
		while (*pointer != '\0')
		{

			if (*pointer == ' ' || *pointer == '\t' || *pointer == '\r')
			{
				pointer++;
				continue;
			}
		
			if (isdigit(*pointer) == 0)
			{
				fprintf(stderr, ERR_BAD_VALUE);
				return 0;
			}
			pointer++;
		}
		
		pointer = line;
		while (*pointer != ':')
		{
			pointer++;
		}
		pointer++;

		while (isdigit(*pointer) == 0)
		{
			pointer++;
		}

		char* pointer2 = pointer;
		while (isdigit(*pointer2))
		{
			pointer2++;
		}
		*pointer2 = '\0';

		int num = atoi(pointer);
		items[n-1].radius = num;

		line = readline(f);
		int k = 0;
		while(*line != '\0')
		{
			if (*line == ',')
			{
				k++;
				break;
			}
			line++;
		}

		if (k == 0)
		{
			break;
		}
		else
		{
			n++;
			items = (ITEM*) realloc(items, n*sizeof(ITEM));
		}
	}
	
	fclose(f);

	if (argc == 1)
	{
		print_items(items, n);
	}
	else
	{
		
		f = fopen(argv[1], "r");
		if (f == NULL)
		{
			fprintf(stderr, ERR_FILE);
			return 0;
		}

		char* riadok = readline(f);
		while (isdigit(*riadok) == 0)
		{
			riadok++;
		}
		int count1 = atoi(riadok);
		if (count1<1 || count1>5)
		{
			fprintf(stderr, ERR_UNIT_COUNT);
			return 0;
		}

		UNIT* units1 = (UNIT*) malloc(count1*sizeof(UNIT));
		
		for (int i = 0; i<count1; i++)
		{
			units1[i].hp = 100;

			riadok = readline(f);
			while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r')
			{
				riadok++;
			}

			int j = 0;
			while (1)
			{
				units1[i].name[j] = *riadok;
				j++;
				riadok++;
				if (*riadok == ' ' || *riadok == '\0' || *riadok == '\r' || *riadok == '\t')
				{
					units1[i].name[j] = '\0';
					break;
				}
			}

			while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r'|| *riadok == '\0')
			{
				if (*riadok == '\0' || *riadok == '\r')
				{
					fprintf(stderr, ERR_ITEM_COUNT);
					return 0;
				}
				riadok++;
			}

			char item1_name[100];
			char item2_name[100];

			j = 0;
			while (1)
			{
				item1_name[j] = *riadok;
				riadok++;
				j++;
				if (*riadok == ' ' || *riadok == '\0' || *riadok == '\r' || *riadok == '\t')
				{
					item1_name[j] = '\0';
					break;
				}
			}
			
			j = 0;
			while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r'|| *riadok == '\0')
			{
				if (*riadok == '\0' || *riadok == '\r')
				{
					item2_name[j] = '\0';
					j++;
					break;
				}
				riadok++;
			}
			
			if (j == 0)
			{
				while(1)
				{
					item2_name[j] = *riadok;
					riadok++;
					j++;
					if (*riadok == ' ' || *riadok == '\0' || *riadok == '\t' || *riadok == '\r')
					{
						item2_name[j] = '\0';
						break;
					}
				}

				while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r'|| *riadok == '\0')
				{
					if (*riadok == '\0' || *riadok == '\r')
					{
						break;
					}
					riadok++;
				}

				if (*riadok != '\0' && *riadok != '\r')
				{
					fprintf(stderr, ERR_ITEM_COUNT);
					return 0;
				}
			}

			int position1 = position_in_database(item1_name, items, n);
			
			if (position1 == -1)
			{
				fprintf(stderr, ERR_WRONG_ITEM);
				return 0;
			}
			else
			{
				units1[i].item1 = &items[position1];
			}

			if (item2_name[0] == '\0')
			{
				units1[i].item2 = NULL;
			}
			else
			{
				int position2 = position_in_database(item2_name, items, n);
				if (position2 == -1)
				{
					fprintf(stderr, ERR_WRONG_ITEM);
					return 0;
				}
				else
				{
					units1[i].item2 = &items[position2];
				}
			}
			
			if (units1[i].item2 != NULL)
			{
				int pocet = units1[i].item1 -> slots;
				pocet += units1[i].item2 -> slots;
				if (pocet > 2)
				{
					fprintf(stderr, ERR_SLOTS);
					return 0;
				}
			}
		}
		
		fclose(f);

		/////////////////////////////////////////////////////

		f = fopen(argv[2], "r");
		if (f == NULL)
		{
			fprintf(stderr, ERR_FILE);
			return 0;
		}

		riadok = readline(f);
		while (isdigit(*riadok) == 0)
		{
			riadok++;
		}
		int count2 = atoi(riadok);
		if (count2<1 || count2>5)
		{
			fprintf(stderr, ERR_UNIT_COUNT);
			return 0;
		}

		UNIT* units2 = (UNIT*) malloc(count2*sizeof(UNIT));
		
		for (int i = 0; i<count2; i++)
		{
			units2[i].hp = 100;

			riadok = readline(f);
			while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r')
			{
				riadok++;
			}

			int j = 0;
			while (1)
			{
				units2[i].name[j] = *riadok;
				j++;
				riadok++;
				if (*riadok == ' ' || *riadok == '\0' || *riadok == '\r' || *riadok == '\t')
				{
					units2[i].name[j] = '\0';
					break;
				}
			}

			while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r'|| *riadok == '\0')
			{
				if (*riadok == '\0' || *riadok == '\r')
				{
					fprintf(stderr, ERR_ITEM_COUNT);
					return 0;
				}
				riadok++;
			}

			char item1_name[100];
			char item2_name[100];

			j = 0;
			while (1)
			{
				item1_name[j] = *riadok;
				riadok++;
				j++;
				if (*riadok == ' ' || *riadok == '\0' || *riadok == '\r' || *riadok == '\t')
				{
					item1_name[j] = '\0';
					break;
				}
			}
			
			j = 0;
			while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r'|| *riadok == '\0')
			{
				if (*riadok == '\0' || *riadok == '\r')
				{
					item2_name[j] = '\0';
					j++;
					break;
				}
				riadok++;
			}

			if (j == 0)
			{
				while(1)
				{
					item2_name[j] = *riadok;
					riadok++;
					j++;
					if (*riadok == ' ' || *riadok == '\0' || *riadok == '\t' || *riadok == '\r')
					{
						item2_name[j] = '\0';
						break;
					}
				}

				while (*riadok == ' ' || *riadok == '\t' || *riadok == '\r'|| *riadok == '\0')
				{
					if (*riadok == '\0' || *riadok == '\r')
					{
						break;
					}
					riadok++;
				}

				if (*riadok != '\0' && *riadok != '\r')
				{
					fprintf(stderr, ERR_ITEM_COUNT);
					return 0;
				}
			}

			int position1 = position_in_database(item1_name, items, n);
			
			if (position1 == -1)
			{
				fprintf(stderr, ERR_WRONG_ITEM);
				return 0;
			}
			else
			{
				units2[i].item1 = &items[position1];
			}

			if (item2_name[0] == '\0')
			{
				units2[i].item2 = NULL;
			}
			else
			{
				int position2 = position_in_database(item2_name, items, n);
				if (position2 == -1)
				{
					fprintf(stderr, ERR_WRONG_ITEM);
					return 0;
				}
				else
				{
					units2[i].item2 = &items[position2];
				}
			}
			
			if (units2[i].item2 != NULL)
			{
				int pocet = units2[i].item1 -> slots;
				pocet += units2[i].item2 -> slots;
				if (pocet > 2)
				{
					fprintf(stderr, ERR_SLOTS);
					return 0;
				}
			}
		}
		
		fclose(f);

		printf("Army 1\n");
		print_army(units1, count1);
		printf("Army 2\n");
		print_army(units2, count2);

		free(units1);
		free(units2);
	}
	free(items);

	return 0;
}