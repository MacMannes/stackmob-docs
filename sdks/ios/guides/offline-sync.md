iOS SDK Caching and Offline Sync
====================

<!---
  ##########
  OVERVIEW
  ##########
-->


## Overview

The offline sync feature enables developers to build apps that can function seamlessly without network connectivity. When the network connection is restored to the device, developers can sync locally changed data with their StackMob database. With the ability to define your own merge policies, error callbacks and fetch cache policies, StackMob offers developers the ability to build powerful, feature rich apps that aren't limited by a device's network connection.

In this guide we will discuss how the cache works, options the developer has to customize the cache behavior, and the process of syncing your local objects with the server.

<!---
  ##########
  HOW THE CACHE WORKS
  ##########
-->

## How the Cache Works

The cache itself is a Core Data stack, equipped with its own private managed object context and persistent store coordinator. It uses SQLite as the persistent store. By implementing the cache as a Core Data stack, results from fetches performed on the network can be cached independently of their original request. You can then perform subsequent local fetches that are able to return subsets of the originally cached data.

<img class="screenshot" src="https://s3.amazonaws.com/static.stackmob.com/images/ios/offlinesync/caching.png" />

For example, suppose you are building a To-Do application which has the option to filter tasks by date, subject, etc. These filters would translate to conditional fetches on the same list of task objects. Rather than needing to execute a fetch on the network every time your query condition changes, you can instead grab all the tasks during application launch with one network call. From there you can perform the conditional fetches on that data locally, without needing to fetch from the network again. By reducing the amount of network calls we are drastically improving the performance of the application, as well as preserving data usage and potentially battery usage as well.

<!---
  ##########
  UPDATING YOUR DATA MODEL
  ##########
-->

## Updating Your Data Model

In order for the offline sync system to work properly, you must edit your data model to include attributes that link to the auto-generated date fields on StackMob.

The easiest way to do this is the following:

<p>1. Make a new version of your managed object model by highlighting your data model file and choosing <b>Editor -> Add Model Version...</b>.</p>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/ios-offline-sync-guide/addmodelversion.png" />

<p>2. In the new model version, add 2 attributes to every entity, <code>createddate</code> and <code>lastmoddate</code>, both of type <code>Date</code>.</p>

<p>3. Highlight your data model file and in the <b>File Inspector</b>, under <b>Versioned Core Data Model -> Current</b>, select your new version.</p>

<p>4. Finally, clean and build your Xcode project.</p>

Lightweight migration should take care of merging your new model with your old database.  If you run into the "Cannot Find Source Store" error, remove the application from the device/simulator (this removes the local database and cache mapping table files) and try running the application again.

<b>Important:</b> If your application currently uses the cache and you are upgrading from v1.x.x to v2.0.0+, the internal cache mapping table will be inconsistent with the new version. You should either remove the application from the device or call the <code>SMCoreDataStore</code> <code><i>resetCache</i></code> method once. Improved migration functionality is in development.

Now you're ready to use the caching and offline sync systems.

<!---
  ##########
  TURNING ON/OFF THE CACHE
  ##########
-->

## Turning On/Off the Cache

**The cache is turned off by default.** To turn on the cache, include `SM_CACHE_ENABLED = YES;` before you initialize the StackMob client and core data store.

<!---
  ##########
  FETCHING INTO THE CACHE
  ##########
-->

## Fetching into the Cache

After successfully performing a fetch from the StackMob database, an equivalent fetch is performed locally on the cache and those results are replaced with the up-to-date objects from the server. The results are returned as faulted managed objects. When you then access an object’s values or relationships, Core Data will use the cached version of the object to fill the fault.

There are a few scenarios where filling faults will require a network call. One is when trying to access values of related objects which themselves have not been cached. If one of these situations arise and there is no network connection Core Data may throw the “Core Data could not fulfill a fault” exception. The SDK is set up to catch this exception in most cases and error out smoothly.

<!--- CHOOSING CACHE POLICY -->

### Choosing a Cache Policy for Fetches

There are 4 policies to choose from, with type `SMCachePolicy`, which determine the location and order in which data is fetched:

* `SMCachePolicyTryNetworkOnly` – **This is the default policy.** Fetches are attempted on the server, and if an error occurs because of no network connection the fetch fails. This may be useful in situations where a developer wants all information from the server, which may be constantly changing, and does not have the need to support offline usage.

* `SMCachePolicyTryNetworkElseCache` – Fetches are attempted on the server, and if an error occurs because of no network connection, the fetch is performed on the cache. This is often used when developers are looking to pull the most recent changes from the server as much as possible, but want to have a cache in place to use when offline.

* `SMCachePolicyTryCacheOnly` – Fetches are attempted on the cache. If there are no results an empty array is returned. This may be useful when an application wants to start with a network only policy, pull down all the information they need, and switch to a cache only policy to avoid any network calls during the duration of the application.

* `SMCachePolicyTryCacheElseNetwork` – Fetches are attempted on the cache, and if the cache yields no results (empty array), the fetch is attempted on the network. This is useful for applications which want to reduce the total amount of network calls and rely on their cache more than the server. 

<!--- CHANGING CACHE POLICY -->

### Changing the Caching Policy

You can change the caching policy for your `SMCoreDataStore` instance at any time by setting its `cachePolicy` property.

You can use the `SMNetworkReachability setNetworkStatusChangeBlockWithCachePolicyReturn:` method to change the cache policy based on the current network status. An `SMNetworkReachability` instance is already initialized through the user session, under the `networkMonitor` property. 

To set a network status change block, define the following after you initialize your SMClient and SMCoreDataStore instance:

```obj-c
[client.session.networkMonitor setNetworkStatusChangeBlockWithCachePolicyReturn:^SMCachePolicy(SMNetworkStatus status) {
        // Customize this block to fit your application's needs
        if (status == SMNetworkStatusReachable) {
            return SMCachePolicyTryNetworkElseCache;
        } else {
            return SMCachePolicyTryCacheOnly;
        }
}];
```

For a list of `SMNetworkStatus` keys, visit the <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMNetworkReachability.html" target="_blank">SMNetworkReachability Class Reference</a>.

<!--- MANUALLY PURGING CACHE -->

### Manually Purging the Cache

At any time you can purge the cache manually of:

* An object by providing its managed object ID.
* A group of objects by providing an array of managed object IDs.
* A group of objects by providing an array of managed objects.
* A group of objects by providing the name of their entity.
* All objects in the cache.

Check out the **Manually Purging the Cache** section of the <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMCoreDataStore.html" target="_blank">SMCoreDataStore Class Reference</a> for all the methods.

<!--- PER REQUEST CACHE POLICY -->

### Per Request Cache Policy

Suppose you want to direct all of your fetches to the local cache, except during application launch, where you will make a few network calls to fetch down the latest server data. You can choose a specific cache policy for a save or fetch options with the `cachePolicy` property of `SMRequestOptions`.

Lets fetch all Todo objects since out last app launch:

```obj-c
NSDate *lastAppLaunch = [[NSUserdefaults standardUserDefaults] objectForKey:@"lastLaunch"];

NSFetchRequest *updatedTodos = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
[updatedTodos setPredicate:[NSPredicate predicateWithFormat:@"lastmoddate > %@", lastAppLaunch]];

SMRequestOptions *options = [SMRequestOptions options];
options.cachePolicy = SMCachePolicyTryNetworkOnly;

// We can pass nil for the callback queues to default to the main thread.
[self.managedObjectContext executeFetchRequest:updatedTodos returnManagedObjectIDs:NO successCallbackQueue:nil failureCallbackQueue:nil options:options onSuccess:^(NSArray *results) {
  // Update UI, etc
} onFailure:^(NSError *error) {
  // Handle error
}];
```

<!--
For method signature and specifics see the `SMRequestOptions` class reference: <a href="http://stackmob.github.io/stackmob-ios-sdk/Classes/SMRequestOptions.html#//api/name/optionsWithCachePolicy:" target="_blank">optionsWithCachePolicy:</a>
-->

This feature is available since v2.1.0.

<p class="alert">In v2.1.0, there is a known issue with using the optionsWithCachePolicy: class method to set a cache policy. Use the 2-step process shown above.</p>


<!---
  ##########
  WRITING TO THE CACHE
  ##########
-->

## Writing to the Cache

With the cache system enabled, writing to the cache takes place automatically in the following fashion:

During a save operation, when the network is reachable, all objects are first sent to StackMob. Upon successful insert/update/delete, the operation is then replicated on the cache.

During a save operation, when the network is not reachable, all operations are completed on the cache. Each object is marked as "dirty" until the network connection has been restored and the "sync with server" process is initiated.

<!---
  ##########
  SYNC
  ##########
-->

## Syncing with the Server

When network connectivity has been restored, the syncing process can be initialized using the `SMCoreDataStore syncWithServer` method.  

If you wish to have this process initiated automatically whenever the device goes back online, call the `syncWithServer` method in your custom defined network status change block, like so:

```obj-c,6
// You'll need a block declared core data store instance
__block SMCoreDataStore *blockCoreDataStore = self.appDelegate.coreDataStore;
[client.session.networkMonitor setNetworkStatusChangeBlockWithCachePolicyReturn:^SMCachePolicy(SMNetworkStatus status) {
        
        if (status == SMNetworkStatusReachable) {
          // Initiate sync
          [blockCoreDataStore syncWithServer];
            return SMCachePolicyTryNetworkElseCache;
        } else {
            return SMCachePolicyTryCacheOnly;
        }
}];
```

<!--- HOW IT WORKS -->

### How It Works

The syncing process starts with the list of modified object IDs that has been populated since the device was offline. Those IDs are then read from both the server and the cache and the resulting objects are then checked for conflict. If there is a conflict, a merge policy is applied to determine which representation of the object should be synced across the server and cache. Each ID thus results in a write operation, be it an update to the server, insert into the cache, etc. When the requests are all ready they are executed as one batch. If everything goes well, the sync completion callback is executed. Any errors cause error callbacks to be executed as well. Here's a diagram to demonstrate the entire process that takes place asynchronously behind the scenes:

<img class="screenshot" src="https://s3.amazonaws.com/static.stackmob.com/images/ios/offlinesync/syncing.png" />

<!--- DECIDING ON MERGE POLICY -->

### Deciding on a Merge Policy

During the sync process, the goal is to send all the save operations completed while offline to the StackMob server, the end result being that client representations of data match the server representations. Conflicts occur when objects modified locally are also modified on the server while the device was offline. If there are conflicts, merge policies are used to decide which representation of an object, client or server, should be synced across the local and remote databases.

The general algorithm for the sync with server process is as follows:

1. Process dirty inserted objects.

2. For each object,

  a. Read object from server

    b. Read object from cache

  c. If conflict, apply merge policy, a block which evaluates the client and server objects and returns a "winner".

  d. If client wins, send request to replace object on server with cache representation.

  e. else server wins, replace object in cache with server representation

3. Repeat step 2 for dirty updated objects followed by dirty deleted objects.

6. When finished, call completion callback as well as any error callbacks, if needed.

<!--- SETTING PREDEFINED POLICY -->

### Setting a Predefined Merge Policy

StackMob gives developers three predefined merge policies to use, and the ability to define your own custom policy. The three predefined policies are:

* `SMMergePolicyServerModifiedWins` - **This is the default policy.** The server object wins if it has been modified since the object was last read when the device was online.
* `SMMergePolicyClientWins` - Simply stated, the client object representation wins.
* `SMMergePolicyLastModifiedWins` - The `lastmoddate` property of the client and server object representations are compared, and the object with the most recent date is the winner.

A default policy can be set using the `SMCoreDataStore defaultSMMergePolicy` property, which can be considered the global merge policy. Here's an example of setting the client wins policy:

```obj-c
self.coreDataStore.defaultSMMergePolicy = SMMergePolicyClientWins;
```

Developers also have the option of setting different merge policies for inserts, updates, and deletes. The per-operation merge policy properties available to set are:

* insertsSMMergePolicy
* updatesSMMergePolicy
* deletesSMMergePolicy

<!--- SETTING CUSTOM MERGE POLICY -->

### Setting a Custom Merge Policy

You can conduct your own merge strategies by defining and setting a block rather than choosing a predefined policy. You are given a dictionary representation of the client and server objects, as well as the date of the last time the object was read from the server. The block must return "client" or "server", represented by the `SMMergeObjectKey` typedef:

```obj-c
typedef enum {
    SMClientObject = 0,
    SMServerObject = 1,
} SMMergeObjectKey;

typedef int (^SMMergePolicy)(NSDictionary *clientObject, NSDictionary *serverObject, NSDate *serverBaseLastModDate);
```

Here's an example of defining your own merge policy, using the server modified wins policy implementation as an example:

```obj-c
self.coreDataStore.defaultSMMergePolicy = ^(NSDictionary *clientObject, NSDictionary *serverObject, NSDate *serverBaseLastModDate){
    
    NSDate *serverLastModDate = [serverObject objectForKey:SMLastModDateKey];
    if (![serverBaseLastModDate isEqualToDate:serverLastModDate]) {
        return SMServerObject;
    } else {
        return SMClientObject;
    }
    
};
```

<!--- SYNC SUCCESS -->

### Successful Sync Callback

Developers have the option of defining a callback that is executed whenever a sync completes. This is the perfect time to save the in-memory context and reload objects from the cache to pick up on any new changes

It is passed all objects that were successfully synced with the server in accordance with the merge policy. Set the callback like so:

```obj-c
[self.coreDataStore setSyncCompletionCallback:^(NSArray *objects){
    // Handle successfully synced objects
}];
```

The objects passed will be of type `SMSyncedObject`, a class with two properties:

<ol>
    <li>objectID - This will be the managed object ID of the synced object, or if the object was deleted it will be the primary key in `NSString` form.</li>
    <li>actionTaken - An `SMSyncAction`, defined as:</li>
</ol>

```obj-c
typedef enum {
    SMSyncActionInsertedOnServer = 0,
    SMSyncActionUpdatedOnServer = 1,
    SMSyncActionDeletedFromServer = 2,
    SMSyncActionUpdatedCache = 3,
} SMSyncAction;
```

<!--- SYNC FAILURE -->

### Handling Merge Errors

Sometimes, objects will not be properly synced to StackMob due to permissions, bad values sent to the server, etc. Because syncing with the server is an asynchronous event, developers have the option of defining callbacks for each type of operation (insert/update/delete) in order to handle failed merges. Set the callback properties using:

* setSyncCallbackForFailedInserts:
* setSyncCallbackForFailedUpdates:
* setSyncCallbackForFailedDeletes:

The structure of the dictionary passed to the callback is exactly like the error user info of a failed Core Data operation (using the <a href="" target="_blank">iOS SDK concurrency API methods</a>). The format is the following:

```obj-c,2-5
[
  {
    SMFailedManagedObjectID : <NSManagedObjectID instance of object>,
    SMFailedManagedObjectError : <NSError instance describing why this object merge failed>
  },
  {
    ...
  }
]
```

When an error occurs while syncing an object, it remains marked as "dirty" i.e. not synced. To mark an object as synced and optionally purge it from the cache as well, pass the dictionary entry to the `SMCoreDataStore markObjectAsSynced:purgeFromCache:` method. To mark multiple entries with one call use `markArrayOfObjectsAsSynced:purgeFromCache:`.

<!---
  ##########
  MISC
  ##########
-->

## Other Utility Properties/Methods

The following are utility properties/methods to assist in your offline sync implementation:

<b>v2.1.0</b>

* <b><i>optionsWithCacheResults:</i> (SMRequestOptions)</b> - For a particular save or fetch request you may choose to not cache the results. Simply create an `SMRequestOptions` instance, set the `cacheResults` property to NO, and pass the options to your save/fetch request. For simplicity you can also initialize with the `[SMRequestOptions optionsWithCacheResults:NO]` class method.


<b>v2.0.0</b>

* <b>SM_ALLOW_CACHE_RESET</b> flag - Use during development when changing your Core Data model often to allow the local Core Data stack to reset itself if needed.
* <b><i>syncInProgress</i> (SMCoreDataStore)</b> - Boolean property which returns YES while a sync with the server is taking place.  Otherwise NO.
* <b><i>isDirtyObject:</i> (SMCoreDataStore)</b> - Takes the managed object ID of an object and returns YES if the object has not yet been synced to the server.  Otherwise returns NO.
* <b><i>sendLocalTimestamps</i> (SMCoreDataStore)</b> - Boolean indicating whether to send the `createddate` and `lastmoddate` keys and values in a request payload (during sync only). 
* <b><i>stringContainsURL:</i> (SMBinaryDataConversion)</b> - Returns whether the value of a string attribute contains an S3 URL or raw data.  This is for string attributes which map to a binary field on StackMob.  The value would be raw data if the object was saved offline and hasn't yet been synced with the server.
* <b><i>dataForString:</i> (SMBinaryDataConversion)</b> - If the value of a string attribute is raw data (because the object has not yet been synced with the server), call this method to translate it back to data.

<!---
  ##########
  BEST PRACTICES
  ##########
-->

## Best Practices

While caching and syncing works out of the box with the iOS SDK, it is important to think about what data you are caching, when you are caching it, and when in your app you are initiating the syncing process. Every app is unique and will have different use cases which determine the best time for caching and syncing. By thinking through a solid caching and syncing strategy, you will produce a more performant and error prone app.

Many of the the features are customizable to fit your app's needs, and we encourage you to try out different combinations of methods to find the right balance of caching and syncing for as smooth of a user experience as possible. Here are some best practices for specific scenarios we often see:

<!--- FETCHING AT LAUNCH -->

### Fetching at Launch

Consider this scenario:

Your app launches and the device is online. Your initial view is connected to a view controller which loads a table view with data. You have also defined a network status change block to sync with the server if the network is reachable. 

If you were to do a little debugging, you would find that the order these processes happen in is:

* Load initial view, which causes the initial view controller to fetch data. If you didn't define a specific cache policy when you initialized your core data store, it uses the default which fetches from StackMob (network).
* All initial processing is complete, and the network status change block can now be executed on the main thread, which initiates a sync with the server.

Now here's the conflict: When you fetched from the network, it replaced any objects in the cache which matched the fetch with the updated ones from the network. This potentially erased objects that needed to be synced. From the developer's perspective it appears that changes to those objects are lost.

But this is not the case! We just need to re-order our operations to allow the sync to complete before we allow our table view to fetch from the network.

Here's the solution: Set your core data store to fetch from the cache, then once the sync completes, update the policy to fetch from the network, and reload the table view, etc.

Here's an example of an `application:didFinishLaunchingWithOptions:` method which accomplishes this:

```obj-c
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    
    SM_CACHE_ENABLED = YES;
        
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    self.coreDataStore.cachePolicy = SMCachePolicyTryCacheOnly;
    
    __block SMCoreDataStore *blockCoreDataStore = self.coreDataStore;
    
    [self.client.networkMonitor setNetworkStatusChangeBlock:^(SMNetworkStatus status) {
        if (status == SMNetworkStatusReachable) {

            // Initiate sync
            [blockCoreDataStore syncWithServer];
        }
        else {
            // Handle offline mode
        }
    }];
    
    [self.coreDataStore setSyncCompletionCallback:^(NSArray *objects){
        
        // Our syncing is complete, so change the policy to fetch from the network
        [blockCoreDataStore setCachePolicy:SMCachePolicyTryNetworkElseCache];

        // Notify other views that they should reload their data from the network
        [[NSNotificationCenter defaultCenter] postNotificationName:@"FinishedSync" object:nil];
    }];
    
    return YES;
}
```


<!--- PREPARING FOR OFFLINE -->

### Preparing for Offline

Consider a database model which has a tree-like hierarchy: Users have posts, posts have comments, comments have likes, and so on and so forth. Your app views probably have a similar hierarchy: Your first view shows all the posts for a user, and when you click on a post the comments for that post are shown, and if you expand the comment details you can see all the likes, etc. The bottom line is that unless all that object data is cached prior to the device going offline, it won't be available when the user goes to specific views. The deeper the content lies in the hierarchy, the more likely it won't have been previously fetched.

Remember that content using Core Data is loaded incrementally. When you fetch a user object, the cache will know that there are posts related to that user, but won't have any of the actual data. It's not until the app specifically calls for that data that the post objects are fetched from the server.

This section is really just food for thought - the best practice here is to fully test your app both online and offline. Was your app designed to work out of the box offline, or do you anticipate that a user will use most of the app online before they ever lose network connection? In that case, you may find that all the content is cached and there are no issues with missing content offline. If you notice data is missing, it's less likely that syncing didn't work and more likely that the data hadn't been fetched when the device was online. You may need to add a few lines of code to pre-fetch some important pieces of data for anticipated offline viewing.

Alternatively, some apps notify their users when the device is offline, and that not all content is available in that state.


<!--- DEBUG FAILED SYNCS -->

### Debugging Failed Syncs

One important use for defining sync failure callbacks is for debugging purposes.

When you are building your app, logging failed synced objects will give you insight as to changes you may need to make so syncs don't fail when your app is in production.

If you consistently see certain objects not being synced, there may be a clear answer depending on the error.

Here are some common (fixable) sync errors:

* 400 (SMErrorBadRequest) - The occurs most often when you have defined a new field or relationship in your managed object model, and your code doesn't set a value for that field before the object is synced. This happens because sync requests send entire objects, including <i>null</i> for any fields or relationships that have not been set. When StackMob sees <i>null</i> for a field you have not created yet on the server, it will return a 400 because it does not know what type to infer the field as. <b>Solution:</b> Quickly go to the StackMob dashboard and manually create the field/relationship in your schema.

* 401 (SMErrorUnauthorized) - You do not have permission to sync whatever object if failing. For example, you may not be logged in when the sync initiates, and this will cause a 401 to be returned if you have permissions on the corresponding schema. <b>Solution:</b> Make adjustments to your code to ensure that your user is logged in, etc. before the app initiates a sync.

If you run into any 500 responses (SMErrorInternalServerError), you should reach out to the StackMob team.



