/**
 * GreenPois0n Absinthe - crashreportmover.c
 * Copyright (C) 2010 Chronic-Dev Team
 * Copyright (C) 2010 Joshua Hill
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libimobiledevice/libimobiledevice.h>

#include "debug.h"
#include "device.h"
#include "lockdown.h"
#include "crashreportmover.h"

crashreportmover_t* crashreportmover_create() {
	crashreportmover_t* mover = NULL;
	if(mover != NULL) {
		memset(mover, '\0', sizeof(mover));
	}
	return mover;
}

crashreportmover_t* crashreportmover_connect(device_t* device) {
	int err = 0;
	uint16_t port = 0;
	crashreportmover_t* mover = NULL;
	lockdown_t* lockdown = NULL;

	lockdown = lockdown_open(device);
	if(lockdown == NULL) {
		error("Unable to open connection to lockdownd\n");
		return NULL;
	}

	err = lockdown_start_service(lockdown, "com.apple.crashreportmover", &port);
	if(err < 0) {
		error("Unable to start CrashReport mover service\n");
		return NULL;
	}
	lockdown_close(lockdown);

	mover = crashreportmover_open(device, port);
	if(mover == NULL) {
		error("Unable to open connection to CrashReporter mover service\n");
		return NULL;
	}

	return mover;
}

crashreportmover_t* crashreportmover_open(device_t* device, uint16_t port) {
	int err = 0;
	crashreportmover_t* mover = crashreportmover_create();

	err = idevice_connect(device->client, port, &(mover->connection));
	if(err < 0) {
		return NULL;
	}
	idevice_disconnect(mover->connection);
	return mover;
}



int crashreportmover_close(crashreportmover_t* mover) {
	return -1;
}

void crashreportmover_free(crashreportmover_t* mover) {
	if(mover) {
		free(mover);
	}
}