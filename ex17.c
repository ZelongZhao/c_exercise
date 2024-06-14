#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//#define MAX_DATA 512
//#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char* name,* email;
};

struct Database {
    size_t max_data;
    size_t max_rows;
    struct Address* rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};


void Connection_close(struct Connection *conn)
{
    if (conn) {
        if (conn->db) {
            if (conn->db->rows) {
                for (size_t i = 0; i < conn->db->max_rows; i++) {
                    struct Address *addr = &conn->db->rows[i];
                    if (addr->name) free(addr->name);
                    if (addr->email) free(addr->email);
                }
                free(conn->db->rows);
            }
            free(conn->db);
        }
        if (conn->file) fclose(conn->file);
        free(conn);
    }
}

void die(struct Connection *conn, const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    if(conn){
        Connection_close(conn);
    }

    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
           addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
//    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
//    if(rc != 1) die(conn, "Failed to load database.");

    int rc = fread(conn->db,sizeof (struct Database),1,conn->file);
    if(rc != 1) die(conn, "Failed to load database.");

    conn->db->rows = (struct Address*) malloc(conn->db->max_rows * sizeof (struct Address));

    for(int i = 0; i < conn->db->max_rows; i++){
        rc = fread(&conn->db->rows[i].id,sizeof (int),1,conn->file);
        if(rc != 1) die(conn, "Failed to load id.");
        rc = fread(&conn->db->rows[i].set,sizeof (int),1,conn->file);
        if(rc != 1) die(conn, "Failed to load set.");

        conn->db->rows[i].name = (char *) malloc(conn->db->max_data);
        conn->db->rows[i].email = (char *) malloc(conn->db->max_data);

        rc = fread(conn->db->rows[i].name,conn->db->max_data,1,conn->file);
        if(rc != 1) die(conn, "Failed to load name.");
        rc = fread(conn->db->rows[i].email,conn->db->max_data,1,conn->file);
        if(rc != 1) die(conn, "Failed to load email.");
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die(conn, "Memory error");

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die(conn, "Memory error");

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die(conn, "Failed to open the file");

    return conn;
}


void Database_write(struct Connection *conn)
{
    rewind(conn->file);

//    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
//    if(rc != 1) die(conn, "Failed to write database.");

    size_t rc = fwrite(conn->db,sizeof (struct Database),1,conn->file);
    if(rc != 1) die(conn, "Failed to write database.");

    for(int i = 0; i < conn->db->max_rows; i++){
        rc = fwrite(&conn->db->rows[i].id,sizeof (int),1,conn->file);
        if(rc != 1) die(conn, "Failed to write id.");
        rc = fwrite(&conn->db->rows[i].set,sizeof (int),1,conn->file);
        if(rc != 1) die(conn, "Failed to write set.");
        rc = fwrite(conn->db->rows[i].name,conn->db->max_data,1,conn->file);
        if(rc != 1) die(conn, "Failed to write name.");
        rc = fwrite(conn->db->rows[i].email,conn->db->max_data,1,conn->file);
        if(rc != 1) die(conn, "Failed to write email.");

    }


    rc = fflush(conn->file);
    if(rc == -1) die(conn, "Cannot flush database.");
}

void Database_create(struct Connection *conn, char * max_rows_str, char * max_data_str)
{
//    int i = 0;

//    for(i = 0; i < MAX_ROWS; i++) {
//        // make a prototype to initialize it
//        struct Address addr = {.id = i, .set = 0};
//        // then just assign it
//        conn->db->rows[i] = addr;
//    }
    conn->db->max_rows = atoi(max_rows_str);
    conn->db->max_data = atoi(max_data_str);

    conn->db->rows = (struct Address*)malloc(conn->db->max_rows * sizeof (struct Address));
    for(int i = 0; i < conn->db->max_rows; i++){
        conn->db->rows[i].id = i;
        conn->db->rows[i].set = 0;
        conn->db->rows[i].name = (char *)malloc(conn->db->max_data);
        memset(conn->db->rows[i].name,0,conn->db->max_data);
        conn->db->rows[i].email = (char *)malloc(conn->db->max_data);
        memset(conn->db->rows[i].email,0,conn->db->max_data);
    }
}

void Database_set(struct Connection *conn, int id,  char *name,  char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die(conn, "Already set, delete it first");

    addr->set = 1;

    name[conn->db->max_data - 1] = '\0';
    email[conn->db->max_data - 1] = '\0';


    // WARNING: bug, read the "How To Break It" and fix this
    char *res = strncpy(addr->name, name, conn->db->max_data);
    // demonstrate the strncpy bug
    if(!res) die(conn, "Name copy failed");

    res = strncpy(addr->email, email, conn->db->max_data);
    if(!res) die(conn, "Email copy failed");
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die(conn, "ID is not set");
    }
}

void Database_delete(struct Connection *conn, int id)
{
//    struct Address addr = {.id = id, .set = 0};
//    conn->db->rows[id] = addr;

    char* empty_name = (char *) malloc(conn->db->max_data);
    char* empty_email = (char *) malloc(conn->db->max_data);

    free(conn->db->rows[id].name);
    free(conn->db->rows[id].email);

    struct Address address = {.id = id, .set = 0 , .name = empty_name, .email= empty_email};
    conn->db->rows[id] = address;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);
    if(action != 'c' && id >= conn->db->max_rows) die(conn, "There's not that many records.");

    switch(action) {
        case 'c':
            Database_create(conn,argv[3],argv[4]);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die(conn, "Need an id to get");

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die(conn, "Need id, name, email to set");

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die(conn, "Need id to delete");

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die(conn, "Invalid action, only: c=create, g=get, s=set, d=del, l=list");
    }

    Connection_close(conn);

    return 0;
}