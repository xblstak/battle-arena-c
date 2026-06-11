#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

// chybove hlasenia
#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"
#define ERR_SLOTS "ERR_SLOTS"

#define MMAX 1000
const char* readline()
{
    static char buffer[MMAX+1];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

int position_in_database(char item[])
{
    for (int i = 0; i < NUMBER_OF_ITEMS; i++)
    {
        
        if (strcmp(item, items[i].name) == 0)
        {
            return i;
        }  
    }
    return -1;
}

int create_army(UNIT army[])
{
    const char *riadok = readline();
    int n = atoi(riadok);
    if (n<1 || n>5)
    {
        printf("%s\n", ERR_UNIT_COUNT);
        return 1;
    }
    
    for (int i = 0; i < n; i++)
    {
        army[i].hp = 100;

        riadok = readline();

        int j = 0;
        while (1)
        {
            army[i].name[j] = *riadok;
            riadok++;
            j++;
            if (*riadok == ' ' || *riadok == '\0')
            {
                break;
            }  
        }
        if (*riadok == '\0')
        {
            printf("%s\n", ERR_ITEM_COUNT);
            return 1;
        }
        riadok++;

        char item1_name[100];
        char item2_name[100];

        j = 0;
        while (1)
        {
            item1_name[j] = *riadok;
            riadok++;
            j++;
            if (*riadok == ' ' || *riadok == '\0')
            {
                item1_name[j] = '\0';
                break;  
            }
        }

        j = 0;
        if (*riadok == '\0')
        {
            item2_name[j] = '\0';
        }
        else
        {
            riadok++;
            while (1)
            {
                item2_name[j] = *riadok;
                riadok++;
                j++;
                if (*riadok == ' ' || *riadok == '\0')
                {
                    item2_name[j] = '\0';
                    break;  
                }
            }
            if (*riadok != '\0')
            {
                printf("%s\n", ERR_ITEM_COUNT);
                return 1;
            }
        }

        int position1 = position_in_database(item1_name);
        if (position1 == -1)
        {
            printf("%s\n", ERR_WRONG_ITEM);
            return 1;
        }
        else
        {
            army[i].item1 = &items[position1];
        }

        if (item2_name[0] == '\0')
        {
            army[i].item2 = NULL;
        }
        else
        {
            int position2 = position_in_database(item2_name);
            if (position2 == -1)
            {
                printf("%s\n", ERR_WRONG_ITEM);
                return 1;
            }
            else
            {
                army[i].item2 = &items[position2];
            }
        }
        
        if (army[i].item2 != NULL)
        {
            int pocet = army[i].item1 -> slots;
            pocet += army[i].item2 -> slots;
            if (pocet > 2)
            {
                printf("%s\n", ERR_SLOTS);
                return 1;
            }
        }
    }

    return 0;
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
        if (i<n-1)
        {
            printf("\n");
        }
    }
}

int is_game_over(UNIT army1[], UNIT army2[])
{
    int j = 0;
    for (int i = 0; i < 5; i++)
    {
        if (army1[i].hp <= 0)
        {
            j++;
        }  
    }

    int k = 0;
    for (int i = 0; i < 5; i++)
    {
        if (army2[i].hp <= 0)
        {
            k++;
        }  
    }

    if (j == 5 && k == 5)
    {
        return 2;
    }
    else if (j == 5)
    {
        return 1;
    }
    else if (k == 5)
    {
        return -1;
    }
    
    return 0;
}

void print_units(UNIT army1[], UNIT army2[])
{
    printf("1: ");
    for (int i = 0; army1[i].hp > 0 && i < 5; i++)
    {
        printf("%s,%d ", army1[i].name, army1[i].hp);
    }
    printf("\n");

    printf("2: ");
    for (int i = 0; army2[i].hp > 0 && i < 5; i++)
    {
        printf("%s,%d ", army2[i].name, army2[i].hp);
    }
    printf("\n");
}

int maximum(int a, int b)
{
    return a<b ? b : a;
}

int update(UNIT army[])
{
    UNIT tmp[5];
    for (int i = 0; i < 5; i++)
    {
        tmp[i] = army[i];
    }

    int j = 0;
    for (int i = 0; i < 5; i++)
    {
        if (tmp[i].hp>0)
        {
            army[j] = tmp[i];
            j++;
        }
    }

    for (j; j < 5; j++)
    {
        army[j].hp = 0;
    }
    
    int k = 0;
    for (int i = 0; i < 5; i++)
    {
        if (army[i].hp > 0)
        {
            k++;
        }  
    }

    return k;
}

void attack_army_n_vs_k(UNIT attacker[], UNIT victim[], int army_num, int a, int b)
{
    for (int i = 0; i < a; i++)
    {
        int dmg_itm1 = 0, dmg_itm2 = 0, def_total = 0;

        def_total += victim[0].item1->def;
        if (victim[0].item2 != NULL)
        {
            def_total += victim[0].item2->def;
        }

        if (i<=attacker[i].item1->range)
        {
            dmg_itm1 += maximum(1, attacker[i].item1->att - def_total);
            victim[0].hp -= dmg_itm1;
            printf("%d,%s,%s:",army_num, attacker[i].name, attacker[i].item1->name);
            printf("\t\t[%s,%d]", victim[0].name, dmg_itm1);

            int radius_itm1 = attacker[i].item1->radius;
            if (radius_itm1>0)
            {
                int j = 1;
                while (j<=(b-1) && j<=radius_itm1)
                {
                    int def = victim[j].item1->def;
                    if (victim[j].item2 != NULL)
                    {
                        def += victim[j].item2->def;
                    }

                    int dmg = maximum(1, attacker[i].item1->att - def);
                    victim[j].hp -= dmg;
                    printf(" [%s,%d]", victim[j].name, dmg);
                    j++;
                }
            }
            printf("\n");
        }

        if (attacker[i].item2 != NULL)
        {
            if (i<=attacker[i].item2->range)
            {
                dmg_itm2 += maximum(1, attacker[i].item2->att - def_total);
                victim[0].hp -= dmg_itm2;
                printf("%d,%s,%s:",army_num, attacker[i].name, attacker[i].item2->name);
                printf("\t\t[%s,%d]", victim[0].name, dmg_itm2);

                int radius_itm2 = attacker[i].item2->radius;
                if (radius_itm2>0)
                {
                    int m = 1;
                    while (m<=(b-1) && m<=radius_itm2)
                    {
                        int def = victim[m].item1->def;
                        if (victim[m].item2 != NULL)
                        {
                            def += victim[m].item2->def;
                        }

                        int dmg = maximum(1, attacker[i].item2->att - def);
                        victim[m].hp -= dmg;
                        printf(" [%s,%d]", victim[m].name, dmg);
                        m++;
                    }
                }
                printf("\n"); 
            }
        }
    }
}

void attack_army(UNIT army1[], UNIT army2[], int attacker)
{
    int dmg_total = 0, dmg_itm1 = 0, dmg_itm2 = 0, def_total = 0;

    def_total += army2[0].item1->def;
    if (army2[0].item2 != NULL)
    {
        def_total += army2[0].item2->def;
    }

    if (0<=army1[0].item1->range)
    {
        dmg_itm1 += maximum(1, army1[0].item1->att - def_total);
        dmg_total += dmg_itm1;
        printf("%d,%s,%s:",attacker, army1[0].name, army1[0].item1->name);
        printf("\t\t[%s,%d]\n", army2[0].name, dmg_itm1);
    }

    if (army1[0].item2 != NULL)
    {
        if (0<=army1[0].item2->range)
        {
            dmg_itm2 += maximum(1, army1[0].item2->att - def_total);
            dmg_total += dmg_itm2;
            printf("%d,%s,%s:",attacker, army1[0].name, army1[0].item2->name);
            printf("\t\t[%s,%d]\n", army2[0].name, dmg_itm2);
        }
    }

    army2[0].hp -= dmg_total;
}

void battle_n_vs_k(UNIT army1[], UNIT army2[], int a, int b)
{
    int round = 1;
    int over = 0;
    while (1)
    {
        over = is_game_over(army1, army2);
        if (over == 2 || over == 1 || over == -1)
        {
            break;
        }
        
        printf("Round %d\n", round);
        round++;

        print_units(army1, army2);
        attack_army_n_vs_k(army1, army2, 1, a, b);
        attack_army_n_vs_k(army2, army1, 2, b, a);
        a = update(army1);
        b = update(army2);
        print_units(army1, army2);
        printf("\n");
    }
    if (over == 2)
    {
        printf("NO WINNER");
    }
    else if (over == 1)
    {
        printf("WINNER: 2");
    } 
    else if (over == -1)
    {
        printf("WINNER: 1");
    }
}

void battle_n_vs_k_w_parameter(UNIT army1[], UNIT army2[], int a, int b, int n)
{
    int round = 1;
    int over = 0;
    for (int i = 0; i < n; i++)
    {
        over = is_game_over(army1, army2);
        if (over == 2 || over == 1 || over == -1)
        {
            break;
        }
        
        printf("Round %d\n", round);
        round++;

        print_units(army1, army2);
        attack_army_n_vs_k(army1, army2, 1, a, b);
        attack_army_n_vs_k(army2, army1, 2, b, a);
        a = update(army1);
        b = update(army2);
        print_units(army1, army2);
        printf("\n");
    }
    over = is_game_over(army1, army2);
    if (over == 2)
    {
        printf("NO WINNER");
    }
    else if (over == 1)
    {
        printf("WINNER: 2");
    } 
    else if (over == -1)
    {
        printf("WINNER: 1");
    }
}

void battle_1vs1(UNIT army1[], UNIT army2[])
{
    int round = 1;
    int over = 0;
    while (1)
    {
        over = is_game_over(army1, army2);
        if (over == 2 || over == 1 || over == -1)
        {
            break;
        }
        
        printf("Round %d\n", round);
        round++;

        print_units(army1, army2);
        attack_army(army1, army2, 1);
        attack_army(army2, army1, 2);
        print_units(army1, army2);
        printf("\n");
    }
    if (over == 2)
    {
        printf("NO WINNER");
    }
    else if (over == 1)
    {
        printf("WINNER: 2");
    } 
    else if (over == -1)
    {
        printf("WINNER: 1");
    }
}

int main(const int argc, char *argv[])
{

    static UNIT army1[5];
    static UNIT army2[5];

    for (int i = 0; i < 5; i++)
    {
        army1[i].hp = 0;
        army2[i].hp = 0;
    }
    
    if (create_army(army1) || create_army(army2))
    {
        return 0;
    }
    
    int a = 1;
    while (army1[a].hp == 100 && a<5)
    {
        a++;
    }

    int b = 1;
    while (army2[b].hp == 100 && b<5)
    {
        b++;
    }
    
    if (argc == 2)
    {
        int n = atoi(argv[1]);
        if (n == 0)
        {
            printf("Army 1\n");
            print_army(army1,a);
            printf("\nArmy 2\n");
            print_army(army2,b);
        }
        else
        {
            printf("Army 1\n");
            print_army(army1,a);
            printf("\nArmy 2\n");
            print_army(army2,b);
            printf("\n");
            battle_n_vs_k_w_parameter(army1, army2, a, b, n);
        }
    }
    else if (argc == 1)
    {
        if (a==1 && b ==1)
        {
            printf("Army 1\n");
            print_army(army1,a);
            printf("\nArmy 2\n");
            print_army(army2,b);
            printf("\n");
            battle_1vs1(army1, army2);
        }
        else
        {
            printf("Army 1\n");
            print_army(army1,a);
            printf("\nArmy 2\n");
            print_army(army2,b);
            printf("\n");
            battle_n_vs_k(army1, army2, a, b);
        }
    }

    return 0;
}