#ifndef FUNCS_H
#define FUNCS_H

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

void print_menu(void);
void flushInputBuffer(void);
int getValidIntInput(const char *prompt, int min, int max);
float getValidFloatInput(const char *prompt, float min);
void cleanStringInput(char *str);
int continueToMenu(void);
int loadMealsFromFile(const char *filename);
int loadStaplesFromFile(const char *filename);
void clearWeekPlan(void);
void listMeals(void);
void getPlannerConfig(PlannerConfig *cfg);
float scoreMeal(const Meal *m, const PlannerConfig *cfg);
void generatePlan(PlannerConfig *cfg);
void printWeekPlan(void);
WeekStats calculateWeekStats(void);
void printWeekStats(void);
int countMealInPlan(int mealIndex);
float calculateTotalStapleUsage(const Staple *s);
void analyseStaples(void);
void saveWeekPlanToFile(const char *filename);
#endif