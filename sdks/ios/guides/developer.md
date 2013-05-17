<h1>Developer Guide</h1>

Core Data allows us to place a familiar wrapper around StackMob REST calls and Datastore API. iOS developers can leverage their existing knowledge of Core Data to quickly integrate StackMob into their applications. For those interested in sticking to the REST-based way of making requests, we provide the full Datastore API as well.

<!---
	///////////////////
	SETUP
	//////////////////
-->

# Setup

<!--- INITIALIZE -->

## Initializing the iOS SDK

Wherever you plan to use StackMob, add `#import "StackMob.h"` to the header file.

Create a variable of class `SMClient`, most likely in your AppDelegate file where you initialize other application wide variables, and initialize it like this:

```obj-c
// Assuming your variable is declared SMClient *client;
client = [[SMClient alloc] initWithAPIVersion:@"YOUR_API_VERSION" publicKey:@"YOUR_PUBLIC_KEY"];
```

For `YOUR_API_VERSION`, pass `@"0"` for Development, `@"1"` or higher for the corresponding version in Production.

If you haven't found your public key yet, check out **Manage App Info** under the **App Settings** sidebar on the <a href="https://dashboard.stackmob.com">Dashboard page</a>.

From then on you can either pass your client instance around, or use `[SMClient defaultClient]` to retrieve it.

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/initWithAPIVersion:publicKey:">initWithAPIVersions:publicKey:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/defaultClient">defaultClient</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/initWithAPIVersion:apiHost:publicKey:userSchema:userPrimaryKeyField:userPasswordField:">initWithAPIVersion:apiHost:publicKey:userSchema:userPrimaryKeyField:userPasswordField:</a></br>
</p>

<!--- USING CORE DATA -->

## Using Core Data

StackMob recommends using Core Data for data persistance. It provides a powerful and robust object graph management system that otherwise would be a nightmare to implement.  Although it may have a reputation for being pretty complex, the basics are easy to grasp and understand.  If you want to learn the basics, check out INSERT LINK TO REFERENCES AND EBOOK.

The three main pieces of Core Data are instances of:

* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html">NSManagedObjectContext</a> - This is what you use to create, read, update and delete objects in your database.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSPersistentStoreCoordinator_Class/NSPersistentStoreCoordinator.html">NSPersistentStoreCoordinator</a> - Coordinates between the managed object context and the actual database, in this case StackMob.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectModel_Class/Reference/Reference.html">NSManagedObjectModel</a> - References a file where you defined your object graph.

<!--- DEF MOM -->

## Defining a Managed Object Model

Your data model is what Core Data uses to do something, TO BE DEFINED LATER

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
<!--- CREATE SUBCLASSES -->

## Creating `NSManagedObject` Subclasses

FILL IN

<!--- INIT CDS -->

## Initialize a Core Data Store

A Core Data store instance gives you everything you need to work with StackMob's Core Data integration. With your `SMCoreDataStore` object you can retrieve a managed object context configured with a `SMIncrementalStore` as it’s persistent store to allow communication to StackMob from Core Data.  

You'll create one `SMCoreDataStore` instance like so:

```obj-c
// self.managedObjectModel returns an initialized instance of NSManagedObjectModel
// client is your instance of SMClient
SMCoreDataStore *coreDataStore = [client coreDataStoreWithManagedObjectModel:aManagedObjectModel];
```

From then on you can either pass your core data store instance around, or use `[[SMClient defaultClient] coreDataStore]` to retrieve it.

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/coreDataStoreWithManagedObjectModel:">coreDataStoreWithManagedObjectModel:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#//api/name/coreDataStore">coreDataStore</a>
</p>

<!--- OBTAIN MOC -->

## Obtaining a Managed Object Context

You can obtain a managed object context configured from your `SMClient` instance like this:

```obj-c

	
// assuming you have a variable called managedObjectContext
self.managedObjectContext = [coreDataStore contextForCurrentThread];
```

Use this instance of `NSManagedObjectContext` for the current thread you are on. We recommend calling `contextForCurrentThread` rather than passing a context instance across files or blocks.

The default Core Data merge policy set for all contexts created by this class is `NSMergeByPropertyObjectTrumpMergePolicy`. Use `setDefaultMergePolicy:applyToMainThreadContextAndParent:` to change the default.

You can obtain the managed object context for the main thread at any time by accessing the `mainThreadContext` property.

If you want to do your own context creation, use the `persistentStoreCoordinator` property to ensure your objects are being saved to the StackMob server.

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#//api/name/contextForCurrentThread">contextForCurrentThread</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#//api/name/setDefaultMergePolicy:applyToMainThreadContextAndParent:">setDefaultMergePolicy:applyToMainThreadContextAndParent:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#//api/name/mainThreadContext">mainThreadContext</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#//api/name/persistentStoreCoordinator">persistentStoreCoordinator</a>
</p>

<!---
	///////////////////
	DATASTORE
	//////////////////
-->

# Datastore

<!--- INSERT OBJECT -->

## Creating an Object

Typically you will create subclasses of `NSManagedObject` for each of your Core Data entities, and create objects simply by instantiating a new instance. Core Data subclasses automatically give you setters and accessors to set an object's values.  Create a new Todo object like so:

```obj-c
Todo *newTodo = [[Todo alloc] init];
    
[newTodo setTitle:self.titleField.text];
[newTodo setTodoId:[newManagedObject assignObjectId]];
```

<p class="alert alert-warning">Object IDs must be assigned to an object before it is persisted. This keeps Core Data IDs and StackMob objects in sync. For random arbitrary IDs, use the `assignObjectId` method.</p>

<b>Without Subclasses</b>

Instantiating a generic managed object requires using the generic `NSEntityDescription` method to create a new object in the context.

```obj-c
NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
[newManagedObject setValue:self.titleField.text forKey:@"title"];
[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
```

<p class="alert">
The newly created object is not persisted until you <a href="#PerformingSaves">save the managed object context</a>.
</p>

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObject+StackMobSerialization.html#//api/name/assignObjectId">assignObjectId</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObject+StackMobSerialization.html#//api/name/primaryKeyField">primaryKeyField</a></br>
	<a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSEntityDescription_Class/NSEntityDescription.html">insertNewObjectForEntityForName:inManagedObjectContext:</a></br>
</p>

<!--- READ OBJECT -->

## Reading Objects

To read objects from a particular Entity, create and execute a Core Data fetch request using methods from the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html">NSManagedObjectContext+Concurrency</a> class.

<p class="alert">By default, objects are returned as faults, and attribute values are not accessed from the persistent store until specifically called upon in your code.  This ensures in-memory usage is as low as possible.</p> 

<!--- ASYNC READ -->

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

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:onSuccess:onFailure:">executeFetchRequest:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:returnManagedObjectIDs:onSuccess:onFailure:">executeFetchRequest:returnManagedObjectIDs:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:">executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:">executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:</a></br>
</p>

<!--- SYNC READ -->

### Synchronous Fetch Requests

To execute synchronous fetches, use the `executeFetchRequestAndWait:error:` method. This works like `executeFetchRequest:error`, but uses the child-parent context model that is used internally by the SDK.

```obj-c
NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
     
// set any predicates or sort descriptors, etc.

// execute the request
NSArray *results = [self.managedObjectContext executeFetchRequestAndWait:fetchRequest error:&error];
```

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequestAndWait:error:">executeFetchRequestAndWait:error:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequestAndWait:returnManagedObjectIDs:error:">executeFetchRequestAndWait:returnManagedObjectIDs:error:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequestAndWait:returnManagedObjectIDs:options:error:">executeFetchRequestAndWait:returnManagedObjectIDs:options:error:</a></br>
</p>

<!--- UPDATE OBJECT -->

## Updating an Object

After fetching an existing mananged object, update it by simply changing the object's values. The updated values are not persisted until the managed object context is saved.

<!--- DELETE OBJECT -->

## Deleting an Object

```obj-c
[self.manangedObjectContext deleteObject:aManagedObject];
```

<p class="alert alert-info">
	<a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html">deleteObject:</a>
</p>

<!--- SAVE -->

## Performing Saves

At some point after inserting, updating, and deleting objects from a managed object context, you will initiate a save which will cause all modified objects to be persisted to the external store.  The external store in this case is StackMob as well as a local cache database, ensuring data is available while the devices is offline.

The StackMob SDK has placed a wrapper around the familiar `save:` core data method to offer synchronous and asynchronous versions for improved performance. All methods can be found in the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html">NSManagedObjectContext+Concurrency</a> class.

<!--- ASYNC SAVES -->

### Asynchronous Saves

Asynchronous saves are great for executing a save off the main thread, ensuring the UI is not blocked.

```obj-c
[self.managedObjectContext saveOnSuccess:^{
    NSLog(@"You created a new object!");
} onFailure:^(NSError *error) {
    NSLog(@"There was an error! %@", error);
}];
```

Use the callbacks to update the UI, notifiy other dependent files, etc.

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveOnSuccess:onFailure:">saveOnSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:">saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveWithSuccessCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:">saveWithSuccessCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:</a></br>
</p>

<!--- SYNC SAVES -->

### Synchronous Saves

Although the SDK synchronous save methods behave like Core Data's native `save:` method, we recommend using them because they follow the child-parent context patterns used internally by the SDK.

```obj-c
NSError *error = nil;
BOOL success = [self.managedObjectContext saveAndWait:&error];
```

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveAndWait:">saveAndWait:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveAndWait:options:">saveAndWait:options:</a></br>
</p>

<!--- PER REQUEST OPTIONS -->

## Per Request Options

Each type of method (asynchronous/synchronous save/fetch) has an overloaded method declaration with a parameter that takes an instance of <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html">SMRequestOptions</a>. The parameter name in all methods is called options. This allows you to provide a custom `SMRequestOptions` instance that will be applied to all calls in that request. For example, provide a `SMRequestOptions` instance with the `isSecure` property set to YES if you wanted a specific save request to run over SSL i.e. all inserts/updates/deletes for that request will be sent over SSL.

<p class="alert">
Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests. The following options are currently safe to customize and will override the default for the duration of the request:</br></br>
&nbsp;&nbsp;&bull;&nbsp;&nbsp;isSecure property (HTTPS)</br></br>
Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
</p>

<!--- LOWER LEVEL DATASTORE API -->

## Lower Level Data Store API

If you want to make direct REST-based calls to the Datastore, check out the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMDataStore.html">SMDataStore</a> class.

# Queries

## Predicates

## Supported Queries

# Relationships

## One to One

## One to Many

## Many to Many

# User Authentication

## Creating a User Object

## Logging in to StackMob

## Checking Logged in Status

## Logging out of StackMob

## Reset a Password

## Forgot Password

# Geolocation

# Social Integration

## Facebook

### 

## Twitter

## Gigya

# Push Notifications

# Offline Sync

Included with version 2.0.0+ of the SDK is a sync system built in to the Core Data Integration to allow for local saving and fetching of objects when a device is offline. When back online, modified data will be synced with the server. Many settings are available to the developer around cache and merge policies, conflict resolution, etc. See the Offline Sync Guide for all information.

# Error Codes

# Debugging

# Network Reachability

# Custom Code

# Core Data Coding Practices

# Core Data Integration Support Specs

# Deprecations

# Core Data References