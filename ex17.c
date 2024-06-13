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

void Database_close(struct Connection *conn);

void die(struct Connection *conn, const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    if(conn){
        Database_close(conn);
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

        rc = fread(&conn->db->rows[i].name,sizeof (conn->db->max_data),1,conn->file);
        if(rc != 1) die(conn, "Failed to load name.");
        rc = fread(&conn->db->rows[i].email,sizeof (conn->db->max_data),1,conn->file);
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

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
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
        rc = fwrite(&conn->db->rows[i].name,conn->db->max_data,1,conn->file);
        if(rc != 1) die(conn, "Failed to write name.");
        rc = fwrite(&conn->db->rows[i].email,conn->db->max_data,1,conn->file);
        if(rc != 1) die(conn, "Failed to write email.");

    }


    rc = fflush(conn->file);
    if(rc == -1) die(conn, "Cannot flush database.");
}

void Database_create(struct Connection *conn)
{
//    int i = 0;

//    for(i = 0; i < MAX_ROWS; i++) {
//        // make a prototype to initialize it
//        struct Address addr = {.id = i, .set = 0};
//        // then just assign it
//        conn->db->rows[i] = addr;
//    }
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

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die(conn, "Already set, delete it first");

    addr->set = 1;
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
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
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
    if(id >= conn->db->max_rows) die(conn, "There's not that many records.");

    switch(action) {
        case 'c':
            Database_create(conn);
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

    Database_close(conn);

    return 0;
}