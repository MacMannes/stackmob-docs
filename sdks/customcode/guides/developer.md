Custom Code Developer Guide
==========================================

StackMob supports your custom server-side code in Java, Scala, and Clojure.  We'll run it on our servers so that you can access it from any client.

# Setup

## GitHub

GitHub is the easiest way to setup Custom Code.

# Declaring Custom Code

Let's add a `helloworld` method to your Custom Code so that clients can call it.

<span class="tab clientcall" title="iOS SDK"></span>
asdf
<span class="tab"></span>

<span class="tab clientcall" title="Android SDK"></span>
asdf
<span class="tab"></span>

<span class="tab clientcall" title="JS SDK"></span>
```js
StackMob.customcode('helloworld', { name: 'bob' })
```
<span class="tab"></span>

how to declare your own methods.

* method name
* implement execute
* specify params
* EntryPointExtender
* Defining own response

# Request Bodies and Parameters

## Fetching Parameters

get params out of request

<span class="tab fetchparams" title="Java"></span>
asdf
<span class="tab"></span>

<span class="tab fetchparams" title="Scala"></span>
asdf
<span class="tab"></span>

## Fetching Body

get body out of request

## Fetching JSON Body

convert request body into JSON

# Datastore

## Access Controls

Custom Code bypasses access control permissions since it runs in your secure environment.

## Persistence

talk about crud and the datastore


# Queries

how to make queries against the datastore

## Equality

isEqual isNotEqual

## Comparison

lessThan, greaterThan

## Array Queries

in

## Pagination Queries

how to paginate

# Relationships

how does the custom code deal with relationships

# Authentication

checking logged in user

# External HTTP Calls

how to make external http calls

# Custom Headers and Response

reading custom headers and writing custom responses (non JSON)

# External Dependencies

how to include external dependencies

## Maven

## JARs

# Security Manager

Limitations of Custom Code

# Deploying Code

## GitHub

Instructions and Dashboard

## JAR

Instructions and Dashboard

# Testing Locally

how to test the custom code locally (local runner)

# Best Practices
