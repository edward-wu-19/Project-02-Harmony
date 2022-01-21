#include "client.h"

// Creates A New Queue Node
struct queue_node *new_node(char *msg) {
    struct queue_node *temp = (struct queue_node *) calloc(1, sizeof(struct queue_node));
    temp->val = strdup(msg);
    temp->next = NULL;
    return temp;
}

// Creates A New Queue
struct harmony_queue *create_queue() {
    struct harmony_queue *Q = (struct harmony_queue *) calloc(1, sizeof(struct harmony_queue));
    Q->size = 0;
    Q->front = NULL;
    Q->back = NULL;
    return Q;
}

// Adds A Queue Node To The Queue
void queue_push(struct harmony_queue *Q, char *msg) {
    // Creating New Queue Node
    struct queue_node *temp = new_node(msg);

    // Adding Queue Node
    if (Q->back == NULL) { // NULL Case
        Q->front = temp;
        Q->back = temp;
    } else { // Non Null Case
        Q->back->next = temp;
        Q->back = temp;
    }

    // Exiting Function
    (Q->size)++;
    return;
}

// Removes A Queue Node From The Queue
void queue_pop(struct harmony_queue *Q) {
    // If Empty Do Nothing
    if (Q->front == NULL) return;

    // Store Current Front Node
    struct queue_node *temp = Q->front;

    // Move Front Of Queue
    Q->front = Q->front->next;

    // If Size 1, Everything Becomes NULL
    if (Q->front == NULL) Q->back = NULL;

    // Exiting Function
    free(temp);
    (Q->size)--;
    return;
}

// Updates The Queue Properly
void update_queue(struct harmony_queue *Q, char *msg) {
    // Adding Message To Queue
    queue_push(Q, msg);

    // If Size Is Too Large Remove Node
    if (Q->size > HARMONY_QUEUE_SIZE) queue_pop(Q);

    // Exiting Function
    return;
}
