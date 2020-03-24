# CircularBuffer

This is a simple circular buffer API implemeneted in C.  Functions are made available through a buffer handle.

Unit testing is is done with Unity.  Copying Unity into a visible library directory is required, and the includes in the testing files under /tests must reflect this location.

## Building

A unit test runner executable can be built by running the /test/build.sh script.

## Usage

// Pass in a storage buffer address and size
// User must allocate memeory for buffer themselves, we will allocate
//  for the circular buffer structure memory
// Returns a circular buffer handle
cbuf_handle_t circular_buf_init (int8_t* buffer, size_t size);

// Free circular buffer structure from memory
// Does not free data buffer, that is user's responsibility
void circular_buf_free (cbuf_handle_t cbuf);

// Reset circular buffer to empty, head == tail
void circular_buf_reset (cbuf_handle_t cbuf);

// Enquque element
// Bool flag if overwriting is desired, or a return of BUFFER_FULL
int cirular_buf_enqueue (cbuf_handle_t cbuf, int8_t element, bool overwrite);

// Dequeue element, write to passed element reference
// Returns SUCCESS or BUFFER_EMPTY
int circular_buf_dequeue (cbuf_handle_t cbuf, int8_t * element );

// Returns if buffer is full or not
bool circular_buf_full (cbuf_handle_t cbuf);

// Returns if buffer is empty or not
bool circular_buf_empty (cbuf_handle_t cbuf);

// Returns maximum capacity of buffer
size_t circular_buf_size (cbuf_handle_t cbuf);

// Returns number of elements currently in buffer
size_t circular_buf_count (cbuf_handle_t cbuf);


## Author and date

Richard JH McCrae
15.01.2019
