iOS Developer Guide
=====================================

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

If you haven't found your public key yet, check out **Manage App Info** under the **App Settings** sidebar on the <a href="https://dashboard.stackmob.com" target="_blank">Dashboard page</a>.

From then on you can either pass your client instance around, or use `[SMClient defaultClient]` to retrieve it.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMClient.html#task_Initialize" target="_blank">Initialize a Client</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- USING CORE DATA -->

## Using Core Data

StackMob recommends using Core Data for data persistance. It provides a powerful and robust object graph management system that otherwise would be a nightmare to implement.  Although it may have a reputation for being pretty complex, the basics are easy to grasp and understand. If you are new to Core Data check out our list of <a href="http://stackmob.github.io/stackmob-ios-sdk/index.html#core_data_references" target="_blank">Core Data References</a>.

INSERT MAKE SURE YOU READ THROUGH CODING PRACTICES AND ARE AWARE OF SUPOPRT SPECS

The three main pieces of Core Data are instances of:

* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html" target="_blank">NSManagedObjectContext</a> - This is what you use to create, read, update and delete objects in your database.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSPersistentStoreCoordinator_Class/NSPersistentStoreCoordinator.html" target="_blank">NSPersistentStoreCoordinator</a> - Coordinates between the managed object context and the actual database, in this case StackMob.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectModel_Class/Reference/Reference.html" target="_blank">NSManagedObjectModel</a> - References a file where you defined your object graph.

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

	
// assuming you have a variable called managedObjectContext
self.managedObjectContext = [coreDataStore contextForCurrentThread];
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
      	<li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#task_Obtaining a Managed Object Context" target="_blank">Obtaining a Managed Object Context</a></li>
        <li><a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html#task_Properties" target="_blank">Persistent Store Coordinator and Main Thread Context properties</a></li>
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
      <strong>Examples</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/tutorials/ios/Create-an-Object" target="_blank">Create an Object</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- READ OBJECT -->

## Reading Objects

To read objects from a particular Entity, create and execute a Core Data fetch request using methods from the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency</a> class.

For a list of StackMob supported `NSFetchRequest` methods, visit the `Fetch Requests` section of the <a href="file://localhost/Users/mattvaz/Documents/stackmob/editAppledocs/ios-sdk/CoreDataSupportSpecs.html#fetch_request" target="_blank">Core Data Support Specs</a>.

<p class="alert">By default, objects are returned as faults, and attribute values are not accessed from the persistent store until specifically called upon in your code.  This ensures in-memory usage is as low as possible.</br></br>All information on specifying conditions for a query can be found in the INSERT LINK TO QUERIES.</p> 

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

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:onSuccess:onFailure:" target="_blank">executeFetchRequest:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:returnManagedObjectIDs:onSuccess:onFailure:" target="_blank">executeFetchRequest:returnManagedObjectIDs:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:" target="_blank">executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:" target="_blank">executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:</a></br>
</p>

<!--- SUB: SYNC READ -->

### Synchronous Fetch Requests

To execute synchronous fetches, use the `executeFetchRequestAndWait:error:` method. This works like `executeFetchRequest:error`, but uses the child-parent context model that is used internally by the SDK.

```obj-c
NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
     
// set any predicates or sort descriptors, etc.

// execute the request
NSArray *results = [self.managedObjectContext executeFetchRequestAndWait:fetchRequest error:&error];
```

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequestAndWait:error:" target="_blank">executeFetchRequestAndWait:error:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequestAndWait:returnManagedObjectIDs:error:" target="_blank">executeFetchRequestAndWait:returnManagedObjectIDs:error:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/executeFetchRequestAndWait:returnManagedObjectIDs:options:error:" target="_blank">executeFetchRequestAndWait:returnManagedObjectIDs:options:error:</a></br>
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
	<a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html" target="_blank">deleteObject:</a>
</p>

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

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveOnSuccess:onFailure:" target="_blank">saveOnSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:" target="_blank">saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveWithSuccessCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:" target="_blank">saveWithSuccessCallbackQueue:failureCallbackQueue:options:onSuccess:onFailure:</a></br>
</p>

<!--- SYNC SAVES -->

### Synchronous Saves

Although the SDK synchronous save methods behave like Core Data's native `save:` method, we recommend using them because they follow the child-parent context patterns used internally by the SDK.

```obj-c
NSError *error = nil;
BOOL success = [self.managedObjectContext saveAndWait:&error];
```

<p class="alert alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveAndWait:" target="_blank">saveAndWait:</a></br>
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html#//api/name/saveAndWait:options:" target="_blank">saveAndWait:options:</a></br>
</p>

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

<!---
	///////////////////
	QUERIES
	//////////////////
-->

# Queries

<!--- Predicates -->

## Predicates

In Core Data, every read starts out with a fetch request. You can then add predicates (conditions), sort descriptiors, offsets (ranges), etc. to return a more granular subset of results.

To learn more about fetch requests visit Apple's documentation on <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Conceptual/CoreData/Articles/cdFetching.html" target="_blank">Fetching Managed Objects</a>.

For a list of StackMob supported predicate types, visit the `Predicates` section of the <a href="file://localhost/Users/mattvaz/Documents/stackmob/editAppledocs/ios-sdk/CoreDataSupportSpecs.html#predicates" target="_blank">Core Data Support Specs</a>.

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

Visit the <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMQuery.html" target="_blank">SMQuery Class Reference</a> for a list of all available methods.

<p class="alert">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMDataStore.html#//api/name/performQuery:onSuccess:onFailure:" target="_blank">performQuery:onSuccess:onFailure:</a>
</p>

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

INSERT PICTURE

<!--- One To Many -->

## One to Many

Add a relationship from one entity to another by creating a new entry in the **Relationships** section of the entity builder UI.  Convention says to name the relationship the plural version of the entity it points to, like so:

INSERT PICTURE

In the data model, be sure to check the box for a To-Many relationship, as shown.

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
 
When you initialize an `SMClient` with `initWithAPIVersion:publicKey:`, the following defaults are set:

* apiHost = @"api.stackmob.com";
* userSchema = @"user";
* userPrimaryKeyField = @"username";
* userPasswordField = @"password";

To change the defaults so they match your schemas and fields on StackMob:

* With your instance of `SMClient`, you can directly set the properties listed above using the dot notation or setters.  For example:

```obj-c
self.client.userSchema = @"teacher";
[self.client setUserPrimaryKeyField:@"email"];
```

* Alternatively, you can set all the properties at once using `initWithAPIVersion:apiHost:publicKey:userSchema:userPrimaryKeyField:userPasswordField:`.

<p class="alert">Don't forget to check the **Create as a User Object** box when <a href="https://developer.stackmob.com/api/schemas/create" target="_blank">creating a new schema</a> for user objects.</p>

<p class="alert-info">

</p>

<!--- Creating User Object -->

## Creating a User Object

In Core Data, you'll first create an entity which maps to your user schema on StackMob. It's most likely called `user`, so your entity will be called `User`.

<p class="alert">Do not create an attribute for the password field. It would not be safe to have passwords floating around, and <code>SMUserManagedObject</code> provides helper methods to safely set a password for a user object</p> 

Next, after creating a managed object subclass for the entity, you'll change the inherited class from `NSManagedObject` to `SMUserManagedObject`. This class provides methods to properly initialize a user object, as they need to link to the client. It also provides methods to set the user's password during creation.

In your entity subclass implementation file, we recommend overriding the init method to call the init method from `SMUserManagedObject`. This ensures that the user object is properly instantiated. Your init method might look something like:

```obj-c
- (id)initIntoManagedObjectContext:(NSManagedObjectContext *)context {
	// initWithEntityName:insertIntoManagedObjectContext is an SMUserManagedObject method.
    self = [super initWithEntityName:entity insertIntoManagedObjectContext:context];
     
    if (self) {
        // assign local variables and do other init stuff here
    }
     
    return self;
}
```

You'll then create new user objects by creating managed object instances of your entity subclass, like so:

```obj-c
User *newUser = [[User alloc] init];
[newUser setUsername:@"Bob"];
[newUser setPassword:@"vkfjakvjvjnfosg"];
[newUser setAge:[NSNumber numberWithInt:34]];

[self.managedObjectContext saveOnSuccess:^{
	// Saved the user object
} onFailure:^(NSError *error){
	// Error
}];
```

<p class="alert-info">
	<a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMUserManagedObject.html" target="_blank">SMUserManagedObject Class Reference</a>
</p>

TUTORIAL: CREATE A USER OBJECT

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

<p class="alert-info">
	ADD LINKS HERE
</p>

<!--- Edit User object -->

## Updating a User Object

The next time the user logs in to your app, you may need to have a copy of the user object around to make updates. To do this, simply create a fetch request and specify a predicate such that the only result should be your logged in user:

```obj-c
// Pull username from login screen text field, for example
NSString *username = self.textField.text;

NSFetchRequest *userFetch = [[NSFetchRequest alloc] initWithEntityName:@"User"];
[userFetch setPredicate:[NSPredicate predicateWithFormat:@"username == %@", username]];

[self.manangedObjectContext executeFetchRequest:userFetch onSuccess:^(NSArray *results){
	if (results count]) { // // There should only be one result };

	User *currentUser = (User *)[results objectAtIndex:0];
	// Edit user object or store ID to pass around

}];

<a class="alert">Managed Objects are not thread safe. Pass object IDs between threads and blocks.</p>

<!--- Get logged in user -->

## Retrieve Logged In User

```obj-c
[[SMClient defaultClient] loginWithUsername:username password:password onSuccess:^(NSDictionary *result){
	// result contains a dictionary representation of the user object
} onFailure:^(NSError *){
	// Error
}];
```

<!--- Check Status -->

## Check Status

To see whether your user is logged in, use the `SMClient` `isLoggedIn` and `isLoggedOut` methods.

INSERT API LINKS

<!--- Auto Refresh -->

## Automatic Login Refresh

The iOS SDK come built in with automatic login refresh functionality.  This means that if an access token has expired and a request comes back unauthorized, the SDK will automatically send a request for a new session access token using the refresh token from the last session.

If a user logs in to one device, then logs into another device, the refresh token from the login on the first device becomes invalid. To handle this situation, provide a block to be executed when a token refresh fails using the `setTokenRefreshFailureBlock:` method, like so:

```obj-c
[[SMClient defaultClient] setTokenRefreshFailureBlock:^(NSError *error, SMFailureBlock originalFailureBlock){
	// You are given the refresh error as well as the failure callback from the original request if you choose to call it.
	// We recommend logging in again by presenting a temporary login screen to the user, then picking up where they left off in the UI
}];
```

<p class="alert-info">
	ADD LINKS
</p>

<!--- Logout -->

## Logout of StackMob

```obj-c
[[SMClient defaultClient] logoutOnSuccess:^(NSDictionary *result){
	// Successful logout
} onFailure:^(NSError *error){
	// Error
}];
```

<p class="alert-info">
	
</p> 

<!--- Reset -->

## Reset a Password

Developers can easily implement a reset password workflow in their application. A basic example workflow might look like the following:

1. Implement a screen for logged in users to reset their password.
2. Allow the user to insert their current password, followed by a new password. Optionally ask them to confirm the new password.
3. Initiate a method which calls the `SMClient` `changeLoggedInUserPasswordFrom:to:onSuccess:onFailure:` method.
4. The logged in user's password is reset!

<p class="alert-info">
	
</p>

<!--- Forgot Pass -->

## Forgot Password

Developers can easily implement a forgot password workflow in their application. To do this, your user schema will need to have a field dedicated to users' email addresses, which can be the username field if your users sign in with their emails, for example. Choose the correct field which maps to emails in the **Forgot Password** section on the user schema configuration page. Once you have done this, a basic example workflow might look like the following:

1. Have a button on the login screen which maps to a forgot password method.
2. When the user presses the button they are propmpted to enter their username.
3. This should trigger the `SMClient` `sendForgotPaswordEmailForUser:onSuccess:onFailure:` method. The user wull receive an email with a temporary password.
4. When they come back to your app, they enter their username and temp password and are prompted to enter a new password.
5. Developers complete the workflow by logging in the user with the `SMClient` `loginWithUsername:temporaryPassword:settingNewPassword:onSuccess:onFailure:` method.
6. The logged in user's password is reset!

<p class="alert-info">
	
</p>

<!---
	///////////////////
	GEO-LOCATION
	//////////////////
-->

# Geolocation

<!--- Add schema field -->

## Add a GeoPoint Schema Field

In order for a schema to allow for geopoints, it must have a field with the GeoPoint type. Follow the <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-GeoPoint-Field-to-Schemas" target="_blank">Adding a GeoPoint Field To Schemas</a> tutorial to get set up.

<p class="alert">Geopoint fields are not inferred by StackMob, and must be added manually through the Dashboard.</p>

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

<!--- SUB: Manual lat/lon -->

## Manually Pulling the Latitude and Longitude ##

If you wish to pull the latitude and longitude data manually from the location manager, do so with the following:

```obj-c
NSNumber *latitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].latitude];
NSNumber *longitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].longitude];
```

Often times it may take a few run loops for the location manager to start receiving actual data after <i>startUpdatingLocation</i> is called.  You may need to implement the necessary logic to ensure you are not pulling nil data.

<!--- SUB: Subclass SMLocationManager -->

## Subclassing SMLocationManager ##

Out of the box, `SMLocationManager` gives you the ability to start/stop pulling GPS location data, as well as retrieve the current latitude and longitude of the device.  If you would like more control and customization, it's recommended you subclass `SMLocationManager`. In the init method of your subclass, you can configure the properties of the `CLLocationManager` as needed.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.apple.com/library/mac/#documentation/CoreLocation/Reference/CLLocationManager_Class/CLLocationManager/CLLocationManager.html" target="_blank">CCLocationManager</a></li>
      </ul>
    </div>
  </div>
</div>

<!--- Save Geo Data -->

## Save Geo-Location Data

The `SMGeoPoint` class provides a simple interface around a geopoint. It inherits from `NSDictionary` and has two instance methods: `latitude` and `longitude`.

Use the provided class method to obtain the current location data of the device:

```obj-c
[SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
     
    // Do something with geoPoint.latitude and geoPoint.longitude
     
} onFailure:^(NSError *error) {

    // Error
}];
```

GeoPoints are stored in Core Data using the `Transformable` attribute type. To save an `SMGeoPoint` in Core Data, it must be archived into `NSData`:

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
     
    /*
     Save the location to StackMob.
     */
   [self.managedObjectContext saveOnSuccess:^{
       NSLog(@"Created new object in Todo schema");
   } onFailure:^(NSError *error) {
        NSLog(@"Error creating object: %@", error);
   }];
     
} onFailure:^(NSError *error) {
    NSLog(@"Error getting SMGeoPoint: %@", error);
}];
```

<p class="alert">Geo Location in iOS relies on the **MapKit** framework.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="" target="_blank"></a></li>
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

Because managed object geopoint data will be contained in a tranformable attribute type, it must be unarchived to be properly read:

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

<p class="alert">Fetching from the cache using <code>SMPredicate</code> is not supported, and will return an empty array of results. Similarly, when a fetch is performed from the network (StackMob), any results are not cached.</p>

<!--- Geo-Location through DS API -->

## Geo-Locatin using the Datastore API

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
	SOCIAL INTEGRATION
	//////////////////
-->

# Social Integration

## Facebook

### 

## Twitter

## Gigya

# Custom Code

# Push Notifications

<!---
  ///////////////////
  OFFLINE SYNC
  //////////////////
-->

# Offline Sync

Included with version 2.0.0+ of the SDK is a sync system built in to the Core Data Integration to allow for local saving and fetching of objects when a device is offline. When back online, modified data will be synced with the server. Many settings are available to the developer around cache and merge policies, conflict resolution, etc. 

Read the <a href="https://developer.stackmob.com/ios-sdk/offline-sync-guide" target="_blank">Offline Sync Guide</a> for all information.

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

# SDK Error Codes

When errors occur at the SDK level, specific error codes are returned to indicate the type of error that occured. A full list can be found in the API Refererence:

<a href="http://stackmob.github.io/stackmob-ios-sdk/ErrorCodeTranslations.html" target="_blank">Error Code Translations</a>

<!---
  ///////////////////
  DEBUGGING
  //////////////////
-->

# Debugging

The SDK provides a few flags to assist in debugging. They can be found in the Debugging section of the API Refererence main page:

<a href="http://stackmob.github.io/stackmob-ios-sdk/index.html#debugging" target="_blank">Debugging</a>

<!---
  ///////////////////
  CODING PRACTICES
  //////////////////
-->

# Core Data Coding Practices

There are some important coding practices to adhere to when developing with StackMob and Core Data. They can be found in the StackMob <--> Core Data Coding Practices section of the API Refererence main page:

<a href="http://stackmob.github.io/stackmob-ios-sdk/index.html#coding_practices" target="_blank">StackMob <--> Core Data Coding Practices</a>

<!---
  ///////////////////
  SUPPORT SPECS
  //////////////////
-->

# Core Data Integration Support Specs

A full list of what Core Data features and methods are supported with StackMob can be found in the API Reference:

<a href="http://stackmob.github.io/stackmob-ios-sdk/CoreDataSupportSpecs.html" target="_blank">StackMob <--> Core Data Support Specifications</a>

<!---
  ///////////////////
  DEPRECATIONS
  //////////////////
-->

# Deprecations

As we improve the SDK, sometimes that means we need to deprecate methods or properties. A full list of deprecations by SDK version can be found in the Deprecations section of the API Reference:

<a href="http://stackmob.github.io/stackmob-ios-sdk/index.html#deprecations" target="_blank">Deprecations</a>
