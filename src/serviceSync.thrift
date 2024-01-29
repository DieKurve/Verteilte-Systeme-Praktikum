namespace java thriftSyncServiceProvider

struct syncData{
    1:  i32 id,
    2:  i32 value,
    3:  string timestamp
}

service syncToSecondary {
    void sync(1: list<syncData> data)
}

