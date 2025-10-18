#include "configMaster.h"

config_maester_citadel_t* load_maester_config(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }

    config_maester_citadel_t* config = malloc(sizeof(config_maester_citadel_t));
    if (!config) {
        close(fd);
        return NULL;
    }

    char buffer[4096];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        free(config);
        return NULL;
    }
    buffer[bytes_read] = '\0';

    char* line = strtok(buffer, "\n");
    int line_num = 0;
    int reading_routes = 0;
    config->num_routes = 0;
    config->routes = malloc(sizeof(route_citadel_t) * MAX_ROUTES);

    while (line != NULL) {
        if (line_num == 0) {
            strncpy(config->kingdom_name, line, MAX_KINGDOM_NAME - 1);
        } else if (line_num == 1) {
            strncpy(config->user_files_path, line, MAX_PATH_LENGTH - 1);
        } else if (line_num == 2) {
            config->num_envoys = atoi(line);
        } else if (line_num == 3) {
            strncpy(config->ip, line, MAX_IP_LENGTH - 1);
        } else if (line_num == 4) {
            config->port = atoi(line);
        } else if (strcmp(line, "--- ROUTES ---") == 0) {
            reading_routes = 1;
        } else if (reading_routes) {
            char kingdom[MAX_KINGDOM_NAME];
            char ip[MAX_IP_LENGTH];
            int port;
            
            if (sscanf(line, "%s %s %d", kingdom, ip, &port) == 3) {
                if (strcmp(kingdom, "DEFAULT") == 0) {
                    strncpy(config->default_ip, ip, MAX_IP_LENGTH - 1);
                    config->default_port = port;
                } else {
                    strncpy(config->routes[config->num_routes].kingdom_name, kingdom, MAX_KINGDOM_NAME - 1);
                    strncpy(config->routes[config->num_routes].ip, ip, MAX_IP_LENGTH - 1);
                    config->routes[config->num_routes].port = port;
                    config->num_routes++;
                }
            }
        }
        
        line = strtok(NULL, "\n");
        line_num++;
    }

    return config;
}

void free_maester_config(config_maester_citadel_t* config) {
    if (config) {
        if (config->routes) {
            free(config->routes);
        }
        free(config);
    }
}

route_citadel_t* find_route(config_maester_citadel_t* config, const char* kingdom_name) {
    if (!config || !kingdom_name) {
        return NULL;
    }

    for (int i = 0; i < config->num_routes; i++) {
        if (strcmp(config->routes[i].kingdom_name, kingdom_name) == 0) {
            return &config->routes[i];
        }
    }

    return NULL;
}
