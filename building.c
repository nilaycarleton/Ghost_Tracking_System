#include <stdlib.h> // <-- ADDED: Defines NULL, malloc, exit
#include <stdio.h>  // <-- ADDED: Defines printf
#include "defs.h"

/*
  Function: building_init
  Purpose:  Initializes the Building structure.
  Params:
    out: building - The building to initialize.
*/
void building_init(Building* building) {
    if (building == NULL) return;
    roomarray_init(&(building->rooms));
    ghostlist_init(&(building->ghosts));
}

/*
  Function: building_cleanup
  Purpose:  Cleans up all memory associated with the building,
            including all rooms and all ghosts.
  Params:
    in/out: building - The building to clean up.
*/
void building_cleanup(Building* building) {
    if (building == NULL) return;

    // 1. Clean up all the Room objects and their GhostList *nodes*
    //    (but not the Ghost data itself).
    roomarray_cleanup(&(building->rooms));

    // 2. Clean up the building's main ghost list.
    //    This frees both the nodes and the Ghost data.
    //    This is safe because step 1 did not free the Ghost data.
    ghostlist_cleanup(&(building->ghosts), true);
}

/* This is just a helper for the way that the sample data loads these */
void util_ghost_create_and_add(Building* building, const char* type, Room* room, float likelihood) {
    Ghost* ghost;
    ghost_create(&ghost, type);
    ghostlist_push(&building->ghosts, ghost);
    room_add_ghost(room, ghost, likelihood);
}

void building_load_sample(Building* building) {
    Room *bedroom, *bathroom, *living_room, *kitchen, *basement, *garage, *hallway, *staircase;

    // Create Rooms
    room_create(&bedroom, 1, "Bedroom");
    roomarray_add(&building->rooms, bedroom);
    room_create(&bathroom, 2, "Bathroom");
    roomarray_add(&building->rooms, bathroom);
    room_create(&living_room, 3, "Living Room");
    roomarray_add(&building->rooms, living_room);
    room_create(&kitchen, 4, "Kitchen");
    roomarray_add(&building->rooms, kitchen);
    room_create(&basement, 5, "Basement");
    roomarray_add(&building->rooms, basement);
    room_create(&garage, 6, "Garage");
    roomarray_add(&building->rooms, garage);
    room_create(&hallway, 7, "Hallway");
    roomarray_add(&building->rooms, hallway);
    room_create(&staircase, 8, "Staircase");
    roomarray_add(&building->rooms, staircase);

    // Create Ghosts and add to Building GhostList and Rooms
    util_ghost_create_and_add(building, "Banshee", kitchen, 82.51f);
    util_ghost_create_and_add(building, "Banshee", bathroom, 19.99f);

    util_ghost_create_and_add(building, "Wraith", basement, 72.21f);
    util_ghost_create_and_add(building, "Wraith", garage, 6.01f);
    util_ghost_create_and_add(building, "Wraith", hallway, 97.99f);
    util_ghost_create_and_add(building, "Wraith", staircase, 47.03f);
    util_ghost_create_and_add(building, "Wraith", bedroom, 88.78f);

    util_ghost_create_and_add(building, "Phantom", living_room, 20.04f);
    util_ghost_create_and_add(building, "Phantom", basement, 18.71f);
    util_ghost_create_and_add(building, "Phantom", hallway, 65.04f);

    util_ghost_create_and_add(building, "Danny", living_room, 20.07f);
    util_ghost_create_and_add(building, "Danny", basement, 18.72f);
    util_ghost_create_and_add(building, "Danny", hallway, 65.05f);

    util_ghost_create_and_add(building, "Bullies", basement, 10.62f);
    util_ghost_create_and_add(building, "Bullies", kitchen, 98.74f);
    util_ghost_create_and_add(building, "Bullies", staircase, 55.43f);
    util_ghost_create_and_add(building, "Bullies", bathroom, 27.75f);
    util_ghost_create_and_add(building, "Bullies", garage, 98.85f);

    util_ghost_create_and_add(building, "Yokai", bathroom, 87.67f);

    util_ghost_create_and_add(building, "Poltergeist", living_room, 87.67f);
    util_ghost_create_and_add(building, "Poltergeist", bedroom, 19.82f);
}