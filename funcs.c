#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "funcs.h"

// global data arrays 
Meal meals[MAX_MEALS];
int mealCount = 0;
WeekPlan currentPlan = { .numDays = 0 }; // Initialize empty plan
Staple staples[MAX_STAPLES];
int stapleCount = 0;

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

void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidIntInput(const char *prompt, int min, int max) {
    char buffer[100];
    long value;
    char *endptr;

    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            continue;
        }
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        value = strtol(buffer, &endptr, 10);
        while (*endptr != '\0' && isspace((unsigned char)*endptr)) {
            endptr++;
        }
        if (endptr == buffer || *endptr != '\0' || value < min || value > max) {
            printf("Invalid input. Please enter a whole number between %d and %d.\n",
                   min, max);
        } else {
            return (int)value;
        }
    }
}

float getValidFloatInput(const char *prompt, float min) {
    char buffer[100];
    float value;
    char *endptr;

    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            continue;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }


        value = strtof(buffer, &endptr);

        while (*endptr != '\0' && isspace((unsigned char)*endptr)) {
            endptr++;
        }
        if (endptr == buffer || *endptr != '\0' || value < min) {
            printf("Invalid input. Please enter a numerical value >= %.2f.\n", min);
        } else {
            return value;
        }
    }
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

// loads meals from meals.txt 
//Format: Name;Cost;Cals;Protein;Cat;Veg
int loadMealsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("ERROR: Could not open meals.txt file \n");
        return 0;
    }
    char line[MAX_NAME_LEN + 100];
    int linesRead = 0;
   
    while (mealCount < MAX_MEALS && fgets(line, sizeof(line), file)) {
        linesRead++;
        cleanStringInput(line);
        if (line[0] == '\0' || line[0] == '#') continue;
       
        Meal *m = &meals[mealCount];
        char tempLine[sizeof(line)];
        strcpy(tempLine, line);
        char *token;

        // Meal Name
        token = strtok(tempLine, ";");
        if (token) strncpy(m->mealName, token, MAX_NAME_LEN - 1); else continue;

        // Cost per serving 
        token = strtok(NULL, ";");
        if (token) m->CostPerServing = (float)atof(token); else continue;

        // total calories
        token = strtok(NULL, ";");
        if (token) m->TotalCalories = atoi(token); else continue;

        // protein intake in grams
        token = strtok(NULL, ";");
        if (token) m->ProteinIntakeGrams = atoi(token); else continue;

        // food category
        token = strtok(NULL, ";");
        if (token) m->FoodCategory = atoi(token); else continue;

        // is the meal vegetarian?
        token = strtok(NULL, ";");
        if (token) m->isVegetarian = atoi(token); else continue;
        mealCount++;
    }
    fclose(file);
    return mealCount;
}



//Loads staples from staples.txt file 
 //Format: Name;PackSize(kg/L);ServingsPerPack;PricePerPack;MealUsageCSV
int loadStaplesFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("WARNING: Could not open %s. Staple analysis will not work.\n", filename);
        return 0;
    }
   
    char line[MAX_MEAL_USAGE_LEN + MAX_NAME_LEN + 100];
   
    while (stapleCount < MAX_STAPLES && fgets(line, sizeof(line), file)) {
        cleanStringInput(line);
        if (line[0] == '\0' || line[0] == '#') continue;
        Staple *s = &staples[stapleCount];
        char tempLine[sizeof(line)];
        strcpy(tempLine, line);
        char *token;

        // staple name 
        token = strtok(tempLine, ";");
        if (token) strncpy(s->name, token, MAX_NAME_LEN - 1); else continue;
       
        // pack size 
        token = strtok(NULL, ";");
        if (token) s->packSize = (float)atof(token); else continue;

        // number of servings per pack
        token = strtok(NULL, ";");
        if (token) s->servingsPerPack = (float)atof(token); else continue;

        // price per pack 
        token = strtok(NULL, ";");
        if (token) s->pricePerPack = (float)atof(token); else continue;

        //which meals use which staples
        token = strtok(NULL, ";");
        if (token) strncpy(s->mealUsageStr, token, MAX_MEAL_USAGE_LEN - 1); else {
            s->mealUsageStr[0] = '\0';
        }
        stapleCount++;
    }
    fclose(file);
    return stapleCount;
}

//resets the current weekly meal plan 
void clearWeekPlan(void) {
    currentPlan.numDays = 0;
    for (int i = 0; i < MAX_DAYS; i++) {
        currentPlan.days[i].numMeals = 0;
        currentPlan.days[i].dayCost = 0.0f;
    }
}

//menu option 1 (list meals)
void listMeals(void) {
    if (mealCount == 0) {
        printf("\n There are no meals in the database \n");
        return;
    }

    printf("\n Meal List (%d meals) \n", mealCount);
    printf("ID | Name %*s| Cost | Cals | Prot | Cat | Veg\n", MAX_NAME_LEN - 4, " ");
    
    for (int i = 0; i < mealCount; i++) {
        printf("%-2d | %-*s | %5.2f | %4d | %4d | %4d | %s\n", i,
               MAX_NAME_LEN, meals[i].mealName,
               meals[i].CostPerServing,
               meals[i].TotalCalories,
               meals[i].ProteinIntakeGrams,
               meals[i].FoodCategory,
               meals[i].isVegetarian == 1 ? "Yes" : "No");
    }
}
//Generate weekly plan 
//get user information 
void getPlannerConfig(PlannerConfig *cfg) {
    printf("\n--- Weekly Plan Configuration ---\n");
    cfg->weeklyBudget = getValidFloatInput("Weekly budget: £", 0.0f);
    cfg->daysToPlan = getValidIntInput("Number of days to plan (1-7): ", 1, MAX_DAYS);
    cfg->mealsPerDay = getValidIntInput("Meals per day (1-3): ", 1, MAX_MEALS_PER_DAY);
    cfg->targetCaloriesPerDay = getValidIntInput("Target calories per day: ", 500, 5000);
    cfg->targetProteinPerDay = getValidIntInput("Target protein per day (grams): ", 10, 500);
    cfg->vegetarianMode = getValidIntInput("Vegetarian mode (0=No, 1=Yes): ", 0, 1);
    cfg->maxRepeatsPerMeal = getValidIntInput("Max repeats per meal within the week (1-7): ", 1, 7);
}
// score the meals (the higher the better)
float scoreMeal(const Meal *m, const PlannerConfig *cfg) {
    float score = 1000.0f;

    // Calculate ideal nutrient targets per meal
    float idealCals = (float)cfg->targetCaloriesPerDay / cfg->mealsPerDay;
    float dailyBudgetShare = cfg->weeklyBudget / cfg->daysToPlan;

    // calculate calorie difference 
    float calDiff = fabsf((float)m->TotalCalories - idealCals);
    score -= calDiff * 0.5f;

    //reward higher protein content
    score += (float)m->ProteinIntakeGrams * 5.0f;

    // penalty for higher cost 
    score -= (m->CostPerServing / dailyBudgetShare) * 500.0f;

    // penalty for non-vegetarian meals if the vegetarian meal option is selected 
    if (cfg->vegetarianMode == 1 && m->isVegetarian == 0) {
        score -= 5000.0f;
    }
    return score;
}
// Builds weekly plan
void generatePlan(PlannerConfig *cfg) {
    if (mealCount == 0) { printf("Sorry, cannot generate a plan\n");
         return; }
   
    clearWeekPlan();
    float scores[MAX_MEALS];
    int mealOrder[MAX_MEALS];
    int mealRepeats[MAX_MEALS] = {0}; 

    for (int i = 0; i < mealCount; i++) {
        scores[i] = scoreMeal(&meals[i], cfg);
        mealOrder[i] = i;
    }
    //Sort
    for (int i = 0; i < mealCount - 1; i++) {
        for (int j = 0; j < mealCount - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                float tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;
                int tempIndex = mealOrder[j];
                mealOrder[j] = mealOrder[j + 1];
                mealOrder[j + 1] = tempIndex;
            }
        }
    }
    //track weekly budget liit and how much has been spent so far
    float weeklyBudgetLimit = cfg->weeklyBudget;
    float currentWeeklyCost = 0.0f;
    int topPoolSize = mealCount / 2;
    if (topPoolSize < 3) topPoolSize = mealCount;
    if (topPoolSize > mealCount) topPoolSize = mealCount;

    for (int day = 0; day < cfg->daysToPlan; day++) {
        DayPlan *dp = &currentPlan.days[day];
        int mealsUsedToday[MAX_MEALS] = {0};
        for (int meal = 0; meal < cfg->mealsPerDay; meal++) {
            int mealAdded = 0;
           //random selection
            const int MAX_ATTEMPTS = 50;
            for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {              
                int rand_pool_index = rand() % topPoolSize;
                int mealIndex = mealOrder[rand_pool_index];
                const Meal *m = &meals[mealIndex];
               
                //constraints
                if (currentWeeklyCost + m->CostPerServing > weeklyBudgetLimit * 1.5f) continue;
                if (mealRepeats[mealIndex] >= cfg->maxRepeatsPerMeal) continue;
                if (mealsUsedToday[mealIndex] > 0) continue;
               
                //checks if a meal is suitable 
                dp->mealIndices[dp->numMeals] = mealIndex;
                dp->numMeals++;
                dp->dayCost += m->CostPerServing;
                currentWeeklyCost += m->CostPerServing;             
                mealRepeats[mealIndex]++;  
                mealsUsedToday[mealIndex]++;  
                mealAdded = 1;
                break;
            }
           
            if (!mealAdded) {
                for (int i = 0; i < mealCount; i++) {
                     int mealIndex = mealOrder[i];
                     const Meal *m = &meals[mealIndex];

                     if (currentWeeklyCost + m->CostPerServing > weeklyBudgetLimit * 1.5f) continue;
                     if (mealRepeats[mealIndex] >= cfg->maxRepeatsPerMeal) continue;
                     if (mealsUsedToday[mealIndex] > 0) continue;

                     dp->mealIndices[dp->numMeals] = mealIndex;
                     dp->numMeals++;
                     dp->dayCost += m->CostPerServing;
                     currentWeeklyCost += m->CostPerServing;
                     mealRepeats[mealIndex]++;
                     mealsUsedToday[mealIndex]++;
                     mealAdded = 1;
                     break;
                }
            }
        }
    }
    currentPlan.numDays = cfg->daysToPlan;
    printf("\n Weekly Plan generated successfully!\n");
}
//prints the weekly plan
void printWeekPlan(void) {
    if (currentPlan.numDays == 0) {
        printf("\n No weekly plan exists.\n");
        return;
    }
    printf("\n Generated Weekly Plan (%d Days)\n", currentPlan.numDays);
   
    for (int i = 0; i < currentPlan.numDays; i++) {
        DayPlan *dp = &currentPlan.days[i];
        printf("\nDay %d (Cost: £%.2f, Meals: %d):\n", i + 1, dp->dayCost, dp->numMeals);
        if (dp->numMeals == 0) {
            printf("No meals planned for this day.\n");
        }
        for (int j = 0; j < dp->numMeals; j++) {
            const Meal *m = &meals[dp->mealIndices[j]];
            printf("  - %s (Original Cost: £%.2f, %d kcal, %d g protein)\n",
                   m->mealName, m->CostPerServing, m->TotalCalories, m->ProteinIntakeGrams);
        }
    }
}
//menu option 3 
WeekStats calculateWeekStats(void) {
    WeekStats stats = { 0 };
    int totalDays = currentPlan.numDays;
    int totalCaloriesSum = 0;
    int totalProteinSum = 0;

    for (int i = 0; i < totalDays; i++) {
        DayPlan *dp = &currentPlan.days[i];
        stats.totalCost += dp->dayCost;
        for (int j = 0; j < dp->numMeals; j++) {
            const Meal *m = &meals[dp->mealIndices[j]];
            stats.totalMeals++;
            totalCaloriesSum += m->TotalCalories;
            totalProteinSum += m->ProteinIntakeGrams;
            if (m->isVegetarian == 1) stats.vegetarianMeals++;
            if (m->FoodCategory >= 1 && m->FoodCategory <= MAX_DAYS) {
                stats.categoryCount[m->FoodCategory]++;
            }
        }
    }
    if (totalDays > 0) {
        stats.avgDailyCalories = (float)totalCaloriesSum / totalDays;
        stats.avgDailyProtein = (float)totalProteinSum / totalDays;
    }
    return stats;
}
// prints the statistics 
void printWeekStats(void) {
    if (currentPlan.numDays == 0) {
        printf("\n No plan generated. Please generate one first using Menu 2 \n");
        return;
    }
    WeekStats stats = calculateWeekStats(); 
    printf("\n Weekly Statistics Report \n");
    printf("Total Days Planned: %d\n", currentPlan.numDays);
    printf("Total Meals Consumed: %d\n", stats.totalMeals);
    printf("--------------------------------\n");
    printf("Total Weekly Cost: £%.2f \n", stats.totalCost);
    printf("Average Daily Calories: %.1f kcal\n", stats.avgDailyCalories);
    printf("Average Daily Protein: %.1f g\n", stats.avgDailyProtein);

    // Vegetarian stats
    float vegPercent = (stats.totalMeals > 0) ?
                       ((float)stats.vegetarianMeals / stats.totalMeals) * 100.0f : 0.0f;
    printf("Vegetarian Meals: %d (%.1f%% of total)\n", stats.vegetarianMeals, vegPercent);
}

//staples and cost sharing 
int countMealInPlan(int mealIndex) {
    int count = 0;
    for (int i = 0; i < currentPlan.numDays; i++) {
        DayPlan *dp = &currentPlan.days[i];
        for (int j = 0; j < dp->numMeals; j++) {
            if (dp->mealIndices[j] == mealIndex) {
                count++;
            }
        }
    }
    return count;
}
//calculate staple useage in meals 
float calculateTotalStapleUsage(const Staple *s) {
    if (s->mealUsageStr[0] == '\0') {
        return 0.0f;
    }
    float totalUsage = 0.0f;
    char tempStr[MAX_MEAL_USAGE_LEN];
    strncpy(tempStr, s->mealUsageStr, MAX_MEAL_USAGE_LEN - 1);
    tempStr[MAX_MEAL_USAGE_LEN - 1] = '\0';
    //separate with comma 
    char *entry = strtok(tempStr, ",");
    while (entry != NULL) {
        char *colon = strchr(entry, ':');
        if (colon) {
            *colon = '\0';
            int mealIndex = atoi(entry);
            float servingsPerMeal = atof(colon + 1);
            if (mealIndex >= 0 && mealIndex < mealCount) {
               //check how many tims this meal appears
                int mealCountInPlan = countMealInPlan(mealIndex);
                totalUsage += servingsPerMeal * mealCountInPlan;
            }
        }
        //move on to the next entry
        entry = strtok(NULL, ",");
    }
    return totalUsage;
}
//analysis
void analyseStaples(void) {
    if (currentPlan.numDays == 0) {
        printf("\n Error: Please generate a plan first in Menu 2 \n");
        return;
    }
    if (stapleCount == 0) {
        printf("\n Error: Staple data not loaded \n");
        return;
    }

    int numPeople = getValidIntInput(" \nHow many people are sharing staples (Yourself + flatmates)? ", 1, 10);
    float totalStapleCost = 0.0f;
    printf("\n Staple Cost Sharing Report: \n");
    printf("Sharing Group Size: %d people\n", numPeople);
    printf("Staple Name | Total Usage (Servings) | Packs Needed | Total Cost | Bulk CPS \n");
    printf("----------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < stapleCount; i++) {
        float packsNeeded = 0.0f;
        float stapleCost = 0.0f;
        float bulkCostPerServing = 0.0f;
        float singlePersonUsage = calculateTotalStapleUsage(&staples[i]);
        float groupUsage = singlePersonUsage * numPeople;

        if (staples[i].servingsPerPack > 0) {
            bulkCostPerServing = staples[i].pricePerPack / staples[i].servingsPerPack;
            packsNeeded = ceil(groupUsage / staples[i].servingsPerPack);
            if (groupUsage > 0 && packsNeeded < 1.0f) packsNeeded = 1.0f;
            stapleCost = packsNeeded * staples[i].pricePerPack;
            totalStapleCost += stapleCost;
        }
        printf("%-11s | %22.1f | %12.0f | %18.2f | %16.2f\n", staples[i].name, groupUsage, packsNeeded, stapleCost, bulkCostPerServing);
    }
    float costPerPerson = (numPeople > 0) ? totalStapleCost / numPeople : 0.0f;
    printf("TOTAL STAPLE COST (For all %d people): £%18.2f \n", numPeople, totalStapleCost);
    printf("COST PER PERSON: £%18.2f \n", costPerPerson);
}
//save the week plan to a text file
void saveWeekPlanToFile(const char *filename) {
    if (currentPlan.numDays == 0) {
        printf("\n There is no plan to save. Please geernerate one first using menu option 2 \n");
        return;
    } 
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error: Could not save weekplan.txt");
        return;
    }
    fprintf(file, "Weekly Meal Plan Report\n");   
    for (int i = 0; i < currentPlan.numDays; i++) {
        DayPlan *dp = &currentPlan.days[i];
        fprintf(file, "\nDay %d (Total Cost: £%.2f):\n", i + 1, dp->dayCost);
        for (int j = 0; j < dp->numMeals; j++) {
            const Meal *m = &meals[dp->mealIndices[j]];
            fprintf(file, "- %s (Original Cost: £%.2f, %d kcal, %d g protein)\n",
                    m->mealName, m->CostPerServing, m->TotalCalories, m->ProteinIntakeGrams);
        }
    }
    fclose(file);
    printf("\n Week plan saved successfully to %s.\n", filename);
}