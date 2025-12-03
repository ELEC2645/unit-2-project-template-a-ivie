#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <ctype.h>

void print_menu(void) { 
    printf("---------------------MENU: -------------------\n");
    printf("|1. List/ Add/ Edit Meals                    |\n"
           "|2. Generate Weekly Plan                     |\n" 
           "|3. Show your statistics                     |\n"
           "|4. Your staples and cost sharing analysis   |\n"
           "|5. Save your current plan to file           |\n"
           "|6. Exit and Save Data                       |\n"
           "---------------------------------------------- \n"); 
    }

    int main(void) { 
    print_menu(); 
    return 0; 
}