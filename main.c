#include "defs.h"
#include <stdio.h>
#include <string.h>  // Add this for strcmp and snprintf in tests

/* A simple enumerator only used here for the menu options */
enum MenuOptions { LOAD_SAMPLE_DATA = 1, PRINT_GHOST_LIST, PRINT_BUILDING_ROOMS, RUN_TEST_FUNCTION, EXIT_PROGRAM };
enum MenuOptions print_menu();
int run_test_function();

int main(void) {
    Building building;
    enum MenuOptions choice;

    building_init(&building);

    do {
        choice = print_menu();

        switch (choice) {
            case LOAD_SAMPLE_DATA:
                building_load_sample(&building);
                break;
            case PRINT_GHOST_LIST:
                ghostlist_print(&building.ghosts);
                break;
            case PRINT_BUILDING_ROOMS:
                roomarray_print(&building.rooms);
                break;
            case RUN_TEST_FUNCTION:
                run_test_function();
                break;
            case EXIT_PROGRAM:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != EXIT_PROGRAM);

    building_cleanup(&building);
    return 0;
}

enum MenuOptions print_menu() {
    int choice, valid = 0;
    printf("Menu:\n");
    printf("1. Load Sample Data\n");
    printf("2. Print Ghost List\n");
    printf("3. Print Building Rooms\n");
    printf("4. Run Your Test Function\n");
    printf("5. Exit\n");

    do {
        printf("Enter your choice (1-5): ");
        valid = scanf("%d", &choice);

        while (getchar() != '\n'); // Clear input buffer
    } while (!valid || choice < 1 || choice > 5);

    return (enum MenuOptions) choice;
}

int run_test_function() {
    printf("Running test function...\n");
    printf("=========================================\n");
    
    // ===================================================================
    // TEST SECTION 1: Ghost Functions
    // ===================================================================
    printf("\n=== SECTION 1: Testing Ghost Functions ===\n");
    
    // Test 1.1: ghost_create - Basic creation
    printf("\nTest 1.1: Creating a single ghost\n");
    Ghost *ghost1;
    ghost_create(&ghost1, "TestSpirit");
    printf("  Expected: Ghost created with type 'TestSpirit', likelihood 0.0, room NULL\n");
    printf("  Actual: type='%s', likelihood=%.2f, room=%s\n", 
           ghost1->type, ghost1->likelihood, 
           ghost1->room == NULL ? "NULL" : "NOT NULL");
    
    // Test 1.2: ghost_create - Unique ID generation
    printf("\nTest 1.2: Testing unique ID generation\n");
    Ghost *ghost2;
    ghost_create(&ghost2, "AnotherSpirit");
    printf("  Expected: ghost2->id should be ghost1->id + 1\n");
    printf("  ghost1 ID: %d, ghost2 ID: %d\n", ghost1->id, ghost2->id);
    printf("  Result: %s\n", (ghost2->id == ghost1->id + 1) ? "PASS" : "FAIL");
    
    // Test 1.3: ghost_cleanup - Cleaning up ghosts
    printf("\nTest 1.3: Cleaning up individual ghosts\n");
    ghost_cleanup(&ghost1);
    ghost_cleanup(&ghost2);
    printf("  Expected: Both ghost pointers set to NULL after cleanup\n");
    printf("  ghost1: %s, ghost2: %s\n", 
           ghost1 == NULL ? "NULL" : "NOT NULL",
           ghost2 == NULL ? "NULL" : "NOT NULL");
    printf("  Result: %s\n", 
           (ghost1 == NULL && ghost2 == NULL) ? "PASS" : "FAIL");
    
    // ===================================================================
    // TEST SECTION 2: GhostList Functions
    // ===================================================================
    printf("\n=== SECTION 2: Testing GhostList Functions ===\n");
    
    // Test 2.1: ghostlist_init - Initialize empty list
    printf("\nTest 2.1: Initializing an empty ghost list\n");
    GhostList list;
    ghostlist_init(&list);
    printf("  Expected: head and tail should be NULL\n");
    printf("  Actual: head=%s, tail=%s\n",
           list.head == NULL ? "NULL" : "NOT NULL",
           list.tail == NULL ? "NULL" : "NOT NULL");
    printf("  Result: %s\n", 
           (list.head == NULL && list.tail == NULL) ? "PASS" : "FAIL");
    
    // Test 2.2: ghostlist_push - Adding to empty list
    printf("\nTest 2.2: Pushing first ghost to empty list\n");
    Ghost *g1;
    ghost_create(&g1, "FirstGhost");
    ghostlist_push(&list, g1);
    printf("  Expected: head and tail should both point to the same node\n");
    printf("  Result: %s\n", 
           (list.head == list.tail && list.head != NULL) ? "PASS" : "FAIL");
    
    // Test 2.3: ghostlist_push - Adding multiple ghosts
    printf("\nTest 2.3: Pushing multiple ghosts to list\n");
    Ghost *g2, *g3;
    ghost_create(&g2, "SecondGhost");
    ghost_create(&g3, "ThirdGhost");
    ghostlist_push(&list, g2);
    ghostlist_push(&list, g3);
    printf("  Expected: List should have 3 nodes, tail should be last added\n");
    printf("  Head ghost: %s, Tail ghost: %s\n", 
           list.head->data->type, list.tail->data->type);
    printf("  Result: %s\n", 
           (list.tail->data == g3) ? "PASS" : "FAIL");
    
    // Test 2.4: ghostlist_cleanup - Cleanup without freeing data
    printf("\nTest 2.4: Cleaning up list nodes without freeing ghost data\n");
    ghostlist_cleanup(&list, false);
    printf("  Expected: List head and tail NULL, but ghosts still exist\n");
    printf("  List: head=%s, tail=%s\n",
           list.head == NULL ? "NULL" : "NOT NULL",
           list.tail == NULL ? "NULL" : "NOT NULL");
    printf("  Ghost still accessible: %s\n", g1->type);
    printf("  Result: %s\n", 
           (list.head == NULL && list.tail == NULL) ? "PASS" : "FAIL");
    
    // Manually clean up ghosts since we didn't free them in cleanup
    ghost_cleanup(&g1);
    ghost_cleanup(&g2);
    ghost_cleanup(&g3);
    
    // Test 2.5: ghostlist_insert_by_likelihood - Empty list
    printf("\nTest 2.5: Inserting into empty list by likelihood\n");
    ghostlist_init(&list);
    Ghost *ghost_mid;
    ghost_create(&ghost_mid, "MidLikelihood");
    ghost_mid->likelihood = 50.0;
    ghostlist_insert_by_likelihood(&list, ghost_mid);
    printf("  Expected: Single ghost becomes both head and tail\n");
    printf("  Result: %s\n", 
           (list.head->data == ghost_mid && list.tail->data == ghost_mid) 
           ? "PASS" : "FAIL");
    
    // Test 2.6: ghostlist_insert_by_likelihood - Insert at head
    printf("\nTest 2.6: Inserting ghost with highest likelihood\n");
    Ghost *ghost_high;
    ghost_create(&ghost_high, "HighLikelihood");
    ghost_high->likelihood = 90.0;
    ghostlist_insert_by_likelihood(&list, ghost_high);
    printf("  Expected: High likelihood ghost should be new head\n");
    printf("  Head likelihood: %.2f (should be 90.0)\n", 
           list.head->data->likelihood);
    printf("  Result: %s\n", 
           (list.head->data == ghost_high) ? "PASS" : "FAIL");
    
    // Test 2.7: ghostlist_insert_by_likelihood - Insert at tail
    printf("\nTest 2.7: Inserting ghost with lowest likelihood\n");
    Ghost *ghost_low;
    ghost_create(&ghost_low, "LowLikelihood");
    ghost_low->likelihood = 10.0;
    ghostlist_insert_by_likelihood(&list, ghost_low);
    printf("  Expected: Low likelihood ghost should be new tail\n");
    printf("  Tail likelihood: %.2f (should be 10.0)\n", 
           list.tail->data->likelihood);
    printf("  Result: %s\n", 
           (list.tail->data == ghost_low) ? "PASS" : "FAIL");
    
    // Test 2.8: ghostlist_insert_by_likelihood - Same likelihood
    printf("\nTest 2.8: Inserting ghost with same likelihood as existing\n");
    Ghost *ghost_dup;
    ghost_create(&ghost_dup, "DupLikelihood");
    ghost_dup->likelihood = 50.0; // Same as ghost_mid
    ghostlist_insert_by_likelihood(&list, ghost_dup);
    printf("  Expected: New ghost inserted BEFORE existing ghost with same likelihood\n");
    // Find the 50.0 likelihood ghosts
    GhostNode *curr = list.head;
    while (curr && curr->data->likelihood > 50.0) {
        curr = curr->next;
    }
    printf("  First 50.0 ghost: %s (should be DupLikelihood)\n", 
           curr ? curr->data->type : "NOT FOUND");
    printf("  Result: %s\n", 
           (curr && strcmp(curr->data->type, "DupLikelihood") == 0) 
           ? "PASS" : "FAIL");
    
    // Test 2.9: Verify complete sort order
    printf("\nTest 2.9: Verifying complete descending sort order\n");
    printf("  Expected order: 90.0, 50.0, 50.0, 10.0\n");
    printf("  Actual order: ");
    curr = list.head;
    int sorted_correctly = 1;
    float prev_likelihood = 100.0;
    while (curr) {
        printf("%.2f ", curr->data->likelihood);
        if (curr->data->likelihood > prev_likelihood) {
            sorted_correctly = 0;
        }
        prev_likelihood = curr->data->likelihood;
        curr = curr->next;
    }
    printf("\n  Result: %s\n", sorted_correctly ? "PASS" : "FAIL");
    
    // Cleanup list with data
    ghostlist_cleanup(&list, true);
    
    // ===================================================================
    // TEST SECTION 3: Room Functions
    // ===================================================================
    printf("\n=== SECTION 3: Testing Room Functions ===\n");
    
    // Test 3.1: room_create - Basic creation
    printf("\nTest 3.1: Creating a room\n");
    Room *room1;
    room_create(&room1, 100, "TestRoom");
    printf("  Expected: Room with id=100, name='TestRoom', empty ghost list\n");
    printf("  Actual: id=%d, name='%s', ghost list head=%s\n",
           room1->id, room1->name,
           room1->ghosts.head == NULL ? "NULL" : "NOT NULL");
    printf("  Result: %s\n", 
           (room1->id == 100 && strcmp(room1->name, "TestRoom") == 0 
            && room1->ghosts.head == NULL) ? "PASS" : "FAIL");
    
    // Test 3.2: room_add_ghost - Adding first ghost
    printf("\nTest 3.2: Adding first ghost to room\n");
    Ghost *room_ghost1;
    ghost_create(&room_ghost1, "RoomSpirit1");
    room_add_ghost(room1, room_ghost1, 75.5);
    printf("  Expected: Ghost added, likelihood set, room pointer updated\n");
    printf("  Ghost likelihood: %.2f, Ghost's room: %s\n",
           room_ghost1->likelihood, room_ghost1->room->name);
    printf("  Result: %s\n", 
           (room_ghost1->likelihood == 75.5f && room_ghost1->room == room1) 
           ? "PASS" : "FAIL");
    
    // Test 3.3: room_add_ghost - Multiple ghosts in sorted order
    printf("\nTest 3.3: Adding multiple ghosts to room (sorted)\n");
    Ghost *room_ghost2, *room_ghost3;
    ghost_create(&room_ghost2, "RoomSpirit2");
    ghost_create(&room_ghost3, "RoomSpirit3");
    room_add_ghost(room1, room_ghost2, 25.0);
    room_add_ghost(room1, room_ghost3, 99.9);
    printf("  Expected order: 99.9, 75.5, 25.0\n");
    printf("  Actual order: ");
    curr = room1->ghosts.head;
    while (curr) {
        printf("%.1f ", curr->data->likelihood);
        curr = curr->next;
    }
    printf("\n  Result: %s\n",
           (room1->ghosts.head->data == room_ghost3) ? "PASS" : "FAIL");
    
    // Test 3.4: room_cleanup - Freeing room but not ghosts
    printf("\nTest 3.4: Cleaning up room (nodes only, not ghost data)\n");
    room_cleanup(&room1);
    printf("  Expected: room1 pointer NULL, but ghosts still accessible\n");
    printf("  room1: %s, room_ghost1 type: %s\n",
           room1 == NULL ? "NULL" : "NOT NULL",
           room_ghost1->type);
    printf("  Result: %s\n", 
           (room1 == NULL) ? "PASS" : "FAIL");
    
    // Clean up the ghosts manually
    ghost_cleanup(&room_ghost1);
    ghost_cleanup(&room_ghost2);
    ghost_cleanup(&room_ghost3);
    
    // ===================================================================
    // TEST SECTION 4: RoomArray Functions
    // ===================================================================
    printf("\n=== SECTION 4: Testing RoomArray Functions ===\n");
    
    // Test 4.1: roomarray_init - Initialize empty array
    printf("\nTest 4.1: Initializing room array\n");
    RoomArray array;
    roomarray_init(&array);
    printf("  Expected: size=0, all elements NULL\n");
    printf("  Actual: size=%d\n", array.size);
    printf("  Result: %s\n", (array.size == 0) ? "PASS" : "FAIL");
    
    // Test 4.2: roomarray_add - Adding single room
    printf("\nTest 4.2: Adding first room to array\n");
    Room *arr_room1;
    room_create(&arr_room1, 1, "ArrayRoom1");
    roomarray_add(&array, arr_room1);
    printf("  Expected: size=1, first element not NULL\n");
    printf("  Actual: size=%d, first element=%s\n",
           array.size, array.elements[0] != NULL ? "NOT NULL" : "NULL");
    printf("  Result: %s\n", 
           (array.size == 1 && array.elements[0] == arr_room1) ? "PASS" : "FAIL");
    
    // Test 4.3: roomarray_add - Adding multiple rooms
    printf("\nTest 4.3: Adding multiple rooms to array\n");
    for (int i = 1; i < 5; i++) {
        Room *r;
        char name[MAX_STR];
        snprintf(name, MAX_STR, "ArrayRoom%d", i + 1);
        room_create(&r, i + 1, name);
        roomarray_add(&array, r);
    }
    printf("  Expected: size=5\n");
    printf("  Actual: size=%d\n", array.size);
    printf("  Result: %s\n", (array.size == 5) ? "PASS" : "FAIL");
    
    // Test 4.4: roomarray_add - Handling full array
    printf("\nTest 4.4: Attempting to add to full array\n");
    // Fill up the array
    while (array.size < MAX_ROOMS) {
        Room *r;
        char name[MAX_STR];
        snprintf(name, MAX_STR, "FillRoom%d", array.size);
        room_create(&r, array.size + 100, name);
        roomarray_add(&array, r);
    }
    printf("  Array is now full: size=%d (MAX_ROOMS=%d)\n", 
           array.size, MAX_ROOMS);
    
    Room *overflow_room;
    room_create(&overflow_room, 999, "OverflowRoom");
    int size_before = array.size;
    roomarray_add(&array, overflow_room); // Should handle gracefully
    printf("  Expected: Array size unchanged, graceful handling\n");
    printf("  Size before: %d, Size after: %d\n", size_before, array.size);
    printf("  Result: %s\n", (array.size == size_before) ? "PASS" : "FAIL");
    
    // Clean up overflow room since it wasn't added
    room_cleanup(&overflow_room);
    
    // Test 4.5: roomarray_cleanup - Freeing all rooms
    printf("\nTest 4.5: Cleaning up entire room array\n");
    roomarray_cleanup(&array);
    printf("  Expected: size=0, all rooms freed\n");
    printf("  Actual: size=%d\n", array.size);
    printf("  Result: %s\n", (array.size == 0) ? "PASS" : "FAIL");
    
    // ===================================================================
    // FINAL SUMMARY
    // ===================================================================
    printf("\n=========================================\n");
    printf("Test function completed.\n");
    printf("Review the results above for PASS/FAIL status.\n");
    printf("=========================================\n");
    
    return 0;
}