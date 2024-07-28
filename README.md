![ForestMQ](assets/fmq_logo.png?raw=true "ForestMQ")
Message Queue 🌲

⚠️ Production ready in v1.0.0

### Project Status
🎉 Forest MQ is now usable for test / hobby projects or the curious.

### Features
- TCP ✅
- ampq-v1.0 `TODO`
- Multiple queues `TODO`
- Message acknowledgement `TODO`
- Message durability / persist `TODO`
- exchange types (e.g. fan) `TODO`
- Logging ✅
- Docker ✅
- Daemon ✅
- Topics `TODO`
- MacOS distribution with brew `TODO`
- Linux distribution with snap `TODO`
- Message encryption `TODO`

### Quick Start
#### Run with Docker
```
docker run -p 8005:8005 -e FORESTMQ_DAEMON=1 bandnoticeboard/forestmq:0.2.0
```

Forest MQ is still in very early stages of development, but
it is usable for test projects. 

As it's missing some fundamental networking, security & stability requirements,
it's not recommended for production environments.

To test Forest MQ end to end you can run the following 2 Python scripts:

- `examples/python/provider.py`
- `examples/python/consumer.py`

This will demonstrate that the message queue excepts messages 
enqueued over HTTP to the queue & the consumer script will
dequeue messages off the queue.


### CLI options
| Arg             | Info                                        | Example            |
|-----------------|---------------------------------------------|--------------------|
| `--msg-size`    | integer e.g                                 | `--msg-size 1024`  |
| `--port`        | integer e.g                                 | `--port 3000`      |
| `--log-level`   | Default is `none` (No logging)              | `--log-level none` |    
| `debug`         | Debugging level logging (includes all logs) |                    |
|`--daemon`       | Run ForestMQ in daemon mode                 |                    |

### Options
#### Provider Options:
- `message` - A string, bytes or JSON object.
- `destroy` - If set to `true` then the queue will be destroyed & no more messages
will be stored on the queue until the `destroy: true` options is removed from the provider's
request body or set to `false`.

### Environment Variables
| Name              | Type | Info                         |
|-------------------|------|------------------------------|
| `FORESTMQ_DAEMON` | INT  | Run in daemon mode.          |
| `FORESTMQ_PORT`   | INT  | TCP Server port to listen on |

### Clients
- ForestMQ Python client `TODO`
- ForestMQ NodeJS client `TODO`
- ForestMQ Go client `TODO`
- ForestMQ Dart client `TODO`

### Health
To check the health of a running ForestMQ:
```
curl http://localhost:8005/health
```
This should respond with a JSON body:
```
{"queue_empty":true,"status":"OK","request_start":"Sun Jul 28 18:59:44 2024\n","request_end":"Sun Jul 28 18:59:44 2024\n"}
```
### Support
TODO

### Contributions
TODO

### Contact
TODO
