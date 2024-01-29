package App;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import thriftServiceProvider.getCentralData;
import thriftSyncServiceProvider.syncToSecondary;
import thriftSyncServiceProvider.syncToSecondary.Processor;

import java.io.IOException;
import java.net.InetAddress;

public class serviceProviderSecondary extends serviceProviderBase {

    private final syncToSecondary.Processor syncSecondaryProcessor;
    private final getCentralData.Processor centralDataProcessor;
    private serviceProviderSyncHandler syncHandler;
    private serviceProviderSecondaryWork primaryWorker;
    private serviceProviderSecondaryWork secondaryWorker;

    serviceProviderSecondary() {
        super("secondarydatabase");
        this.setSyncHandler(new serviceProviderSyncHandler(this));
        this.syncSecondaryProcessor = new Processor(
                this.getSyncHandler());
        serviceProviderHandler takeOverHandler = new serviceProviderHandler(this);
        this.centralDataProcessor = new getCentralData.Processor(takeOverHandler);
    }

    public serviceProviderSyncHandler getSyncHandler() {
        return syncHandler;
    }

    private void setSyncHandler(serviceProviderSyncHandler serviceProviderSyncHandler) {
        this.syncHandler = serviceProviderSyncHandler;
    }

    /**
     * @param
     */
    public void simpleSecondary(String hostNamePrimary) {
        secondaryWorker = new serviceProviderSecondaryWork(9875, syncSecondaryProcessor);
        secondaryWorker.start();

        Runnable runnable = () -> {
            while (true) {

                // check if the primary server is offline
                if (!isReachable(hostNamePrimary)) {

                    // stop this secondary server if it is running
                    if (secondaryWorker != null) {
                        this.stopSecondaryServer();
                    }

                    // start this secondary server as primary server
                    // if it is not serving as primary already
                    if (primaryWorker == null || !primaryWorker.isAlive()) {
                        this.startPrimaryServer();
                    }

                } else {

                    // stop this auxiliary primary server
                    // if the original primary server is online again
                    if (primaryWorker != null) {
                        this.stopPrimaryServer();
                    }

                    // start the original secondary server again
                    // if it is not running already
                    if (secondaryWorker == null || !secondaryWorker.isAlive()) {
                        this.startSecondaryServer();
                    }

                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        };

        runnable.run();
    }

    /**
     * @param hostname of the target
     * @return boolean if the target is reachable
     */
    private Boolean isReachable(String hostname) {
        try {
            return (InetAddress.getByName(hostname).isReachable(5000));
        } catch (IOException e) {
            return false;
        }
    }

    /**
     * Syncs the data between primary and secondary server after an outage
     *
     * @throws TException
     * @throws InterruptedException
     */
    private void sync() throws TException, InterruptedException {
        TTransport transport = new TSocket("serviceproviderprimary", 9875);

        while (!transport.isOpen()) {
            try {
                transport.open();
            } catch (Exception e) {
                Thread.sleep(1000);
            }
        }

        TProtocol protocol = new TBinaryProtocol(transport);
        syncToSecondary.Client client = new syncToSecondary.Client(protocol);
        client.sync(convertPrimaryData(bufferData));
        System.out.println("Synced");
        this.bufferData.clear();
        transport.close();
    }

    /**
     * starting this server as primary in a new thread
     */
    private void startPrimaryServer() {
        this.primaryWorker = new serviceProviderSecondaryWork(9876, centralDataProcessor);

        try {
            this.primaryWorker.start();
        } catch (Exception ignored) {

        }

        System.out.println("Initializing Secondary as Primary");
    }

    /**
     * stopping the primary server thread
     */
    private void stopPrimaryServer() {
        this.primaryWorker.getServer().stop();

        try {
            this.primaryWorker.interrupt();
        } catch (Exception ignored) {

        }

        System.out.println("Stopping Primary auxiliary");
        this.primaryWorker = null;
    }

    /**
     * starting this server as secondary in a new thread
     */
    private void startSecondaryServer() {
        this.secondaryWorker = new serviceProviderSecondaryWork(9875, syncSecondaryProcessor);
        this.secondaryWorker.start();

        System.out.println("Starting original Secondary Server");

        if (!this.bufferData.isEmpty()) {
            try {
                sync();

                bufferData.clear();
            } catch (Exception ignored) {

            }
        }
    }

    /**
     * starting the secondary server thread
     */
    private void stopSecondaryServer() {
        this.secondaryWorker.getServer().stop();
        this.secondaryWorker.interrupt();

        System.out.println("Stopping Secondary Server");

        this.secondaryWorker = null;
    }

}
