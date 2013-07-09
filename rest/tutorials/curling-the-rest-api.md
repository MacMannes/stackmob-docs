# Exploring the REST API with cURL

Below we'll show you how to test your new StackMob enabled API with a few simple command line tools. This tutorial assumes you already have a <a href="https://dashboard.stackmob.com/settings">StackMob application</a>.

## Install cURL

<a href="http://curl.haxx.se/">cURL</a> provides a simple means of making HTTP requests with valid OAuth headers. First, <a href="http://curl.haxx.se/download.html">download</a> and install cURL.

## Install OAuth Proxy

Now, let's install <a href="https://github.com/mojodna/oauth-proxy">oauth-proxy</a>. If you're a mac user, you can simply type the following in the command prompt:

    > easy_install oauth-proxy

That should do it! The oauth-proxy utility is a proxy server that will generate proper OAuth headers for your cURL requests when provided with your <a href="https://dashboard.stackmob.com/settings">StackMob public and private API keys</a>.

Using your Development API keys, let's start the oauth-proxy in the command prompt:

    > oauth-proxy --consumer-key [your public key] --consumer-secret [your private key]

You should get output similar to below:

    > 2011-08-31 00:41:49-0700 [-] Log opened.
    > 2011-08-31 00:41:49-0700 [-] twistd 8.2.0 (/usr/bin/python2.6 2.6.1) starting up.
    > 2011-08-31 00:41:49-0700 [-] reactor class: twisted.internet.selectreactor.SelectReactor.
    > 2011-08-31 00:41:49-0700 [-] oauth_proxy.oauth_proxy.OAuthProxyFactory starting on 8001
    > 2011-08-31 00:41:49-0700 [-] Starting factory <oauth_proxy.oauth_proxy.OAuthProxyFactory instance at 0x1011fd878>

## Making REST API calls with cURL

Now, before we can make an API request let's create a simple schema named `message` with a field named `content` so that we can interact with it. To do so, follow the <a href="https://stackmob.com/platform/help/tutorials/tutorial_platform_hello_world#step1">"Create your Object" section of the Platform Hello World tutorial</a>.

Assuming you now have a `message` object created, let's proxy an API call via the oauth-proxy that we started earlier.

### POST - Create an Object Instance

We'll create a new `message` object via a POST call to StackMob. Let's go to the command prompt and type in the example below. Replace the subdomain and app name with your <a href="https://dashboard.stackmob.com/settings">subdomain and app name</a>.

    > curl -svx localhost:8001 -H "Accept: application/vnd.stackmob+json; version=0" -H "Content-Type: application/json" -X POST -d  "{\"content\":\"Hello World\"}"  http://api.[YOUR APPNAME].stackmob.com/message

Formatted, your JSON response should look something like:

```javascript
{
  "content": "Hello World",
  "lastmoddate": 1314777270642,
  "createddate": 1314777270642,
  "message_id": "200d1ede917e43fda9319630ffd3e57a"
}
```

You just created an HTTP request with a JSON body that was proxied via oauth-proxy and then forwarded onto the StackMob REST API. The JSON you see is the response from the StackMob REST API shows that your object was created successfully. You just saved an object instance on StackMob.

Note: If you don't set a `message_id` in the JSON object when creating an object instance, StackMob will auto-generate a unique ID.

### GET - Read an Object Instance

To show that it's persisted, let's retrieve the object now via a GET request.

    > curl -sx localhost:8001 http://[YOUR APPNAME].mob1.stackmob.com/api/0/[YOUR APPNAME]/message/200d1ede917e43fda9319630ffd3e57a

This makes a request to the REST API saying you want an object instance of `message` with `message_id` of message1, which we specified. The result should look like:

```javascript
{
  "content": "Hello World!",
  "lastmoddate": 1314777270642,
  "createddate": 1314777270642,
  "message_id": "200d1ede917e43fda9319630ffd3e57a"
}
```

To get *all* objects of type `message`, simply leave off the id parameter.

    > curl -sx localhost:8001 http://[YOUR SUBDOMAIN].mob1.stackmob.com/api/0/[YOUR APPNAME]/message

The result will look similar, but notice it's now in an array with one item (since so far we've only created one instance).

```javascript
[
  {
    "content": "Hello World!",
    "lastmoddate": 1314777270642,
    "createddate": 1314777270642,
    "message_id": "200d1ede917e43fda9319630ffd3e57a"
  }
]
```

### PUT - Update an Object Instance

Now that we know we have the object saved in the datastore, let's update it with a PUT request.

    > curl -sx localhost:8001 -H "Accept: application/vnd.stackmob+json; version=0" -H "Content-Type: application/json" -X PUT -d  "{\"content\":\"Hello Bodie\"}"  http://api.[YOUR SUBDOMAIN].stackmob.com/message/200d1ede917e43fda9319630ffd3e57a

The response should look like:

```javascript
{
  "content": "Goodbye World!",
  "lastmoddate": 1314777270642,
  "createddate": 1314777270642,
  "message_id": "message1"
}
```

### DELETE - Delete an Object Instance

Now that we've said our proper goodbye's, let's delete message1 from the datastore.

    > curl -sx localhost:8001  -X DELETE http://[YOUR APPNAME].mob1.stackmob.com/api/0/[YOUR APPNAME]/message/200d1ede917e43fda9319630ffd3e57a

This produces:

```javascript
{
  "status": "OK"
}
```

## Done!

You've explored how to make StackMob API requests using cURL.
