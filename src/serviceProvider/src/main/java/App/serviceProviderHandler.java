package App;

import java.util.ArrayList;
import java.util.List;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import thriftServiceProvider.*;
import thriftSyncServiceProvider.syncToSecondary;

public class serviceProviderHandler implements getCentralData.Iface {

    private final serviceProviderBase provider;

    public serviceProviderHandler(serviceProviderBase serviceProviderBase) {
        this.provider = serviceProviderBase;
    }

    @Override
    public void pingServer() {
        System.out.println("pingServer()");
    }

    @Override
    public void sendData(List<sensorData> data) {
        this.provider.insertData(data);

        try {
            TTransport transport = new TSocket("serviceproviderSecondary", 9875);
            transport.open();

            TProtocol protocol = new TBinaryProtocol(transport);
            syncToSecondary.Client client = new syncToSecondary.Client(protocol);

            if (!this.provider.bufferData.isEmpty()) {
                data.addAll(0, this.provider.bufferData);
            }

            syncToSecondary(client, data);

            transport.close();
        } catch (TException x) {
            x.printStackTrace();
            this.provider.addToBuffer(data);
        }
    }

    private void syncToSecondary(syncToSecondary.Client syncClient, List<sensorData> data)
            throws TException {
        syncClient.sync(this.provider.convertPrimaryData(data));

        this.provider.bufferData.clear();
    }
}
