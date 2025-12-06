#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 
#include <time.h> 

//print main menu 
void print_menu(void) {
    printf("\n--------------------MENU:------------------\n");
    printf("| 1. List Available Meals                   |\n");
    printf("| 2. Generate Weekly Plan                   |\n");
    printf("| 3. Show your statistics                   |\n");
    printf("| 4. Your staples and cost sharing analysis |\n");
    printf("| 5. Save your current plan to file         |\n");
    printf("| 6. Exit Program                           |\n");
    printf("--------------------------------------------\n");
}

int main(void) {
    print_menu(); 
    return 0; 
}

//define constants 
#define MAX_MEALS 50
#define MAX_NAME_LEN 128
#define MAX_DAYS 7
#define MAX_MEALS_PER_DAY 3
#define MAX_STAPLES 10
#define MAX_MEAL_USAGE_LEN 512

// meal structure 
typedef struct { 
    char mealName[MAX_NAME_LEN]; 
    float CostPerServing; 
    int TotalCalories; 
    int ProteinIntakeGrams; 
    int FoodCategory; 
    int isVegetarian; 
} Meal; 

// daily plan 
