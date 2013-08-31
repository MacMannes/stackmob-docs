iOS Developer Guide
=====================================

## Overview

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/developer_guide/ios_sdk_architecture.png" />

The architecture of the iOS SDK can be broken into a few key parts:

<b>Persistence:</b>
<p></p>
The iOS SDK provides two routes for persistence: 

The "Datastore API" offers simple asynchronous wrapper methods for each of the core operations: creating, reading, updating, and deleting. It also provides methods for performing queries, atomic increments/decrements, and related object manipulation, just to name a few.

For developers who want a more powerful, complex persistence solution, complete with a caching and offline system, we have built a Core Data layer that sits on top of the Datastore API. Core Data, Apple's Persistence framework, allows us to place a familiar wrapper around StackMob REST calls and Datastore API. iOS developers can leverage their existing knowledge of Core Data to quickly integrate StackMob into their applications.

Beneath each of the persistence classes there are additional helper classes used for specialized features like file storage, geolocation, and custom code.

<b>How do I know if I should use Core Data or the Datastore API directly for persistence?</b>

Which route you use for persistence depends mainly on your app's data model. ADD MORE

#### User Management #####
<p></p>

The user management piece of the SDK includes all the classes and methods used for creating user objects and creating and maintaining user sessions. authenticating with StackMob is done via a username/password, Facebook, or Twitter credentials. Password management is also available. 

<b>Push</b>
<p></p>

Push is considered a separate piece of the SDK because it uses OAuth1, requiring a public and private key. Thus, it uses its own stack of classes for authentication and general session maintenance. Using the push methods you can register a device token with the StackMob server, broadcast messages as well as push to specific devices.

<b>Custom Code</b>
<p></p>
Custom code is a powerful feature offered by StackMob which allows users to upload custom server side code which is then callable by a unique rest endpoint. You'll create a custom code request using the `SMCustomCodeRequest` class, then pass the instance to a parameter of the Datastore API `performCustomCodeRequest:` method.

<br/>
<br/>
At the foundation level, all methods which cause requests to StackMob are first translated into the proper REST format. They then pass through an authentication layer which is responsible for signing the request if needed, and the request is sent to StackMob. The response from the server is then translated into the correct format for the calling method's callback or return value.

<br/>
<br/>
<b>The contents of this guide use the Datastore API functions for persistence. All equivalent operations using the Core Data integration for persistence can be found in the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide" target="_blank">Core Data Integration Guide</a></b>.

<br/>
<br/>
In each section of this guide you may see colored boxes which are meant to highlight important information:

<p class="alert">Gold boxes call out warnings, gotchas, and information we don't want you to miss.</p>

<p class="alert alert-info">Blue boxes contain links to sections in the full API reference, as well as full working projects for you to download and in-depth tutorials for you to read through.</p>

<!---
	///////////////////
	SETUP
	//////////////////
-->

## Setup

<!--- Download & Configure -->

### Download and Configure

First things first, download the SDK files and configure them into your Xcode project.

Come back to this page when you're finished.

<a href="https://dashboard.stackmob.com/sdks/ios/config" target="_blank">Download and Configure the iOS SDK.</a>


<!--- INITIALIZE -->

### Initializing the iOS SDK

Wherever you plan to use StackMob, add `#import "StackMob.h"` to the header file.

Define a property of type `SMClient`, most likely in your `AppDelegate` file where you define other application wide properties, and initialize it like this:

```obj-c
self.client = [[SMClient alloc] initWithAPIVersion:@"YOUR_API_VERSION" publicKey:@"YOUR_PUBLIC_KEY"];
```

<p class="alert">If you haven't found your public key yet, check out <b>Manage App Info</b> under the <b>App Settings</b> sidebar on the <a href="https://dashboard.stackmob.com" target="_blank">Dashboard page</a>.</p>

For `YOUR_API_VERSION`, pass `@"0"` for Development, `@"1"` or higher for the corresponding version in Production.

You should only instantiate one instance of `SMClient`. You can use `[SMClient defaultClient]` to retrieve the instance at any point after its been initialized, in conjunction with standard techniques like passing it between controllers or accessing it through an instance of the shared app delegate.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Initialize" target="_blank">SMClient: Initialize a Client</a></li>
      </ul>
    </div>
  </div>
</div>

### Start Persisting Data

You will interact with the Datastore persistence layer by obtaining an instance of `SMDataStore` from your `SMClient` object.

The easiest way to get a datastore instance is through the default client, starting your calls with the following:

```obj-c
[[SMClient defaultClient] dataStore] someDataStoreMethod...];
```

Alternatively you can initialize a separate `SMDataStore` instance from the client:

```obj-c
SMClient *client = ...;
SMDataStore *myDataStore = [client dataStore];
```

<p class="alert">If you are looking for the Core Data integration for persistence, head over to the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide" target="_blank">Core Data Integration Guide</a>.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/dataStore" target="_blank">SMClient: datastore</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/defaultClient" target="_blank">SMClient: defaultClient</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
	///////////////////
	DATASTORE
	//////////////////
-->

## Datastore

The following sections include everything you need to know about creating, reading, updating and deleting objects. All methods are asynchronous and allow you to provide a success and failure block, which will be performed on the main thread by default. All methods have alternative version which include parameters for request options and/or success and failure callback queues. Refer to the Resources sections for links to all available method signatures.

<b>The contents of this guide use the Datastore API functions for persistence. All equivalent operations using the Core Data integration for persistence can be found in the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide" target="_blank">Core Data Integration Guide</a></b>.


<!--- INSERT OBJECT -->

### Creating an Object

To create an object, simply define a dictionary where the keys map to their corresponding StackMob fields. Then pass the dictionary to the `SMDataStore` `createObject:inSchema:onSuccess:onFailure:` method.

Lets create a new todo object:

```obj-c
NSDictionary *newTodo = [NSDictionary dictionaryWithObjectsAndKeys:@"my todo", @"title", nil];

[[SMClient defaultClient] dataStore] createObject:newTodo inSchema:@"todo" onSuccess:^(NSDictionary *object, NSString *schema) {
  // object contains the full created object, which has been assigned a primary key automatically by the server.
  // You can also create objects with primary keys if you choose, but it's probably easier to let the server auto-generate them.
} onFailure:^(NSError *error, NSDictionary* object, NSString *schema) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Create an Object" target="_blank">SMDataStore: Create an Object</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- CREATING OBJECTS IN BULK -->

### Creating Objects in Bulk

You can create many objects in the same schema all in one call.

Lets create 5 todo objects. For simplicity purposes they will all have the same title:

```obj-c
NSMutableArray *todoArray = [NSMutableArray array];

for (int i = 0; i < 5; i++) {
  [todoArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:@"my todo", @"title", nil]];
}

[[SMClient defaultClient] dataStore] createObjects:todoArray inSchema:@"todo" onSuccess:^(NSArray* succeeded, NSArray *failed, NSString *schema) {
  // succeeded contains an array of IDs of all objects that were successfully persisted.
  // failed contains an array of IDs of all objects that were unsuccessfully persisted, possibly because of duplicate key issues, etc.
} onFailure:^(NSError *error, NSArray *objects, NSString *schema) {
  // Handle error
}];
```
<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Create Objects" target="_blank">SMDataStore: Create Objects</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- READ OBJECT -->

### Reading an Object

To read an existing object, use the `readObjectWithId:onSuccess:onFailure:` method.

Lets read an existing todo object with primary key "1234":

```obj-c
[[[SMClient defaultClient] dataStore] readObjectWithId:@"1234" inSchema:@"todo" onSuccess:^(NSDictionary* object, NSString *schema) {
  // object contains the read object from the server.
} onFailure:^(NSError *error, NSString* objectId, NSString *schema) {
  // Handle error
}];
```

<p class="alert">Check out the Queries section for information on how to read all or a subset of objects in a schema.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Read an Object" target="_blank">SMDataStore: Read an Object</a></li>
      </ul>
    </div>
  </div>
</div>


<!--- UPDATE OBJECT -->

### Updating an Object

You can update an existing object at any time by creating a dictionary with the fields/values to update and passing it to the `updateObjectWithId:inSchema:onSuccess:onFailure:` method.

Lets update our existing todo object with a new title:

```obj-c
NSDictionary *updatedTodo = [NSDictionary dictionaryWithObjectsAndKeys:@"updated todo", @"title", nil];

[[SMClient defaultClient] dataStore] updateObjectWithId:updatedTodo inSchema:@"todo" onSuccess:^(NSDictionary *object, NSString *schema) {
  // object contains the full updated object.
} onFailure:^(NSError *error, NSDictionary* object, NSString *schema) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Update an Object" target="_blank">SMDataStore: Update an Object</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- UPDATE ATOMIC COUNTER -->

### Update a Counter Atomically

If you have a field of `Integer` type, you can atomically increment it, meaning increment the value without worrying about it being incremented at the same time by another request.

Suppose you had an existing todo object with a `priority` field. You can increment the field by any value (here by 1) like so:

```obj-c
[[[SMClient defaultClient] dataStore] updateAtomicCounterWithId:@"1234" field:@"priority" inSchema:@"todo" by:1 onSuccess:^(NSDictionary* object, NSString *schema) {
  // object contains the full updated object
} onFailure:^(NSError *error, NSDictionary* object, NSString *schema) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Update an Object" target="_blank">SMDataStore: Update an Object</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- DELETE OBJECT -->

### Deleting an Object

To delete an existing object, simply pass its primary key to the `deleteObjectId:inSchema:onSuccess:onFailure:` method.

Lets delete our existing todo object from the Stackmob database:

```obj-c
[[[SMClient defaultClient] dataStore] deleteObjectId:@"1234" inSchema:@"todo" onSuccess:^(NSString* objectId, NSString *schema) {
  // Successful deletion of objectId
} onFailure:^(NSError *error, NSString* objectId, NSString *schema) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Delete an Object" target="_blank">SMDataStore: Delete an Object</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- PER REQUEST OPTIONS -->

### Per Request Options

Each type of method (create, read, update, etc) has an overloaded method declaration with a parameter that takes an instance of <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html" target="_blank">SMRequestOptions</a>. The parameter name in all methods is called options. This allows you to provide a custom `SMRequestOptions` instance that will be applied to customize certain settings for that request. 

For example, provide a `SMRequestOptions` instance with the `isSecure` property set to YES if you wanted a specific create request to run over SSL.

<p class="alert">
Caching options provided by the SMRequestOptions class are not taken into account during datastore API requests.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html" target="_blank">SMRequestOptions Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>


<!---
  ///////////////////
  QUERIES
  //////////////////
-->


## Queries

To perform queries using the datastore API, you'll first instantiate an instance of `SMQuery`:

```obj-c
SMQuery *newQuery = [[SMQuery alloc] initWithSchema:@"todo"];
```

Next, you'll add conditions, pagination, sorting, etc as needed:

```obj-c
[newQuery where:@"title" isEqualTo:@"Do Homework"];
// add additional conditions
```

Finally, perform the query through the datastore property of your client instance:

```obj-c
[[[SMClient defaultClient] datastore] performQuery:newQuery onSuccess:^(NSArray *results) {
  // results contains an array of dictionary objects that match the query
} onFailure:^(NSError *error) {
  // Error
}];
```

Read through the `SMQuery` class reference, listed below, for all conditions you can place on your queries.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMQuery.html" target="_blank">SMQuery Class Reference</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Performing Queries" target="_blank">SMDataStore: Performing Queries</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Performing Count Queries" target="_blank">SMDataStore: Performing Count Queries</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
  ///////////////////
  RELATIONSHIPS
  //////////////////
-->

## Relationships

Our REST API offers many endpoints to make it super easy to create and append related objects, append existing object atomically, and update multiple related objects in one request, to name a few.

<p class="alert">You do not need inverse relationships if you are solely using the datastore API. Core Data requires inverse relationships, and the iOS SDK in conjunction with Core Data manages these relationships, especially when it comes to delete rule propogation.</p>

<!--- RELATE OBJECTS -->

### Relate Objects Via Primary Keys

You can create or update objects with relationships by specifying the primary keys of the related objects as the values.

Suppose your `todo` schema has a one to one relationship named `thecategory` which relates to objects in the `category` schema. You can create a new `todo` object with a relationship to a `category` object like so:

```obj-c
// Assumes the category object with primary key 1234 exists
NSDictionary *todoObject = [NSDictionary dictionaryWithObjectsAndKeys:@"new todo", @"title", @"1234", @"thecategory", nil];

// Make request
[[[SMClient defaultClient] dataStore] createObject:todoObject inSchema:@"todo" onSuccess:^(NSDictionary *theObject, NSString *) {
  // Handle success
} onFailure:^(NSError *error) {
  // Handle error
}];
```

Suppose the relationship was one-to-many and called `thecategories`. You can create the todo object and relate it to many `category` objects like so:

```obj-c
// Assumes the category object with primary key 1234 exists
NSDictionary *todoObject = [NSDictionary dictionaryWithObjectsAndKeys:@"new todo", @"title", [NSArray arrayWithObjects:@"1234", @"5678", nil], @"thecategories", nil];

// Make request
[[[SMClient defaultClient] dataStore] createObject:todoObject inSchema:@"todo" onSuccess:^(NSDictionary *result) {
  // Handle success
} onFailure:^(NSError *error) {
  // Handle error
}];
```
<br/>
**A Note About Inference**

While working in your **Development** environment, if the `thecategory` or `thecategories` relationships didn't exist in the `todo` schema, the methods above would infer a field of type `String`. 

To specify that relationships should get inferred, you'll create an instance of `SMRequestOptions` and use the `associateKey:withSchema:` method to manually specify which schema the relationship key relates to. Regardless of whether you are sending a create or update, you need to use the `createObject:...` method, which translates to a POST. For updates, simply add a key/value pair for the primary key.

Here's an example using our one-to-one example above:

```obj-c
// Assumes the category object with primary key 1234 exists
NSDictionary *todoObject = [NSDictionary dictionaryWithObjectsAndKeys:@"new todo", @"title", @"1234", @"thecategory", nil];

// Specify that "thecategory" is a relationship to the "category" schema
SMRequestOptions *options = [SMRequestOptions options];
[options associateKey:@"thecategory" withSchema:@"category"];

// Make request
[[[SMClient defaultClient] dataStore] createObject:todoObject options:options inSchema:@"todo" onSuccess:^(NSDictionary *result) {
  // Handle success
} onFailure:^(NSError *error) {
  // Handle error
}];
```

Behind the scenes the SDK will build a request header that lets StackMob know that `thecategory` is a relationship key on the `category` schema, and the relationship will get properly inferred.

<!-- Creating and Appending Related Objects -->

### Create and Append Related Objects

You can create and save related objects, then subsequently save them to another object's relationship value, all in one call.

This is instead of making a single call for each object, then another call to update the related object with the relationships.

Suppose we have an existing `todo` object with primary key `1234`. The `todo` schema has a one-to-many relationship `categories` to the `category` schema. Lets go ahead and create and two new `category` objects and relate them to the `todo` object:

```obj-c
NSDictionary *category1 = [NSDictionary dictionaryWithObjectsAndKeys:@"category1", @"name", nil];
NSDictionary *category2 = [NSDictionary dictionaryWithObjectsAndKeys:@"category2", @"name", nil];
NSArray *categories = [NSArray arrayWithObjects:category1, category2, nil];
            
[[[SMClient defaultClient] dataStore] createAndAppendRelatedObjects:categories toObjectWithId:@"1234" inSchema:@"todo" relatedField:@"categories" onSuccess:^(NSArray *succeeded, NSArray *failed) {
  // The succeeded array contains a list of the IDs for the objects which were persisted.
  // the failed array contains a list of the IDs for the objects which failed, either because of a duplicate key or internal server error.
} onFailure:^(NSError *error) {
  // Handle Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Create and Append Related Objects" target="_blank">SMDataStore: Create and Append Related Objects</a></li>
      </ul>
    </div>
  </div>
</div>

<!-- Fetch Expand -->


### Fetch with Expand

Relationships are represented in an object dictionary as either a string primary ID for one-to-one relationships or arrays of string primary IDs for one-to-many relationships. 

You can opt to fetch an object with an expand depth set; this will cause related objects to be returned as full dictionary objects rather than just primary IDs. The expand depth you set determines how deep related objects are expanded out. This is useful when you know you will be working with an object's related objects directly because you will only have to make one network call.

<p class="alert">The expand depth limit is 3.</p>

Suppose you have a `user` schema with a `friends` relationship field, which is a one-to-many relationship to the schema `user`. Normally the friends relationship value is represented as an array of primary keys (usernames in this case), but you can use the expand feature to have that array expanded as an array of full user objects instead.

To do this, define an instance of `SMRequestOptions`, set the `setExpandDepth:` method, and pass the instance to your datastore request.

Lets do a read on our user `john`, who has 3 friends: `jane`, `jill`, and `jack`:

```obj-c
SMRequestOptions *options = [SMRequestOptions options];
[options setExpandDepth:1];

[[[SMClient defaultClient] dataStore] readObjectWithId:@"john" inSchema:@"user" options:options onSuccess:^(NSDictionary *object, NSString *schema) {
  // object contains the expanded john object
} onFailure:^(NSError *error, NSString *objectId, NSString *schema) {
  // Handle Error
}];
```

`object` would have the following structure:

```bash
{
  username : "john"
  age : 25
  friends: [
            {
              username : "jane",
              age : 26,
              friends : [1234,5678,...]
            },
            {
              username : "jill",
              age : 29,
              friends : [4753,3855,...]
            },
            {
              username : "jack",
              age : 28,
              friends : [1734,7465,...]
            }
           ]
}
```

If we would have set expand depth `2`, we would have also got full objects for all of `jane`, `jill`, and `jack`'s friends, too.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html#task_Expanding Relationships" target="_blank">SMRequestOptions: Expanding Relationships</a></li>
      </ul>
    </div>
  </div>
</div>


<!-- Append Existing object -->


### Append Existing Objects

You can append objects to an array without needing to update the entire object at once. This goes for fields of type `Array` as well as one-to-many relationships.

This method also handles any concurrency issues if two users are modifying the same object at the same time by doing atomic appending.

If the field is an array, the objects will be appended to it with no uniqueness constraint.

If the field is a relationship, just append the object primary IDs to the array, not the objects themselves. The resulting array will be deduped so that there are no duplicate references to related object IDs.

Suppose you have an `Array` field called `lucky_numbers` in the `user` schema. You can append new lucky numbers to the array atomically like so:

```obj-c
NSArray *newLuckyNumbers = [NSArray arrayWithObjects:[NSNumber numberWithInt:13], [NSNumber numberWithInt:36], [NSNumber numberWithInt:23], nil];

[[[SMClient defaultClient] dataStore] appendObjects:newLuckyNumbers toObjectWithId:@"Bob" inSchema:@"user" field:@"lucky_numbers" onSuccess:^(NSDictionary* object, NSString *schema) {
  // object contains the parent object dictionary
} onFailure:^(NSError *error, NSString *objectId, NSArray* values, NSString *schema) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Append Existing Objects" target="_blank">SMDataStore: Append Existing Objects</a></li>
      </ul>
    </div>
  </div>
</div>


<!-- Deleting Related Objects -->

### Deleting Existing Objects/References

Just like you can append existing objects from array field types or relationship references to an object, you can delete them in the same fashion as well.

For a relationship reference, you also have the option of not only removing the reference, but deleting that object in its entirety at the same time.

Lets delete 2 `category` related object references from our `todo` object with primary key `12345678`:

```obj-c
NSArray *categoryIDsToDelete = [NSArray arrayWithObjects:@"1234", @"5678", nil];

[[[SMClient defaultClient] dataStore] deleteObjects:categoryIDsToDelete fromObjectWithId:@"12345678" schema:@"todo" field:@"categories" onSuccess:^(){
  // We have deleted the category references from the relationship value, but the category objects still exist.
} onFailure:^(NSError *error, NSString *objectId, NSArray* objects, NSString *schema){
  // Handle error
}];
```

If, in the same call, we also want to delete those category objects altogether, we just set the `cascadeDelete:` parameter to `YES`:


```obj-c
NSArray *categoryIDsToDelete = [NSArray arrayWithObjects:@"1234", @"5678", nil];

[[[SMClient defaultClient] dataStore] deleteObjects:categoryIDsToDelete fromObjectWithId:@"12345678" schema:@"todo" field:@"categories" cascadeDelete:YES onSuccess:^(){
  // We have deleted the category references from the relationship value as well as the category objects themselves.
} onFailure:^(NSError *error, NSString *objectId, NSArray* objects, NSString *schema){
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Delete Existing Objects" target="_blank">SMDataStore: Delete Existing Objects</a></li>
      </ul>
    </div>
  </div>
</div>


<!-- Upsert -->

### Upsert with Nested Objects

There might be times when you want create or update an object while creating or updating related objects, all in one request.

To do so, in the top level dictionary you would include a key/value pair where the key is the relationship name and the value is a nested dictionary or array of dictionaries.

However, you must manually define what schemas the related objects are associated with.

The first thing to note is that you will always send this type of request as a POST. Even if you are updating an object, you'll pass the dictionary object to the `createObject:...` method and simply include a key/value pair for the existing primary key.

To manually specify what schema a related object is associated with, you'll create an instance of `SMRequestOptions` and use the `associateKey:withSchema:` method for each relationship key in the request dictionary.

Lets create a new `todo` object which has a one to one relationship named `category`, associated with the schema `category`. In the same request we will be creating the new `category` object and relating it to the `todo` object:

```obj-c
// Create category object. We can specify a manual primary key if we wish, otherwise one will be automatically assigned when it's created.
NSDictionary *categoryObject = [NSDictionary dictionaryWithObjectsAndKeys:@"Home Projects", @"name", nil];

NSDictionary *todoObject = [NSDictionary dictionaryWithObjectsAndKeys:@"new todo", @"title", categoryObject, @"category", nil];

// Correlate the key "category" to the StackMob "category" schema
SMRequestOptions *options = [SMRequestOptions options];
[options associateKey:@"category" withSchema:@"category"];

// Execute request
[[[SMClient defaultClient] dataStore] createObject:todoObject inSchema:@"todo" options:options onSuccess:^(NSDictionary *result) {
  // Result will contain the entire todo object, as well as the entire nested category object.
} onFailure:^(NSError *error) {
  // Handle error
}];
```

The dictionary payload will end up looking like this:

```bash
// Request to "todo" schema
{
  title : "new todo",
  category : {
                name : "Home Projects"
             }
}
```

Behind the scenes the SDK will build a request header that lets StackMob know that the `category` key contains an object that should be created in the `category` schema.

<p class="alert">If you are working in development and the "category" relationship doesn't exist, it will be inferred and automatically created as a one-to-one relationship. If you send an array of objects as the value and the relationship doesn't exist yet, it will be inferred and automatically created as a one-to-many relationship.</p>

Here's an example of updating an existing `todo` object while updating a related `category` object with a new name:

```obj-c
NSDictionary *categoryUpdate = [NSDictionary dictionaryWithObjectsAndKeys:@"updated name", @"name", @"1234", @"category_id", nil];

NSDictionary *todoUpdate = [NSDictionary dictionaryWithObjectsAndKeys:@"updated title", @"title", categoryObject, @"category", @"5678", @"todo_id", nil];

// Associate the key "category" with the StackMob "category" schema
SMRequestOptions *options = [SMRequestOptions options];
[options associateKey:@"category" withSchema:@"category"];

// We always use a POST (create) request when upserting, even though this is technically an update
[[[SMClient defaultClient] dataStore] createObject:todoObject inSchema:@"todo" options:options onSuccess:^(NSDictionary *result) {
  // Result will contain the entire todo object, as well as the entire updated category object as a nested value.
} onFailure:^(NSError *error) {
  // Handle error
}];
```

The payload will end up looking like this:

```bash
// Request to "todo" schema
{
  todo_id : "5678"
  title : "updated todo",
  category : {
                category_id : "1234",
                name : "updated name"
             }
}
```
<br/>
Finally, you can also nest full related objects within full related objects. The trick is that all associated keys need to be specified with the top level relationship key as the reference, using dot notation.

Here's an example of creating a `todo` object while updating a one-to-one relationship to the `category` schema, and also updating a one-to-one relationship `genre` in that object:

```obj-c
NSDictionary *genreObject = [NSDictionary dictionaryWithObjectsAndKeys:@"Home and Living", @"type", @"5678", @"genre_id", nil];

NSDictionary *categoryObject = [NSDictionary dictionaryWithObjectsAndKeys:@"Updated Name", @"name", genreObject, @"genre", @"1234", @"category_id", nil];

NSDictionary *todoObject = [NSDictionary dictionaryWithObjectsAndKeys:@"new todo", @"title", categoryObject, @"category", nil];

// Associate the key "category" with the "category" schema as well as the key "genre" with the "genre" schema
SMRequestOptions *options = [SMRequestOptions options];
[options associateKey:@"category" withSchema:@"category"];
[options associateKey:@"category.genre" withSchema:@"genre"];

// Make request
[[[SMClient defaultClient] dataStore] createObject:todoObject inSchema:@"todo" options:options onSuccess:^(NSDictionary *result) {
  // Result will contain the entire todo object, as well as the entire updated category and genre objects as a nested values.
} onFailure:^(NSError *error) {
  // Handle error
}];
```

The payload will end up looking like this:

```bash
// Request to "todo" schema
{
  title : "new todo",
  category : {
                category_id : "1234",
                name : "Updated Name",
                genre : {
                          genre_id : @"5678",
                          type : "Home and Living"
                        }
             }
}
```

<p class="alert">This all works for one-to-many relationships as well. Just use an array of dictionaries to specify that the relationship is one-to-many.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html#//api/name/associateKey:withSchema:" target="_blank">associateKey:withSchema:</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Creating through dashboard -->

### Dashboard Relationship Creation

While relationships, like fields and schemas, are inferred by StackMob, sometimes you'll want to manually create them yourself.  Here we'll assume that we have both `todo` and `user` schemas already defined.

<a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank">Edit the user schema</a> and add a new relationship.

<p class="screenshot"><a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/relationships/dashboard-schemas-relationships-add.png" alt=""/></a></p>

Fill in the relationship details:

<p class="screenshot"><a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/relationships/add_relationship_dashboard.png" alt=""/></a></p>

Press **Add Relationship** and you'll get:

<p class="screenshot"><a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/relationships/todos_relationship_added.png" alt=""/></a></p>

**Save the schema** and that's it - you have a relationship.

When you work with Core Data, you'll want to make sure you define inverse relationships as well, so in this case you'll also add a relationship to the `todo` schema pointing back to the `user` schema.


<!---
	///////////////////
	USER AUTHENTICATION
	//////////////////
-->

## User Authentication

It won't be uncommon for your app to be built around the concept of having users. They'll need to sign up and login to use your service, and depending on their role may have access to certain data not available to others. Luckily for you, StackMob has you covered.

Authentication with StackMob was built based on the Oauth 2.0 protocol.

<p class="alert">For the parts of User Authentication that deal with Core Data, namely creating a user object and retrieving a user managed object, refer to the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#UserAuthentication" target="_blank">User Authentication section of the Core Data Integration Guide</a>.</p>

<!--- The User Schema -->

### The User Schema

When you create an application on StackMob, a **user** schema is automatically generated, with **username** as it's primary key field as well as a **password** field. This is the default schema for user objects.
 
When you initialize an `SMClient` with `initWithAPIVersion:publicKey:`, the following property defaults are set:

* apiHost = @"api.stackmob.com";
* userSchema = @"user";
* userPrimaryKeyField = @"username";
* userPasswordField = @"password";

You have a few ways you can change the defaults so they match your schemas and fields on StackMob.

With your instance of `SMClient`, you can directly set the properties listed above using the dot notation or setters.  For example:

```obj-c
self.client.userSchema = @"teacher";
[self.client setUserPrimaryKeyField:@"email"];
```

Alternatively, you can set all the properties at once using `initWithAPIVersion:apiHost:publicKey:userSchema:userPrimaryKeyField:userPasswordField:`.

<p class="alert">Don't forget to check the <b>Create as a User Object</b> box when <a href="https://developer.stackmob.com/api/schemas/create" target="_blank">creating a new schema</a> for user objects.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Initialize" target="_blank">SMClient: Initialize</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Creating User Object -->

### Creating a User Object

Creating a user is no different than creating any other object. You'll create a dictionary and include at least the username and password fields and values, and pass the dictionary to the `SMDataStore createObject:` method.

For password protection, we recommend sending the request over HTTPS. To do so, create an instance of `SMRequestOptions` with HTTPS set to `YES`, then pass the instance to the overloaded create method. 

Here's an example of creating a user object:

```obj-c
// In many cases you will pull the username and password from a text field
NSDictionary *newUser = [NSDictionary dictionaryWithObjectsAndKeys:self.usernameField.text, @"username", self.passwordField.text, @"password"];
SMRequestOptions *requestOptions = [SMRequestOptions optionsWithHTTPS];

[[SMClient defaultClient] dataStore] createObject:newUser inSchema:@"user" options:requestOptions onSuccess:^(NSDictionary* object, NSString *schema) {
  // object contains full user object dictionary
} onFailure:^(NSError *error) {
  // Handle error
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#//api/name/createObject:inSchema:options:onSuccess:onFailure:" target="_blank">SMDataStore: createObject:inSchema:options:onSuccess:onFailure:</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html#//api/name/optionsWithHTTPS" target="_blank">SMRequestOptions: optionsWithHTTPS
      </ul>
    </div>
  </div>
</div>

<!--- Login -->

### Logging in to StackMob

Logging into StackMob using the standard username/password pattern is done through the `SMClient` instance:

```obj-c
[[SMClient defaultClient] loginWithUsername:username password:password onSuccess:^(NSDictionary *result){
	// result contains a dictionary representation of the user object
} onFailure:^(NSError *){
	// Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Basic Authentication" target="_blank">SMClient: Basic Authentication</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/user-authentication-tutorial" target="_blank">User Authentication Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Get logged in user -->

### Retrieve Logged In User Info

At any point in time you can send a request to StackMob to receive a dictionary representation for the currently logged in user:

```obj-c
[[SMClient defaultClient] getLoggedInUserOnSuccess:^(NSDictionary *result){
	// Result contains a dictionary representation of the user object
} onFailure:^(NSError *){
	// Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Retrieve the Logged In User" target="_blank">SMClient: Retrieve Logged In User</a></li>
      </ul>
    </div>
  </div>
</div>


<!--- Check Status -->

### Check Status

To see whether your user is logged in, use the `SMClient` `isLoggedIn` and `isLoggedOut` methods.

<p class="alert">These methods are from the point of the view of the client-side SDK. If the current user session has actually expired and the client has a valid refresh token it will return <code>YES</code> to <code>isLoggedIn</code> because the next request will trigger an <a href="#AutomaticLoginRefresh">automatic refresh of the current user session</a>. To check whether a user is logged from the point of view of the server, use <code>getLoggedInUserOnSuccess:onFailure:</code></p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Check Status" target="_blank">SMClient: Check Status</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/user-authentication-tutorial" target="_blank">User Authentication Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>


<!--- Auto Refresh -->

### Automatic Login Refresh

The iOS SDK comes built in with automatic login refresh functionality. In order to understand how this works, lets first define the way a user session works:

When a request to login a user is successful, the response includes, amongst other things, an access token and a refresh token. The access token is used for subsequent requests to "authorize" the request. However, it has an expiration date because allowing the access token to work until a logout request was specifically made would be insecure. When the access token does expire, instead of having to initiate a new session using the standard login methods which require passwords, etc., we can make a request to refresh the session using the refresh token. If this is successful, the server returns a response with a new access token as well as a new refresh token. This process is repeated to keep the user session alive for as long as needed.

Here's something important to note: If a user logs in on one device, then logs in on another device, the refresh token from the login on the first device becomes invalid. What will happen is that when that device attempts to refresh the login, the server will deny the request and indicate that the refresh token is invalid. To handle this situation as seamlessly as possible, provide a block to be executed when a token refresh fails using the `setTokenRefreshFailureBlock:` method, like so:

```obj-c
[[SMClient defaultClient] setTokenRefreshFailureBlock:^(NSError *error, SMFailureBlock originalFailureBlock){
	// You are given the refresh error as well as the failure callback from the original request if you choose to call it.
	// We recommend logging in again by presenting a temporary login screen to the user, then picking up where they left off in the UI
}];
```

If a refresh token attempt fails and causes a failure callback to be executed, the error will always be -109 (SMErrorRefreshTokenFailed).

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/setTokenRefreshFailureBlock:" target="_blank">setTokenRefreshFailureBlock:</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Logout -->

### Logout of StackMob

Here's how to log a user out of StackMob:

```obj-c
[[SMClient defaultClient] logoutOnSuccess:^(NSDictionary *result){
	// Successful logout
} onFailure:^(NSError *error){
	// Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Logout" target="_blank">SMClient: Logout</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/user-authentication-tutorial" target="_blank">User Authentication Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Reset -->

### Reset Password

Developers can easily implement a reset password workflow in their application. A basic example workflow might look like the following:

1. Implement a screen for logged in users to reset their password.
2. Allow the user to insert their current password, followed by a new password. Optionally ask them to confirm the new password.
3. Initiate a method which calls the `SMClient` `changeLoggedInUserPasswordFrom:to:onSuccess:onFailure:` method.
4. The logged in user's password is reset!

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Resetting Password" target="_blank">SMClient: Resetting Password</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Forgot Pass -->

### Forgot Password

Developers can easily implement a forgot password workflow in their application. To do this, your user schema will need to have a field dedicated to users' email addresses, which can be the username field if your users sign in with their emails, for example. Choose the correct field which maps to emails in the **Forgot Password** section on the user schema configuration page. Once you have done this, a basic example work-flow might look like the following:

1. Have a button on the login screen which maps to a forgot password method.
2. When the user presses the button they are prompted to enter their username.
3. This should trigger the `SMClient` `sendForgotPaswordEmailForUser:onSuccess:onFailure:` method. The user will receive an email with a temporary password.
4. When they come back to your app, they enter their username and temp password and are prompted to enter a new password.
5. Developers complete the workflow by logging in the user with the `SMClient` `loginWithUsername:temporaryPassword:settingNewPassword:onSuccess:onFailure:` method.
6. The logged in user's password is reset!

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Resetting Password" target="_blank">SMClient: Resetting Password</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Basic Authentication" target="_blank">SMClient: Basic Authentication</a>
      </ul>
    </div>
  </div>
</div>

<!---
  ///////////////////
  ACCESS CONTROLS
  //////////////////
-->

## Security

StackMob provides the ability to lock down access to your data with Access Controls.  You can disallow access to Create, Read, Update or Delete.  With user authentication, you can even restrict permissions at a user level.

Here, we set it so that only logged in users can create objects for this schema.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/Scrumptious/dashboard-acl.png" alt=""/>

If a user who was not logged in then attempted to create an object, the response would return a 401 Unauthorized error.  Because the logic is handled server-side, access controls cannot be tampered with from client side code.

Find which access controls work best for your app's use case. <a href="https://developer.stackmob.com/modules/acl/docs" target="_blank">Read about all the Access Control options.</a> 


<!---
  ///////////////////
  GEOLOCATION
  //////////////////
-->


## Geolocation

StackMob provides GeoPoint field types which allow you to save objects with latitude and longitude coordinates. Once you have objects that contain geolocation data, you can query for a subset of those objects near a particular coordinate pair, within a specific radius, or even within a box formed from coordinates. This makes building apps that show all restaurants around a user's current location, or tracking a user's path as they jog through the city, as simple as ever.

<!--- Add schema field -->

### Add a GeoPoint Schema Field

In order for a schema to allow for geopoints, it must have a field with the GeoPoint type. To manually add a geopoint field to your schema, follow the <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-GeoPoint-Field-to-Schemas" target="_blank">Adding a GeoPoint Field To Schemas</a> tutorial to get set up.

<!--- Track Geo-Location -->

### Start Tracking Geolocation

`SMLocationManager` is a built-in `CLLocationManager` singleton for use in retrieving `CLLocationCoordinate2D` points. 
 
Many apps make use of geolocation data; `SMLocationManager` aides in this process by eliminating the boilerplate code needed to build a `CLLocationManager` singleton.

<!--- SUB: Using SMLocationManager -->

#### Using SMLocationManager

First, import the class where needed: 

```obj-c
#import "SMLocationManager.h"
```

To start retrieving data, first prompt `SMLocationManager` to start listening for updates from the device GPS:

```obj-c
[[[SMLocationManager sharedInstance] locationManager] startUpdatingLocation];
```

When you are finished, tell the location manager to stop listening for GPS updates.

```obj-c
[[[SMLocationManager sharedInstance] locationManager] stopUpdatingLocation];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMLocationManager.html" target="_blank">SMLocationManager Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Manual lat/lon -->

#### Obtaining Latitude/Longitude Values

If you wish to pull the latitude and longitude data manually from the location manager, do so with the following:

```obj-c
NSNumber *latitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].latitude];
NSNumber *longitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].longitude];
```

Often times it may take a few run loops for the location manager to start receiving actual data after <i>startUpdatingLocation</i> is called.  You may need to implement the necessary logic to ensure you are not pulling nil data.

<!--- SUB: Subclass SMLocationManager -->

#### Subclassing SMLocationManager

Out of the box, `SMLocationManager` gives you the ability to start/stop pulling GPS location data, as well as retrieve the current latitude and longitude of the device.  If you would like more control and customization, it's recommended you subclass `SMLocationManager`. In the init method of your subclass, you can configure the properties of the `CLLocationManager` as needed.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.apple.com/library/mac/#documentation/CoreLocation/Reference/CLLocationManager_Class/CLLocationManager/CLLocationManager.html" target="_blank">CCLocationManager Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Save Geo Data -->

### Saving Geolocation Data 

The `SMGeoPoint` class provides a simple interface around a geopoint. It inherits from `NSDictionary` and has two instance methods: `latitude` and `longitude`.

Use the provided class method to obtain the current location data of the device:

```obj-c
[SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
     
    // Do something with geoPoint.latitude and geoPoint.longitude
     
} onFailure:^(NSError *error) {

    // Error
}];
```

For example, we can immediately immediately create/read/update an object once we get out geopoint data:

```obj-c
[SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
     
    NSDictionary *arguments = [NSDictionary dictionaryWithObjectsAndKeys:@"My Location", @"name", geoPoint, @"location", nil];
 
  [[[SMClient defaultClient] dataStore] createObject:arguments inSchema:@"todo" onSuccess:^(NSDictionary *theObject, NSString *schema) {
      NSLog(@"Created object %@ in schema %@", theObject, schema);
 
  } onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
      NSLog(@"Error creating object: %@", theError);
  }];
     
} onFailure:^(NSError *error) {
    // Error
}];
```

You can alternatively make an `SMGeoPoint` with a latitude and a longitude:

```obj-c
NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];

SMGeoPoint *location = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];
```
 
Furthermore, you can use a `CLLocationCoordinate2D` coordinate:

```obj-c
CLLocationCoordinate2D renoCoordinate = CLLocationCoordinate2DMake(39.537940, -119.783936);

SMGeoPoint *reno = [SMGeoPoint geoPointWithCoordinate:renoCoordinate];
```

Here's an example of saving an `SMGeoPoint` from a `CLLocationCoordinate2D`, storing it in a dictionary of arguments and creating an object with the data:

```obj-c
CLLocationCoordinate2D renoCoordinate = CLLocationCoordinate2DMake(39.537940, -119.783936);
  
SMGeoPoint *location = [SMGeoPoint geoPointWithCoordinate:renoCoordinate];
 
NSDictionary *arguments = [NSDictionary dictionaryWithObjectsAndKeys:@"My Location", @"name", location, @"location", nil];
 
[[[SMClient defaultClient] dataStore] createObject:arguments inSchema:@"todo" onSuccess:^(NSDictionary *object, NSString *schema) {
    // Create success
} onFailure:^(NSError *error, NSDictionary *object, NSString *schema) {
    Handle error
}];
```

<p class="alert">Geolocation in iOS relies on the <b>MapKit</b> framework.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMGeoPoint.html" target="_blank">SMGeoPoint Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Read Geo-Location -->

### Reading Geolocation Values

Geopoint data will always be returned in a dictionary object as an `NSDictionary` instance with `lat` and `lon` keys.


<!--- Query Geo-Location -->

### Query based on Geolocation

You can query geo data by using conditions found in the `SMQuery` class. Here is an example of querying for all people within 5 miles of a given location:

```obj-c
NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];

SMGeoPoint *location = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];

SMQuery *query = [[SMQuery alloc] initWithSchema:@"people"];

[query where:@"location" isWithin:5 milesOfGeoPoint:location];

[[[SMClient defaultClient] datastore] performQuery:query onSuccess:^(NSArray *results) {
  // Successful query
} onFailure:^(NSError *error) {
  // Handle error
}];

```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMQuery.html" target="_blank">SMQuery Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>


<!---
  ///////////////////
  BINARY DATA
  //////////////////
-->

## File Storage

Files and images in the form binary data works by linking StackMob to your personal Amazon S3 account. You'll then declare a Binary type field in your schema and all files, images, etc. that you save will actually be stored in your S3 library, and the value of the field will be the URL that points to that data.

<!--- Add schema field -->

### Add a Binary Schema Field

Follow the <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-Binary-Field-to-Schemas" target="_blank">Adding a Binary Field To Schemas</a> tutorial to get the S3 module and a Binary field in your schema set up.

### Saving Binary Data

You will use the `SMBinaryDataConversion` class to convert instances of `NSData` into strings ready to be saved to StackMob.

`SMBinaryDataConversion` offers a class method `stringForBinaryData:name:contentType:` to decode binary data into a string. This is then used to send to StackMob as the value for a field with type `Binary`. The contents of the string will be parsed and the content will be stored on S3. StackMob will then store the URL as the value.

Here's an example of converting a picture stored in the app's bundle:

```obj-c
NSBundle *bundle = [NSBundle bundleForClass:[self class]];
NSString* pathToImageFile = [bundle pathForResource:@"coolPic" ofType:@"jpg"];
NSData *theData = [NSData dataWithContentsOfFile:pathToImageFile];

NSString *picData = [SMBinaryDataConversion stringForBinaryData:theData name:@"whateverNameYouWant" contentType:@"image/jpg"];
```

Now create a dictionary that includes the `picData` string as the value for your Binary field key, and create/update your object. Here we create a new `comment` object with our image:

```obj-c
NSDictionary *objectToCreate = [NSDictionary dictionaryWithObjectsAndKeys:picData, @"photo", nil];

[[[SMClient defaultClient] dataStore] createObject:objectToCreate inSchema:@"comment" onSuccess:^(NSDictionary *object, NSString *schema) {
  // [object objectForKey:@"photo"] will contain the S3 URL where the image is stored in string form.
} onFailure:^(NSError *theError, NSDictionary *object, NSString *schema) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMBinaryDataConversion.html" target="_blank">SMBinaryDataConversion Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>

### Reading Binary Data

Once you have saved an object to StackMob containing a field with binary data, the value will then contain a string representation of the S3 URL. You will most likely convert this string into an instance of `NSURL`, and in turn use that URL to get and set data to a variable. For example, suppose we have saved an image in our attribute `photo`, and now want to set an image for our UI. After reading the object we would convert the string URL to an image like this:

```obj-c
NSURL *imageURL = [NSURL URLWithString:[object objectForKey:@"photo"]]; 
NSData *imageData = [NSData dataWithContentsOfURL:imageURL]; 
UIImage *image = [UIImage imageWithData:imageData];
```

<!---
  ///////////////////
  NETWORK REACHABILITY
  //////////////////
-->

## Network Reachability

The iOS SDK provides an interface for determining the current status of the device's network connection. This comes in handy when you want to perform specific operations based on whether the device is online or offline. The interface allows you to execute a block of code whenever the network status changes, which is perfect for optimizing your application to only send requests when the device is online.

<!--- SMNetworkReachability -->

### SMNNetworkReachability Class

`SMNetworkReachability` provides an interface to monitor the network reachability from the device to StackMob.
 
Network reachability checks are already built into the SDK.  When the network is not reachable and a request is made, an error with domain `SMError` and status code -105 (SMNetworkNotReachable) will be returned.

An instance of `SMNetworkReachability` is created during the initialization of an `SMUserSession`, accessible by the `networkMonitor` property.

A shorthand is available through the `SMClient` instance: `client.networkMonitor`.

### Checking the Current Network Status

To manually check the current network status, use the `currentNetworkStatus` method:

```obj-c
SMNetworkStatus currentStatus = [self.client.networkMonitor currentNetworkStatus];
```

This method will return an `SMNetworkStatus`, defined as:

```obj-c
typedef enum {
    SMNetworkStatusUnknown = -1,
    SMNetworkStatusNotReachable  = 0,
    SMNetworkStatusReachable = 1,
} SMNetworkStatus;
```
 
* Reachable - the device has a network connection and can successfully reach StackMob.
* Not Reachable - StackMob is not reachable either because there is no network connection on the device or the service is down.
* Unknown - Typically this status arises during in-between times of network connection initialization.
 
An example of testing reachability before sending a request would look like this:

```obj-c
if ([self.client.networkMonitor currentNetworkStatus] == SMNetworkStatusReachable) {
    // send request
}
```
 
You can also handle each state case by case in a switch statement:

```obj-c
switch([client.session.networkMonitor currentNetworkStatus]) {
    case  SMNetworkStatusReachable:
        // do Reachable stuff
        break;
    case SMNetworkStatusNotReachable:
        // do NotReachable stuff
        break;
    case SMNetworkStatusUnknown:
        // do Unknown stuff
        break;
    default:
        break;
}
```

<!--- Status Change Notifications -->

### Status Change Notifications

You can register to receive notifications when the network status changes by simply adding an observer for the notification name `SMNetworkStatusDidChangeNotification`.  The notification will have a `userInfo` dictionary containing one entry with key `SMCurrentNetworkStatusKey` and `NSNumber` representing the `SMNetworkStatus` value.

In order to access the value of `SMCurrentNetworkStatusKey` in a format for comparing to specific states or use in a switch statement, retrieve the intValue like this:

```obj-c
if ([[[notification userInfo] objectForKey:SMCurrentNetworkStatusKey] intValue] == SMNetworkStatusReachable) {
    // do Reachable stuff
}
```

<p class="alert">Remember to remove your notification observer before the application terminates.</p>
 
 
### Status Changes Blocks 

Often times you may want to change the cache policy, or initiate a sync with the server depending on the status of the network. You can set a block that will be executed every time the network status changes with the `setNetworkStatusChangeBlock:` method:

```obj-c
[self.client.networkMonitor setNetworkStatusChangeBlock:^(SMNetworkStatus status){
    
    if (status == SMNetworkStatusReachable) {
      ...
    } else {
      ...
    }

}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMNetworkReachability.html" target="_blank">SMNetworkReachability Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
  ///////////////////
  OFFLINE SYNC
  //////////////////
-->

## Caching and Offline Sync

Included with version 2.0.0+ of the SDK is a sync system built in to the Core Data Integration to allow for local saving and fetching of objects when a device is offline. When back online, modified data will be synced with the server. Many settings are available to the developer around cache and merge policies, conflict resolution, etc. 

Read through the <a href="https://developer.stackmob.com/ios-sdk/offline-sync-guide" target="_blank">Caching and Offline Sync Guide</a> for all information.

<!---
	///////////////////
	SOCIAL INTEGRATION
	//////////////////
-->

## Social Integration

StackMob provides integrations with Facebook and Twitter to allow users to login to StackMob using preexisting credentials, as well as features interfaces to post status updates. 

<!--- Facebook -->

### Facebook

<!--- SUB: FB Auth Workflow -->

#### Facebook Auth Workflow

You will need to download the Facebook SDK and follow their tutorials to get authentication working in your application. Once you have a Facebook session open you will direct the application in the Facebook `sessionStateChanged:state:error` method to login to StackMob using the provided Facebook access token.

All the implementation details for Facebook's SDK can be found in our <a href="https://developer.stackmob.com/ios-sdk/integrating-with-facebook-tutorial" target="_blank">Facebook Integration Tutorial</a>.

<!--- SUB: Login with Facebook -->

#### StackMob Login Using Facebook

Logging into StackMob using Facebook credentials requires that a StackMob user object already exists and is linked to the user's Facebook access token. Fortunately, you can do all of that with one method.

Assuming you have opened a Facebook session and have initiated a custom method from the Facebook `sessionStateChanged:state:error` method, you would login to StackMob like so:

```obj-c,3-8
[[FBRequest requestForMe] startWithCompletionHandler:^(FBRequestConnection *connection,NSDictionary<FBGraphUser> *user, NSError *error) {
 if (!error) {
     [[SMClient defaultClient] loginWithFacebookToken:FBSession.activeSession.accessTokenData.accessToken createUserIfNeeded:YES usernameForCreate:user.username onSuccess:^(NSDictionary *result) {
         NSLog(@"Logged in with StackMob");
         [self updateView];
     } onFailure:^(NSError *error) {
         NSLog(@"Error: %@", error);
     }];
 } else {
     // Handle error accordingly
     NSLog(@"Error getting current Facebook user data, %@", error);
 }
}
}];
```

<p class="alert">By wrapping the login call in the `requestForMe` block, we can assign the Facebook user's username as the primary key of the StackMob user object. This is completely optional, and if nil is passed to the <code>usernameForCreate</code> parameter the Facebook user's ID is used.</p> 

This login method will create a StackMob user object and link them to the provided Facebook token if one does not already exist.

Alternatively, you can implement a more manual workflow using the methods listed in the <b>Create a User Object</b> and <b>Link/Unlink Facebook Tokens sections</b>.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Login with Facebook" target="_blank">SMClient: Login With Facebook</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/integrating-with-facebook-tutorial" target="_blank">Facebook Integration Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Create User -->

#### Create a User Object

You can create a new StackMob user object and link a Facebook access token for future login using Facebook credentials:

```obj-c
[[SMClient defaultClient] createUserWithFacebook:activeSession.accessTokenData.accessToken onSuccess:^(NSDictionary *result) {
  // New user created
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Create a User with Facebook" target="_blank">SMClient: Create a User With Facebook</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Link/Unlink Token -->

#### Link/Unlink Facebook Tokens

If you have an existing StackMob user object and you want to link that object with a Facebook token to enable future login using Facebook credentials, use the link token method:

<p class="alert">The user must be logged in for the link to work.</p>

```obj-c
[[SMClient defaultClient] linkLoggedInUserWithFacebookToken:activeSession.aceessTokenData.accessToken onSuccess:^(NSDictionary *result) {
  // Successful link
} onFailure:^(NSError *error) {
  // Error
}];
```

You can also unlink a logged in user object from a token at any time:

```obj-c
[[SMClient defaultClient] unlinkLoggedInUserFromFacebookOnSuccess:^{
  // Successful unlink
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Link/Unlink a User From Facebook" target="_blank">SMClient: Link/Unlink a User From Facebook</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Update Status -->

#### Update Facebook Status

The logged in user can post a new Facebook status like so:

```obj-c
[[SMClient defaultClient] updateFacebookStatusWithMessage:@"Can't wait for the weekend!" onSuccess:^(NSDictionary *result) {
  // Success
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Update Facebook Status" target="_blank">SMClient: Update Facebook Status</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Get User Info -->

#### Get Facebook User Info

Sometimes you may want to get the logged in user's Facebook info, either for UI purposes or to update the StackMob user object. You can use the `[FBRequest requestForMe]` method from the Facebook SDK. Alternatively, StackMob also provides an equivalent method:

```obj-c
[[SMClient defaultClient] getLoggedInUserFacebookInfoWithOnSuccess:^(NSDictionary *result) {
  // Result contains dictionary of Facebook user info
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Get Facebook User Info" target="_blank">SMClient: Get Facebook User Info</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Twitter -->

### Twitter

<!--- SUB: Twitter Auth Workflow -->

#### Twitter Auth Workflow

StackMob provides additional files to make the Twitter authentication workflow easy and painless.

First you will download, unzip and add the <a href="https://s3.amazonaws.com/static.stackmob.com/resources/ios/SMTwitterCredentials.zip">SMTwitterCredentials files</a> to your Xcode project.

For the files to compile you will need to link your target with the **Social**, **Twitter**, and **Accounts** frameworks.

Next you will declare an `SMTwitterCredentials` property, either in your App Delegate or where ever you encapsulate the StackMob login logic:

```obj-c
@property (nonatomic, strong) SMTwitterCredentials *twitterCredentials;
```

To properly initialize the property you'll need your Twitter App Consumer key and secret. In order to get your app keys, you will need a Twitter app. For instructions, see the <a href="https://developer.stackmob.com/ios-sdk/integrating-with-twitter-tutorial#CreateTwitterApp" target="_blank">Create Twitter App</a> section of the Twitter Ingegration Tutorial.

Once you have your keys, you'll also need to add your keys to the StackMob Twitter Module. For instructions, see the <a href="https://developer.stackmob.com/ios-sdk/integrating-with-twitter-tutorial#AddKeysToTwitterModule" target="_blank">Add Keys To Twitter Module</a> section of the Twitter Ingegration Tutorial.

When you are all set, initialize your `SMTwitterCredentials` property:

```obj-c
self.twitterCredentials = [[SMTwitterCredentials alloc] initWithTwitterConsumerKey:@"TWITTER_APP_KEY" secret:@"TWITTER_APP_SECRET"];
```

Finally, when you are ready to open and authenticate a Twitter session, `SMTwitterCredentials` provides the `retrieveTwitterCredentialsForAccount:onSuccess:onFailure` method, which takes care of everything for you:

```obj-c
[self.twitterCredentials retrieveTwitterCredentialsForAccount:nil onSuccess:^(NSString *token, NSString *secret, NSDictionary *fullResponse) {

  // fullResponse includes key/value pairs for the token and secret as well as the account screen name and id.
  // Save the screen name, login, etc. Example login is shown below.

  /*
    StackMob method to login with Twitter token and secret.  A StackMob user will be created with the username provided if one doesn't already exist attached to the provided credentials.
  */
  [[SMClient defaultClient] loginWithTwitterToken:token twitterSecret:secret createUserIfNeeded:YES usernameForCreate:fullResponse[@"screen_name"] onSuccess:^(NSDictionary *result) {
    NSLog(@"Successful Login with Twitter: %@", result);
  } onFailure:^(NSError *error) {
    NSLog(@"Login failed: %@", error);
  }];

} onFailure:^(NSError *error) {
  NSLog(@"Twitter Auth Error: %@", error);
}];
```

<p class="alert">If you pass nil for the account name, it will automatically display an action sheet pop-up with the available accounts, prompting the user to choose one. Once they have done that, you can save and pass the username for future login calls. No user interaction is therefore needed, and this is a good way to simulate a "stay logged in" feature.</p>

`SMTwitterCredentials` also provides `twitterAccountsAvailable`, a Boolean property indicating whether the user has an accounts set up on their device. This feature does not work for the simulator, it will always return true.

The entire authentication workflow can be found in our <a href="https://developer.stackmob.com/ios-sdk/integrating-with-twitter-tutorial" target="_blank">Twitter Integration Tutorial</a>.

<!--- SUB: Login with Facebook -->

#### StackMob Login Using Twitter

Logging into StackMob using Twitter credentials requires that a StackMob user object already exists and is linked to the user's Twitter access token and secret. Fortunately, you can do all of that with one method.

Assuming you have opened a Twitter session, most likely using the methods found in the `SMTwitterCredentials` files. You can then login to StackMob like so:

```obj-c
[[self.appDelegate client] loginWithTwitterToken:oauthToken twitterSecret:oauthTokenSecret createUserIfNeeded:YES usernameForCreate:@"Joe Schmoe" onSuccess:^(NSDictionary *result) {
  // Successful Login
} onFailure:^(NSError *error) {
  // Error
}];
```

This login method will create a StackMob user object and link them to the provided Twitter tokens if one does not already exist.

Alternatively, you can implement a more manual workflow using the methods listed in the <b>Create a User Object</b> and <b>Link/Unlink Twitter Tokens sections</b>.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Login With Twitter" target="_blank">SMClient: Login With Twitter</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/integrating-with-twitter-tutorial" target="_blank">Twitter Integration Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Create User -->

#### Create a User Object

You can create a new StackMob user object and link a Twitter access token and secret for future login using Twitter credentials:

```obj-c
[[SMClient defaultClient] createUserWithTwitterToken:oauthToken twitterSecret:oauthTokenSecret onSuccess:^(NSDictionary *result) {
  // New user created
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Create a User With Twitter" target="_blank">SMClient: Create a User With Twitter</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Link/Unlink -->

#### Link/Unlink Twitter Tokens

If you have an existing StackMob user object and you want to link that object with a Twitter tokens to enable future login using Twitter credentials, use the link token method:

<p class="alert">The user must be logged in for the link to work.</p>

```obj-c
[[SMClient defaultClient] linkLoggedInUserWithTwitterToken:oauthToken twitterSecret:oauthTokenSecret onSuccess:^(NSDictionary *result) {
  // Successful link
} onFailure:^(NSError *error) {
  // Error
}];
```

You can also unlink a logged in user object from a token at any time:

```obj-c
[[SMClient defaultClient] unlinkLoggedInUserFromTwitterOnSuccess:^{
  // Successful unlink
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Link/Unlink a User From Twitter" target="_blank">SMClient: Link/Unlink a User From Twitter</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Update Status -->

#### Update Twitter Status

The logged in user can post a new Twitter status like so:

```obj-c
[[SMClient defaultClient] updateTwitterStatusWithMessage:@"Can't wait for the weekend!" onSuccess:^(NSDictionary *result) {
  // Success
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Update Twitter Status" target="_blank">SMClient: Update Twitter Status</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: Get User Info -->

#### Get Twitter User Info

Sometimes you may want to get the logged in user's Twitter info, either for UI purposes or to update the StackMob user object:

```obj-c
[[SMClient defaultClient] getLoggedInUserTwitterInfoWithOnSuccess:^(NSDictionary *result) {
  // Result contains dictionary of Facebook user info
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Get Twitter User Info" target="_blank">SMClient: Get Twitter User Info</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
  ///////////////////
  CUSTOM CODE
  //////////////////
-->

## Custom Code

With the Custom Code module, StackMob lets you write code that runs and executes on the server. Sending batch push messages? Running some jobs? Write it in Custom Code and call it from the SDK or anything that can hit a REST API. You can define custom JSON responses so that your server can also notify the client of the result.

<!--- Write Custom Code -->

### Write Custom Code

The first step to performing custom code requests from the client is... writing and uploading them to the server!

Head to the <a href="https://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK" target="_blank">Custom Code SDK Guide</a> to get started with writing your server side logic and getting up uploaded to StackMob.

<!--- Perform Custom Code Requests -->

### Performing Custom Code Requests

Once you have your custom code methods uploaded to the StackMob server, you'll perform client side requests to execute those methods.

First, you'll initialize an instance of `SMCustomCodeRequest`, passing it the name of your custom code method. Equivalent methods exist for initializing POST, GET, PUT, and DELETE requests. Let's initiate a GET request for a custom code method named `aggregate_tasks`:

```obj-c
SMCustomCodeRequest *ccRequest = [[SMCustomCodeRequest alloc] initGetRequestWithMethod:@"aggreate_tasks"];
```

In the case of a GET request, we can set query string parameters using the `addQueryStringParameterWhere:equals:` method.

In the case of a POST or PUT request, we can set a request body if we need to using the `requestBody` property.

When we are ready to send our request to execute our custom code method, we will use the `SMDataStore` instance provided by our client:

```obj-c
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:ccRequest onSuccess:^(NSURLRequest *request, NSHTTPURLResponse *response, id responseBody) {
  // Success
} onFailure:^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id responseBody){
  // Failure
}];
```

You can expect the `responseBody` parameter of the success callback to be in the form of:

* Serialized JSON (NSDictionary/NSArray) if the content type of the response is application/vnd.stackmob+json or application/json, which it will be if you use the map response type in your custom code method.
* A string (NSString) if the content type of the response is text/plain, which it will be if you use the string response type in your custom code method..
* Raw data (NSData) for any other response content type, including if you use the byte array response type in your custom code method.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCustomCodeRequest.html" target="_blank">SMCustomCodeRequest Class Reference</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Performing Custom Code Methods" target="_blank">SMDataStore: Performing Custom Code Methods</a>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/custom-code-tutorial" target="_blank">Custom Code Requests Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Setting Request Headers -->

### Setting Request Headers

To set request headers for your custom code request, instantiate an instance of `SMRequestOptions` using the `optionsWithHeaders:` class method:

```obj-c
NSDictionary *headers = [NSDictionary dictionaryWithObjectsAndKeys:@"valueOfHeader", @"headerName", nil];
SMRequestOptions *options = [SMRequestOptions optionsWithHeaders:headers];
```

When you go to perform your custom code request, use the method which includes the `options:` parameter:

```obj-c
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:ccRequest options:options onSuccess:^(NSURLRequest *request, NSHTTPURLResponse *response, id responseBody) {
  // Success
} onFailure:^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id responseBody){
  // Failure
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html" target="_blank">SMRequestOptions Class Reference</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#task_Performing Custom Code Methods" target="_blank">SMDataStore: Performing Custom Code Methods</a>
      </ul>
    </div>
  </div>
</div>

<!--- Setting Content Types -->

### Setting Response Content Types

The default whitelisted response content types are:

* application/vnd.stackmob+json (StackMob vendor specific)
* application/json
* text/plain
* application/octet-stream

Set the `responseContentType` property custom code request if you expect the content type to be something other than the default, for example `image/jpeg`. If you do not do this, the response will be rejected and the failure callback will be executed.

```obj-c,2
SMCustomCodeRequest *ccRequest = [[SMCustomCodeRequest alloc] initGetRequestWithMethod:@"random_pic"];
ccRequest.responseContentType = @"image/jpeg";
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCustomCodeRequest.html#//api/name/responseContentType" target="_blank">responseContentType property</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Retry Blocks -->

### Custom Retry Blocks

If you make a custom code request has hasn't been executed recently, the server needs a few seconds to activate your environment. The request will return a 503 response code with a header specifying when to retry. <b>The iOS SDK reads this header and retries the request automatically.</b> As the developer you have the option to specify a retry block, so in the case of a 503 response, the SDK will call your retry block instead of automatically retrying the request.

The first step is to define an instance of `SMRequestOptions` that you'll pass to your `performCustomCodeRequest:` method. Next you'll define a service unavailable retry block:

```obj-c
SMRequestOptions *options = [SMRequestOptions options];

[options addSMErrorServiceUnavailableRetryBlock:^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id responseBody, SMRequestOptions *options, SMFullResponseSuccessBlock successBlock, SMFullResponseFailureBlock failureBlock) {
  
  // Make changes and updates to parameters

  // If you choose to do so, execute the retry method:
  [[[SMClient defaultClient] dataStore] retryCustomCodeRequest:request options:options onSuccess:successBlock onFailure:failureBlock];
}];
```

The retry blocks takes an instance of `SMFailureRetryBlock`, defined as:

```obj-c
typedef void (^SMFailureRetryBlock)(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON, SMRequestOptions *options, SMFullResponseSuccessBlock successBlock, SMFullResponseFailureBlock failureBlock);
```

As you can see, you have the option at the end of the block to retry the custom code request with the `retryCustomCodeRequest:` method.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html#//api/name/addSMErrorServiceUnavailableRetryBlock:" target="_blank">addSMErrorServiceUnavailableRetryBlock:</a>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#//api/name/retryCustomCodeRequest:options:onSuccess:onFailure:" target="_blank">retryCustomCodeRequest:options:onSuccess:onFailure:</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
  ///////////////////
  PUSH
  //////////////////
-->

## Push Notifications

StackMob integrates with Apple's APNS service for push notifications on iOS. Before you start using push notifications, you should familiarize yourself with the client-side aspects of APNS by reading the <a href="http://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Introduction.html" target="_blank">Apple Local and Push Notification Programming Guide</a>. Don't worry about the server-side aspects of push - that's what we're here for.

<!--- Module -->

### The Push Module

The first step in getting push notifications working is to set up the Push Module for you StackMob application.

The <a href="https://developer.stackmob.com/ios-sdk/push-guide" target="_blank">iOS Push Notifications Guide</a> will walk you through the steps of setting up Apple Push certs and uploading them to the Push Module settings.

<!--- Init -->

### Initializing a Push Client

The StackMob push notification API is accessed through an instance of `SMPushClient`:

```obj-c
SMPushClient *pushClient = [[SMPushClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY" privateKey:@"YOUR_PRIVATE_KEY"];
```

<p class="alert">You supply your private key to the init methods because Push Notifications on StackMob use the Oauth 1 protocol.</p>

You should only instantiate one instance of `SMPushClient` and use it throughout your application, and it's easiest to initialize it at the same time you initialize the `SMClient` instance.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#task_Initialize" target="_blank">SMPushClient: Initialize</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/push-notifications-tutorial" target="_blank">iOS Push Notifications Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Register -->

### Registering Devices

To register a device, call the `registerRemoteNotificationTypes:` method:

```obj-c
[[UIApplication sharedApplication] registerForRemoteNotificationTypes:UIRemoteNotificationTypeAlert];
```

Next, implement the delegate methods for remote notifications. 

Inside `didRegisterForRemoteNotificationsWithDeviceToken:`, register the device token on StackMob with the `registerDeviceToken:` method:

```obj-c,9-14
- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
  
    NSString *token = [[deviceToken description] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    token = [[token componentsSeparatedByString:@" "] componentsJoinedByString:@""];
     
    // Persist token if needed
     
     // Register the token with StackMob.  User is an arbitrary string to associate with the token.
    [self.pushClient registerDeviceToken:token withUser:@"Person123" onSuccess:^{
        // Successful registration
    } onFailure:^(NSError *error) {
        // Error
    }];
}
 
- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err {
    // Error in registration
}
```

<p class="alert">The <code>withUser:</code> parameter takes an arbitrary string to associate with the token. Typically this should be a username so that in the future you can push to devices using usernames rather than token strings.</p>

Once your token is registered with StackMob, you can begin sending pushes to that device!

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#task_Registering a Device Token" target="_blank">SMPushClient: Registering a Device Token</a></li>
        <li><a href="http://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/IPhoneOSClientImp.html#//apple_ref/doc/uid/TP40008194-CH103-SW2" target="_blank">Apple: Registering For Remote Notifications</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/push-notifications-tutorial" target="_blank">iOS Push Notifications Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Receive Notifications -->

### Receiving Notifications

To receive remote notifications in your app, implement the `UIApplication` `didReceiveRemoteNotification:` method:

```obj-c
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
  
  // Here's an example of displaying an alert with the push message
  UIApplicationState state = [application applicationState];
  if (state == UIApplicationStateActive) {
      NSString *cancelTitle = @"Close";
      NSString *showTitle = @"Show";
      NSString *message = [[userInfo valueForKey:@"aps"] valueForKey:@"alert"];
      UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"StackMob Message"
                                                          message:message
                                                         delegate:self
                                                cancelButtonTitle:cancelTitle
                                                otherButtonTitles:showTitle, nil];
      [alertView show];

  } else {
      //Do stuff that you would do if the application was not active
  }
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://developer.apple.com/library/mac/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/IPhoneOSClientImp.html#//apple_ref/doc/uid/TP40008194-CH103-SW4" target="_blank">Apple: Handling Local and Remote Notifications</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/push-notifications-tutorial" target="_blank">iOS Push Notifications Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Broadcast -->

### Broadcasting Messages

To broadcast a push message from a device, create your dictionary payload and pass it to the `broadcastMessage:` method.

The payload can consist of the `alert`, `badge`, and `sound` keys.

```obj-c
NSDictionary *pushMessage = [NSDictionary dictionaryWithObjectsAndKeys:@"Hello, this is a push!", @"alert", [NSNumber numberWithInt:1], @"badge", nil];
[self.pushClient broadcastMessage:pushMessage onSuccess:^{
  // Successful push
} onFailure:^(NSError *error) {
  // Error
}];
```

<p class="alert">The value of the `alert` key can only be a string. Dictionary values are currently not supported.</p>

<p class="alert">The notification payload for using standalone APNS requires you encapsulate the alert, badge, and sound keys in an dictionary identified by the key `aps`, which is nested in an outer dictionary. When sending message payloads to StackMob push methods, you only need to pass the dictionary would be the value of the `aps` key.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#//api/name/broadcastMessage:onSuccess:onFailure:" target="_blank">broadcastMessage:onSuccess:onFailure:</a></li>
        <li><a href="http://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/ApplePushService.html#//apple_ref/doc/uid/TP40008194-CH100-SW1" target="_blank">Apple: The Notification Payload</a>
      </ul>
    </div>
  </div>
</div>

<!--- Send to Users -->

### Sending Messages to Users

You can send push messages to other devices via the username strings that were originally linked with those tokens during registration:

```obj-c
NSDictionary *pushMessage = [NSDictionary dictionaryWithObjectsAndKeys:@"It's your turn!", @"alert", [NSNumber numberWithInt:1], @"badge", nil];
NSArray *users = [NSArray arrayWithObjects:@"Ted", @"Mary", @"Joe", nil];

[self.pushClient sendMessage:pushMessage toUsers:users onSuccess:^{
  // Successful push
} onFailure:^(NSError *error){
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#//api/name/sendMessage:toUsers:onSuccess:onFailure:" target="_blank">sendMessage:toUsers:onSuccess:onFailure:</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Send to Tokens -->

### Sending Messages to Tokens

You can send push messages to other devices via the registered tokens:

```obj-c
NSDictionary *pushMessage = [NSDictionary dictionaryWithObjectsAndKeys:@"It's your turn!", @"alert", [NSNumber numberWithInt:1], @"badge", nil];
NSArray *tokens = [NSArray arrayWithObjects:@"57383958", @"543224738", @"2857732", nil];

[self.pushClient sendMessage:pushMessage toTokens:tokens onSuccess:^{
  // Successful push
} onFailure:^(NSError *error){
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#//api/name/sendMessage:toTokens:onSuccess:onFailure:" target="_blank">sendMessage:toTokens:onSuccess:onFailure:</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Get Tokens -->

### Getting Tokens For Users

If you need the registered tokens for a set of users, you can do so with the `getTokensForUsers` method:

```obj-c
NSArray *users = [NSArray arrayWithObjects:@"Ted", @"Mary", @"Joe", nil];

[self.pushClient getTokensForUsers:users onSuccess:^(NSDictionary *results) {
  // Results contains a dictionary of user:token pairs
} onFailure:^(NSError *error){
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#//api/name/getTokensForUsers:onSuccess:onFailure:" target="_blank">getTokensForUsers:onSuccess:onFailure:</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Delete Token -->

### Deleting a Token

If you need to delete a token registered on the StackMob server from the client, use the `deleteToken:` method:

```obj-c
[self.pushClient deleteToken:@"12345678" onSuccess:^{
  // Token delete success
} onFailure:^(NSError *error) {
  // Error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPushClient.html#//api/name/deleteToken:onSuccess:onFailure:" target="_blank">deleteToken:onSuccess:onFailure:</a></li>
      </ul>
    </div>
  </div>
</div>

### Push Dev vs. Prod

Once you set your version to 1 and use the production keys, StackMob will use Apple's production push server instead of the Apple sandbox.

You should only use the dev keys with version 0 and the production keys with version 1.

The user tokens don't change, so you would still test in the same manner.

Here is some additional info on <a href="https://developer.stackmob.com/module/apiversions">API versioning with development and production environments</a>.

### Using StackMob For Push Only

While the Push API comes built into the Core SDK, a separate Push SDK is available for those using StackMob only for push notifications.  You can download the library from [Github](https://github.com/downloads/stackmob/stackmob-ios-push-sdk/stackmob-ios-push-sdk-v1.0.2.zip).  Drag the StackMobPush-vx.x.x folder into your project with the **Copy items into destination group's folder** checkbox selected, and set your Target's **Other Linker Flags** build setting to **-ObjC**.

<!---
  ///////////////////
  DEPLOY
  //////////////////
-->

## Deploy

You're done developing your app! You've been working in the StackMob development environment, and you now want to get everything into the production environment. StackMob lets you easily do that with our Deploy UI.


### API

StackMob gives you separate development and production environments so that you can keep your test data and custom code separate from your production set. Deploying your API is a *critical* step you need to do when deploying. StackMob provides a simple UI to help you do this.

Read about how to <a href="https://developer.stackmob.com/tutorials/dashboard/Deploying-your-StackMob-App" target="_blank">Deploy your API</a>

<p class="screenshot"><a href="https://dashboard.stackmob.com/deploy" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/apiversions/modules-apiversions-deploy.png" alt=""/></a></p>

After deploying, you'll point your iOS SDK to your production API Version (in this case `1`) as well as your Production public key:

```obj-c
self.client = [[SMClient alloc] initWithAPIVersion:@"1" publicKey:@"YOUR_PRODUCTION_PUBLIC_KEY"];
```

That's it! With your server rolled out to API Version 1 and your iOS SDK pointing to your production public key, all requests will be funneled to your production database.  Your production environment uses a separate database than your development one, so you'll be able to continue developing in API Version 0 without affecting your customers using the production version of your app.

<p class="alert">
  With StackMob's <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">Multiple API Versions module</a> you can have multiple Productions running concurrently, ensuring backwards compatibility with apps already on the market.
</p>


### HTML5

If you're using StackMob's HTML5 hosting service, you'll also need to <a href="https://developer.stackmob.com/tutorials/html5/Deploying-your-HTML5-App-to-Production" target="_blank">deploy your HTML files to production</a>.

<p class="screenshot"><a href="https://dashboard.stackmob.com/deploy" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/html5/modules-html5-deploy.png" alt=""/></a></p>


<!---
  ///////////////////
  ERROR CODES
  //////////////////
-->

## Error Handling

When errors occur at the SDK level, specific error codes are returned to indicate the type of error that occurred.

### Core Data Save Failure Errors

When a Core Data save fails because objects could not be inserted/updated/deleted on StackMob, the error returned is formatted in the following way:

<ul>
  <li>The error code will be -108, <b>SMErrorCoreDataSave</b></li>
  <li>The error userInfo property may contain any of the following keys, which are constants you can reference, depending on what the save request consisted of: <b>SMInsertedObjectFailures</b>, <b>SMUpdatedObjectFailures</b>, <b>SMDeletedObjectFailures</b>.</li>
  <li>If any of those keys exist, its value will be an NSArray instance of NSDictionary instances, one dictionary for each failed object.</li>
  <li>Each dictionary will include 2 key constants: <b>SMFailedManagedObjectID</b>, whose value is an NSManagedObjectID instance of the managed object that was not saved properly, and <b>SMFailedManagedObjectError</b>, whose value is an NSError instance of what caused the object to not get saved properly.</li>
</ul>

Here's an example of an error returned when trying to save a user object with primary key "Jack" when a user named "Jack" already exists on the server:

```bash
Error Domain=SMError Code=-108 "The operation couldn't be completed. (SMError error -108.)" UserInfo=0xc99d030 {
  SMInsertedObjectFailures=(
    {
      SMFailedManagedObjectError = "Error Domain=HTTP Code=409 \"The operation couldn\U2019t be completed. 
          (HTTP error 409.)\" UserInfo=0x8661680 {error=Duplicate key for schema user: \"jack\"}";
      SMFailedManagedObjectID = "0xc9a0210 <x-coredata://FDCF65C5-02D3-45E9-BA03-793E1A125FCE-12897-00000C7A7DD73EAA/User/pjack>";
    }
  )
}
```

### StackMob Specific Errors

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>Error Code</th>
        <th>Typedef</th>
    </tr>
    <tr>
        <td>-100</td>
        <td>SMErrorInvalidArguments</td>
    </tr>
    <tr>
        <td>-101</td>
        <td>SMErrorTemporaryPasswordResetRequired</td>
    </tr>
    <tr>
        <td>-102</td>
        <td>SMErrorNoCountAvailable</td>
    </tr>
    <tr>
        <td>-103</td>
        <td>SMErrorRefreshTokenInProgress</td>
    </tr>
    <tr>
        <td>-104</td>
        <td>SMErrorPasswordForUserObjectNotFound</td>
    </tr>
    <tr>
        <td>-105</td>
        <td>SMErrorNetworkNotReachable</td>
    </tr>
    <tr>
        <td>-106</td>
        <td>SMErrorCacheIDNotFound</td>
    </tr>
    <tr>
        <td>-107</td>
        <td>SMErrorCouldNotFillRelationshipFault</td>
    </tr>
    <tr>
        <td>-108</td>
        <td>SMErrorCoreDataSave</td>
    </tr>
    <tr>
        <td>-109</td>
        <td>SMErrorRefreshTokenFailed</td>
    </tr>
</table>

### General HTTP Errors

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>Error Code</th>
        <th>Typedef</th>
    </tr>
    <tr>
        <td>400</td>
        <td>SMErrorBadRequest</td>
    </tr>
    <tr>
        <td>401</td>
        <td>SMErrorUnauthorized</td>
    </tr>
    <tr>
        <td>403</td>
        <td>SMErrorForbidden</td>
    </tr>
    <tr>
        <td>404</td>
        <td>SMErrorNotFound</td>
    </tr>
    <tr>
        <td>408</td>
        <td>SMErrorTimeout</td>
    </tr>
    <tr>
        <td>409</td>
        <td>SMErrorConflict</td>
    </tr>
    <tr>
        <td>500</td>
        <td>SMErrorInternalServerError</td>
    </tr>
    <tr>
        <td>501</td>
        <td>SMErrorNotImplemented</td>
    </tr>
    <tr>
        <td>502</td>
        <td>SMErrorBadGateway</td>
    </tr>
    <tr>
        <td>503</td>
        <td>SMErrorServiceUnavailable</td>
    </tr>
    <tr>
        <td>504</td>
        <td>SMErrorGatewayTimeout</td>
    </tr> 
</table>

### String Constants

The following is a list of string constants used for the domains of errors and names of exceptions.

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>Constant</th>
        <th>String Value</th>
    </tr>
    <tr>
        <td>SMErrorDomain</td>
        <td>@"SMError"</td>
    </tr>
    <tr>
        <td>HTTPErrorDomain</td>
        <td>@"HTTP"</td>
    </tr>
    <tr>
        <td>SMExceptionIncompatibleObject</td>
        <td>@"SMExceptionIncompatibleObject"</td>
    </tr>
    <tr>
        <td>SMExceptionUnknownSchema</td>
        <td>@"SMExceptionUnknownSchema"</td>
    </tr>
    <tr>
        <td>SMExceptionAddPersistentStore</td>
        <td>@"SMExceptionAddPersistentStore"</td>
    </tr>
    <tr>
        <td>SMExceptionCannotFillRelationshipFault</td>
        <td>@"SMExceptionCannotFillRelationshipFault"</td>
    </tr>
    <tr>
        <td>SMExceptionCacheError</td>
        <td>@"SMExceptionCacheError"</td>
    </tr>
    <tr>
        <td>SMOriginalErrorCausingRefreshKey</td>
        <td>@"SMOriginalErrorCausingRefresh"</td>
    </tr>
    <tr>
        <td>SMRefreshErrorObjectKey</td>
        <td>@"SMRefreshErrorObject"</td>
    </tr>
</table>

<!---
  ///////////////////
  DEBUGGING
  //////////////////
-->

## Debugging

The iOS SDK gives developers access to two global variables that will enable additional logging statements when using the Core Data integration:

* **SM_CORE_DATA_DEBUG** - In your AppDelegate's `application:DidFinishLaunchingWithOptions:` method, include the line `SM_CORE_DATA_DEBUG = YES;` to turn on log statements from `SMIncrementalStore`. This will provide information about the Datastore calls to StackMob happening behind the scenes during Core Data saves and fetches. The default is `NO`.
* **SM_MAX_LOG_LENGTH** - Used to control how many characters are printed when logging objects. The default is **10,000**, which is plenty, so you will almost never have to set this.  The only time you will see the string representation of an object truncated is when you have an Attribute of type String that maps to a field of type Binary on StackMob, because you are sending a string containing the binary of the image, etc. String representations of objects that have been truncated end with MAX\_LOG\_LENGTH\_REACHED.

<!---
  ///////////////////
  DEPRECATIONS
  //////////////////
-->

## SDK Deprecations

As we improve the SDK, sometimes that means we need to deprecate methods or properties.

### v2.0.0

* <b>(SMCoreDataStore)</b> <i>setDefaultMergePolicy: applyToMainThreadContextAndParent:</i>
    * Use <i>setDefault<b>CoreData</b>MergePolicy: applyToMainThreadContextAndParent:</i>
* <b>(SMNetworkReachability)</b> <i>SMNetworkStatus</i> options
    * Use <b>SMNetworkStatusUnknown</b>, <b>SMNetworkStatusNotReachable</b>, <b>SMNetworkStatusReachable</b>

### v1.4.0

* <b>(SMClient)</b> <i>loginWithFacebookToken: options: onSuccess: onFailure:</i>
    * Use <i>loginWithFacebookToken: <b>createUserIfNeeded: </b>options: <b>successCallbackQueue: failureCallbackQueue: </b>onSuccess: onFailure:</i>
* <b>(SMClient)</b> <i>loginWithTwitterToken: twitterSecret: options: onSuccess: onFailure:</i>
    * Use <i>loginWithTwitterToken: twitterSecret: <b>createUserIfNeeded: </b>options: <b>successCallbackQueue: failureCallbackQueue: </b>onSuccess: onFailure:</i>
* <b>(SMClient)</b> <i>loginWithGigyaUID: uidSignature: signatureTimestamp: options: onSuccess: onFailure:</i>
    * Use <i>loginWithGigyaUID: uidSignature: signatureTimestamp: options: <b>successCallbackQueue: failureCallbackQueue:</b> onSuccess: onFailure:</i>

### v1.2.0

* <b>(SMCoreDataStore)</b> <i>managedObjectContext</i> property
    * Use <i>contextForCurrentThread</i>
