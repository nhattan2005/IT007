#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_BURST 1

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]);
void printReadyQueue(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void pushProcess(int *n, PCB P[], PCB Q);
void removeProcess(int *n, int index, PCB P[]);
int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);
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

    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    while (iTerminated < iNumberOfProcess) {
        // Add processes that have arrived to the ready queue
        while (iRemain > 0 && Input[0].iArrival <= currentTime) {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
            quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST); // Sort ready queue by burst time
        }

        if (iReady > 0) {
            // Process the shortest job
            ReadyQueue[0].iStart = (iTerminated == 0) ? ReadyQueue[0].iArrival : (ReadyQueue[0].iArrival > currentTime ? ReadyQueue[0].iArrival : currentTime);
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
            
            // Update current time
            currentTime = ReadyQueue[0].iFinish;

            // Move process to terminated array
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
        } else {
            // If no processes are ready, increment time
            currentTime++;
        }
    }

    printf("\n===== SJF Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

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

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int partition(PCB P[], int low, int high, int iCriteria) {
    PCB pivot = P[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if ((iCriteria == SORT_BY_ARRIVAL && P[j].iArrival < pivot.iArrival) ||
            (iCriteria == SORT_BY_BURST && P[j].iBurst < pivot.iBurst)) {
            i++;
            PCB temp = P[i];
            P[i] = P[j];
            P[j] = temp;
        }
    }
    PCB temp = P[i + 1];
    P[i + 1] = P[high];
    P[high] = temp;
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
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
