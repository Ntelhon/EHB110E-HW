// Furkan Karabulut
// Electronics and Communication Engineering
// EHB110E CRN:10628 Homework-1
// 11/11/2021
// https://github.com/ntelhon

// Standard input-output file
#include <stdio.h>

// Standard library file
#include <stdlib.h>

// Time file for clock_t() class
#include <time.h>

// The functions to be used by program
void numSelect();
void method1(double num1, double num2);
void method2(double num1);
void test();

// Global variables
double num = -1;    // User-entered number.
int mode = -1;      // Mode of app.
int error = 0;      // The variable to taking "1" value when an unexpected char entered.

int main(){
    while (1)
    {
        mode = -1;

        printf("\n\n\n\nHello, I am Furkan Karabulut and that is my EHB110E Homework-1!\nPlease select what you want to do");
        printf("\n(1) Finding square-root with methot 1");
        printf("\n(2) Finding square-root with methot 2");
        printf("\n(3) Speed test of 1 and 2 methots\n");

        // If block that is activating when a "scanf" error occurs.
        if(error){
            char text[100];
            gets(text);
            error = 0;
        }

        // The app reads the mode type
        scanf("%d", &mode);

        // If user select one of methods, the app redirects to "numSelect" function.
        if(mode == 1 || mode == 2){
            numSelect();
        }

        if(mode == 3)
        {
            test();
        }
        
        // If the user enters a non-numeric char, there is an error occur and next time "scanf" method is not working. To fix that, the app will call "gets" method to "scanf" starts working.
        if(mode <= 0){
            printf("\nPlease select one of the modes: 1, 2 or 3!");
            error = 1;
        }
    }
}

void numSelect(){
    while(1){
        num = -1;
        printf("Please enter a positive real number: ");

        // The app reads the num.
        scanf("%lf", &num);

        // If the user enters a non-numeric char, there is an error occur and next time "scanf" method is not working. To fix that, the app redirects the user to "main" function and make "error" variable "1".
        if(num < 0){
            printf("\nInput error! Try Again.");
            error = 1;
            break;
        }

        // If "num = 0" both methods will not work. Hence there is an exception block for "0" input.
        else if(num == 0) printf("\nThe answer is: %lf \n\n\n\n", num);

        // For other inputs, the functions are called with "num" variable.
        else if(mode == 1) method1(0, num);
        else if(mode == 2) method2(num);
    }
}

void method1(double num1, double num2){

    // Number array for the "X-axis" points.
    double Xpoints[11];

    // Number array for the corresponding "Y-axis" points of "X-axis" points. 
    double Ypoints[11];

    // The loop to divide interval of "num1" to "num2" into 10 equal intervals, and find corresponding y-points of that points.
    for(int i=0; i<11; i++){
        Xpoints[i] = num1 + (num2-num1) * i * 0.1;
        Ypoints[i] = (Xpoints[i] * Xpoints[i]) - num;
    }

    // The loop to check all "Ypoints".
    for(int i=0; i<11; i++){

        // If "Ypoint" is equal to "0" thats mean we find to root.
        if(Ypoints[i] == 0){
            if(mode != 3){ // This is for checking test mode, because in test mode the answer must not print.
                printf("\nThe answer is: %lf", Xpoints[i]);
                printf("\n\n\n\n");
            }
            break;
        }

        // If statement to checking signs of numbers. If the signs of consecutive two numbers are different, it will be true.
        else if((i != 0) && (Ypoints[i] > 0 != Ypoints[i-1] > 0)){

            // If the difference between two consecutive numbers is less than 0.0001, write first number.
            if(Ypoints[i] - Ypoints[i-1] < 0.0001){
                if(mode != 3){
                    printf("\nThe answer is: %lf", Xpoints[i-1]);
                    printf("\n\n\n\n");
                }
                break;
            }

            // If interval is bigger than our acceptable range, call the function again with new variables.
            else{
                method1(Xpoints[i-1], Xpoints[i]);
            }
        }
    }

}

void method2(double num1){
    // Find "num2" with Newton-Raphson methot.
    double num2 = num1 - ((num1 * num1) - num) * 0.5 / num1;

    // If the difference of "num1" and "num2" is less than our acceptable range, the answer is "num1".
    if( ((num1 > num2) && (num1 - num2 < 0.0001)) || ((num1 < num2) && (num2 - num1 < 0.0001)) || (num1 == num2) ){
        if(mode != 3){ // This is for checking test mode, because in test mode the answer must not print.
            printf("\nThe answer is: %lf", num1);
            printf("\n\n\n\n");
        }
    }
    
    // If interval is bigger than our acceptable range, call the function again with num2.
    else method2(num2);
}

void test(){
    // The "clock_t" variables.
    clock_t t1i, t1f, t2i, t2f;
    int executionTime1, executionTime2;

    // Start time is saving into "t1i" and after the calling function with all integers up to 100000 end time saving into "t1f".
    t1i = clock();
    for (int i = 1; i < 100000; i++)
    {
        num = i;
        method1(0, i);
    }
    t1f = clock();

    // Start time is saving into "t2i" and after the calling function with all integers up to 100000 end time saving into "t2f"
    t2i = clock();
    for (int i = 1; i < 100000; i++)
    {
        num = i;
        method2(i);
    }
    t2f = clock();

    // The consumed times by method-1 and method-2 are calculated.
    executionTime1 = (int)(t1f - t1i);
    executionTime2 = (int)(t2f - t2i);

    printf("\nTotal time for Method1: %d", executionTime1);
    printf("\nTotal time for Method2: %d", executionTime2);
}