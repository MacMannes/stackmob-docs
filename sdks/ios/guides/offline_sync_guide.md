<h1>iOS SDK Offline Sync</h1>

The offline sync feature enables developers to build apps that can function seamlessly without network connectivity. When the network connection is restored to the device, developers can sync locally changed data with their StackMob database. With the ability to define your own merge policies, error callbacks and fetch cache policies, StackMob offers developers the ability to build powerful, feature rich apps that aren't limited by a device's network connection.

In this guide we will discuss how the cache works, options the developer has to customize the cache behavior, and the process of syncing your local objects with the server.

<h2>How the Cache Works</h2>

The cache itself is a Core Data stack, equipped with its own private managed object context and persistent store coordinator. It uses SQLite as the persistent store. By implementing the cache as a Core Data stack, results from fetches performed on the network can be cached independently of their original request. You can then perform subsequent local fetches that are able to return subsets of the originally cached data. 

For example, suppose you are building a To-Do application which has the option to filter tasks by date, subject, etc. These filters would translate to conditional fetches on the same list of task objects. Rather than needing to execute a fetch on the network every time your query condition changes, you can instead grab all the tasks during application launch with one network call. From there you can perform the conditional fetches on that data locally, without needing to fetch from the network again. By reducing the amount of network calls we are drastically improving the performance of the application, as well as preserving data usage and potentially battery usage as well.

<h2>Updating your Data Model</h2>

In order for the offline sync system to work properly, you must edit your data model to include attributes that link to the auto-generated date fields on StackMob.

The easiest way to do this is the following:

<ol>

<li>Make a new version of your managed object model by highlighting your data model file and choosing <b>Editor -> Add Model Version...</b>.</li>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/ios-offline-sync-guide/addmodelversion.png" />

<li>In the new model version, define a new entity named StackMob.</li>
<li>Add 2 attributes, <code>createddate</code> and <code>lastmoddate</code>, both of type <code>Date</code>.</li>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/ios-offline-sync-guide/stackmobentity.png" />

<li>In the <b>Data Model Inspector</b>, check <b>Abstract Entity</b>.</li>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/ios-offline-sync-guide/absractentity.png" />

<li>In the <b>Data Model Inspector</b> of every other entity in your model, choose <b>StackMob</b> as the <b>Parent Entity</b>.</li>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/ios-offline-sync-guide/parententity.png" />

<li>Highlight your data model file and in the <b>File Inspector</b>, under <b>Versioned Core Data Model -> Current</b>, select your new version.</li>
<li>Finally, clean and build your Xcode project.</li>

</ol>

If you choose the Parent Entity route, be sure to remove any existing attributes that map to `createddate` or `lastmoddate`, as to avoid duplicates.

Lightweight migration should take care of merging your new model with your old database.  If you run into the "Cannot Find Source Store" error, remove the application from the device/simulator (this removes the local database and cache mapping table files) and try running the application again.

<b>Important:</b> If your application currently uses the cache and you are upgrading to v2.0.0+, the internal cache mapping table will be inconsistent with the new version. You should either remove the application from the device or call the <code>SMCoreDataStore</code> <code><i>resetCache</i></code> method once. Improved migration functionality is in development.

Now you're ready to use offline sync.

<h2>Turning On/Off the Cache</h2>

**The cache is turned off by default.** To turn on the cache, include `SM_CACHE_ENABLED = YES;` before you initialize the StackMob client and core data store.

<h2>Fetching into the Cache</h2>

After successfully performing a fetch from the StackMob database, an equivalent fetch is performed locally on the cache and those results are replaced with the up-to-date objects from the server. The results are returned as faulted managed objects. When you then access an object’s values or relationships, Core Data will use the cached version of the object to fill the fault.

There are a few scenarios where filling faults will require a network call. One is when trying to access values of related objects which themselves have not been cached. If one of these situations arise and there is no network connection Core Data may throw the “Core Data could not fulfill a fault” exception. The SDK is set up to catch this exception in most cases and error out smoothly.

<h3>Choosing a Cache Policy for Fetches</h3>

There are 4 policies to choose from, with type `SMCachePolicy`, which determine the location and order in which data is fetched:

* `SMCachePolicyTryNetworkOnly` – **This is the default policy.** Fetches are attempted on the server, and if an error occurs because of no network connection the fetch fails. This may be useful in situations where a developer wants all information from the server, which may be constantly changing, and does not have the need to support offline usage.

* `SMCachePolicyTryNetworkElseCache` – Fetches are attempted on the server, and if an error occurs because of no network connection, the fetch is performed on the cache. This is often used when developers are looking to pull the most recent changes from the server as much as possible, but want to have a cache in place to use when offline.

* `SMCachePolicyTryCacheOnly` – Fetches are attempted on the cache. If there are no results an empty array is returned. This may be useful when an application wants to start with a network only policy, pull down all the information they need, and switch to a cache only policy to avoid any network calls during the duration of the application.

* `SMCachePolicyTryCacheElseNetwork` – Fetches are attempted on the cache, and if the cache yields no results (empty array), the fetch is attempted on the network. This is useful for applications which want to reduce the total amount of network calls and rely on their cache more than the server. 

<h3>Changing the Caching Policy</h3>

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

<h3>Manually Purging the Cache</h3>

At any time you can purge the cache manually of:

* An object by providing its managed object ID.
* A group of objects by providing an array of managed object IDs.
* A group of objects by providing an array of managed objects.
* A group of objects by providing the name of their entity.
* All objects in the cache.

Check out the <a href="" target="_blank">Manually Purging the Cache</a> section of the `SMCoreDataStore` Class Reference for all the methods.

<h2>Writing to the Cache</h2>

With the cache system enabled, writing to the cache takes place automatically in the following fashion:

During a save operation, when the network is reachable, all objects are first sent to StackMob. Upon successful insert/update/delete, the operation is then replicated on the cache.

During a save operation, when the network is not reachable, all operations are completed on the cache. Each object is marked as "dirty" until the network connection has been restored and the "sync with server" process is initiated.

<h2>Syncing with the Server</h2>

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

<h3>Deciding on a Merge Policy</h3>

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

<h3>Setting a Predefined Merge Policy</h3>

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

<h3>Setting a Custom Merge Policy</h3>

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

<h3>Successful Sync Callback</h3>

Developers have the option of defining a callback that is executed whenever a sync completes. This is the perfect time to save the in-memory context and reload objects from the cache to pick up on any new changes

It is passed all objects that were successfully synced with the server in accordance with the merge policy. Set the callback like so:

```obj-c
[self.coreDataStore setSyncCompletionCallback:^(NSArray *objects){
    // Handle successfully synced objects
}];
```

The objects passed will be of type `SMSyncedObject`, a class with two properties:

1. objectID - This will be the managed object ID of the synced object, or if the object was deleted it will be the primary key in `NSString` form.
2. actionTaken - An `SMSyncAction`, defined as:

```obj-c
typedef enum {
    SMSyncActionInsertedOnServer = 0,
    SMSyncActionUpdatedOnServer = 1,
    SMSyncActionDeletedFromServer = 2,
    SMSyncActionUpdatedCache = 3,
} SMSyncAction;
```

<h3>Handling Merge Errors</h3>

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

<h2>Additional Utility Properties/Methods</h2>

The following are utility properties/methods to assist in your offline sync implementation:

* <b><i>syncInProgress</i> (SMCoreDataStore)</b> - Boolean property which returns YES while a sync with the server is taking place.  Otherwise NO.
* <b><i>isDirtyObject:</i> (SMCoreDataStore)</b> - Takes the managed object ID of an object and returns YES if the object has not yet been synced to the server.  Otherwise returns NO.
* <b><i>sendLocalTimestamps</i> (SMCoreDataStore)</b> - Boolean indicating whether to send the `createddate` and `lastmoddate` keys and values in a request payload (during sync only). 
* <b><i>stringContainsURL:</i> (SMBinaryDataConversion)</b> - Returns whether the value of a string attribute contains an s3 url or raw data.  This is for string attributes which map to a binary field on StackMob.  The value would be raw data if the object was saved offline and hasn't yet been synced with the server.
* <b><i>dataForString:</i> (SMBinaryDataConversion)</b> - If the value of a string attribute is raw data (because the object has not yet been synced with the server), call this method to translate it back to data.



