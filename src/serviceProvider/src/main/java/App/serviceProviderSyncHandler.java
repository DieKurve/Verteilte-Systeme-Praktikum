package App;

import java.util.List;

import thriftSyncServiceProvider.*;

public class serviceProviderSyncHandler implements syncToSecondary.Iface {

    private final serviceProviderBase provider;

    public serviceProviderSyncHandler(serviceProviderBase syncProvider) {
        this.provider = syncProvider;
    }

    @Override
    public void sync(List<syncData> data) {
        this.provider.insertData(provider.convertSecondaryData(data));
    }

}
