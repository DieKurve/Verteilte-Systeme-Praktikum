package App;

import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TThreadPoolServer;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TServerTransport;
import thriftServiceProvider.getCentralData;
import thriftSyncServiceProvider.syncToSecondary.Processor;

public class serviceProviderSecondaryWork extends Thread {

    private final int port;
    private TServer server;
    private Processor syncProcessor = null;
    private getCentralData.Processor centralProcessor = null;

    public serviceProviderSecondaryWork(int port, Processor syncProcessor) {
        this.port = port;
        this.syncProcessor = syncProcessor;
    }

    public serviceProviderSecondaryWork(int port, getCentralData.Processor getCentralProcessor){
        this.port = port;
        this.centralProcessor = getCentralProcessor;
    }

    @Override
    public void run() {
        super.run();

        try {
            TServerTransport serverTransport = new TServerSocket(port);

            // Use this for a multithreaded server
            if (syncProcessor != null){
                this.server = new TThreadPoolServer(
                    new TThreadPoolServer.Args(serverTransport).processor(syncProcessor));
            }else {
                this.server = new TThreadPoolServer(
                    new TThreadPoolServer.Args(serverTransport).processor(centralProcessor));
            }

            if (port == 9876) {
                System.out.println("Starting the simpleSecondary server as PRIMARY...");
            } else {
                System.out.println("Starting the simpleSecondary server as SECONDARY...");
            }

            this.server.serve();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public TServer getServer() {
        return this.server;
    }
}
