#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// const number
#define MAX_QUEUE_SIZE 100
#define NUM_OF_COUNTER 4
#define CUSTOMER_THRESHOLD 7
#define SERVICE_TIME_LIMIT 8

// element = customer, QueueType = Queue
typedef struct 
{
    int id;
    int arrival_time;
    int service_time;
} element;

typedef struct 
{
    element data[MAX_QUEUE_SIZE];
    int front, rear;
} QueueType;

// function prototype
void error(char*);
void init_queue(QueueType*);
void queue_print(QueueType*);
int is_empty(QueueType*);
int is_full(QueueType*);
void enqueue(QueueType*, element);
element dequeue(QueueType*);

// main
int main(void)
{
    QueueType queue;
    element counter[NUM_OF_COUNTER];
    bool flag[NUM_OF_COUNTER] = {false};
    bool is_counter_full = false;
    int minutes = 60;
    int total_wait = 0;
    int total_customer = 0;
    int i;
    
    // Initialize Queue
    init_queue(&queue);

    srand(time(NULL));
    for(int clock = 0; clock < minutes; clock++)
    {   
        printf("\nCLOCK - Current time : %d\n", clock);
        
        // Create Customer
        if((rand() % 10) < CUSTOMER_THRESHOLD)
        {
            element customer;

            customer.id = total_customer++;
            customer.arrival_time = clock;
            customer.service_time = rand() % SERVICE_TIME_LIMIT + 1;
            enqueue(&queue, customer);

            printf("IN    - Customer %d is coming in, Arrival time : %d, Estimated service time : %d\n",
                    customer.id, customer.arrival_time, customer.service_time);
        }

        // Reducing service time & Checking which counters are done
        for(i = 0; i < NUM_OF_COUNTER; i++)
        {
            if(flag[i])
            {
                if((counter[i].service_time--) == 0)
                {
                    flag[i] = false;
                    printf("OUT   - Customer %d is done, End time : %d, Used counter : %d\n", counter[i].id, clock, i);
                }
            }
        }

        // Enter counter
        if(!is_empty(&queue))
        {
            for(i = 0; i < NUM_OF_COUNTER; i++)
            {
                if(!flag[i])
                {
                    if(!is_empty(&queue))
                    {
                        flag[i] = true;
                        counter[i] = dequeue(&queue);
                    }
                    else
                        break;

                    printf("START - Customer : %d, Starting time : %d, Waiting time : %d, Counter number : %d\n",
                            counter[i].id, clock, clock - counter[i].arrival_time, i);
                    
                    total_wait += clock - counter[i].arrival_time;
                }
            }
        }

        // If all counters are full,
        if(!is_empty(&queue))
        {
            printf("FULL  - All counters are full!!\n");
            printf("        Waiting Customer : ");
            queue_print(&queue);
        }
    }
    printf("\nEND   - Average waiting time : %.2f", (float)total_wait / (float)total_customer);

    return 0;
}

void error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

void init_queue(QueueType *q)
{
    q->rear = q->front = 0;
}

void queue_print(QueueType *q)
{
    for(int i = q->front + 1; i <= q->rear; i++)
        if(i < q->rear)
            printf("%d, ", q->data[i].id);
        else
            printf("%d | ", q->data[i].id);
    printf("Total : %d\n", q->rear - q->front);
}

int is_empty(QueueType *q)
{
    return (q->rear == q->front);
}

int is_full(QueueType *q)
{
    return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType *q, element item)
{
    if(is_full(q))
        error("Queue is full\n");

    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}

element dequeue(QueueType *q)
{
    if(is_empty(q))
        error("Queue is empty\n");

    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}