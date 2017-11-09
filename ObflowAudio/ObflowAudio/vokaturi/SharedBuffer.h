/*
 * SharedBuffer.h
 *
 * Copyright (C) 2016 Vokaturi
 * version 2016-07-06
 *
 * This code is part of the Vokamono demo app.
 *
 * You can freely adapt this code for your own software.
 * It comes with no warranty of any kind.
 */

#include <stdint.h>   // for int64_t
#include <libkern/OSAtomic.h>   // for OSAtomicAdd64Barrier

/*
    A buffer is shared between the render callback,
    which writes into it from the recording thread,
    and the analysis software, which reads from it in a timer callback in the GUI thread.

    The recording thread typically puts 512 samples into the buffer
    during the render callback every 1.2 milliseconds.
    After writing the 512 samples, the render callback has to update the sample pointer
    in an atomic manner.

    The GUI timer callback, which is typically called every 40 milliseconds,
    feeds any new samples to the analysis software and then performs an analysis;
    after this, the timer callback notifies the GUI.
    To know how many samples to feed, the time callback has to inspect
    the sample pointer in an atomic manner.
    
    With the atomic update and the atomic read, there is no need for locks.
    The samples that are read from the shared buffer will be a safe distance away
    from where the recording thread is writing new samples.
*/

#define SHARED_BUFFER_SIZE  220500

/*
    The buffer is actually a ring buffer,
    i.e. it will start to overwrite its first samples once the buffer is full.

    Instead of a sample pointer that keeps track of exactly where in the buffer
    we are writing, we maintain a sample count ("numberOfReceivedSamples"),
    which keeps track of how many samples have been written
    into the buffer since this count was set to zero (typically, at program start-up).

    The sample pointer can be computed from the sample count as follows:

        int64_t sampleCount =
            OSAtomicAdd64Barrier (0, theSharedBuffer.numberOfReceivedSamples);
        samplePointer = sampleCount % SHARED_BUFFER_SIZE;
*/
extern struct SharedBuffer
{
    double samples [SHARED_BUFFER_SIZE];   // so that's approximately 1.76 megabytes
    /*
        The sample count has to be:
        - volatile, because addresses of atomically updated variables have to be
          volatile, which is because multiple threads could write to such variables;
          in our case, the recording thread updates the sample count,
          and the GUI thread reads it.
        - aligned to an 8-byte boundary, as required by OSAtomicAdd64Barrier ();
          on 32-bit iOS,
          it is not guaranteed that an int64_t is automatically so aligned.
    */
    volatile
    __attribute__((aligned(8)))   // the iOS way to denote byte alignment of variables
        int64_t numberOfReceivedSamples;
    int64_t numberOfSentSamples;
} theSharedBuffer;

/* End of file SharedBuffer.h */
