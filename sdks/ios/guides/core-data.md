StackMob and Core Data Integration Guide
=====================================

## Overview

Core Data provides a powerful and robust object graph management system that otherwise would be a nightmare to implement. Although it may have a reputation for being complex, the basics are easy to grasp and understand.

The iOS SDK offers a persistence layer tightly integrated with Core Data, allowing developers to use native Core Data code to make requests to StackMob's remote database. We've even taken it a step further by adding asynchronous save and fetch methods, caching and offline syncing systems, and more.

<br/>
<br/>
<b>The contents of this guide use the Core Data integration for persistence. We understand that using Core Data for persistence might be too extensive for some applications. All equivalent operations using the Datastore API, a simple wrapper on the REST API, can be found in the <a href="https://developer.stackmob.com/ios-sdk/developer-guide" target="_blank">Developer Guide</a></b>.

<br/>
<br/>
In each section of this guide you may see colored boxes which are meant to highlight important information:

<p class="alert">Gold boxes call out warnings, gotchas, and information we don't want you to miss.</p>

<p class="alert alert-info">Blue boxes contain links to sections in the full API reference, as well as full working projects for you to download and in-depth tutorials for you to read through.</p>

## The Basics

If you are new to Core Data check out our list of <a href="#CoreDataReferences">Core Data References</a>.

The three main pieces of Core Data are instances of:

* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html" target="_blank">NSManagedObjectContext</a> - This is what you use to create, read, update and delete objects in your database.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSPersistentStoreCoordinator_Class/NSPersistentStoreCoordinator.html" target="_blank">NSPersistentStoreCoordinator</a> - Coordinates between the managed object context and the actual database, in this case StackMob.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectModel_Class/Reference/Reference.html" target="_blank">NSManagedObjectModel</a> - References a file where you defined your object graph.

<!---
    ///////////////////
    SETUP
    //////////////////
-->

## Setup

<!--- DEF MOM -->

### Defining a Managed Object Model

To use Core Data you will define an `NSManagedObjectModel` instance which points to your local object graph, a `.xcdatamodeld` file. This file defines a local representation of your database, using Entities which have attributes and relationships.

Your Core Data model should replicate your StackMob schemas. The <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#SupportSpecifications" target="_blank">Support Specifications</a> section of the Core Data Guide details the mappings between StackMob field types and Core Data attribute types, ensuring proper translations to and from Core Data.

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

<p class="alert">If you are using the cache, make sure you include the <b>createddate</b> and <b>lastmoddate</b> attributes as <b>Date</b> types in your entities. We also recommend not inheriting from a parent entity to do so, as it drastically affects cache performance.<br/><br/>Also, if there are fields on StackMob you don't plan on interacting with through the client, don't include them in your model.</p>

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

### Creating `NSManagedObject` Subclasses

Creating classes for each one of your entities provides a lot of convenience around constructors, accessors, validation, formatting, etc. for when you work with managed objects. We recommend you create these custom classes, which you can do by selecting an Entity in the Model Editor and going to `Editor -> Create NSManagedObject Subclass`:

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

### Initialize a Core Data Store

You will interact with the Core Data persistence layer by obtaining an instance of `SMCoreDataStore` via your `SMClient` object.

A Core Data store instance gives you everything you need to work with StackMob's Core Data integration. With your `SMCoreDataStore` object you can retrieve a managed object context configured with a `SMIncrementalStore` as its persistent store to allow communication to StackMob from Core Data.  

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

### Obtaining a Managed Object Context

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

## Coding Practices

The following are coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data.  This allows StackMob to seamlessly translate to and from the language that Core Data speaks.

### Table Mapping

This is a table of how Core Data, StackMob and regular databases map to each other:
<table class="table table-bordered table-pretty-header">
	<tr align="center">
		<th>Core Data</th>
		<th>StackMob</th>
		<th>Database</th>
	</tr>
	<tr>
		<td>Entity</td>
		<td>Schema</td>
		<td>Table</td>
	</tr>
	<tr>
		<td>Attribute</td>
		<td>Field</td>
		<td>Column</td>
	</tr>
	<tr>
		<td>Relationship</td>
		<td>Relationship</td>
		<td>Reference Column</td>
	</tr>
</table>

### Naming Conventions

#### Entity Names

Core Data entities are encouraged to start with a capital letter and will translate to all lowercase on StackMob. Example: **Superpower** entity on Core Data translates to **superpower** schema on StackMob.

#### Property Names

Core Data attribute and relationship names are encouraged to be in camelCase, but can also be in StackMob form, all lowercase with optional underscores. Acceptable formats are therefore **yearBorn**, **year_born**, or **yearborn**. All camelCased names will be converted to and from their equivalent form on StackMob, i.e. the property yearBorn will appear as year\_born on StackMob.

#### StackMob Schema Primary Keys

All StackMob schemas have a primary key field that is always **schemaName\_id**, unless the schema is a user object, in which case it defaults to "username" but can be changed manually by setting the `userPrimaryKeyField` property in your `SMClient` instance.

### Best Practices

#### Include Entity Primary Keys

Following the section above on primary keys, each Core Data entity must include an attribute of type string that maps to the primary key field on StackMob. Acceptable formats are <b><i>schemaName</i>Id</b> or <b><i>schemaName</i>_id</b>. 

If the managed object subclass for the Entity inherits from `SMUserManagedObject`, meaning it is intended to define user objects, you may use either of the above formats or whatever lowercase string with optional underscores matches the primary key field on StackMob. 

For example: entity **Soda** should have attribute **sodaId** or **soda_id**, whereas your **User** entity primary key field defaults to **@"username"**.

#### Index Primary Keys

<p class="alert">Indexing primary keys increases performance only when using the cache.</p>

You can get a performance boost from the cache if you index the attributes that map to StackMob schema primary key fields. This is because the Core Data integration fetches objects from the cache using predicates on those fields. You can set an attribute as Indexed by clicking on it in the data model, then clicking the **Indexed** box in the **Data Model Inspector**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/coredata/indexedAttribute.png" />

#### Assign IDs

When inserting new objects into your managed object context, you must assign an id value to the attribute which maps to the StackMob primary key field BEFORE you make save the context. 90% of the time you can get away with assigning ids like this:


```obj-c
// assuming your instance is called newManagedObject
[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];

// now you can save your context
```


The other 10% of the time is when you want to assign your own ids that aren't unique strings based on a UUID algorithm. A great example of this is user objects, where you would probably assign the user's name to the primary key field.  In that case, your code might look more like this:


```obj-c
// assuming your instance is called newManagedObject
[newManagedObject setValue:@"bob" forKey:[newManagedObject primaryKeyField]];

// now you can save your context
```

#### Use NSManagedObjectContext+Concurrency Category

The <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency category</a> is where all the StackMob async and sync versions of the <i>save:</i>, <i>executeFetchRequest:error:</i>, and <i>countForFetchRequest:error:</i> can be found. They should be only methods you use to perform save and fetch operations.


#### NSManagedObject Subclasses

Creating an NSManagedObject subclass for each of your entities is highly recommended for convenience. You can add an init method to each subclass and include the ID assignment line from above - then you don't have to remember to do it each time you create a new object!

#### SMUserManagedObject Subclasses

After creating an NSManagedObject subclass for an entity that maps to a user object on StackMob, change the inherited class to SMUserManagedObject.  This class will give you a method to securely set a password for the user object, without directly setting any attributes in Core Data.  It is important to make sure you initialize an SMUserManagedObject instance properly.

#### Define Relationship Inverses

Although it is not strictly required, defining inverse relationships will allow Core Data to maintain referential integrity, handle delete rules. We have seen developers run into issues before that were easily solvable by ensuring all relationships have inverses. 

#### Create Before Relate

Before saving updated objects and depending on the merge policy, Core Data will grab persistent values from the server to compare against.  Problems arise when a relationship is updated with an object that hasn't been saved on the server yet.  To play it safe, try to create and save objects before relating them to one another. 

#### Working With NSDate Attributes

As of v1.4.0, NSDate attribute values are serialized to the StackMob server as integers in ms.  Declare the fields on StackMob as Integer.  By keeping consistency with the way the auto-generated **createddate** and **lastmoddate** fields are stored (ms), NSDate attributes will be deserialized correctly.

If you want to check if one date is equal to another, use the **timeIntervalSinceDate:** method.  Dates being equal to the second is equivalent to the time interval being less than 1.  Methods like <b>isEqualToDate:</b> track sub-second differences between dates, which are not present in the serialized integers.  Here's an example of how to check if two dates are equal:


```obj-c
NSDate *date1 = [object1 valueForKey:@"date"];
NSDate *date2 = [object2 valueForKey:@"date"];
if ([date1 timeIntervalSinceDate:date2] < 1) {
	// The dates are equal.
}
```

Feel free to try other ways of comparing dates to find the method that works best for your app's needs.

<!---
    ///////////////////
    DATASTORE
    //////////////////
-->

## Datastore

The following sections include everything you need to know about creating, reading, updating and deleting objects. When you use Core Data you'll perform write operations on a managed object context, sort of like an in-memory scratch pad, and persist the changes by performing a save operation.

<!--- INSERT OBJECT -->

### Creating an Object

Typically you will create an `NSManagedObject` subclass for each of your Core Data entities, giving you convenience methods, accessors, etc. You might also find it valuable to define a custom init method. The easiest way to declare a new instance of an entity and fill in its values is like so:

```obj-c
Todo *newTodo = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];

// Assumes Todo has a title attribute and an attribute which maps to the primary key on StackMob.
[newManagedObject assignObjectId];
[newTodo setTitle:@"Take out the trash"];
```

The `assignObjectId` method is provided by the SDK to assign an arbitrary ID to the attribute which maps to the primary key field on StackMob. Object IDs must be assigned to managed objects before they are persisted. This keeps Core Data IDs and StackMob objects in sync. You are free to assign your own IDs if you wish, `assignObjectId` is simply a convenience method which masks a random UUID generation algorithm.

<b>Without Subclasses</b>

Instantiating and assigning values to a generic managed object requires using key-value methods in accordance with key value coding.

```obj-c
NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
[newManagedObject setValue:@"Take out the trash" forKey:@"title"];
[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
```

The `primaryKeyField` method is provided by the SDK for convenience to return the attribute which maps to the primary key field on StackMob. The SDK knows which attribute is the primary key field because of its format. The format is defined in the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#EntityPrimaryKeys" target="_blank">Entity Primary Keys</a> section of the Core Data Guide. 

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

### Reading Objects

To read objects from a particular Entity, create and execute a Core Data fetch request using methods from the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency</a> class.

For a list of StackMob supported `NSFetchRequest` methods, see the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#FetchRequests" target="_bank">Fetch Requests</a> section of the Core Data Guide.

<p class="alert">By default, objects are returned as faults, and attribute values are not accessed from the persistent store until specifically called upon in your code.  This ensures in-memory usage is as low as possible.</p>

All information on specifying conditions for a query can be found in the <a href="#Queries">Queries</a> section. 

<!--- SUB: ASYNC READ -->

#### Asynchronous Fetch Requests

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
        <li><a href="https://developer.stackmob.com/ios-sdk/read-into-table-view-tutorial" target="_blank">Read Into Tableview Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- SUB: SYNC READ -->

#### Synchronous Fetch Requests

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

### Updating an Object

After fetching an existing managed object, update it by simply changing the object's values. The updated values are not persisted until the managed object context is saved.

```obj-c
NSManagedObject *todoObject = [fetchRequestResults lastObject];

[todoObject setValue:@"New Title" forKey:@"title"];

[self.managedObjectContext saveOnSuccess:^{
  // Object updated
} onFailure:^(NSError *error) {
  // Handle error
}];
```

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

### Deleting an Object

Delete an object by simply calling the managed object context `deleteObject:` method and passing the manage object you wish to delete. The delete will not be finalized until you save the managed object context.

```obj-c
[self.managedObjectContext deleteObject:aManagedObject];
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

### Performing Saves

At some point after inserting, updating, and deleting objects from a managed object context, you will initiate a save which will cause all modified objects to be persisted to the external store.  The external store in this case is StackMob as well as a local cache database, ensuring data is available while the devices is offline.

The StackMob SDK has placed a wrapper around the familiar `save:` core data method to offer synchronous and asynchronous versions for improved performance. All methods can be found in the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency</a> class.

<!--- ASYNC SAVES -->

#### Asynchronous Saves

Asynchronous saves are great for executing a save off the main thread, ensuring the UI is not blocked. If other events rely on the successful save, place them in the callbacks.

```obj-c
[self.managedObjectContext saveOnSuccess:^{
    NSLog(@"You created a new object!");
} onFailure:^(NSError *error) {
    NSLog(@"There was an error! %@", error);
}];
```

Use the callbacks to update the UI, notify other dependent files, etc.

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

#### Synchronous Saves

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

### Per Request Options

Each type of method (asynchronous/synchronous save/fetch) has an overloaded method declaration with a parameter that takes an instance of <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html" target="_blank">SMRequestOptions</a>. The parameter name in all methods is called options. This allows you to provide a custom `SMRequestOptions` instance that will be applied to all calls in that request. For example, provide a `SMRequestOptions` instance with the `isSecure` property set to YES if you wanted a specific save request to run over SSL i.e. all inserts/updates/deletes for that request will be sent over SSL.

<p class="alert">
Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests. The following options are currently safe to set and will override the default for the duration of the request:</br></br>
&nbsp;&nbsp;&bull;&nbsp;&nbsp;isSecure property (HTTPS)
&nbsp;&nbsp;&bull;&nbsp;&nbsp;savePolicy property (for saves only)
&nbsp;&nbsp;&bull;&nbsp;&nbsp;fetchPolicy property (for fetches only)
&nbsp;&nbsp;&bull;&nbsp;&nbsp;cacheResults property (for saves/fetches)
</br></br>
Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
</p>


<!---
    ///////////////////
    QUERIES
    //////////////////
-->

## Queries

While you will perform fetch requests to do all reading in Core Data, predicates enable you to return a subset of a schema's objects by placing conditions on the read, for example retrieving todos from today, or friends with birthdays this week. The Core Data fetch requests implementation is built on top of the datastore query API. 

<!--- Predicates -->

### Predicates

In Core Data, every read begins by creating a fetch request. You can then add predicates (conditions), sort descriptors, offsets (ranges), etc. to return a more granular subset of results. To complete the read you execute the fetch request on a managed object context.

See the <a href="#ReadingObjects">Reading Objects</a> section for information on creating/executing a fetch request.

To learn more about fetch requests in general visit Apple's documentation on <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Conceptual/CoreData/Articles/cdFetching.html" target="_blank">Fetching Managed Objects</a>.

For a list of StackMob supported predicate types, visit the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#Predicates" target="_blank">Predicates</a> section of the Core Data Guide.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/basic-queries-tutorial" target="_blank">Basic Queries Tutorial</a></li>
        <li><a href="https://developer.stackmob.com/ios-sdk/advanced-queries-tutorial" target="_blank">Advanced Queries Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Counts -->

### Fetch Counts

If you just need to know the number of object that would be returned for a given fetch request, use the `countForFetchRequest:onSuccess:onFailure:` method (also comes in synchronous form):

```obj-c
NSFetchRequest *fetch = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
[fetch setPredicate:[NSPredicate predicateWithFormat:@"type == 'Home'"]];

[self.managedObjectContext countForFetchRequest:fetch onSuccess:^(NSUInteger count) {
  // count contains the number of object that would be retuned by the fetch.
} onFailure:^(NSError *error) {
  // Handle error
}];
```

You can also do the same with the datastore API. Create your `SMQuery` instance and pass it to the `performCount:onSuccess:onFailure:` method, like so:

```obj-c
SMQuery *query = [[SMQuery alloc] initWithSchema:@"todo"];
[query addConditionWhere:@"type" equals:@"Home"];

[[[SMClient defaultClient] dataStore] performCount:query onSuccess:^(NSNumber *count) {
  // count contains the number of object that would be retuned by the query.
} onFailure:^(NSError *error) {
  // Handle error
}];
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#task_Asynchronous Count" target="_blank">NSManagedObjectContext Category: Asynchronous/Synchronous Count</a></li>
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

When you create relationships between entities in your Core Data model, they will translate directly to relationships on StackMob.  Here are the important things to note:

* Core Data recommends creating inverse relationships to maintain referential integrity. This ensures that when you delete one object, any related objects are updated as well.
* Core Data supports <a href="http://developer.apple.com/library/mac/#documentation/cocoa/conceptual/coredata/Articles/cdRelationships.html">Delete Rules</a>. Take this into consideration if you are building cross-platform.
* The great thing about Core Data handling relationship logic is that it will all translate from the client to StackMob, so no extra work is necessary.

When you eventually save objects which include relationships to other entities, those relationships will get inferred by the server i.e. StackMob will detect that those relationships do not exist and create them on the fly. This is a feature included only in development. If you need to create relationships manually, see <a href="#CreatingRelationshipswiththeDashboard">Creating Relationships with the Dashboard</a>.

<!--- One To One -->

### One to One

Add a relationship from one entity to another by creating a new entry in the **Relationships** section of the entity builder UI.  Convention says to name the relationship the singular version of the entity it points to, like so:

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/relationships/one-to-one-guide.png" />

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/one-to-one-relationships-tutorial" target="_blank">One-To-One Relationships Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- One To Many -->

### One to Many

Add a relationship from one entity to another by creating a new entry in the **Relationships** section of the entity builder UI.  Convention says to name the relationship the plural version of the entity it points to, like so:

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/relationships/one-to-many-guide.png" />

In the data model, be sure to check the box for a To-Many relationship, as shown.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/one-to-many-relationships-tutorial" target="_blank">One-To-Many Relationships Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Many To Many -->

### Many to Many

You can achieve a many to many relationship by simply creating two relationships which are the inverse of each other and both to-many relationships.

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

<p class="alert">This section only covers the parts of User Authentication that deal with Core Data, namely creating a user object and retrieving a user managed object. Refer to the <a href="https://developer.stackmob.com/ios-sdk/developer-guide#UserAuthentication" target="_blank">User Authentication section of the Developer Guide</a> for all other topics.</p>


<!--- Creating User Object -->

### Creating a User Object

In Core Data, you'll first create an entity which maps to your user schema on StackMob. It's most likely called `user`, so your entity will be called `User`.

<p class="alert">Do not create an attribute for the password field. It would not be safe to have passwords floating around in Core Data. The <code>SMUserManagedObject</code> class provides helper methods to safely set a password for a user object.</p> 

After creating a managed object subclass for the entity, you'll change the inherited class from `NSManagedObject` to `SMUserManagedObject`. This class provides methods to properly initialize a user object by linking to the `SMClient` instance. It also provides methods to set the user's password during creation.

In your entity subclass implementation file, we recommend creating a custom init method which calls the `SMUserManagedObject` `initWithEntityName:insertIntoManagedObjectContext` method. We have overridden this method to store a reference to `[SMClient defaultClient]`. This ensures that the SDK knows the primary key, etc of the user schema. Your init method might look something like:

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

The above example assumes an entity `User` with attributes `username` and `age`. The `setPassword` method is provided by the `SMUserManagedObject` class to securely set a password for your user which is persisted to StackMob but never saved locally on the device. 

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
        <li><a href="https://developer.stackmob.com/ios-sdk/create-user-object-tutorial" target="_blank">Create a User Object Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>


<!--- Edit User object -->


### Retrieve User Managed Object

The next time the user logs in to your app, you may need to have a copy of the user object around to make updates. To do this, simply create a fetch request and specify a predicate such that the only result should be your logged in user:

```obj-c
// Pull username from login screen text field, for example
NSString *username = self.textField.text;

NSFetchRequest *userFetch = [[NSFetchRequest alloc] initWithEntityName:@"User"];
[userFetch setPredicate:[NSPredicate predicateWithFormat:@"username == %@", username]];

[self.managedObjectContext executeFetchRequest:userFetch onSuccess:^(NSArray *results){
  if ([results count] != 1) { 
    // There should only be one result 
  };

  User *currentUser = (User *)[results objectAtIndex:0];
  // Edit user object or store ID to pass around

}];
```

<p class="alert">Managed Objects are not thread safe. Pass object IDs between threads and blocks.</p>

<!---
    ///////////////////
    GEO-LOCATION
    //////////////////
-->

## Geolocation

StackMob provides GeoPoint field types which allow you to save objects with latitude and longitude coordinates. Once you have objects that contain geolocation data, you can query for a subset of those objects near a particular coordinate pair, within a specific radius, or even within a box formed from coordinates. This makes building apps that show all restaurants around a user's current location, or tracking a user's path as they jog through the city, as simple as ever.

<!--- Add schema field -->

### Add a GeoPoint Schema Field

In order for a schema to allow for geopoints, it must have a field with the GeoPoint type. To manually add a geopoint field to your schema, follow the <a href="https://developer.stackmob.com/module/geo" target="_blank">Adding a GeoPoint Field To Schemas</a> tutorial to get set up.

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
  [todo assignObjectId];
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
  // Error
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
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/ios-sdk/saving-geolocation-tutorial" target="_blank">Saving Geolocation Data Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Read Geo-Location -->

### Reading Geolocation Values

Because managed object geopoint data will be contained in a `Tranformable` attribute type, it must be unarchived to be properly read:

```obj-c
NSData *data = [managedObject objectForKey:@"location"];

SMGeoPoint *geoPoint = [NSKeyedUnarchiver unarchiveObjectWithData:data];
```

<!--- Query Geo-Location -->

### Query based on Geolocation

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
        // Error
}];
```

The `SMPredicate` class provides method to query based on range in miles or kilometers, as well as bounds by SW and NE corners, or even just near a provided point.

All <code>SMPredicate</code> methods to query based on an instance of <code>SMGeoPoint</code> have an equivalent method to query based on an instance of <code>CLLocationCoordinate2D</code>.

<p class="alert">Fetching from the cache using <code>SMPredicate</code> is not supported, and will return an empty array of results. Similarly, when a fetch is performed from the network (StackMob), any results are not cached.</br></br>Fetched Results Controllers that use fetch requests with conditions around geolocation is not supported. Use refresh controls instead.</p>

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
        <li><a href="https://developer.stackmob.com/ios-sdk/querying-geolocation-tutorial" target="_blank">Querying Geolocation Tutorial</a></li>
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

Follow the <a href="https://developer.stackmob.com/module/s3#AddaBinaryField" target="_blank">Adding a Binary Field To Schemas</a> tutorial to get the S3 module and a Binary field in your schema set up.

### Saving Binary Data

To save Binary data to StackMob, first create an attribute in your entity of type `String`. This attribute should map to a field on StackMob of type `Binary`.

Next, you will use the `SMBinaryDataConversion` class to convert instances of `NSData` into strings ready to be saved to StackMob.

`SMBinaryDataConversion` offers a class method `stringForBinaryData:name:contentType:` to decode binary data into a string. This is then used to send to StackMob as the value for a field with type `Binary`. The contents of the string will be parsed and the content will be stored on S3. StackMob will then store the URL as the value. A call to `refreshObject:mergeChanges:` on the managed object context will update the in-memory value to the URL in the persistent store.

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

`[newManagedObject valueForKey:@"pic"]` now returns the S3 URL for the binary data.

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
        <li><a href="https://developer.stackmob.com/ios-sdk/upload-files-to-s3-tutorial" target="_blank">Upload Files to S3 Tutorial</a></li>
      </ul>
    </div>
  </div>
</div>

### Reading Binary Data

Once you have saved an object to StackMob containing a field with binary data, the value will then contain a string representation of the S3 URL. You will most likely convert this string into an instance of `NSURL`, and in turn use that URL to get and set data to a variable. For example, suppose we have saved an image in our attribute `photo`, and now want to set an image for our UI. After reading the object we would convert the string URL to an image like this:

```obj-c
NSURL *imageURL = [NSURL URLWithString:[object valueForKey:@"photo"]]; 
NSData *imageData = [NSData dataWithContentsOfURL:imageURL]; 
UIImage *image = [UIImage imageWithData:imageData];
```

### Working Offline

When you save an object with binary data while the device is offline, the value of the attribute will contain a data representation, ready to be saved to StackMob. In order to properly read it at that point, the data must be extracted from the string and decoded.

In order to check if the attribute value is a URL or not, use the `stringContainsURL:` method. To convert the string value back to data, use the `dataForString:` method:

```obj-c
NSString *picString = [newManagedObject valueForKey:@"pic"];
if ([SMBinaryDataConversion stringContainsURL:picString]) {
  NSURL *urlForPic = [NSURL URLForString:picString];
  // Set image from URL
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

<!---
  ///////////////////
  NETWORK REACHABILITY
  //////////////////
-->

## Network Reachability

The iOS SDK provides an interface for determining the current status of the device's network connection. This comes in handy when you want to perform specific operations based on whether the device is online or offline. The interface allows you to execute a block of code whenever the network status changes, which is perfect for changing your cache policy or syncing with the server (See <a href="#CachingandOfflineSync">Caching and Offline Sync</a>).

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

Often times you may want to change the fetch policy, or initiate a sync with the server depending on the status of the network. You can set a block that will be executed every time the network status changes with the `setNetworkStatusChangeBlock:` method:

```obj-c
[self.client.networkMonitor setNetworkStatusChangeBlock:^(SMNetworkStatus status){
    
    if (status == SMNetworkStatusReachable) {
      ...
    } else {
      ...
    }

}];
```

Alternatively you can use the `setNetworkStatusChangeBlockWithFetchPolicyReturn:` method, which requires you to return a fetch policy to set. Here's an example which sets points fetches to either the cache or the network based on the current network status:

```obj-c
[self.client.networkMonitor setNetworkStatusChangeBlockWithFetchPolicyReturn:^(SMNetworkStatus status){
    
    if (status == SMNetworkStatusReachable) {
      return SMFetchPolicyNetworkOnly;
    } else {
      return SMFetchPolicyCacheOnly;
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

## Error Handling

When errors occur at the SDK level, specific error codes are returned to indicate the type of error that occurred.

<p class="alert">This section only covers Core Data specific errors. Refer to the <a href="https://developer.stackmob.com/ios-sdk/developer-guide#ErrorHandling" target="_blank">Error Handling section of the Developer Guide</a> for all error information.</p>

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

<!---
    ///////////////////
    SUPPORT SPECS
    //////////////////
-->

## Support Specifications

As part of the integration between StackMob and Core Data, our goal is to support as much Core Data functionality as we can, including attribute types, fetch request settings and predicate formats.

As we continually improve our SDK this page will stay up to date with everything that is currently supported.

Features without date to indicate starting support date are assumed v1.0.0+

### Data Type Maps

##### Core Data Attribute Type to StackMob Field Type

<br/>

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>Core Data Attribute Type</th>
        <th>StackMob Field Type</th>
    </tr>
    <tr>
        <td>Integer 16</td>
        <td>Integer</td>
    </tr>
    <tr>
        <td>Integer 32</td>
        <td>Integer</td>
    </tr>
    <tr>
        <td>Integer 64</td>
        <td>Integer</td>
    </tr>
    <tr>
        <td>Decimal</td>
        <td>Float</td>
    </tr>
    <tr>
        <td>Double</td>
        <td>Float</td>
    </tr>
    <tr>
        <td>Float</td>
        <td>Float</td>
    </tr>
    <tr>
        <td>String</td>
        <td>String, Binary (see <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMBinaryDataConversion.html">SMBinaryDataConversion</a>)</td>
    </tr>
    <tr>
        <td>Boolean <i>(v1.1.1+)</i></td>
        <td>Boolean</td>
    </tr>
    <tr>
        <td>NSDate</td>
        <td>Integer, Representing the number of milliseconds since 1970</td>
    </tr>
    <tr>
        <td>Binary Data</td>
        <td>Currently Not Supported</td>
    </tr>
    <tr>
        <td>Transformable <i>(v1.3.0+)</i></td>
        <td>Geopoint (lat/lon) - See <a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data">Saving Geo Location Data Tutorial</td>
    </tr>
</table>

##### StackMob Field Type to Core Data Attribute Type

<br/>

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>StackMob Field Type</th>
        <th>Core Data Attribute Type</th>
    </tr>
    <tr>
        <td>String</td>
        <td>String</td>
    </tr>
    <tr>
        <td>Integer</td>
        <td>Integer 16, 32, & 64</td>
    </tr>
    <tr>
        <td>Float</td>
        <td>Decimal</td>
    </tr>
    <tr>
        <td>Boolean</td>
        <td>Boolean</td>
    </tr>
    <tr>
        <td>Array</td>
        <td>Currently not supported with Core Data, use the Datastore API (See <a href="https://developer.stackmob.com/tutorials/ios/Lower-Level-CRUD-API">Lower Level CRUD API Tutorial</a>)</td>
    </tr>
    <tr>
        <td>Binary</td>
        <td>String</td>
    </tr>
    <tr>
        <td>Geopoint (lat/lon) <i>(v1.3.0+)</i></td>
        <td>Transformable - See <a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data">Saving Geo Location Data Tutorial</a></td>
    </tr>
</table>

### Fetch Requests

Support Table is based on `NSFetchRequest` methods.  Additional support, especially in regards to managing how results are returned, is planned for future releases.

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>NSFetchRequest method</th>
        <th>Supported</th>
    </tr>
    <tr>
        <td>fetchRequestWithEntityName:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>entityName / initWithEntityName:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>entity / setEntity:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>includesSubentities / setIncludesSubEntities:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>predicate / setPredicate:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>fetchLimit / setFetchLimit:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>fetchOffset / setFetchOffset:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>fetchBatchSize / setFetchBatchSize:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>affectedStores / setAffectedStores:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>sortDescriptors / setSortDescriptors:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>relationshipKeyPathsForPrefetching / setRelationshipKeyPathsForPrefetching:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>resultType / setResultType:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>includesPendingChanges / setIncludesPendingChanges:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>propertiesToFetch / setPropertiesToFetch:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>returnsDistinctResults / setReturnsDistinctResults:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>includesPropertyValues / setIncludesPropertyValues:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>shouldRefreshRefetchedObjects / setShouldRefreshRefetchedObjects:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>returnObjectsAsFaults / setReturnObjectsAsFaults:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>propertiesToGroupBy / setPropertiesToGroupBy:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>havingPredicate / setHavingPredicate:</td>
        <td>NO</td>
    </tr>
</table>

<b>Additional Supported Features:</b>
<ul>
	<li>Fetch Entity with predicates where "relationship key == managed object or managed object ID".</li>
</ul>
<b>Not Supported</b>
<ul>
	<li>Fetching Entity with key paths in predicate, i.e. "relationshipKey.attribute == value".</li>
	<li>Fetching Geo-point data from the cache.</li>
</ul>


#### Predicates

##### <b>Comparison Predicates</b>

<br/>

Predicate's <b>leftExpression</b> must be of type <b>NSKeyPathExpressionType</b>.

Predicate's <b>rightExpression</b> must be of type <b>NSConstantValueExpressionType</b>.

<br/>

##### Supported Predicate Operator Types (based on NSPredicateOperatorType):

<br/>

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>NSPredicateOperatorType</th>
        <th>Supported</th>
    </tr>
    <tr>
        <td>NSLessThanPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSLessThanOrEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSGreaterThanPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSGreaterThanOrEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSNotEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSMatchesPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSLikePredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSBeginsWithPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSEndsWithPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSInPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSCustomSelectorPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSContainsPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSBetweenPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
</table>

##### <b>Compound Predicates</b>

<p>For a tutorial on using compound predicates see the <a href="https://developer.stackmob.com/tutorials/ios/Advanced-Queries" target="_blank">Advanced Query Tutorial</a>.
<ul>
	<li>NSAndPredicateType - Use [NSPredicate andPredicateWithSubpredicates:arrayOfPredicates] or AND in predicate format string.</li>
	<li>NSOrPredicateType - Use [NSPredicate orPredicateWithSubpredicates:arrayOfPredicates] or OR in predicate format string. Requests directed to the network are limited to one subset of OR conditions i.e. `(A AND (B OR C OR D))` but not `((A OR B) AND (C OR D))`.</li>
    <li>NSNotPredicateType - Request directed to the network can use NOT only for IN queries i.e. `@"!(field IN [1,2,3,4])`.</li>
</ul>

### Concurrency API

The concurrency API <i>(v1.2.0+)</i> brings a new set of asynchronous and synchronous operations to Core Data.  All methods follow patterns to execute saves and fetches on background queues on private contexts.  It is recommended to use these methods to achieve optimal performance between Core Data, StackMob and the local caching system.

All methods can be found in the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">SMManagedObjectContext+Concurrency Class Reference</a>.

### Miscellaneous Support

#### Default Values

Setting default values for attributes of string, number (integer, decimal, etc.), or boolean types will get included in the object dictionary during insert.

<b>Check your attributes with number types</b> because they automatically default to 0 unless you deselect the `Default` checkbox.

## Core Data References

* [Core Data Primer eBook](http://go.stackmob.com/learncoredata.html) - StackMob has put together an eBook based on one of our tutorial series which walks you through the full implementation of an app that uses Core Data.
* [Getting Started With Core Data](http://www.raywenderlich.com/934/core-data-on-ios-5-tutorial-getting-started) - Ray Wenderlich does a great tutorial on the basics of Core Data.
* [iPhone Core Data: Your First Steps](http://mobile.tutsplus.com/tutorials/iphone/iphone-core-data/) - Well organized tutorial on Core Data.
* [Introduction To Core Data](https://developer.apple.com/library/ios/\#documentation/Cocoa/Conceptual/CoreData/cdProgrammingGuide.html\#//apple\_ref/doc/uid/TP40001075) - Apple's Core Data Programming Guide
* [Introduction To Predicates](https://developer.apple.com/library/ios/\#documentation/Cocoa/Conceptual/Predicates/predicates.html\#//apple\_ref/doc/uid/TP40001789) - Apple's Predicates Programming Guide
