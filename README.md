# Ghost Tracking System

A C-based ghost tracking and management system that organizes suspected paranormal activity across building rooms.
## Overview

This program implements a data management system for tracking ghosts and their likelihood of appearing in different rooms of a building. It demonstrates dynamic memory allocation, linked list operations, and proper memory management in C.

### Key Features

- **Dynamic Ghost Management**: Create and track ghosts with unique auto-incrementing IDs
- **Singly Linked Lists**: Efficient ghost storage with head/tail pointer maintenance
- **Sorted Insertion**: Ghosts are automatically sorted by likelihood percentage (descending)
- **Fixed-Size Room Arrays**: Building structure with room capacity limits
- **Memory Safety**: Proper allocation and deallocation without leaks or double-frees
- **Comprehensive Testing**: Built-in test suite validating all functionality

## Project Structure

```
.
├── main.c              # Program entry point and interactive menu
├── ghost.c             # Ghost and GhostList implementation
├── room.c              # Room and RoomArray implementation
├── building.c          # Building structure and sample data loader
├── defs.h              # Type definitions and function declarations
└── README.md           # This file
```

## Data Structures

### Ghost Structure
```c
struct Ghost {
    int id;                 // Unique identifier (auto-incremented from 1031)
    char type[MAX_STR];     // Ghost type (e.g., "Banshee", "Wraith")
    float likelihood;       // Percentage likelihood of presence (0-100)
    Room* room;            // Pointer to associated room
};
```

### GhostList (Singly Linked List)
```c
struct GhostList {
    GhostNode* head;       // First node in list
    GhostNode* tail;       // Last node in list (for O(1) append)
};

struct GhostNode {
    Ghost* data;           // Pointer to ghost data
    GhostNode* next;       // Next node in list
};
```

### Room Structure
```c
struct Room {
    int id;                // Room identifier
    char name[MAX_STR];    // Room name
    GhostList ghosts;      // Sorted list of ghosts (by likelihood)
};
```

### RoomArray (Fixed-Size Array)
```c
struct RoomArray {
    Room* elements[MAX_ROOMS];  // Array of room pointers (max 16)
    int size;                   // Current number of rooms
};
```

### Building Structure
```c
struct Building {
    RoomArray rooms;       // All rooms in the building
    GhostList ghosts;      // Master list of all ghosts
};
```

## Requirements

- **Compiler**: GCC with C standard library support
- **System**: Linux/Unix environment (tested on Ubuntu)
- **Tools**: Make or direct GCC compilation
- **Optional**: Valgrind for memory leak detection

## Installation & Building

### Extract Files (if archived)
```bash
tar -xvf a4_101345637.tar
cd assignment4
```

### Compilation
```bash
gcc -g -Wall -o ghost_hunter main.c ghost.c room.c building.c
```

**Compiler Flags Explained**:
- `-g`: Include debugging symbols for GDB/Valgrind
- `-Wall`: Enable all warnings
- `-o ghost_hunter`: Output executable name

### Successful Compilation
No error messages should appear. Verify executable creation:
```bash
ls -l ghost_hunter
```

## Usage

### Running the Program
```bash
./ghost_hunter
```

### Interactive Menu

```
Menu:
1. Load Sample Data
2. Print Ghost List
3. Print Building Rooms
4. Run Your Test Function
5. Exit
Enter your choice (1-5):
```

### Menu Options Explained

#### 1. Load Sample Data
Loads predefined building with:
- **8 rooms**: Bedroom, Bathroom, Living Room, Kitchen, Basement, Garage, Hallway, Staircase
- **21 ghosts**: 6 ghost types (Banshee, Wraith, Phantom, Danny, Bullies, Yokai, Poltergeist)
- **Multiple associations**: Ghosts can appear in multiple rooms with different likelihoods

**Sample Data Distribution**:
- Banshee: Kitchen (82.51%), Bathroom (19.99%)
- Wraith: 5 rooms with varying likelihoods
- Bullies: 5 rooms with varying likelihoods
- Poltergeist: Living Room (87.67%), Bedroom (19.82%)

#### 2. Print Ghost List
Displays all ghosts in the building's master list in order of creation:

```
  - {id: 1031, type: Banshee, likelihood: 82.51%, room: Kitchen}
  - {id: 1032, type: Banshee, likelihood: 19.99%, room: Bathroom}
  - {id: 1033, type: Wraith, likelihood: 72.21%, room: Basement}
  - {id: 1034, type: Wraith, likelihood: 6.01%, room: Garage}
  - {id: 1035, type: Wraith, likelihood: 97.99%, room: Hallway}
  ...
```

#### 3. Print Building Rooms
Displays each room with its ghosts **sorted by likelihood** (highest to lowest):

```
{id: 1, name: Bedroom}
  Ghosts:
  - {id: 1037, type: Wraith, likelihood: 88.78%, room: Bedroom}
  - {id: 1051, type: Poltergeist, likelihood: 19.82%, room: Bedroom}

{id: 2, name: Bathroom}
  Ghosts:
  - {id: 1049, type: Yokai, likelihood: 87.67%, room: Bathroom}
  - {id: 1047, type: Bullies, likelihood: 27.75%, room: Bathroom}
  - {id: 1032, type: Banshee, likelihood: 19.99%, room: Bathroom}

{id: 4, name: Kitchen}
  Ghosts:
  - {id: 1044, type: Bullies, likelihood: 98.74%, room: Kitchen}
  - {id: 1031, type: Banshee, likelihood: 82.51%, room: Kitchen}
```

**Note**: Ghosts are sorted in **descending order** by likelihood within each room.

#### 4. Run Your Test Function
Executes comprehensive test suite covering:

**Section 1: Ghost Functions**
- Ghost creation and initialization
- Unique ID auto-increment verification
- Ghost cleanup and pointer nullification

**Section 2: GhostList Functions**
- List initialization (empty state)
- Push operation (head/tail management)
- Sorted insertion by likelihood (descending)
- Cleanup with/without freeing ghost data
- Same likelihood handling (insertion before existing)
- Complete sort order verification

**Section 3: Room Functions**
- Room creation and initialization
- Adding ghosts to rooms with likelihood
- Multiple ghosts in sorted order
- Room cleanup (nodes only, preserving ghost data)

**Section 4: RoomArray Functions**
- Array initialization
- Adding rooms to array
- Overflow handling (graceful rejection)
- Complete array cleanup

**Test Output Format**:
```
Test X.Y: Description
  Expected: What should happen
  Actual: What did happen
  Result: PASS/FAIL
```

#### 5. Exit
Properly cleans up all allocated memory and exits gracefully.

## Memory Management Strategy

### Ownership Model

**Building** owns:
- All Ghost data (freed in `building_cleanup`)
- All Room data (freed via `roomarray_cleanup`)

**Rooms** contain:
- Ghost list **nodes only** (not the ghost data itself)
- Nodes freed in `room_cleanup` with `free_data = false`

**Ghost Lists** have dual cleanup modes:
```c
ghostlist_cleanup(list, true);   // Free nodes AND ghost data
ghostlist_cleanup(list, false);  // Free nodes only (preserve data)
```

### Cleanup Order
1. `building_cleanup()` called first
2. Rooms cleaned (list nodes only): `roomarray_cleanup()`
3. Building's master ghost list cleaned (nodes + data): `ghostlist_cleanup(..., true)`

This prevents **double-free errors** since ghost data is freed only once.

## Algorithm Analysis

### Time Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Ghost Creation | O(1) | Static ID counter |
| Ghost List Push | O(1) | Direct tail append |
| Sorted Insert | O(n) | Linear scan for position |
| Room Lookup | O(n) | Linear array search |
| Print Operations | O(n) | Traverse all elements |

### Space Complexity

| Structure | Space | Notes |
|-----------|-------|-------|
| Ghost | O(1) | Fixed size struct |
| GhostNode | O(1) | Data pointer + next pointer |
| GhostList | O(n) | n = number of ghosts |
| Room | O(1 + m) | m = ghosts in room |
| RoomArray | O(k) | k ≤ MAX_ROOMS (16) |

## Key Implementation Details

### Unique ID Generation
```c
void ghost_create(Ghost** ghost, const char* type) {
    static int next_id = GHOST_INITIAL_ID;  // 1031
    
    *ghost = malloc(sizeof(Ghost));
    (*ghost)->id = next_id++;  // Auto-increment
    strcpy((*ghost)->type, type);
    (*ghost)->likelihood = 0.0;
    (*ghost)->room = NULL;
}
```

**Static variable** ensures IDs persist across function calls.

### Sorted Insertion Algorithm
```c
void ghostlist_insert_by_likelihood(GhostList* list, Ghost* ghost) {
    // Case 1: Empty list - new node becomes head and tail
    
    // Case 2: Insert at head (highest likelihood)
    if (ghost->likelihood >= list->head->data->likelihood) {
        // Insert before current head
    }
    
    // Case 3: Insert in middle or tail
    // Find node BEFORE insertion point
    while (curr->next != NULL && 
           curr->next->data->likelihood > ghost->likelihood) {
        curr = curr->next;
    }
    // Insert after curr, update tail if necessary
}
```

**Maintains descending order**: Highest likelihoods appear first.

### Room-Ghost Association
```c
void room_add_ghost(Room* room, Ghost* ghost, float likelihood) {
    ghost->room = room;               // Bidirectional link
    ghost->likelihood = likelihood;   // Set likelihood
    
    ghostlist_insert_by_likelihood(&room->ghosts, ghost);  // Sorted add
}
```

**Bidirectional linking** allows navigation from ghost → room and room → ghosts.

## Testing & Validation

### Memory Leak Detection
```bash
valgrind --leak-check=full --show-leak-kinds=all ./ghost_hunter
```

**Expected output**:
```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  total heap usage: X allocs, X frees, Y bytes allocated

All heap blocks were freed -- no leaks are possible
```

### Automated Testing
Select **Option 4** from the menu to run all test cases:
- 9 GhostList tests
- 4 Room tests  
- 5 RoomArray tests
- Edge case handling

**All tests should display**: `Result: PASS`

### Manual Verification
1. Load sample data (Option 1)
2. Print ghost list (Option 2) - verify 21 ghosts
3. Print building rooms (Option 3) - verify 8 rooms, sorted ghosts
4. Exit (Option 5) - run Valgrind to confirm no leaks

## Sample Usage Session

```bash
$ ./ghost_hunter

Menu:
1. Load Sample Data
2. Print Ghost List
3. Print Building Rooms
4. Run Your Test Function
5. Exit
Enter your choice (1-5): 1

Menu:
1. Load Sample Data
2. Print Ghost List
3. Print Building Rooms
4. Run Your Test Function
5. Exit
Enter your choice (1-5): 3

{id: 1, name: Bedroom}
  Ghosts:
  - {id: 1037, type: Wraith, likelihood: 88.78%, room: Bedroom}
  - {id: 1051, type: Poltergeist, likelihood: 19.82%, room: Bedroom}

{id: 2, name: Bathroom}
  Ghosts:
  - {id: 1049, type: Yokai, likelihood: 87.67%, room: Bathroom}
  - {id: 1047, type: Bullies, likelihood: 27.75%, room: Bathroom}
  - {id: 1032, type: Banshee, likelihood: 19.99%, room: Bathroom}

...

Menu:
1. Load Sample Data
2. Print Ghost List
3. Print Building Rooms
4. Run Your Test Function
5. Exit
Enter your choice (1-5): 5
Exiting program.
```

## Common Issues & Troubleshooting

### Compilation Errors

**Problem**: Undefined references
```bash
undefined reference to `ghostlist_init'
```
**Solution**: Ensure all `.c` files are included in compilation command:
```bash
gcc -g -Wall -o ghost_hunter main.c ghost.c room.c building.c
```

### Memory Leaks

**Problem**: Valgrind reports "still reachable" blocks
**Solution**: Verify `building_cleanup` is called before program exit

**Problem**: Double-free detected
**Solution**: Check `ghostlist_cleanup` free_data flag usage:
- Rooms: `ghostlist_cleanup(..., false)` 
- Building: `ghostlist_cleanup(..., true)`

### Segmentation Faults

**Problem**: Crash when printing rooms
**Solution**: Ensure rooms are properly initialized with `room_create` before use

**Problem**: Crash in ghostlist operations
**Solution**: Always initialize lists with `ghostlist_init` before first use

## Known Limitations

- Maximum 16 rooms per building (`MAX_ROOMS`)
- Maximum 32-character names/types (`MAX_STR`)
- No persistence (data lost on exit)
- Single building instance
- No ghost removal functionality
- Fixed sample data only (no user input for custom data)

## Learning Objectives Demonstrated

- ✅ Dynamic memory allocation (`malloc`, `free`)
- ✅ Linked list implementation (singly linked)
- ✅ Sorted insertion algorithms
- ✅ Fixed-size array management
- ✅ Pointer manipulation (single, double, and structure pointers)
- ✅ Structure design and relationships
- ✅ Memory ownership models
- ✅ Preventing memory leaks and double-frees
- ✅ Modular C programming
- ✅ Function documentation
- ✅ Comprehensive testing strategies
- ✅ Interactive menu systems

## Constants Reference

```c
#define MAX_STR 32           // Maximum string length for names/types
#define MAX_ROOMS 16         // Maximum rooms in building
#define GHOST_INITIAL_ID 1031 // Starting ID for ghosts
```

## Credits

- **Developer**: Nilay Sorathia (101345637)
- **Course**: COMP 2401 - Introduction to Systems Programming
- **Institution**: Carleton University
- **Course Instructors**: COMP 2401 Teaching Staff

## License

This project was created for academic purposes at Carleton University. All rights reserved.
