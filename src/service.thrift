namespace java thriftServiceProvider
namespace cpp serviceProvider

struct sensorData{
    1:  i32 id,
    2:  i32 value,
    3:  string timestamp

}

service getCentralData {
    void pingServer(),
    void sendData(1: list<sensorData> data)
}

