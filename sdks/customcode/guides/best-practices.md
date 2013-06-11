# StackMob Custom Code SDK: A Best-Practices Guide

With great power comes great responsibility, and custom server-side logic is about as powerful as it gets. While we at StackMob encourage you to be creative with your code, we also have developed a short list of suggestions that will help ensure the greatest performance against our backend, the quickest response to service issues that may arise, and as a result the best user experience for your application.

Unless otherwise stated, nothing here is *required* in your codebase; these are merely recommendations. So, without further ado...

## Don't call out from one Custom Code Method to another.

Custom Code Methods are ideally self-contained units of computation that, when given some input, will process on that input plus whatever information is needed from the services exposed within the Custom Code SDK. Calling from one Method to another is not only unnecessary, in that any one Method has the same tools available to it as another, but is also less performant than simply encapsulating the whole of the computation in one Method. Reaching out to another Custom Code Method entails the whole of request serialization/deserialization as well as HTTP overhead, and your method runs the risk of timing out if the second method call doesn't complete for some reason. If you find yourself needing to reuse code, consider moving that code into helper utilities within your Custom Code Jar file, and simply call from that set of code when needed.

Additionally, if you have two or more applications that need to share information, using Custom Code endpoints on one or more apps can be a convenient way to accomplish this. In the worst of cases, however, this can introduce loops between Methods, which end up being very difficult for us to debug. Please exercise caution if you find yourself needing to use such a pattern.

## Client-side: Cache information on your users' client applications where possible.

We understand the need to serve fresh information to your users, but consider architecting your codebase so as to include client-side caching of Custom Code results wherever possible. This can be short in-memory caching - 60 seconds is generally suitable for most types of web-facing data. Consider also building out Methods that will act in the spirit of `HTTP 304 Not Modified`, in that they will only send back information that has changed, or that needs an update. All of this will help to ensure a balance between client performance and information freshness, so that your users may have the best experience possible with your app.

We also offer [pub-sub](http://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) module solutions in our [Marketplace](https://marketplace.stackmob.com/), if real-time streaming is a hard requirement of your application.

## Server-side: Don't use `static` objects in your Custom Code Methods. Don't rely on state.

The best Custom Code Methods are stateless - a request with some data comes in, a computation occurs, and a response is sent back out. You should rely as little as possible on state held within your Custom Code classes, and your Methods should absolutely **not** try to retain `static` objects between calls. Since `static` objects never get cleared by the JVM garbage collector, heavy reliance on such objects can increase the risk of your Custom Code instance running out of memory when brought to scale.

Do not assume that the Custom Code instance that handles a call will be the same instance every time, or that an instance will even be alive when a request comes in. We have code on our side to balance load and restart Custom Code instances when needed; you should not make any assumptions about the state of those instances between calls.

If you would like to preserve server-side information between methods, use the `CachingService` added in Custom Code SDK 0.5.5. This gives you programmatic access to a fast, distributed key-value store, which should be very familiar to users of Memcached and other in-memory stores. See the [Custom Code SDK guide](https://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK#a-caching) for more information on server-side caching.

If you're still concerned with performance under load, contact us directly and we can talk about the load you're anticipating and what we know our servers can handle.

## Make use of query fields. Don't get an entire collection when you don't need it.

Our Datastore API includes an entire framework for making very specific queries against your schema, just as if it were a directly-accessible database. As of Custom Code SDK 0.5.4, you can compose `AND` and `OR` queries (with some limitations), allowing you to very specifically drill down to the information you need. With this in mind, there are increasingly fewer cases where retrieving an entire collection should be necessary, except perhaps to initially seed one of your users' client applications.

## Use the `LoggerService`!

While some of the suggestions here portray Custom Code Methods as black-box computation units, you can still build them to yield very verbose log feedback. The `LoggerService` is the way to accomplish this, and its usage should be familiar to users of Log4J, SLF4J, Logback, and other logging frameworks. You can view log output in the [Dashboard](https://dashboard.stackmob.com/data/logs), and a usage example is included in the [Custom Code SDK guide](https://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK#a-logging).

## Conclusion

Some of these patterns (Methods as self-contained units, non-reliance on state) may sound familiar to [certain functional programming paradigms](http://en.wikipedia.org/wiki/Monad_(functional_programming)), and we may or may not have [several](https://blog.stackmob.com/2013/03/why-we-avoid-throwing-exceptions-at-stackmob/) [blog](https://blog.stackmob.com/2013/01/free-yourself-from-the-tyranny-of-null-part-1/) [posts](https://blog.stackmob.com/2012/12/learning-functional-programming-without-growing-a-neckbeard-2/) on similar structures. Admittedly, these have influenced how we designed the Custom Code Method, how we expect Methods to behave, and in turn how we hope our users will engineer their own Methods. 