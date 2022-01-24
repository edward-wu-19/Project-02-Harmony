#include "client.h"

// Function: Helper function that creates a new harmony_message struct
// Arguments: Data that should be saved in the struct
// Return Values: A pointer to the struct created
struct harmony_message *new_node(char *msg, char* usr, int chn, char* time, int id) {
    struct harmony_message *temp = (struct harmony_message *) calloc(1, sizeof(struct harmony_message));
    strncpy(temp->val, msg, HARMONY_BUFFER_SIZE);
    strncpy(temp->time, time, HARMONY_USERNAME_SIZE);

    // Adding server as username
    if (strcmp(usr, "") == 0){
        strncpy(temp->sender, "Server", HARMONY_USERNAME_SIZE);
    }
    else{
        strncpy(temp->sender, usr, HARMONY_USERNAME_SIZE);
    }

    temp->next = NULL;
    temp->channel = chn; // 0 is default
    temp->id = id; // 0 is default
    return temp;
}

// Function: Helper function that creates a harmony_queue struct
// Arguments: None
// Return Values: A pointer to the created harmony_queue
struct harmony_queue *create_queue() {
    struct harmony_queue *Q = (struct harmony_queue *) calloc(1, sizeof(struct harmony_queue));
    Q->size = 0;
    Q->front = NULL;
    Q->back = NULL;
    return Q;
}

// Function: Adds a new harmony_message to the end of a harmony_queue
// Arguments: The harmony_queue and data representing the harmony_message
// Return Values: None
void queue_push(struct harmony_queue *Q, char *msg, char *usr, int chn, int id) {
    // Creating New Queue Node
    struct harmony_message *temp = new_node(msg, usr, chn, get_time(), id);

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

// Function: Removes the first harmony_message of a harmony_queue
// Arguments: The harmony_queue that is getting a node removed
// Return Values: None
void queue_pop(struct harmony_queue *Q) {
    // If Empty Do Nothing
    if (Q->front == NULL) return;

    // Store Current Front Node
    struct harmony_message *temp = Q->front;

    // Move Front Of Queue
    Q->front = Q->front->next;

    // If Size 1, Everything Becomes NULL
    if (Q->front == NULL) Q->back = NULL;

    // Exiting Function
    free(temp);
    (Q->size)--;
    return;
}

// Function: A function that updates a harmony_queue with a new harmony_message and limits the size of the queue if needed
// Arguments: The harmony_queue and the harmony_message
// Return Values: None
void update_queue(struct harmony_queue *Q, struct harmony_message *data) {
    // Adding Message To Queue
    queue_push(Q, data->val, data->sender, data->channel, data->id);

    // If Size Is Too Large Remove Node
    if (Q->size > HARMONY_QUEUE_SIZE) queue_pop(Q);

    // Exiting Function
    return;
}

// Function: A function that loops through a harmony_queue and free each node
// Arguments: The harmony_queue to be freed
// Return Values: None
void free_queue(struct harmony_queue *Q) {
    // If Empty Do Nothing
    if (Q->front == NULL) return;

    // Store Current Front Node
    struct harmony_message *prev = Q->front, *temp = Q->front;

    // Freeing Stuff Individually
    while (temp != NULL) {
        temp = temp->next;
        free(prev);
        prev = temp;
    }

    // Exiting Function
    free(Q);
    return;
}
