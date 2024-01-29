package App;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.util.ArrayList;
import java.util.List;

import thriftServiceProvider.*;
import thriftSyncServiceProvider.syncData;

public class serviceProviderBase {

  private Connection databaseConnection = null;
  protected List<sensorData> bufferData = new ArrayList<>();

  public serviceProviderBase(String dbName) {

    int tries = 1;
    try {
      while (this.databaseConnection == null) {
        this.databaseConnection = createConnection(dbName);
        if (tries <= 5 && !createTables(this.databaseConnection)) {
          tries++;
          Thread.sleep(10000);
        } else {
          return;
        }
      }
    } catch (Exception x) {
      String exception = "Retry " + tries;
      System.out.println(exception);
    }
  }

  private boolean createTables(Connection connection) {
    try {
      PreparedStatement stmt;
      String trafficSensorTable, fuelSensorTable, meanSpeedSensorTable, speedSensorTable;
      trafficSensorTable = "CREATE TABLE IF NOT EXISTS TRAFFICSENSOR  "
          + "(ID              SERIAL PRIMARY KEY  NOT NULL,"
          + " VALUE           INT                 NOT NULL, "
          + " TIMESTAMP       BIGINT                 NOT NULL, "
          + " SENSORID        INT) ";
      stmt = connection.prepareStatement(trafficSensorTable);
      stmt.execute();

      fuelSensorTable = "CREATE TABLE IF NOT EXISTS FUELSENSOR "
          + "(ID              SERIAL PRIMARY KEY  NOT NULL,"
          + " VALUE           INT                  NOT NULL, "
          + " TIMESTAMP       BIGINT                 NOT NULL, "
          + " SENSORID        INT) ";
      stmt = connection.prepareStatement(fuelSensorTable);
      stmt.execute();

      meanSpeedSensorTable = "CREATE TABLE IF NOT EXISTS MEANSPEEDSENSOR "
          + "(ID              SERIAL PRIMARY KEY  NOT NULL,"
          + " VALUE           INT                 NOT NULL, "
          + " TIMESTAMP       BIGINT              NOT NULL, "
          + " SENSORID        INT) ";
      stmt = connection.prepareStatement(meanSpeedSensorTable);
      stmt.execute();

      speedSensorTable = "CREATE TABLE IF NOT EXISTS SPEEDSENSOR "
          + "(ID              SERIAL PRIMARY KEY  NOT NULL,"
          + " VALUE           INT                 NOT NULL, "
          + " TIMESTAMP       BIGINT                 NOT NULL, "
          + " SENSORID        INT) ";
      stmt = connection.prepareStatement(speedSensorTable);
      stmt.execute();

      stmt.close();
    } catch (Exception e) {
      System.err.println(e.getClass().getName() + ": " + e.getMessage());
      return false;
    }
    System.out.println("Table created successfully");
    return true;
  }

  public void initDatabase() {
    createTables(getConnection());
  }

  protected Connection getConnection() {
    return this.databaseConnection;
  }

  public Boolean insertData(List<sensorData> data) {
    PreparedStatement insertStatement;
    String insertSQL;
    for (sensorData sensors : data) {
      try {
        switch (sensors.id) {
          case 1:
            insertSQL = "INSERT INTO FUELSENSOR (VALUE, TIMESTAMP, SENSORID)"
                + "VALUES(" + sensors.value + ", " + sensors.timestamp + ", " + sensors.id + ");";
            insertStatement = this.databaseConnection.prepareStatement(insertSQL);
            insertStatement.execute();
            break;
          case 2:
            insertSQL = "INSERT INTO MEANSPEEDSENSOR (VALUE, TIMESTAMP, SENSORID)"
                + "VALUES(" + sensors.value + ", " + sensors.timestamp + ", " + sensors.id + ");";
            insertStatement = this.databaseConnection.prepareStatement(insertSQL);
            insertStatement.execute();
            break;
          case 3:
            insertSQL = "INSERT INTO SPEEDSENSOR (VALUE, TIMESTAMP, SENSORID)"
                + "VALUES(" + sensors.value + ", " + sensors.timestamp + ", " + sensors.id + ");";
            insertStatement = databaseConnection.prepareStatement(insertSQL);
            insertStatement.execute();
            break;
          case 4:
            insertSQL = "INSERT INTO TRAFFICSENSOR (VALUE, TIMESTAMP, SENSORID)"
                + "VALUES(" + sensors.value + ", " + sensors.timestamp + ", " + sensors.id + ");";
            insertStatement = databaseConnection.prepareStatement(insertSQL);
            insertStatement.execute();
            break;
          default:
            break;

        }

      } catch (Exception e) {
        System.err.println(e.getClass().getName() + ": " + e.getMessage());
        return false;
      }
    }
    return true;
  }


  protected Connection createConnection(String dbName) {
    String connectionName = "jdbc:postgresql://" + dbName + ":5432/praktikumdb";
    try {
      Class.forName("org.postgresql.Driver");
      return DriverManager
          .getConnection(connectionName,
              "postgres", "vspraktikum");
    } catch (Exception e) {
      e.printStackTrace();
      System.err.println(e.getClass().getName() + ": " + e.getMessage());
      return null;
    }
  }

  protected List<syncData> convertPrimaryData(List<sensorData> data) {
    List<syncData> convertedPrimaryData = new ArrayList<>();

    for (sensorData convertData : data) {
      thriftSyncServiceProvider.syncData syncData = new thriftSyncServiceProvider.syncData();
      syncData.id = convertData.id;
      syncData.value = convertData.value;
      syncData.timestamp = convertData.timestamp;
      convertedPrimaryData.add(syncData);
    }

    return convertedPrimaryData;
  }

  protected List<sensorData> convertSecondaryData(List<syncData> data) {
    List<sensorData> convertedSecondaryData = new ArrayList<>();

    for (syncData convertData : data) {
      sensorData syncData = new sensorData();
      syncData.id = convertData.id;
      syncData.value = convertData.value;
      syncData.timestamp = convertData.timestamp;
      convertedSecondaryData.add(syncData);
    }

    return convertedSecondaryData;
  }

  protected void addToBuffer(List<sensorData> data) {
    bufferData.addAll(data);
  }

}
