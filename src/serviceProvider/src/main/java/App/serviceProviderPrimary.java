package App;

import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TThreadPoolServer;
import org.apache.thrift.server.TThreadPoolServer.Args;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TServerTransport;
import thriftServiceProvider.getCentralData;
import thriftSyncServiceProvider.syncToSecondary;
import thriftSyncServiceProvider.syncToSecondary.Processor;


public class serviceProviderPrimary extends serviceProviderBase {

    private serviceProviderHandler handler;
    private final serviceProviderSyncHandler syncHandler;
    public getCentralData.Processor centralDataProcessor;
    public syncToSecondary.Processor syncProcessor;

    serviceProviderPrimary() {
        super("primarydatabase");
        this.setHandler(new serviceProviderHandler(this));
        this.syncHandler = new serviceProviderSyncHandler(this);
        this.centralDataProcessor = new getCentralData.Processor(this.getHandler());
        this.syncProcessor = new Processor(this.syncHandler);
    }

    /**
     * @param processor
     */
    public void simplePrimary(getCentralData.Processor processor) {
        try {
            TServerTransport serverTransport = new TServerSocket(9876);

            // Use this for a multithreaded server
            TServer server = new TThreadPoolServer(
                    new TThreadPoolServer.Args(serverTransport).processor(processor));

            System.out.println("Starting the simplePrimary server...");
            server.serve();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void simpleSync(syncToSecondary.Processor syncProcessor) {
        try {
            TServerTransport serverTransport = new TServerSocket(9875);

            // Use this for a multithreaded server
            TServer server = new TThreadPoolServer(new Args(serverTransport).processor(syncProcessor));

            System.out.println("Starting the simpleSync server...");
            server.serve();
        } catch (Exception exceptionSync) {
            exceptionSync.printStackTrace();
        }
    }

    public void startServer() {
        // start primary server in new thread
        Runnable simplePrimary = () -> this
                .simplePrimary(this.centralDataProcessor);
        new Thread(simplePrimary).start();

        // start synchronization thread to sync after outage
        Runnable simpleSync = () -> this.simpleSync(this.syncProcessor);
        new Thread(simpleSync).start();
    }

    public serviceProviderHandler getHandler() {
        return handler;
    }

    private void setHandler(serviceProviderHandler handler) {
        this.handler = handler;
    }
}
