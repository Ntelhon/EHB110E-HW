// Furkan Karabulut
// Electronics and Communication Engineering
// EHB110E CRN:10628 Homework-2
// 31/12/2021
// https://github.com/ntelhon



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// arrays initilazing number
#define MAX 10486

// initilaze packed struct
struct packedS
{
    int dIP;
    int pLenght;
    int priority;
} packed;

// Prototype of functions
void enqueue(struct packedS packed);
void drop(struct packedS packed, int equeue[][3]);
void dequeue();

// initilaze the equeue arrays
int equeue1[MAX+1][3];
int equeue2[MAX+1][3];
int equeue3[MAX+1][3];
int equeue4[MAX+1][3];

// required int arrays
unsigned int portsLastNums[5] = {0, 0, 0, 0, 0};
unsigned int portsFirstNums[5] = {0, 0, 0, 0, 0};
unsigned int portsPackedNums[5] = {0, 0, 0, 0, 0};
long unsigned int totalDatas[5] = {0, 0, 0, 0, 0};

// active port number
int activePort = 0;

// will display datas
long unsigned int droppedLP = 0;
long unsigned int droppedHP = 0;
long unsigned int routedLP = 0;
long unsigned int routedHP = 0;
long long unsigned int routedTotalByte = 0;

int main(){
    srand(time(NULL));

    float cRatio = 0.5;
    int equeueNum = 0;
    int dequeueNum = 0;

    // initilaze the time variables
    float sTime = 1.00;
    clock_t time;
    double timeTaken = 0.00;
    double tempTime = 0;
    
    // user inputs for ratio and total simulation time
    printf("Plesa enter a Congestion Ratio between 0.00 and 1.00: ");
    scanf("%f", &cRatio);
    printf("\nPlease enter total simulaton time: ");
    scanf("%f", &sTime);

    // take and save the clock variable
    time = clock();

    // loop will work until reaching simulaton time
    while(timeTaken < sTime){

        // creating a random IP adress for packed
        switch (rand() % 4)
        {
        case 1:
            packed.dIP = 0x0A001400 + (rand() % 255) + 1;
            break;

        case 2:
            packed.dIP = 0x0A000C00 + (rand() % 255) + 1;
            break;

        case 3:
            packed.dIP = 0x0A003800 + (rand() % 255) + 1;
            break;
        
        default:
            packed.dIP = 0x0A004600 + (rand() % 255) + 1;
            break;
        }
        
        // creating random packed lenght and priority
        packed.pLenght = (rand() % 1401) + 100;
        packed.priority = rand() % 2;

        // call the enqueue function with randomly created packed
        enqueue(packed);
        equeueNum++;

        // according to user defined ratio call the dequeue function
        if(equeueNum == 400){
            int dequeueNumber = 100 - (int)(cRatio*100);
            while(dequeueNumber > 0){
                dequeue();
                dequeueNumber--;
            }
            equeueNum = 0;
        }

        // calculate the time taken
        timeTaken = (double)(clock() - time) / CLOCKS_PER_SEC;

        // print the datas in every 1 seconds
        if((timeTaken - tempTime) >= 1.0){
            printf("Queue Size-1: %d\n", portsPackedNums[1]);
            printf("Queue Size-2: %d\n", portsPackedNums[2]);
            printf("Queue Size-3: %d\n", portsPackedNums[3]);
            printf("Queue Size-4: %d\n", portsPackedNums[4]); 
            printf("Number of Low P. packeds dropped: %ld\n", droppedLP);
            printf("Number of High P. packeds dropped: %ld\n", droppedHP);
            printf("Number of Low P. packeds routed: %ld\n", routedLP);
            printf("Number of High P. packeds dropped: %ld\n", routedHP);
            printf("Number of bytes routed: %llu\n\n", routedTotalByte);
            tempTime = timeTaken;
        }
    }

    printf("\nEnd of the simulaton.");
    system("Pause");
}

// enqueue function for adding packeds to related port equeue
void enqueue(struct packedS packed){

    // determine the packed's port and set global activePort variable
    if(packed.dIP >= 0x0A004600)
        activePort = 4;

    else if(packed.dIP >= 0x0A003800)
        activePort = 3;

    else if(packed.dIP >= 0x0A001400)
        activePort = 1;

    else if(packed.dIP >= 0x0A000C00)
        activePort = 2;


    // if the related port equeue is full call the drop function with packed and their equeue
    if((0x00100000 - totalDatas[activePort]) < packed.pLenght){
        switch (activePort)
        {
        case 1:
            drop(packed, equeue1);
            break;
        case 2:
            drop(packed, equeue2);
            break;
        case 3:
            drop(packed, equeue3);
            break;
        case 4:
            drop(packed, equeue4);
            break;
        }
    }

    // if related port equeue is suitable add packed to end of equeue
    if((0x00100000 - totalDatas[activePort]) >= packed.pLenght){
        switch (activePort)
        {
        case 1:
            equeue1[portsLastNums[activePort]][0] = packed.dIP;
            equeue1[portsLastNums[activePort]][1] = packed.pLenght;
            equeue1[portsLastNums[activePort]][2] = packed.priority;
            break;
        case 2:
            equeue2[portsLastNums[activePort]][0] = packed.dIP;
            equeue2[portsLastNums[activePort]][1] = packed.pLenght;
            equeue2[portsLastNums[activePort]][2] = packed.priority;
            break;
        case 3:
            equeue3[portsLastNums[activePort]][0] = packed.dIP;
            equeue3[portsLastNums[activePort]][1] = packed.pLenght;
            equeue3[portsLastNums[activePort]][2] = packed.priority;
            break;
        case 4:
            equeue4[portsLastNums[activePort]][0] = packed.dIP;
            equeue4[portsLastNums[activePort]][1] = packed.pLenght;
            equeue4[portsLastNums[activePort]][2] = packed.priority;
            break;
        }

        // if array is full set arrays new end to 0
        if(portsLastNums[activePort] == MAX)
            portsLastNums[activePort] = 0;

        else
            portsLastNums[activePort]++;

        // save the added packed lenght and added packed number in related arrays
        totalDatas[activePort] += packed.pLenght;
        portsPackedNums[activePort]++;
    }
    
}


// drop function for droping to low priority packeds for more high priority ones
void drop(struct packedS packed, int equeue[][3]){ 

    // the last packed's number added to equeue
    int n;

    if(portsLastNums[activePort] == 0)
        n = MAX;
    else
        n = (portsLastNums[activePort] - 1);

    // initilaze a new array to temporary store to high priority packed datas
    int tempN = 0;
    int tempDropped = 0;
    int tempEqueue[MAX+1][3];

    int try = 0;

    // if packed priority is high, drop some low priority ones to open enought space
    if(packed.priority == 1){
        long int freeSpace = 0x00100000 - totalDatas[activePort];

        // the while loop will work until the enought space opened
        while(freeSpace < packed.pLenght){

            // if not have enought low priorty packed in the equeue drop the current packed
            if(try >= portsPackedNums[activePort]){
                droppedHP++;
                break;
            }

            // if n th packed have high priority, add it to a temp array
            if(equeue[n][2] == 1){
                tempEqueue[tempN][0] = equeue[n][0];
                tempEqueue[tempN][1] = equeue[n][1];
                tempEqueue[tempN][2] = equeue[n][2];

                tempN++;
            }

            // if n th packed have low priority, drop it and increse the free space variable
            else if(equeue[n][2] == 0){
                totalDatas[activePort] -= equeue[n][1];
                freeSpace += equeue[n][1];
                tempDropped++;
                droppedLP++;
            }

            // scan all equeue from tail to head
            if(n == 0)
                n = MAX;
            else
                n--;

            try++;
        }

        // if there is any high priority packed stored in temp array, readdd the to main equeue
        if(tempDropped != 0){

            if(portsLastNums[activePort] >= (tempDropped + tempN))
                portsLastNums[activePort] -= (tempDropped + tempN);
            else
                portsLastNums[activePort] = MAX - ((tempDropped + tempN) - portsLastNums[activePort]);

            portsPackedNums[activePort] -= (tempDropped + tempN);

            // add all temp packeds to main equeue
            while(tempN >= 1){
                equeue[portsLastNums[activePort]][0] = tempEqueue[tempN-1][0];
                equeue[portsLastNums[activePort]][1] = tempEqueue[tempN-1][1];
                equeue[portsLastNums[activePort]][2] = tempEqueue[tempN-1][2];
                portsPackedNums[activePort]++;
                tempN--;

                if(portsLastNums[activePort] == MAX)
                    portsLastNums[activePort] = 0;
                else
                    portsLastNums[activePort]++;
            }
        }


    }

    // if packed has low priority, drop it
    else{
        droppedLP++;
    }
}

// dequeue function for route the packeds from equeues
void dequeue(){
    int i = 1;

    while (i < 5)
    {
        // if equeue has less tha 1 packed continue
        if(portsPackedNums[i] < 1){
            i++;
            continue;
        }

        // route all packeds in the head of equeues and decrase to totalData variable
        switch (i)
        {
            case 1:
                totalDatas[1] -= equeue1[portsFirstNums[i]][1];
                routedTotalByte += equeue1[portsFirstNums[i]][1];
                if(equeue1[portsFirstNums[i]][2] == 1) routedHP++;
                else routedLP++;
                break;
            
            case 2:
                totalDatas[2] -= equeue2[portsFirstNums[i]][1];
                routedTotalByte += equeue2[portsFirstNums[i]][1];
                if(equeue2[portsFirstNums[i]][2] == 1) routedHP++;
                else routedLP++;
                break;

            case 3:
                totalDatas[3] -= equeue3[portsFirstNums[i]][1];
                routedTotalByte += equeue3[portsFirstNums[i]][1];
                if(equeue3[portsFirstNums[i]][2] == 1) routedHP++;
                else routedLP++;
                break;
            
            case 4:
                totalDatas[4] -= equeue4[portsFirstNums[i]][1];
                routedTotalByte += equeue4[portsFirstNums[i]][1];
                if(equeue4[portsFirstNums[i]][2] == 1) routedHP++;
                else routedLP++;
                break;
            
        }

        // update the new head of equeue
        if(portsFirstNums[i] == MAX){
            portsFirstNums[i] = 0;
        }
            
        else{
            portsFirstNums[i]++;
        }

        portsPackedNums[i]--;
        i++;
    }
    
}