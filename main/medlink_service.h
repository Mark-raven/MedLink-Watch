#ifndef MEDLINK_SERVICE_H
#define MEDLINK_SERVICE_H

void medlink_service_init(void);

/* Public API for sending BLE notifications */
void medlink_notify(const char *message);


#endif