#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        printf("Enter PID, Arrival time, and Burst time for Process %d: ", i + 1);
        scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
        P[i].iStart = P[i].iFinish = P[i].iWaiting = P[i].iResponse = P[i].iTaT = 0;
    }
}

void printProcess(int n, PCB P[]) {
    printf("PID\tArrival\tBurst\tStart\tFinish\tWaiting\tResponse\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t\t%d\n", P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}

void exportGanttChart (int n, PCB P[]) {
    printf("Gantt Chart:\n");
    // Print process IDs
    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < P[i].iBurst - 1; j++) printf(" ");
        printf("P%d", P[i].iPID);
        for (j = 0; j < P[i].iBurst - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");

    printf("0");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < P[i].iBurst; j++) printf("  ");
        if (P[i].iFinish > 9) printf("\b");
        printf("%d", P[i].iFinish);
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

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 1;
}

int partition(PCB P[], int low, int high, int iCriteria) {
    PCB pivot = P[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        int compare = 0;
        if (iCriteria == SORT_BY_ARRIVAL) compare = P[j].iArrival < pivot.iArrival;
        else if (iCriteria == SORT_BY_PID) compare = P[j].iPID < pivot.iPID;
        else if (iCriteria == SORT_BY_BURST) {
            if (P[j].iBurst == pivot.iBurst)
                compare = P[j].iArrival < pivot.iArrival;
            else
                compare = P[j].iBurst < pivot.iBurst;
        }
        else if (iCriteria == SORT_BY_START) compare = P[j].iStart < pivot.iStart;
        if (compare) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return i + 1;
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        totalWaiting += P[i].iWaiting;
    }
    printf("Average Waiting Time: %.2f\n", (float)totalWaiting / n);
}

void calculateATaT(int n, PCB P[]) {
    int totalTaT = 0;
    for (int i = 0; i < n; i++) {
        totalTaT += P[i].iTaT;
    }
    printf("Average Turnaround Time: %.2f\n", (float)totalTaT / n);
}

int main() {
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    PCB ganttchart[50];
    int startmap[50];
    int burstmap[50];
    int QTime;

    for (int i = 0; i < 50; i++) {
        startmap[i] = -1;
    }

    printf("Please input quantum time: ");
    scanf("%d",  &QTime);
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, iGantt = 0;
    inputProcess(iNumberOfProcess, Input);
    for (int i = 0; i < iNumberOfProcess; i++)
        burstmap[Input[i].iPID]  = Input[i].iBurst;

    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    int time = 0;
    while (iTerminated < iNumberOfProcess) {
        while (iRemain > 0) {
            if (Input[0].iArrival <= time + MIN(QTime, ReadyQueue[0].iBurst)) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            }
            else break;
        }

        if (iReady > 0) {
            if (startmap[ReadyQueue[0].iPID] == -1) {
                startmap[ReadyQueue[0].iPID] = time;
            }
            int temp = MIN(QTime, ReadyQueue[0].iBurst); 
            pushProcess(&iGantt, ganttchart, ReadyQueue[0]);
            ganttchart[iGantt - 1].iBurst = temp;
            ganttchart[iGantt - 1].iFinish = time + temp;

            if (temp == ReadyQueue[0].iBurst) {
                ReadyQueue[0].iStart = startmap[ReadyQueue[0].iPID];
                ReadyQueue[0].iFinish = time + temp;
                ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - burstmap[ReadyQueue[0].iPID];

                pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            }
            else {
                ReadyQueue[0].iBurst = ReadyQueue[0].iBurst - temp;
                pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
            }
            removeProcess(&iReady, 0, ReadyQueue);
            time += temp;
        }
    }
    int pos = 0;
    for (int i = 1; i < iGantt; i++) {
        if (ganttchart[pos].iPID == ganttchart[i].iPID) {
            ganttchart[pos].iBurst = ganttchart[pos].iBurst + ganttchart[i].iBurst;
            ganttchart[pos].iFinish = ganttchart[i].iFinish;
        }
        else {
            pos++;
            ganttchart[pos] = ganttchart[i];
        }
    }
    iGantt = ++pos;

    printf("\n===== Round Robin Scheduling with quantum time = %d =====\n", QTime);
    exportGanttChart(iGantt, ganttchart);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);

    printProcess(iTerminated, TerminatedArray);

    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    printf("\n");
    return 0;
}
