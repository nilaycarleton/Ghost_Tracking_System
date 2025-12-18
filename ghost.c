#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

/* This file should contain all Ghost and GhostList specific functionality. */

/*
  Function: ghost_create
  Purpose:  Dynamically allocates and initializes a new Ghost structure.
  Params:
    out: ghost - A double pointer to store the new Ghost's address.
    in:  type  - The type of the ghost to initialize.
*/
void ghost_create(Ghost* *ghost, const char* type) {
    // Use a static variable to assign a unique ID
    static int next_id = GHOST_INITIAL_ID;

    *ghost = (Ghost*) malloc(sizeof(Ghost));
    if (*ghost == NULL) {
        printf("Error: malloc failed in ghost_create\n");
        exit(1);
    }

    (*ghost)->id = next_id++;
    strcpy((*ghost)->type, type);
    (*ghost)->likelihood = 0.0;
    (*ghost)->room = NULL;
}

/*
  Function: ghost_print
  Purpose:  Prints the details of a single ghost.
  Params:
    in: ghost - A pointer to the Ghost to print.
*/
void ghost_print(const Ghost* ghost) {
    if (ghost == NULL) return;

    // Determine room name, default to "Unknown" if NULL
    char* roomName;
    if (ghost->room == NULL) {
        roomName = "Unknown";
    } else {
        roomName = ghost->room->name;
    }

    // Print in the format matching the screenshots
    printf("  - {id: %d, type: %s, likelihood: %.2f%%, room: %s}\n",
           ghost->id,
           ghost->type,
           ghost->likelihood,
           roomName);
}

/*
  Function: ghost_cleanup
  Purpose:  Frees the memory for a single Ghost object.
  Params:
    in/out: ghost - A double pointer to the Ghost to be freed.
*/
void ghost_cleanup(Ghost* *ghost) {
    if (ghost == NULL || *ghost == NULL) return;
    free(*ghost);
    *ghost = NULL;
}

/*
  Function: ghostlist_init
  Purpose:  Initializes a GhostList to an empty state.
  Params:
    out: list - The list to initialize.
*/
void ghostlist_init(GhostList* list) {
    if (list == NULL) return;
    list->head = NULL;
    list->tail = NULL;
}

/*
  Function: ghostlist_push
  Purpose:  Adds a Ghost to the end (tail) of the list. Used by the building's main list.
  Params:
    in/out: list  - The list to add to.
    in:     ghost - The Ghost to add.
*/
void ghostlist_push(GhostList* list, Ghost* ghost) {
    if (list == NULL || ghost == NULL) return;

    GhostNode* newNode = (GhostNode*) malloc(sizeof(GhostNode));
    if (newNode == NULL) {
        printf("Error: malloc failed in ghostlist_push\n");
        exit(1);
    }
    newNode->data = ghost;
    newNode->next = NULL;

    if (list->head == NULL) { // List is empty
        list->head = newNode;
        list->tail = newNode;
    } else { // List has nodes
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

/*
  Function: ghostlist_insert_by_likelihood
  Purpose:  Adds a Ghost to the list in descending order of likelihood.
            Required by room_add_ghost.
  Params:
    in/out: list  - The list to insert into.
    in:     ghost - The Ghost to insert.
*/
void ghostlist_insert_by_likelihood(GhostList* list, Ghost* ghost) {
    if (list == NULL || ghost == NULL) return;

    GhostNode* newNode = (GhostNode*) malloc(sizeof(GhostNode));
    if (newNode == NULL) {
        printf("Error: malloc failed in ghostlist_insert_by_likelihood\n");
        exit(1);
    }
    newNode->data = ghost;
    newNode->next = NULL;

    // Case 1: List is empty
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        return;
    }

    // Case 2: Insert at head (new likelihood is >= head's likelihood)
    if (ghost->likelihood >= list->head->data->likelihood) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    // Case 3: Insert in middle or at tail
    GhostNode* curr = list->head;
    // Find the node *before* the insertion point
    while (curr->next != NULL && curr->next->data->likelihood > ghost->likelihood) {
        curr = curr->next;
    }

    // Insert newNode after curr
    newNode->next = curr->next;
    curr->next = newNode;

    // Update tail if newNode was inserted at the end
    if (newNode->next == NULL) {
        list->tail = newNode;
    }
}


/*
  Function: ghostlist_print
  Purpose:  Prints all Ghosts in a GhostList.
  Params:
    in: list - The list to print.
*/
void ghostlist_print(const GhostList* list) {
    if (list == NULL) return;

    GhostNode* curr = list->head;
    while (curr != NULL) {
        ghost_print(curr->data);
        curr = curr->next;
    }
}

/*
  Function: ghostlist_cleanup
  Purpose:  Frees all nodes in the list, and optionally the Ghost data itself.
  Params:
    in/out: list      - The list to clean up.
    in:     free_data - Flag to indicate whether to free the Ghost data.
*/
void ghostlist_cleanup(GhostList* list, bool free_data) {
    if (list == NULL) return;

    GhostNode* curr = list->head;
    while (curr != NULL) {
        GhostNode* temp = curr;
        curr = curr->next;

        if (free_data) {
            ghost_cleanup(&(temp->data)); // Free the Ghost struct itself
        }
        free(temp); // Free the list node
    }

    list->head = NULL;
    list->tail = NULL;
}