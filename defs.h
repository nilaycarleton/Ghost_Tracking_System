#include <stdbool.h>

#define MAX_STR 32
#define MAX_ROOMS 16
#define GHOST_INITIAL_ID 1031

// Type definitions
typedef struct Ghost Ghost;
typedef struct GhostNode GhostNode;
typedef struct GhostList GhostList;
typedef struct Room Room;
typedef struct RoomArray RoomArray;
typedef struct Building Building;

// Structure for a single Ghost
struct Ghost {
    int id;
    char type[MAX_STR];
    float likelihood;
    Room* room;
};

// Node for the GhostList
struct GhostNode {
    Ghost* data;
    GhostNode* next;
};

// Singly-linked list for Ghosts
struct GhostList {
    GhostNode* head;
    GhostNode* tail;
};

// Structure for a single Room
struct Room {
    int id;
    char name[MAX_STR];
    GhostList ghosts;
};

// Fixed-size array structure for Rooms
struct RoomArray {
    Room* elements[MAX_ROOMS];
    int size;
};

// Main building structure
struct Building {
    struct RoomArray rooms;
    struct GhostList ghosts;
};


// Ghost Functions
void ghost_create(Ghost** ghost, const char* type);
void ghost_print(const Ghost* ghost);
void ghost_cleanup(Ghost** ghost);

// GhostList Functions
void ghostlist_init(GhostList* list);
void ghostlist_push(GhostList* list, Ghost* ghost);
void ghostlist_print(const GhostList* list);
void ghostlist_cleanup(GhostList* list, bool free_data);
// Helper function for sorted insertion, required by room_add_ghost
void ghostlist_insert_by_likelihood(GhostList* list, Ghost* ghost);

// Room Functions
void room_create(Room** room, int id, const char* name);
void room_add_ghost(Room* room, Ghost* ghost, float likelihood);
void room_print(const Room* room);
void room_cleanup(Room** room);

// RoomArray Functions
void roomarray_init(RoomArray* array);
void roomarray_add(RoomArray* array, Room* room);
void roomarray_print(const RoomArray* array);
void roomarray_cleanup(RoomArray* array);

// Building Functions
void building_init(Building* building);
void building_cleanup(Building* building);

// Sample Data Loading Function (provided)
void building_load_sample(Building* building);