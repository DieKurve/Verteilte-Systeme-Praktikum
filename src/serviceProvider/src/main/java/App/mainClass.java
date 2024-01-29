package App;

public class mainClass {

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Please provide an argument! e.g primary or secondary");
            System.exit(0);
        }

        try {
            if (args[0].equals("primary")) {
                serviceProviderPrimary providerPrimary = new serviceProviderPrimary();
                providerPrimary.startServer();

            } else if (args[0].equals("secondary")) {
                serviceProviderSecondary providerSecondary = new serviceProviderSecondary();
                providerSecondary.simpleSecondary("serviceproviderprimary");

            } else {
                System.out.println("Invalid argument!");
                System.exit(0);
            }
        } catch (Exception x) {
            x.printStackTrace();
        }
    }
}
