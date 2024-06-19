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
- Topics `TODO`
- MacOS distribution with brew `TODO`
- Linux distribution with snap `TODO`

### Quick Start
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
- `--msg-size`  - 32bit integer e.g `--msg-size 1024`
- `--port`      - 16 bit integer e.g `--port 3000`
- `--log-level` - Default is `none` e.g `--log-level none`
  - `none` No logging
  - `debug` Debugging level logging (includes all logs)

### Options
#### Provider Options:
- `message` - A string, bytes or JSON object.
- `destroy` - If set to `true` then the queue will be destroyed & no more messages
will be stored on the queue until the `destroy: true` options is removed from the provider's
request body or set to `false`.

### Support
TODO

### Contributions
TODO

### Contact
TODO
