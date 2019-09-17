

struct stateVector {
    int16_t posX;
    int16_t posY;
    int16_t posZ;
    int16_t velX;
    int16_t velY;
    int16_t velZ;
    int16_t accX;
    int16_t accY;
    int16_t accZ;
    time_t time;
};


void dbus_assert(const int r, const char* err);
int method_thread_init(void);
int method_thread_clear(void);
