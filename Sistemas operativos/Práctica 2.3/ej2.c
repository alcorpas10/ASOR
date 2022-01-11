#include <sched.h>
#include <stdio.h>
#include <errno.h>

int main() {
    int sched = sched_getscheduler(0);

    printf("Politica de planificacion: ");
    if (sched == SCHED_OTHER)
        printf("SCHED_OTHER\n");
    else if (sched == SCHED_FIFO)
        printf("SCHED_FIFO\n");
    else
        printf("SCHED_RR\n");
    
    struct sched_param params;
    int priority = sched_getparam(0, &params);
    if (priority == -1) {
        printf("Error: %d %s\n", errno, strerror(errno));
        return 1;
    }
    printf("Prioridad: %d\n", params.sched_priority);

    int max = sched_get_priority_max(sched);
    int min = sched_get_priority_min(sched);
    printf("Valor maximo: %d Valor minimo: %d\n", max, min);

    return 0;
}