#define MAX_PLATE_LEN 16
#define MAX_HASH_LEN 64
#define MAX_TIMESTAMP_LEN 32

struct infraction_data {
    int speed_record;
    char plate[MAX_PLATE_LEN];
    char timestamp[MAX_TIMESTAMP_LEN];
    char image_hash[MAX_HASH_LEN];
};

extern struct k_msgq infraction_msgq;