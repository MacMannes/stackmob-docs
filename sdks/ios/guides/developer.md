iOS Developer Guide
=====================================

Core Data allows us to place a familiar wrapper around StackMob REST calls and Datastore API. iOS developers can leverage their existing knowledge of Core Data to quickly integrate StackMob into their applications. For those interested in sticking to the REST-based way of making requests, we provide the full Datastore API as well.

In each section of this guide you may see colored boxes which are meant to highlight important information:

<p class="alert">Gold boxes call out warnings, gotchas, and information we don't want you to miss.</p>

<p class="alert alert-info">Blue boxes contain links to sections in the full API reference, as well as full working projects for you to download and in-depth tutorials for you to read through.</p> 

<!---
	///////////////////
	SETUP
	//////////////////
-->

# Setup

<!--- INITIALIZE -->

## Initializing the iOS SDK

Wherever you plan to use StackMob, add `#import "StackMob.h"` to the header file.

Define a property of type `SMClient`, most likely in your `AppDelegate` file where you define other application wide properties, and initialize it like this:

```obj-c
self.client = [[SMClient alloc] initWithAPIVersion:@"YOUR_API_VERSION" publicKey:@"YOUR_PUBLIC_KEY"];
```

For `YOUR_API_VERSION`, pass `@"0"` for Development, `@"1"` or higher for the corresponding version in Production.

From then on you can either pass your client instance around, or use `[SMClient defaultClient]` to retrieve it at any point.

<p class="alert">If you haven't found your public key yet, check out <b>Manage App Info</b> under the <b>App Settings</b> sidebar on the <a href="https://dashboard.stackmob.com" target="_blank">Dashboard page</a>.</p>

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

<!--- USING CORE DATA -->

## Using Core Data

StackMob recommends using Core Data for data persistance. It provides a powerful and robust object graph management system that otherwise would be a nightmare to implement.  Although it may have a reputation for being pretty complex, the basics are easy to grasp and understand.

We've created a separate guide on using StackMob with Core Data, which contains everything from Core Data basics to Coding Practices to Specifications on what is supported by the StackMob integration.

<p class="alert">Since a lot of your interactions with StackMob will be through Core Data, please be sure to read through the guide.</p>

<a href="https://developer.stackmob.com/ios-sdk/core-data-guide" target="_blank">StackMob and Core Data Guide</a>

<!--- DEF MOM -->

## Defining a Managed Object Model

To use Core Data you will define an `NSManagedObjectModel` instance which points to your local object graph, a `.xcdatamodeld` file. This file defines a local representation of your database, using Entities which have attributes and relationships.

Your Core Data model should replicate your StackMob schemas. The <a href="http://stackmob.github.io/stackmob-ios-sdk/CoreDataSupportSpecs.html" target="_blank">StackMob - Core Data Support Specifications</a> page details the mappings between StackMob field types and Core Data atrribute types, ensuring proper translations to and from Core Data.

Suppose your data model is called `mydatamodel`. In your `AppDelegate` file, declare a property called `managedObjectModel` of type `NSManagedObjectModel` and include the following method in your implementation file:

```obj-c
- (NSManagedObjectModel *)managedObjectModel
{
    if (_managedObjectModel != nil) {
        return _managedObjectModel;
    }
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"mydatamodel" withExtension:@"momd"];
    _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return _managedObjectModel;
}
```
<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectModel_Class/Reference/Reference.html" target="_blank">NSManagedObjectModel Class Reference</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- CREATE SUBCLASSES -->

## Creating `NSManagedObject` Subclasses

Creating classes for each one of your entities provides a lot of convience around constructors, accessors, validation, formatting, etc. for when you work with managed objects. We recommend you create these custom classes, which you can do by selecting an Entity in the Model Editor and going to `Editor -> Create NSManagedObject Subclass`:

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/coredata/nsmanagedobjectsubclass.png" /> 

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.apple.com/library/ios/#documentation/cocoa/Conceptual/CoreData/Articles/cdManagedObjects.html#//apple_ref/doc/uid/TP40003397-BBCEHEGG" target="_blank">Core Data Programming Guide: Managed Objects</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- INIT CDS -->

## Initialize a Core Data Store

A Core Data store instance gives you everything you need to work with StackMob's Core Data integration. With your `SMCoreDataStore` object you can retrieve a managed object context configured with a `SMIncrementalStore` as it’s persistent store to allow communication to StackMob from Core Data.  

You can create an `SMCoreDataStore` instance like so:

```obj-c
// This assumes you have already initialized your SMClient instance,
// and have a NSManagedObjectModel property called managedObjectModel
SMCoreDataStore *coreDataStore = [[SMClient defaultClient] coreDataStoreWithManagedObjectModel:self.managedObjectModel];
```

From then on you can either pass your core data store instance around, or if you are using iOS SDK v2.0.0+, use `[[SMClient defaultClient] coreDataStore]` to retrieve it at any point.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Retrieving a Datastore" target="_blank">SMClient: Retrieving a Datastore</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- OBTAIN MOC -->

## Obtaining a Managed Object Context

You can obtain a managed object context configured from your `SMClient` instance like this:

```obj-c
NSManagedObjectContext *context = [[[SMClient defaultClient] coreDataStore] contextForCurrentThread];
```

Use this instance of `NSManagedObjectContext` for the current thread you are on. We recommend calling `contextForCurrentThread` rather than passing a context instance across files or blocks.

The default Core Data merge policy set for all contexts created by this class is `NSMergeByPropertyObjectTrumpMergePolicy`. Use `setDefaultMergePolicy:applyToMainThreadContextAndParent:` to change the default.

You can obtain the managed object context for the main thread at any time by accessing the `mainThreadContext` property.

If you want to do your own context creation, use the `persistentStoreCoordinator` property to ensure your objects are being saved to the StackMob server.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
      	<li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#task_Obtaining a Managed Object Context" target="_blank">SMCoreDataStore: Obtaining a Managed Object Context</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
	///////////////////
	DATASTORE
	//////////////////
-->

# Datastore

<!--- INSERT OBJECT -->

## Creating an Object

Typically you will create an `NSManagedObject` subclass for each of your Core Data entities, giving you convience methods, accessors, etc. You might also find it valuable to define a custom init method. The easiest way to declare a new instance of an entity and fill in its values is like so:

```obj-c
Todo *newTodo = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.manangedObjectContext];

// Assumes Todo has attributes title and todoId
[newTodo setTitle:@"Take out the trash"];
[newTodo setTodoId:[newManagedObject assignObjectId]];
```

The `assignObjectId` method is provided by the SDK to assign an arbitrary ID to the attribute which maps to the primary key field on StackMob. Object IDs must be assigned to a managed object before it is persisted. This keeps Core Data IDs and StackMob objects in sync. You are free to assign your own IDs if you wish, `assignObjectId` is simply a convience method which masks a random UUID generation algorithm.

<b>Without Subclasses</b>

Instantiating and assigning values to a generic managed object requires using key-value methods in accordance with key value coding.

```obj-c
NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
[newManagedObject setValue:@"Take out the trash" forKey:@"title"];
[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
```

The `primaryKeyField` method is provided by the SDK for convience to return the attribute which maps to the primary key field on StackMob. The SDK knows which attribute is the primary key field because of its format. The format is defined in the <a href="http://stackmob.github.io/stackmob-ios-sdk/index.html#coding_practices" target="_blank">StackMob - Core Data Coding Practices</a>. 

<p class="alert">
The newly created object is not persisted until you <a href="#PerformingSaves">save the managed object context</a>.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObject+StackMobSerialization.html#//api/name/assignObjectId" target="_blank">assignObjectId</a></li>
		<li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObject+StackMobSerialization.html#//api/name/primaryKeyField" target="_blank">primaryKeyField</a></li>
		<li><a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSEntityDescription_Class/NSEntityDescription.html" target="_blank">NSEntityDescription Class Reference</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/create.zip">Download Sample Project</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- READ OBJECT -->

## Reading Objects

To read objects from a particular Entity, create and execute a Core Data fetch request using methods from the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency</a> class.

For a list of StackMob supported `NSFetchRequest` methods, see the `Fetch Requests` section of the <a href="file://localhost/Users/mattvaz/Documents/stackmob/editAppledocs/ios-sdk/CoreDataSupportSpecs.html#fetch_request" target="_blank">Core Data Support Specs</a>.

<p class="alert">By default, objects are returned as faults, and attribute values are not accessed from the persistent store until specifically called upon in your code.  This ensures in-memory usage is as low as possible.</p>

All information on specifying conditions for a query can be found in the <a href="#Queries">Queries</a> section. 

<!--- SUB: ASYNC READ -->

### Asynchronous Fetch Requests

We've created an asynchronous wrapper around the familiar `executeFetchRequest:error` method which ensures that fetches are performed off the main thread and will not block your applications UI.

```obj-c
NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
     
// set any predicates or sort descriptors, etc.

// execute the request
[self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
    NSLog(@"%@",results);
} onFailure:^(NSError *error) {
    NSLog(@"Error fetching: %@", error);
}];
```

If you wish to return managed object IDs rather than objects, use the following method:

```obj-c
[self.managedObjectContext executeFetchRequest:fetchRequest returnManagedObjectIDs:YES onSuccess:^(NSArray *results) {
    NSLog(@"%@",results);
} onFailure:^(NSError *error) {
    NSLog(@"Error fetching: %@", error);
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#task_Asynchronous Fetch" target="_blank">NSManagedObjectContext Category: Asynchronous Fetches</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read.zip">Download Sample Project</a></li>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Read-into-Table-View" target="_blank">Read Into Tableview Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: SYNC READ -->

### Synchronous Fetch Requests

To execute synchronous fetches, use the `executeFetchRequestAndWait:error:` method. This works like `executeFetchRequest:error`, but uses the child-parent context model that is used internally by the SDK.

```obj-c
NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
     
// Set any predicates or sort descriptors, etc.

// Execute the request
NSError *error = nil;
NSArray *results = [self.managedObjectContext executeFetchRequestAndWait:fetchRequest error:&error];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#task_Synchronous Fetch" target="_blank">NSManagedObjectContext Category: Synchronous Fetches</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- UPDATE OBJECT -->

## Updating an Object

After fetching an existing mananged object, update it by simply changing the object's values. The updated values are not persisted until the managed object context is saved.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/update.zip" target="_blank">Download Sample Project</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- DELETE OBJECT -->

## Deleting an Object

Delete an object by simply calling the managed object context `deleteObject` method and passing the manage object you wish to delete. The delete will not be finalized until you sve the managed object context.

```obj-c
[self.manangedObjectContext deleteObject:aManagedObject];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/delete.zip" target="_blank">Download Sample Project</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SAVE -->

## Performing Saves

At some point after inserting, updating, and deleting objects from a managed object context, you will initiate a save which will cause all modified objects to be persisted to the external store.  The external store in this case is StackMob as well as a local cache database, ensuring data is available while the devices is offline.

The StackMob SDK has placed a wrapper around the familiar `save:` core data method to offer synchronous and asynchronous versions for improved performance. All methods can be found in the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency</a> class.

<!--- ASYNC SAVES -->

### Asynchronous Saves

Asynchronous saves are great for executing a save off the main thread, ensuring the UI is not blocked. If other events rely on the sucessful save, place them in the callbacks.

```obj-c
[self.managedObjectContext saveOnSuccess:^{
    NSLog(@"You created a new object!");
} onFailure:^(NSError *error) {
    NSLog(@"There was an error! %@", error);
}];
```

Use the callbacks to update the UI, notifiy other dependent files, etc.

The SDK also provides save methods that allow you to pass request options and queues to perform the callbacks on. Check out the link below.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#task_Asynchronous Save" target="_blank">NSManagedObjectContext Category: Asynchronous Saves</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SYNC SAVES -->

### Synchronous Saves

Although the SDK synchronous save methods behave like Core Data's native `save:` method, we recommend using them because they follow the child-parent context patterns used internally by the SDK.

```obj-c
NSError *error = nil;
BOOL success = [self.managedObjectContext saveAndWait:&error];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#task_Synchronous Save" target="_blank">NSManagedObjectContext Category: Synchronous Saves</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- PER REQUEST OPTIONS -->

## Per Request Options

Each type of method (asynchronous/synchronous save/fetch) has an overloaded method declaration with a parameter that takes an instance of <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html" target="_blank">SMRequestOptions</a>. The parameter name in all methods is called options. This allows you to provide a custom `SMRequestOptions` instance that will be applied to all calls in that request. For example, provide a `SMRequestOptions` instance with the `isSecure` property set to YES if you wanted a specific save request to run over SSL i.e. all inserts/updates/deletes for that request will be sent over SSL.

<p class="alert">
Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests. The following options are currently safe to customize and will override the default for the duration of the request:</br></br>
&nbsp;&nbsp;&bull;&nbsp;&nbsp;isSecure property (HTTPS)</br></br>
Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
</p>

<!--- LOWER LEVEL DATASTORE API -->

## Lower Level Data Store API

If you want to make direct REST-based calls to the Datastore, check out the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMDataStore.html" target="_blank">SMDataStore</a> class.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Lower-Level-CRUD-API" target="_blank">Lower Level CRUD API Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!---
	///////////////////
	QUERIES
	//////////////////
-->

# Queries

<!--- Predicates -->

## Predicates

In Core Data, every read begins by creating a fetch request. You can then add predicates (conditions), sort descriptiors, offsets (ranges), etc. to return a more granular subset of results. To complete the read you execute the fetch request on a managed object context.

See the <a href="#ReadingObjects">Reading Objects</a> section for information on creating/executing a fetch request.

To learn more about fetch requests in general visit Apple's documentation on <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Conceptual/CoreData/Articles/cdFetching.html" target="_blank">Fetching Managed Objects</a>.

For a list of StackMob supported predicate types, visit the `Predicates` section of the <a href="file://localhost/Users/mattvaz/Documents/stackmob/editAppledocs/ios-sdk/CoreDataSupportSpecs.html#predicates" target="_blank">Core Data Support Specs</a>.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Basic-Queries" target="_blank">Basic Queries Tutorial</a></li>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Advanced-Queries" target="_blank">Advanced Queries Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Lower Level Query API -->

## Lower Level Query API

To perform queries using the lower level API, you'll first instantiate an instance of `SMQuery`:

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
	// Successful query
} onFailure:^(NSError *error) {
	// Error
}];
```

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

# Relationships

When you create relationships between entities in your Core Data model, they will translate directly to relationships on StackMob.  Here are the important things to note:

* Core Data recommends creating inverse relationships to maintain referential integrity. This ensures that when you delete one object, any related objects are updated as well.
* Core Data supports <a href="http://developer.apple.com/library/mac/#documentation/cocoa/conceptual/coredata/Articles/cdRelationships.html">Delete Rules</a>. Take this into considertation if you are building cross-platform.
* The great thing about Core Data handling relationship logic is that it will all translate from the client to StackMob, so no extra work is necessary.

<!--- One To One -->

## One to One

Add a relationship from one entity to another by creating a new entry in the **Relationships** section of the entity builder UI.  Convention says to name the relationship the singluar version of the entity it points to, like so:

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/relationships/one-to-one-guide.png" />

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/One-to-One-Relationships" target="_blank">One-To-One Relationships Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- One To Many -->

## One to Many

Add a relationship from one entity to another by creating a new entry in the **Relationships** section of the entity builder UI.  Convention says to name the relationship the plural version of the entity it points to, like so:

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/relationships/one-to-many-guide.png" />

In the data model, be sure to check the box for a To-Many relationship, as shown.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/One-to-Many-Relationships" target="_blank">One-To-Many Relationships Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Many To Many -->

## Many to Many

You can achieve a many to many relationship by simply creating two relationships which are the inverse of each other and both to-many relationships.

<!---
	///////////////////
	USER AUTHENTICATION
	//////////////////
-->

# User Authentication

<!--- The User Schema -->

## The User Schema

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

## Creating a User Object

In Core Data, you'll first create an entity which maps to your user schema on StackMob. It's most likely called `user`, so your entity will be called `User`.

<p class="alert">Do not create an attribute for the password field. It would not be safe to have passwords floating around in Core Data. The <code>SMUserManagedObject</code> class provides helper methods to safely set a password for a user object.</p> 

After creating a managed object subclass for the entity, you'll change the inherited class from `NSManagedObject` to `SMUserManagedObject`. This class provides methods to properly initialize a user object by linking to the `SMClient` instance. It also provides methods to set the user's password during creation.

In your entity subclass implementation file, we recommend creating a custom init method which calls the `SMUserManagedObject` `initWithEntityName:insertIntoManagedObjectContext` method. We have overriden this method to store a reference to `[SMClient defaultClient]`. This ensures that the SDK knows the primary key, etc of the user schema. Your init method might look something like:

```obj-c
- (id)initNewUserInContext:(NSManagedObjectContext *)context {

    self = [super initWithEntityName:@"User" insertIntoManagedObjectContext:context];
     
    if (self) {
        // assign local variables, etc.
    }
     
    return self;
}
```

You'll then create new user objects by creating managed object instances of your entity subclass:

```obj-c
NSManagedObjectContext *context = [[[SMClient defaultClient] coreDataStore] contextForCurrentThread];
User *newUser = [[User alloc] initNewUserInContext:context];
[newUser setUsername:@"Bob"];
[newUser setPassword:@"vkfjakvjvjnfosg"];
[newUser setAge:[NSNumber numberWithInt:34]];

[context saveOnSuccess:^{
	// Saved the user object
} onFailure:^(NSError *error){
	// Error
}];
```

The above example assumes an entity `User` with attributes `username` and `age`. The `setPassword` method is provided by the `SMUserManagedObject` class to securly set a password for your user which is persisted to StackMob but never saved locally on the device. 

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMUserManagedObject.html" target="_blank">SMUserManagedObject Class Reference</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Create-a-User-Object" target="_blank">Create a User Object Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Login -->

## Logging in to StackMob

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
        <li><a href="https://developer.stackmob.com/tutorials/ios/User-Authentication" target="_blank">User Authentication Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>


<!--- Edit User object -->


## Retrieve User Managed Object

The next time the user logs in to your app, you may need to have a copy of the user object around to make updates. To do this, simply create a fetch request and specify a predicate such that the only result should be your logged in user:

```obj-c
// Pull username from login screen text field, for example
NSString *username = self.textField.text;

NSFetchRequest *userFetch = [[NSFetchRequest alloc] initWithEntityName:@"User"];
[userFetch setPredicate:[NSPredicate predicateWithFormat:@"username == %@", username]];

[self.manangedObjectContext executeFetchRequest:userFetch onSuccess:^(NSArray *results){
	if ([results count] != 1) { 
    // There should only be one result 
  };

	User *currentUser = (User *)[results objectAtIndex:0];
	// Edit user object or store ID to pass around

}];
```

<p class="alert">Managed Objects are not thread safe. Pass object IDs between threads and blocks.</p>

<!--- Get logged in user -->

## Retrieve Logged In User Info

At any point in time you can send a request to StackMob to recieve a dictionary representation for the currently logged in user:

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

## Check Status

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
        <li><a href="https://developer.stackmob.com/tutorials/ios/User-Authentication" target="_blank">User Authentication Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>


<!--- Auto Refresh -->

## Automatic Login Refresh

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

## Logout of StackMob

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
        <li><a href="https://developer.stackmob.com/tutorials/ios/User-Authentication" target="_blank">User Authentication Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Reset -->

## Reset Password

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

## Forgot Password

Developers can easily implement a forgot password workflow in their application. To do this, your user schema will need to have a field dedicated to users' email addresses, which can be the username field if your users sign in with their emails, for example. Choose the correct field which maps to emails in the **Forgot Password** section on the user schema configuration page. Once you have done this, a basic example workflow might look like the following:

1. Have a button on the login screen which maps to a forgot password method.
2. When the user presses the button they are propmpted to enter their username.
3. This should trigger the `SMClient` `sendForgotPaswordEmailForUser:onSuccess:onFailure:` method. The user wull receive an email with a temporary password.
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
	GEO-LOCATION
	//////////////////
-->

# Geolocation

StackMob provides GeoPoint field types which allow you to save objects with latitude and longitude coordinates. Once you have objects that contain geo location data, you can query for a subset of those objects near a particular coordinate pair, within a specific radius, or even within a box formed from coordinates. This makes building apps that show all restaurants around a user's current location, or tracking a user's path as they jog through the city, as simple as ever.

<!--- Add schema field -->

## Add a GeoPoint Schema Field

<p class="alert">Geopoint field inference was introduced in iOS SDK v2.0.0+.</p>

In order for a schema to allow for geopoints, it must have a field with the GeoPoint type. To manually add a geopoint field to your schema, follow the <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-GeoPoint-Field-to-Schemas" target="_blank">Adding a GeoPoint Field To Schemas</a> tutorial to get set up.

<!--- Track Geo-Location -->

## Start Tracking Geo-Location

`SMLocationManager` is a built-in `CLLocationManager` singleton for use in retrieving `CLLocationCoordinate2D` points. 
 
Many apps make use of geo location data; `SMLocationManager` aides in this process by eliminating the boilerplate code needed to build a `CLLocationManager` singleton.

<!--- SUB: Using SMLocationManager -->

### Using SMLocationManager

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

### Obtaining Latitude/Longitude Values

If you wish to pull the latitude and longitude data manually from the location manager, do so with the following:

```obj-c
NSNumber *latitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].latitude];
NSNumber *longitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].longitude];
```

Often times it may take a few run loops for the location manager to start receiving actual data after <i>startUpdatingLocation</i> is called.  You may need to implement the necessary logic to ensure you are not pulling nil data.

<!--- SUB: Subclass SMLocationManager -->

### Subclassing SMLocationManager

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

## Saving Geo-Location Data

GeoPoints are stored in Core Data using the `Transformable` attribute type. 

The `SMGeoPoint` class provides a simple interface around a geopoint. It inherits from `NSDictionary` and has two instance methods: `latitude` and `longitude`.

Use the provided class method to obtain the current location data of the device:

```obj-c
[SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
     
    // Do something with geoPoint.latitude and geoPoint.longitude
     
} onFailure:^(NSError *error) {

    // Error
}];
```

To save an `SMGeoPoint` in Core Data, it must be archived into `NSData`:

```obj-c
NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];

SMGeoPoint *location = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];

NSData *data = [NSKeyedArchiver archivedDataWithRootObject:location];
```

 Since `getGeoPointForCurrentLocationOnSuccess:onFailure:` gives us back an instance of `SMGeoPoint`, we can easily save objects with geo data like so:

```obj-c
[SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
     
  Todo *todo = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
  todo.todoId = [todo assignObjectId];
  todo.title = @"My Location";

  todo.location = [NSKeyedArchiver archivedDataWithRootObject:geoPoint];
   
  // Save the location to StackMob
  NSManagedObjectContext *context = [[[SMClient defaultClient] coreDataStore] contextForCurrentThread];
  [context saveOnSuccess:^{
     NSLog(@"Created new object in Todo schema");
  } onFailure:^(NSError *error) {
      NSLog(@"Error creating object: %@", error);
  }];
     
} onFailure:^(NSError *error) {
  NSLog(@"Error getting SMGeoPoint: %@", error);
}];
```

<p class="alert">Geo Location in iOS relies on the <b>MapKit</b> framework.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMGeoPoint.html" target="_blank">SMGeoPoint Class Reference</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data" target="_blank">Saving Geo Location Data</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Read Geo-Location -->

## Reading Geo-Location Values

Because managed object geopoint data will be contained in a `Tranformable` attribute type, it must be unarchived to be properly read:

```obj-c
NSData *data = [managedObject objectForKey:@"location"];

SMGeoPoint *geoPoint = [NSKeyedUnarchiver unarchiveObjectWithData:data];
```

<!--- Query Geo-Location -->

## Query based on Geo-Location

To query using `SMGeoPoint`, use the special predicate methods provided by the `SMPredicate` class:

```obj-c
NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"EntityName"];

// Fisherman's Wharf
CLLocationCoordinate2D coordinate;
coordinate.latitude = 37.810317;
coordinate.longitude = -122.418167;

SMGeoPoint *geoPoint = [SMGeoPoint geoPointWithCoordinate:coordinate];
SMPredicate *predicate = [SMPredicate predicateWhere:@"geopoint" isWithin:3.5 milesOfGeoPoint:geoPoint];
[fetchRequest setPredicate:predicate];

// Execute fetch request
[self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
    
    // Once you've made a fetch request, make sure to unarchive the NSData

    NSManagedObject *object = [results objectAtIndex:0];
    NSData *data = [object objectForKey:@"location"];
    SMGeoPoint *geoPoint = [NSKeyedUnarchiver unarchiveObjectWithData:data];

} onFailure:^(NSError *error) {

        NSLog(@"Error: %@", error);
}];
```

The `SMPredicate` class provides method to query based on range in miles or kilometers, as well as bounds by SW and NE corners, or even just near a provided point.

All <code>SMPredicate</code> methods to query based on an instance of <code>SMGeoPoint</code> have an equivalent method to query based on an instance of <code>CLLocationCoordinate2D</code>.

<p class="alert">Fetching from the cache using <code>SMPredicate</code> is not supported, and will return an empty array of results. Similarly, when a fetch is performed from the network (StackMob), any results are not cached.</br></br>Fetched Results Controllers that use fetch requests with conditions around geo-location is not supported. Use refresh controls instead.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMPredicate.html" target="_blank">SMPredicate Class Reference</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Querying-Geo-Location-Data" target="_blank">Querying Geo Location Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Geo-Location through DS API -->

## Geo-Location using the Datastore API

<!--- SUB: Saving -->

### Saving

You can make an `SMGeoPoint` with a latitude and a longitude:

```obj-c
NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];

SMGeoPoint *location = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];
```
 
Alternatively, you can use a `CLLocationCoordinate2D` coordinate:

```obj-c
CLLocationCoordinate2D renoCoordinate = CLLocationCoordinate2DMake(39.537940, -119.783936);

SMGeoPoint *reno = [SMGeoPoint geoPointWithCoordinate:renoCoordinate];
```

To save an SMGeoPoint, store it in a dictionary of arguments to be uploaded to StackMob:

```obj-c
CLLocationCoordinate2D renoCoordinate = CLLocationCoordinate2DMake(39.537940, -119.783936);
  
SMGeoPoint *location = [SMGeoPoint geoPointWithCoordinate:renoCoordinate];
 
NSDictionary *arguments = [NSDictionary dictionaryWithObjectsAndKeys:@"My Location", @"name", location, @"location", nil];
 
[[[SMClient defaultClient] dataStore] createObject:arguments inSchema:@"todo" onSuccess:^(NSDictionary *theObject, NSString *schema) {
    NSLog(@"Created object %@ in schema %@", theObject, schema);
 
} onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
    NSLog(@"Error creating object: %@", theError);
}];
```

<!--- SUB: Reading -->

### Reading

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
  // Error
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

# Binary Data

Binary data works by linking StackMob to your personal Amazon S3 account. You'll then declare a Binary type field in your schema and all files, images, etc. that you save will actually be stored in your S3 library, and the value of the field will be the URL that points to that data.

<!--- Add schema field -->

## Add a Binary Schema Field

<p class="alert">Binary field inference was introduced in iOS SDK v2.0.0+. This means that, while you still need to maunally configure the S3 module settings if you haven't already, you will not need to manually add a Binary field to your schema.</p>

Follow the <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-Binary-Field-to-Schemas" target="_blank">Adding a Binary Field To Schemas</a> tutorial to get the S3 module and a Binary field in your schema set up.

## Saving Binary Data

To save Binary data to StackMob, first create an attribute in your entity of type `String`. This attribute should map to a field on StackMob of type `Binary`.

Next, you will use the `SMBinaryDataConversion` class to convert instances of `NSData` into strings ready to be saved to StackMob.

`SMBinaryDataConversion` offers a class method `stringForBinaryData:name:contentType:` to decode binary data into a string. This is then used to send to StackMob as the value for a field with type `Binary`. The contents of the string will be parsed and the content will be stored on S3. StackMob will then store the url as the value. A call to `refreshObject:mergeChanges:` on the managed object context will update the in-memory value to the url in the persistent store.

Here's an example of converting a picture stored in the app's bundle:

```obj-c
NSBundle *bundle = [NSBundle bundleForClass:[self class]];
NSString* pathToImageFile = [bundle pathForResource:@"coolPic" ofType:@"jpg"];
NSData *theData = [NSData dataWithContentsOfFile:pathToImageFile];

NSString *picData = [SMBinaryDataConversion stringForBinaryData:theData name:@"whateverNameYouWant" contentType:@"image/jpg"];
```

Now set the string value to your managed object, save the managed object context, and refresh your in-memory copy of the object to grab the url pointing to your data:

```obj-c
[newManagedObject setValue:picData forKey:@"pic"];

NSManagedObjectContext *context = [[[SMClient defaultClient] coreDataStore] contextForCurrentThread];
[context saveOnSuccess:^{
  [context refreshObject:newManagedObject mergeChanges:YES];
} onFailure:^(NSError *error) {
  // Error
}];
```

`[newManagedObject valueForKey:@"pic"]` now returns the S3 url for the binary data.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMBinaryDataConversion.html" target="_blank">SMBinaryDataConversion Class Reference</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Upload-to-S3" target="_blank">Upload To S3 Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

## Working Offline

When you save an object with binary data while the device is offline, the value of the attribute will contain a data representation, ready to be saved to StackMob. In order to properly read it at that point, the data must be extracted from the string and decoded.

In order to check if the attribute value is a url or not, use the `stringContainsURL:` method. To convert the string value back to data, use the `dataForString:` method:

```obj-c
NSString *picString = [newManagedObject valueForKey:@"pic"];
if ([SMBinaryDataConversion stringContainsURL:picString]) {
  NSURL *urlForPic = [NSURL URLForString:picString];
  // Set image from url
} else {
  UIImage *image = [UIImage imageWithData:[SMBinaryDataConversion dataForString:picString]];
  // Set image directly
}
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

## Binary Data using the Datastore API

When saving objects using the datastore API that include binary data, you must format the value of the field correctly so that it can be parsed by Amazon. The correct value format should look like the following:

`"Content-Type: REPLACE_WITH_CONTENT_TYPE\nContent-Disposition: attachment; filename=REPLACE_WITH_FILE_NAME_NO_PATH_NEEDED\nContent-Transfer-Encoding: base64\n\nBASE_64_ENCODED_STRING_OF_THE_BYTE_ARRAY"`

Here's a real world example of saving a comment with a photo:

```obj-c
NSString *picData = @"Content-Type: image/png\nContent-Disposition: attachment; filename=profile_pic.png\nContent-Transfer-Encoding: base64\n\niVBORw0KGgoAAAANSUhEUgAAA.....";

NSDictionary *objectToCreate = [NSDictionary dictionaryWithObjectsAndKeys:@"1234", @"comment_id", picData, @"photo", nil];

[[[SMClient defaultClient] dataStore] createObject:objectToCreate inSchema:@"Comment" onSuccess:^(NSDictionary *theObject, NSString *schema) {
  // Saved object
} onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
  // Error
}];
```

<!---
	///////////////////
	SOCIAL INTEGRATION
	//////////////////
-->

# Social Integration

StackMob provides integrations with Facebook, Twitter and Gigya to allow users to login to StackMob using pre-existing credentials. The Facebook and Twitter integration also feature interfaces to post status updates. 

<!--- Facebook -->

## Facebook

<!--- SUB: FB Login Workflow -->

### Implement Facebook Login Workflow

You will need to download the Facebook SDK and follow their tutorials to get login working in your application. Once you have a Facebook session open you will direct the application in the `sessionStateChanged:state:error` Facebook method to login to StackMob using the provided Facebook access token.

All the implementation details for Facebook's SDK can be found in our <a href="https://developer.stackmob.com/tutorials/ios/Integrating-with-Facebook" target="_blank">Facebook Tutorial</a>.

<!--- SUB: Login with Facebook -->

### StackMob Login Using Facebook

Logging into StackMob using Facebook credentials requires that a StackMob user object already exists and is linked to the user's Facebook access token. Fortunately, you can do all of that with one method.

Assuming you have opened a Facebook session and have initiated a custom method from the Facebook `sessionStateChanged:state:error` method, you would login to StackMob like so:

```obj-c,3-13
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
}];
```

<p class="alert">By wrapping the login call in the `requestForMe` block, we can assign the Facebook user's username as the primary key of the StackMob user object. This is completely optional, and if nil is passed to the <code>usernameForCreate</code> parameter the Facebook user's ID is used.</p> 

This login method will create a StackMob user object and link them to the provided Facebook token if one does not already exist.

Alternatively, you can implement a more manual workflow using the methods listed in LINK TO CREATE A STACKMOB USER and LINK/UNLINK FB TOKEN.

<!--- SUB: Create User -->

### Create a User Object

You can create a new StackMob user object and link a Facebook access token for future login using Facebook credentials:

```obj-c
[[SMClient defaultClient] createUserWithFacebook:activeSession.accessTokenData.accessToken onSuccess:^(NSDictionary *result) {
  // New user created
} onFailure:^(NSError *error) {
  // Error
}];
```

### Link/Unlink Facebook Tokens

If you have an existing StackMob user object and you want to link that object with a Facebook token to enable future login using Facebook credentials, use the link token method:

<p class="alert">The user must be logged in for the link to work.</p>

```obj-c
[[SMClient defaultClient] linkLoggedInUserWithFacebookToken:activeSession.aceessTokenData.accessToken onSuccess:^(NSDictionary *result) {
  // Succssful link
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

### Update Facebook Status



### Get Facebook User Info

<!--- Twitter -->

## Twitter

### Implement Twitter Login Workflow

You will need to download the Twitter SDK and follow their tutorials to get login working in your application. Once you have a Twitter session open you will direct the application to login to StackMob using the provided Twitter access token.

All the implementation details on Twitter's end can be found in our <a href="https://developer.stackmob.com/tutorials/ios/Integrating-with-Twitter" target="_blank">Twitter Tutorial</a>.

<!--- Gigya -->

## Gigya

<!---
  ///////////////////
  CUSTOM CODE
  //////////////////
-->

# Custom Code

## Write Custom Code

LINK TO CUSTOM CODE DOCS

## Performing Custom Code Requests

## Setting Request Headers

## Setting Response Content Types

<!---
  ///////////////////
  PUSH
  //////////////////
-->


# Push Notifications

## The Push Module

## Initalizing a Push Client

## Registering the Device

## Broadcasting Messages

## Sending Messages to Users

## Sending Messages to Tokens

## Getting Tokens For Users

## Deleting Tokens

<!---
  ///////////////////
  OFFLINE SYNC
  //////////////////
-->

# Offline Sync

Included with version 2.0.0+ of the SDK is a sync system built in to the Core Data Integration to allow for local saving and fetching of objects when a device is offline. When back online, modified data will be synced with the server. Many settings are available to the developer around cache and merge policies, conflict resolution, etc. 

Read through the <a href="https://developer.stackmob.com/ios-sdk/offline-sync-guide" target="_blank">Offline Sync Guide</a> for all information.

<!---
  ///////////////////
  NETWORK REACHABILITY
  //////////////////
-->

# Checking Network Reachability

<!--- SMNetworkReachability -->

## Using the SMNNetworkReachability Class

`SMNetworkReachability` provides an interface to monitor the network reachability from the device to StackMob.
 
 Network reachability checks are already built into the SDK.  When the network is not reachable and a request is made, an error with domain `SMError` and status code -105 (SMNetworkNotReachable) will be returned.
 
 An instance of `SMNetworkReachability` is created during the initialization of an `SMUserSession`, accessible by the `networkMonitor` property.

 A shorthand is available through the `SMClient` instance:

 ```obj-c
 client.networkMonitor
 ```
 
 ## Checking the Current Network Status
 
 To manually check the current network status, use the <currentNetworkStatus> method:

 ```obj-c
 SMNetworkStatus currentStatus = [self.client.networkMonirot currentNetworkStatus];
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

## Registering For Network Status Change Notifications

You can register to receive notifications when the network status changes by simply adding an observer for the notification name `SMNetworkStatusDidChangeNotification`.  The notification will have a `userInfo` dictionary containing one entry with key `SMCurrentNetworkStatusKey` and `NSNumber` representing the `SMNetworkStatus` value.

In order to access the value of `SMCurrentNetworkStatusKey` in a format for comparing to specific states or use in a switch statement, retrieve the intValue like this:

```obj-c
if ([[[notification userInfo] objectForKey:SMCurrentNetworkStatusKey] intValue] == SMNetworkStatusReachable) {
    // do Reachable stuff
}
```

<p class="alert">Remember to remove your notification observer before the application terminates.</p>
 
 
## Executing a Block Whenever the Network Status Changes ## 

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

Alternatively you can use the `setNetworkStatusChangeBlockWithCachePolicyReturn:` method, which requires you to return a cache policy to set. Here's an example which sets points fetches to either the cache or the network based on the current network status:

```obj-c
[self.client.networkMonitor setNetworkStatusChangeBlockWithCachePolicyReturn:^(SMNetworkStatus status){
    
    if (status == SMNetworkStatusReachable) {
      return SMCachePolicyTryNetworkOnly;
    } else {
      return SMCachePolicyTryCacheOnly;
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
  ERROR CODES
  //////////////////
-->

# Error Handling

When errors occur at the SDK level, specific error codes are returned to indicate the type of error that occured.

## Core Data Save Failure Errors

When a Core Data save fails because objects could not be inserted/updated/deleted on StackMob, the error returned is formatted in the following way:

<ul>
  <li>The error code will be -108, <b>SMErrorCoreDataSave</b></li>
  <li>The error userInfo property may contain any of the following keys, which are constants you can reference, depending on what the save request consisted of: <b>SMInsertedObjectFailures</b>, <b>SMUpdatedObjectFailures</b>, <b>SMDeletedObjectFailures</b>.</li>
  <li>If any of those keys exist, its value will be an NSArray instance of NSDictionary instances, one dictionary for each failed object.</li>
  <li>Each dictionary will include 2 key constants: <b>SMFailedManagedObjectID</b>, whose value is an NSManagedObjectID instance of the managed object that was not saved properly, and <b>SMFailedManagedObjectError</b>, whose value is an NSError instance of what caused the object to not get saved properly.</li>
</ul>

Here's an example of an error returned when trying to save a user object with primary key "Jack" when a user named "Jack" already exists on the server:

```bash
Error Domain=SMError Code=-108 "The operation couldn’t be completed. (SMError error -108.)" UserInfo=0xc99d030 {
  SMInsertedObjectFailures=(
    {
      SMFailedManagedObjectError = "Error Domain=HTTP Code=409 \"The operation couldn\U2019t be completed. 
          (HTTP error 409.)\" UserInfo=0x8661680 {error=Duplicate key for schema user: \"jack\"}";
      SMFailedManagedObjectID = "0xc9a0210 <x-coredata://FDCF65C5-02D3-45E9-BA03-793E1A125FCE-12897-00000C7A7DD73EAA/User/pjack>";
    }
  )
}
```

## StackMob Specific Errors

<table cellpadding="8px" width="600px">
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

## General HTTP Errors

<table cellpadding="8px" width="600px">
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

## String Constants

The following is a list of string contants used for the domains of errors and names of exceptions.

<table cellpadding="8px" width="600px">
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

# Debugging

The iOS SDK gives developers access to two global variables that will enable additional logging statements when using the Core Data integration:

* **SM_CORE_DATA_DEBUG** - In your AppDelegate's `application:DidFinishLaunchingWithOptions:` method, include the line `SM_CORE_DATA_DEBUG = YES;` to turn on log statements from `SMIncrementalStore`. This will provide information about the Datastore calls to StackMob happening behind the scenes during Core Data saves and fetches. The default is `NO`.
* **SM_MAX_LOG_LENGTH** - Used to control how many characters are printed when logging objects. The default is **10,000**, which is plenty, so you will almost never have to set this.  The only time you will see the string representation of an object truncated is when you have an Attribute of type String that maps to a field of type Binary on StackMob, because you are sending a string containing the binary of the image, etc. String representations of objects that have been truncated end with MAX\_LOG\_LENGTH\_REACHED.

<!---
  ///////////////////
  DEPRECATIONS
  //////////////////
-->

# SDK Deprecations

As we improve the SDK, sometimes that means we need to deprecate methods or properties.

## v2.0.0

* <b>(SMCoreDataStore)</b> <i>setDefaultMergePolicy:applyToMainThreadContextAndParent:</i>, Use <i>setDefault<b>CoreData</b>MergePolicy:applyToMainThreadContextAndParent:</i>
* <b>(SMNetworkReachability)</b> <i>SMNetworkStatus</i> options, Use <b>SMNetworkStatusUnknown</b>, <b>SMNetworkStatusNotReachable</b>, <b>SMNetworkStatusReachable</b>

## v1.4.0

* <b>(SMClient)</b> <i>loginWithFacebookToken:options:onSuccess:onFailure:</i>, Use <i>loginWithFacebookToken:<b>createUserIfNeeded:</b>options:<b>successCallbackQueue:failureCallbackQueue:</b>onSuccess:onFailure:</i>
* <b>(SMClient)</b> <i>loginWithTwitterToken:twitterSecret:options:onSuccess:onFailure:</i> , Use <i>loginWithTwitterToken:twitterSecret:<b>createUserIfNeeded:</b>options:<b>successCallbackQueue:failureCallbackQueue:</b>onSuccess:onFailure:</i>
* <b>(SMClient)</b> <i>loginWithGigyaUID:uidSignature:signatureTimestamp:options:onSuccess:onFailure:</i>, Use <i>loginWithGigyaUID:uidSignature:signatureTimestamp:options:<b>successCallbackQueue:failureCallbackQueue:</b>onSuccess:onFailure:</i>

## v1.2.0

* <b>(SMCoreDataStore)</b> <i>managedObjectContext</i> property, Use <i>contextForCurrentThread</i>
