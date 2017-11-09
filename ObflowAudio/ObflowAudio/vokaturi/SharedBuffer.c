/*
 * SharedBuffer.c
 *
 * Copyright (C) 2016 Vokaturi
 * version 2016-07-06
 *
 * This code is part of the Vokamono demo app.
 *
 * You can freely adapt this code for your own software.
 * It comes with no warranty of any kind.
 */

#include "SharedBuffer.h"

/*
	The definition of the global struct via which the recording thread communicates
	with the GUI thread.
*/
struct SharedBuffer theSharedBuffer;

/* End of file SharedBuffer.c */
