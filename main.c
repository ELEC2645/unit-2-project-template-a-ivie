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

// daily plan structure
typedef struct {
    int mealIndices[MAX_MEALS_PER_DAY]; 
    int numMeals;
    float dayCost;
} DayPlan;

// Weekly plan structure 
typedef struct {
    DayPlan days[MAX_DAYS];
    int numDays;
} WeekPlan;

// staples structure 
// Order: Name;PackSize(kg/L);ServingsPerPack;PricePerPack;MealUsageCSV
typedef struct {
    char name[MAX_NAME_LEN];
    float packSize;          
    float servingsPerPack;  
    float pricePerPack;      
    char mealUsageStr[MAX_MEAL_USAGE_LEN]; // New: CSV string of usage, e.g., "0:1.0,3:0.1"
} Staple;

//plan structure 
typedef struct {
    float weeklyBudget;
    int daysToPlan;
    int mealsPerDay;
    int targetCaloriesPerDay;
    int targetProteinPerDay;
    int vegetarianMode;
    int maxRepeatsPerMeal;
} PlannerConfig;

// weekly statistics structure 
typedef struct {
    float totalCost;
    float avgDailyCalories;
    float avgDailyProtein;
    int categoryCount[MAX_DAYS + 1];
    int totalMeals;
    int vegetarianMeals;
} WeekStats;

// global data arrays 
Meal meals[MAX_MEALS];
int mealCount = 0;
WeekPlan currentPlan = { .numDays = 0 }; // Initialize empty plan
Staple staples[MAX_STAPLES];
int stapleCount = 0;

void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidIntInput(const char *prompt, int min, int max) {
    int value;
    int inputStatus;
    do {
        printf("%s", prompt);
        inputStatus = scanf("%d", &value);
        flushInputBuffer();
        if (inputStatus != 1 || value < min || value > max) {
            printf("Invalid input. Please enter a whole number between %d and %d.\n", min, max);
            inputStatus = 0;
        }
    } while (inputStatus != 1);
    return value;
}

float getValidFloatInput(const char *prompt, float min) {
    float value;
    int inputStatus;
    do {
        printf("%s", prompt);
        inputStatus = scanf("%f", &value);
        flushInputBuffer();
        if (inputStatus != 1 || value < min) {
            printf("Invalid input. Please enter a numerical value >= %.2f.\n", min);
            inputStatus = 0;
        }
    } while (inputStatus != 1);
    return value;
}

void cleanStringInput(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
    if (len > 1 && str[len - 2] == '\r') str[len - 2] = '\0';
}

//prompts the user to continue or exit 
int continueToMenu() {
    char response[5];
    printf("\n Press enter to continue, or q to quit: ");
    if (fgets(response, sizeof(response), stdin) == NULL) {
        return 0; // Error reading, treat as quit
    }

// makes sure that when the user enters q it allows them to quit 
    cleanStringInput(response);
    if (tolower(response[0]) == 'q') {
        return 0; // Quit
    }
    return 1; // Continue
}


