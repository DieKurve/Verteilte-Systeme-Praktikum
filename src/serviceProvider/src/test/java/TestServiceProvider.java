import App.serviceProviderBase;
import java.io.File;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.testcontainers.containers.DockerComposeContainer;
import thriftServiceProvider.sensorData;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class TestServiceProvider {

  private static serviceProviderBase provider;

  public static DockerComposeContainer environment =
      new DockerComposeContainer(new File("src/test/resources/docker-test.yml"))
          .withExposedService("primarydatabase", 5432);


  @BeforeAll
  static void initialize() {
    environment.start();
    provider = new serviceProviderBase("localhost");
    provider.initDatabase();
  }

  @Test
  void testPositiveInsertData() {
    List<sensorData> data1 = new ArrayList<>();

    sensorData s1 = new sensorData();
    s1.id = 2;
    s1.timestamp = "218128902";
    s1.value = 13;

    data1.add(s1);

    assertTrue(provider.insertData(data1));
  }

  @Test
  void testNegativeInsertData() {
    List<sensorData> data2 = new ArrayList<>();

    sensorData s2 = new sensorData();
    s2.id = 4;
    s2.timestamp = "218128902test";
    s2.value = 1;

    data2.add(s2);

    assertFalse(provider.insertData(data2));
  }

  @AfterAll
  static void stop(){
    environment.stop();
  }

}
