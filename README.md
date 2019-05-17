# xtrace-cpp
Barebones C++ implementation of X-Trace compatible with latest brown tracing framework + baggage version

# Building

```
mkdir build
cd build
cmake ..
make
sudo make install
```

# Creating Traces

To start an X-Trace Trace:

```
#include "xtrace.h"

XTrace::StartTrace();
```

To log something with X-Trace:

```
#include "xtrace.h"

XTRACE("hello world! I am an X-Trace event");
```

Or alternatively

```
XTrace::log("hello world! I am an X-Trace event");
```


# X-Trace reports server

This implementation sends reports to the `xtrace-server` hostname, on the default X-Trace port of 5563.  These are specified in `pubsub.h`.  If you just want to run locally, add the line `127.0.0.1 xtrace-server` to your `/etc/hosts` file.

To download and run the X-Trace server:

```
git clone https://github.com/JonathanMace/xtrace-server
cd xtrace-server
xtrace-server/bin/backend
```

Traces can be viewed at http://xtrace-server:4080

# Context Propagation

X-Trace uses thread-local storage to automatically link events together in the same thread.  If you want to log things from a new thread, you have to manually pass this context to a new thread.

```
#include "thread_local_baggage.h"

Baggage branched_baggage = ThreadLocalBaggage::Branch();

std::thread branched_thread([&branched_baggage]() {
  ThreadLocalBaggage::Set(branched_baggage);	
  XTRACE("hello from a new thread!");
});
```

To include baggage in an RPC, serialize it as follows:

```
#include "thread_local_baggage.h""

Baggage baggage_for_rpc = ThreadLocalBaggage::Branch();

std::vector<uint8_t> serialized_bytes = baggage_for_rpc.serialize();
```

To instantiate baggage received over RPC, deserialize as follows:

```
#include "atomlayer.h"

std::vector<uint8_t> serialized_bytes;

Baggage received_baggage = Baggage::deserialize(serialized_bytes);

ThreadLocalBaggage::Set(received_baggage);
```

You can combine multiple baggages into one

```
#include "atomlayer.h"

Baggage a, b;

Baggage joined = Baggage::merge(a, b);

```

Equivalently

```
#include "thread_local_baggage.h"

Baggage a, b;

ThreadLocalBaggage::Set(a);
ThreadLocalBaggage::Join(b);
```

The file `main.cc` gives some simple examples of X-Trace usage and context propagation

# Read more

Our paper on Eurosys gives more on baggage: [Universal Context Propagation for Distributed System Instrumentation](https://people.mpi-sws.org/~jcmace/papers/mace2018universal.pdf) Jonathan Mace and Rodrigo Fonseca, Eurosys 2018

See also http://github.com/tracingplane

