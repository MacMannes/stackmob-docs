Custom Code Developer Guide
==========================================

StackMob supports your custom server-side code in Java, Scala, and Clojure.

# Setup

## GitHub


# Methods

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
