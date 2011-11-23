// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <ChatSessionI.h>

using namespace std;
using namespace Demo;

class ChatSessionManagerI : public Glacier2::SessionManager
{
public:

    virtual Glacier2::SessionPrx
    create(const string& userId, const Glacier2::SessionControlPrx&, const Ice::Current& current)
    {
        return Glacier2::SessionPrx::uncheckedCast(current.adapter->addWithUUID(new ChatSessionI(userId)));
    }
};

class ChatServer : public Ice::Application
{
public:

    virtual int
    run(int argc, char* argv[])
    {
        if(argc > 1)
        {
            cerr << appName() << ": too many arguments" << endl;
            return EXIT_FAILURE;
        }

        Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("ChatServer");
        
        Glacier2::SessionManagerPtr csm = new ChatSessionManagerI;
        adapter->add(csm, communicator()->stringToIdentity("ChatSessionManager"));
        adapter->activate();
        communicator()->waitForShutdown();
        
        return EXIT_SUCCESS;
    }
};

int
main(int argc, char* argv[])
{
    ChatServer app;
    return app.main(argc, argv, "config.server");
}
