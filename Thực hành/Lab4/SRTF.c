#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iRemainingTime;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
    int isStarted;
} PCB;

void inputProcess(int n, PCB P[]);
void printReadyQueue(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void calculateAWT(int n, PCB P[]);
void calculateATaT(int n, PCB P[]);

int main() {
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;

    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, currentTime = 0;
    int completed = 0;
    
    inputProcess(iNumberOfProcess, Input);
    
    // Initialize processes in Input
    for (int i = 0; i < iNumberOfProcess; i++) {
        Input[i].iRemainingTime = Input[i].iBurst;
        Input[i].isStarted = 0;
    }

    // SRTF Scheduling Algorithm
    while (completed < iNumberOfProcess) {
        int shortest = -1;
        int minRemainingTime = 1e9;

        // Check which processes have arrived and find the shortest remaining time
        for (int i = 0; i < iNumberOfProcess; i++) {
            if (Input[i].iArrival <= currentTime && Input[i].iRemainingTime > 0 && Input[i].iRemainingTime < minRemainingTime) {
                shortest = i;
                minRemainingTime = Input[i].iRemainingTime;
            }
        }

        if (shortest == -1) {
            // If no process is ready, move the current time forward
            currentTime++;
            continue;
        }

        // Start the process if it hasn't been started
        if (Input[shortest].isStarted == 0) {
            Input[shortest].iStart = (currentTime >= Input[shortest].iArrival) ? currentTime : Input[shortest].iArrival;
            Input[shortest].isStarted = 1;
            Input[shortest].iResponse = Input[shortest].iStart - Input[shortest].iArrival;
        }

        // Process the task for 1 unit of time
        Input[shortest].iRemainingTime--;
        currentTime++;

        // If the process finishes, update the necessary fields
        if (Input[shortest].iRemainingTime == 0) {
            Input[shortest].iFinish = currentTime;
            Input[shortest].iTaT = Input[shortest].iFinish - Input[shortest].iArrival;
            Input[shortest].iWaiting = Input[shortest].iTaT - Input[shortest].iBurst;

            // Move the completed process to the TerminatedArray
            TerminatedArray[iTerminated++] = Input[shortest];
            completed++;
        }
    }

    printf("\n===== SRTF Scheduling =====\n");
    exportGanttChart(iNumberOfProcess, TerminatedArray);
    calculateAWT(iNumberOfProcess, TerminatedArray);
    calculateATaT(iNumberOfProcess, TerminatedArray);

    return 0;
}

void inputProcess(int n, PCB P[]) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21; // Arrival time between 0 and 20
        P[i].iBurst = 2 + rand() % 11; // Burst time between 2 and 12
        printf("Process %d - Arrival Time: %d, Burst Time: %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void printReadyQueue(int n, PCB P[]) {
    printf("Ready Queue:\n");
    printf("%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n", "PID", "Arrival", "Burst", "Start", "Finish", "Waiting", "Response", "TaT");
    for (int i = 0; i < n; i++) {
        printf("%-10d%-10d%-10d%-10d%-10d%-10d%-10d%-10d\n", P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}

void exportGanttChart(int n, PCB P[]) {
    printf("Gantt Chart:\n| ");
    for (int i = 0; i < n; i++) {
        printf("P%d | ", P[i].iPID);
    }
    printf("\n0    ");
    for (int i = 0; i < n; i++) {
        printf("%-5d", P[i].iFinish);
    }
    printf("\n");
}

void calculateAWT(int n, PCB P[]) {
    double totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        totalWaiting += P[i].iWaiting;
    }
    printf("Average Waiting Time: %.2f\n", totalWaiting / n);
}

void calculateATaT(int n, PCB P[]) {
    double totalTaT = 0;
    for (int i = 0; i < n; i++) {
        totalTaT += P[i].iTaT;
    }
    printf("Average Turnaround Time: %.2f\n", totalTaT / n);
}
