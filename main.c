#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 
#include <time.h> 
#include "funcs.h"

int main(void) {
    printf("--- Welcome to your Nigerian Meal Planner!!! ---\n");
    int mealCount = loadMealsFromFile("meals.txt");
    printf("All meals have now been loaded! %d.\n", mealCount);
    int stapleCount = loadStaplesFromFile("staples.txt");
    printf("All staple ingredients have also now been loaded! %d.\n", stapleCount);
    clearWeekPlan();
    //random number generator 
    srand(time(NULL));
    // check if there are meals 
    if (mealCount == 0) {
        printf("\n ERROR: There are no meals :( \n");
        return 1;
    }
    int choice;
    int runMenu = 1;
    PlannerConfig plannerConfig = {0};
    // Main Menu Loop
    while (runMenu) {
        print_menu();
        choice = getValidIntInput("Enter your choice (1-6): ", 1, 6);
        switch (choice) {
            case 1:
                listMeals();
                break;
            case 2:
                getPlannerConfig(&plannerConfig);
                generatePlan(&plannerConfig);
                printWeekPlan();
                break;
            case 3:
                printWeekStats();
                break;
            case 4:
                analyseStaples();
                break;
            case 5:
                saveWeekPlanToFile("weekplan.txt");
                break;
            case 6:
                runMenu = 0;
                break;
        }
       
        // continue if the user does not want to exit 
        if (runMenu) {
            runMenu = continueToMenu();
        }
    }
    printf("\n Thank you for using my meal planner! See you next time! \n");
    return 0;
}