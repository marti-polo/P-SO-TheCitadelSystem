#ifndef CONFIGMASTER_H
#define CONFIGMASTER_H

#define _GNU_SOURCE 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define MAX_KINGDOM_NAME 100
#define MAX_PATH_LENGTH 256
#define MAX_IP_LENGTH 16
#define MAX_ROUTES 50

typedef struct {
    char kingdom_name[MAX_KINGDOM_NAME];
    char ip[MAX_IP_LENGTH];
    int port;
} route_citadel_t;

typedef struct {
    char kingdom_name[MAX_KINGDOM_NAME];
    char user_files_path[MAX_PATH_LENGTH];
    int num_envoys;
    char ip[MAX_IP_LENGTH];
    int port;
    route_citadel_t* routes;
    int num_routes;
    char default_ip[MAX_IP_LENGTH];
    int default_port;
} config_maester_citadel_t;

config_maester_citadel_t* load_maester_config(const char* filename);
void free_maester_config(config_maester_citadel_t* config);
route_citadel_t* find_route(config_maester_citadel_t* config, const char* kingdom_name);

#endif 