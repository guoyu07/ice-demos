// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

class HelloClient : public Ice::Application
{
public:

    HelloClient();

    virtual int run(int, char*[]) override;

private:

    void menu();
};

int
main(int argc, char* argv[])
{
    HelloClient app;
    return app.main(argc, argv, "config.client");
}

HelloClient::HelloClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::SignalPolicy::NoSignalHandling)
{
}

int
HelloClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto twoway = Ice::checkedCast<HelloPrx>(
        communicator()->propertyToProxy("Hello.Proxy")->ice_twoway()->ice_timeout(-1)->ice_secure(false));
    if(!twoway)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }
    auto oneway = twoway->ice_oneway();
    auto batchOneway = twoway->ice_batchOneway();
    auto datagram = twoway->ice_datagram();
    auto batchDatagram = twoway->ice_batchDatagram();

    bool secure = false;

    menu();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;
            if(c == 't')
            {
                twoway->sayHello();
            }
            else if(c == 'o')
            {
                oneway->sayHello();
            }
            else if(c == 'O')
            {
                batchOneway->sayHello();
            }
            else if(c == 'd')
            {
                if(secure)
                {
                    cout << "secure datagrams are not supported" << endl;
                }
                else
                {
                    datagram->sayHello();
                }
            }
            else if(c == 'D')
            {
                if(secure)
                {
                    cout << "secure datagrams are not supported" << endl;
                }
                else
                {
                    batchDatagram->sayHello();
                }
            }
            else if(c == 'f')
            {
                batchOneway->ice_flushBatchRequests();
                batchDatagram->ice_flushBatchRequests();
            }
            else if(c == 'S')
            {
                secure = !secure;

                twoway = twoway->ice_secure(secure);
                oneway = oneway->ice_secure(secure);
                batchOneway = batchOneway->ice_secure(secure);
                datagram = datagram->ice_secure(secure);
                batchDatagram = batchDatagram->ice_secure(secure);

                if(secure)
                {
                    cout << "secure mode is now on" << endl;
                }
                else
                {
                    cout << "secure mode is now off" << endl;
                }
            }
            else if(c == 'x')
            {
                // Nothing to do
            }
            else if(c == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    return EXIT_SUCCESS;
}

void
HelloClient::menu()
{
    cout <<
        "usage:\n"
        "t: send greeting as twoway\n"
        "o: send greeting as oneway\n"
        "O: send greeting as batch oneway\n"
        "d: send greeting as datagram\n"
        "D: send greeting as batch datagram\n"
        "f: flush all batch requests\n"
        "S: switch secure mode on/off\n"
        "x: exit\n"
        "?: help\n";
}
