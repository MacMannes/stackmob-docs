iOS Datastore API Developer Guide
=====================================

## Overview

The datastore API provides an easy-to-use wrapper on top of a REST API used to communicate with your StackMob database.

Once you have initialized your `SMClient` instance, you can perform any datastore method (create, read, update, delete, perform query, etc.) by obtaining an `SMDataStore` instance. This can be done with the `dataStore` method.

The easiest way to get a datastore instance is through the default client, starting your calls with the following:

```obj-c
[[SMClient defaultClient] dataStore] someDataStoreMethod...];
```

Alternatively you can initialize a separate `SMDataStore` instance from the client:

```obj-c
SMClient *client = ...;
SMDataStore *myDataStore = [client dataStore];
```

For a full list of all `SMDataStore` methods please visit the <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html" target="_blank">SMDataStore Class Reference</a>

The following sections highlight persistance with the datastore API, and should be an alternative to using the Core Data integration for persistence. Authentication, push notification, social integration, and custom code are done the same way regardless of which method of persistence you choose.

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

