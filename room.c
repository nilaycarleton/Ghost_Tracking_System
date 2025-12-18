#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

/* This file should contain all Room and RoomArray specific functionality. */

/*
  Function: room_create
  Purpose:  Dynamically allocates and initializes a new Room structure.
  Params:
    out: room - A double pointer to store the new Room's address.
    in:  id   - The ID for the new room.
    in:  name - The name for the new room.
*/
void room_create(Room** room, int id, const char* name) {
    *room = (Room*) malloc(sizeof(Room));
    if (*room == NULL) {
        printf("Error: malloc failed in room_create\n");
        exit(1);
    }

    (*room)->id = id;
    strcpy((*room)->name, name);
    ghostlist_init(&((*room)->ghosts)); // Initialize the room's ghost list
}

/*
  Function: room_add_ghost
  Purpose:  Associates a ghost with a room and adds it to the room's
            ghost list, sorted by likelihood.
  Params:
    in/out: room       - The room to add the ghost to.
    in/out: ghost      - The ghost to add.
    in:     likelihood - The likelihood of this ghost being in the room.
*/
void room_add_ghost(Room* room, Ghost* ghost, float likelihood) {
    if (room == NULL || ghost == NULL) return;

    ghost->room = room;
    ghost->likelihood = likelihood;

    // Add to the room's list, sorted by likelihood
    ghostlist_insert_by_likelihood(&(room->ghosts), ghost);
}

/*
  Function: room_print
  Purpose:  Prints the details of a single room and its ghosts.
  Params:
    in: room - The room to print.
*/
void room_print(const Room* room) {
    if (room == NULL) return;

    // Print room details matching screenshot format
    printf("{id: %d, name: %s}\n", room->id, room->name);
    printf("  Ghosts:\n");
    
    // Print the list of ghosts in this room
    ghostlist_print(&(room->ghosts));
}

/*
  Function: room_cleanup
  Purpose:  Frees a Room and its ghost list *nodes*, but not the Ghost data itself.
  Params:
    in/out: room - A double pointer to the Room to be freed.
*/
void room_cleanup(Room** room) {
    if (room == NULL || *room == NULL) return;

    // Clean up the room's ghost list nodes, but NOT the ghost data
    ghostlist_cleanup(&((*room)->ghosts), false);

    free(*room); // Free the room struct itself
    *room = NULL;
}

/*
  Function: roomarray_init
  Purpose:  Initializes a RoomArray to an empty state.
  Params:
    out: array - The RoomArray to initialize.
*/
void roomarray_init(RoomArray* array) {
    if (array == NULL) return;
    array->size = 0;
    
    // Also initialize all pointers to NULL for safety
    for (int i = 0; i < MAX_ROOMS; i++) {
        array->elements[i] = NULL;
    }
}

/*
  Function: roomarray_add
  Purpose:  Adds a Room pointer to the RoomArray.
  Params:
    in/out: array - The array to add to.
    in:     room  - The room pointer to add.
*/
void roomarray_add(RoomArray* array, Room* room) {
    if (array == NULL || room == NULL) return;

    // Add if array is not full
    if (array->size < MAX_ROOMS) {
        array->elements[array->size] = room;
        array->size++;
    } else {
        // Handle gracefully by doing nothing, as requested
        printf("Warning: RoomArray is full. Could not add room.\n");
    }
}

/*
  Function: roomarray_print
  Purpose:  Prints all Rooms in the RoomArray.
  Params:
    in: array - The array to print.
*/
void roomarray_print(const RoomArray* array) {
    if (array == NULL) return;

    for (int i = 0; i < array->size; i++) {
        room_print(array->elements[i]);
    }
}

/*
  Function: roomarray_cleanup
  Purpose:  Frees all Rooms stored within the RoomArray.
  Params:
    in/out: array - The array to clean up.
*/
void roomarray_cleanup(RoomArray* array) {
    if (array == NULL) return;

    for (int i = 0; i < array->size; i++) {
        room_cleanup(&(array->elements[i]));
    }
    array->size = 0;
}