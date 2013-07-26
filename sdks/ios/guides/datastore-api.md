iOS Datastore API Developer Guide
=====================================

## Overview

The datastore API provides an easy-to-use wrapper on top of a REST API used to communicate with your StackMob database.

Once you have initialized your `SMClient` instance, you can call any datastore method (create, read, update, delete, perform query, etc.) by obtaining an `SMDataStore` instance. This can be done with the `dataStore` method. From there you can call any method found in the `SMDataStore` class.

The easiest way to get a datastore instance is through the default client, starting your calls with the following:

```obj-c
[[SMClient defaultClient] dataStore] someDataStoreMethod...];
```

Alternatively you can initialize a separate `SMDataStore` instance from the client:

```obj-c
SMClient *client = ...;
SMDataStore *myDataStore = [client dataStore];
```

The following sections highlight persistance with the datastore API, and should be an alternative to using the Core Data integration for persistence. Authentication, push notification, social integration, and custom code are done the same way.

Caching and offline sync are not available using the datastore API.

In each section of this guide you may see colored boxes which are meant to highlight important information:

<p class="alert">Gold boxes call out warnings, gotchas, and information we don't want you to miss.</p>

<p class="alert alert-info">Blue boxes contain links to sections in the full API reference, as well as full working projects for you to download and in-depth tutorials for you to read through.</p>

<!---
	///////////////////
	DATASTORE
	//////////////////
-->

## Datastore

The following sections include everything you need to know about creating, reading, updating and deleting objects. All methods are asynchronous and allow you to provide a success and failure block, which will be performed on the main thread by default. All methods have alternative version which include parameters for request options and/or success and failure callback queues. Refer to the Resources sections for links to all available method signatures.


<!--- INSERT OBJECT -->

### Creating an Object

To create an object, simply define a dicitonary where the keys map to their corresponding StackMob fields. Then pass the dictionary to the `SMDataStore` `createObject:inSchema:onSuccess:onFailure:` method.

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

<!--- CREATING OBJECTS IN BULK -->

### Creating Objects in Bulk

You can create many objects in the same schema all in one call.

Lets create 5 todo objects, all with the same title:

```obj-c
NSMutableArray *todoArray = [NSMutableArray array];

for (int i = 0; i < 5; i++) {
	[todoArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:@"my todo", @"title", nil]];
}

[[SMClient defaultClient] dataStore] createObjects:todoArray inSchema:@"todo" onSuccess:^(NSArray* succeeded, NSArray *failed, NSString *schema) {
	// succeeded contains an array of IDs of all objects that were sucessfully persisted.
	// failed contains an array of IDs of all objects that were unsucessfully persisted, possibly because of duplicate key issues, etc.
} onFailure:^(NSError *error, NSArray *objects, NSString *schema) {
	// Handle error
}];
```


<!--- READ OBJECT -->

### Reading an Object

To read an existing object, use the `readObjectWithId:onSuccess:onFailure:` method.

Lets read a todo object with primary key "1234":

```obj-c
[[[SMClient defaultClient] dataStore] readObjectWithId:@"1234" inSchema:@"todo" onSuccess:^(NSDictionary* object, NSString *schema) {
	// object contains the read object from the server.
} onFailure:^(NSError *error, NSString* objectId, NSString *schema) {
	// Handle error
}];
```

<p class="alert">Check out the Queries section for information on how to read all or a subset of objects in a schema.</p>


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

<!--- PER REQUEST OPTIONS -->

### Per Request Options

Each type of method (asynchronous/synchronous save/fetch) has an overloaded method declaration with a parameter that takes an instance of <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html" target="_blank">SMRequestOptions</a>. The parameter name in all methods is called options. This allows you to provide a custom `SMRequestOptions` instance that will be applied to all calls in that request. For example, provide a `SMRequestOptions` instance with the `isSecure` property set to YES if you wanted a specific save request to run over SSL i.e. all inserts/updates/deletes for that request will be sent over SSL.

<p class="alert">
Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests. The following options are currently safe to set and will override the default for the duration of the request:</br></br>
&nbsp;&nbsp;&bull;&nbsp;&nbsp;isSecure property (HTTPS)
&nbsp;&nbsp;&bull;&nbsp;&nbsp;cacheResults property (for saves/fetches)
&nbsp;&nbsp;&bull;&nbsp;&nbsp;cachePolicy property (for fetches only)
</br></br>
Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
</p>

<!--- LOWER LEVEL DATASTORE API -->

### Lower Level Data Store API

All asynchronous methods which serve as basic wrappers on top of the REST API can be found in the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMDataStore.html" target="_blank">SMDataStore</a> class.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/lower-level-crud-api-tutorial" target="_blank">Lower Level CRUD API Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

