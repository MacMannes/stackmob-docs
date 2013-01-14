/*
 * Copyright 2012 StackMob
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <CoreData/CoreData.h>
#import "StackMob.h"

/**
 Category which provides methods for performing 
 */
@interface NSManagedObjectContext (Concurrency)

/**
 Asynchronous save method.
 
 A callback based save method which pushes changes to private parent context and saves in the background, off of the main thread.  Callbacks are performed on the main thread.  Use <saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks.
 
 @param successBlock <i>typedef void (^SMSuccessBlock)())</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 */
- (void)saveOnSuccess:(SMSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous save method.
 
 A callback based save method which pushes changes to private parent context and saves in the background, off of the main thread.
 
 @param successCallbackQueue Upon successful save, the queue to perform the success block on.
 @param failureCallbackQueue Upon unsuccessful save, the queue to perform the failure block on.
 @param successBlock <i>typedef void (^SMSuccessBlock)())</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 */
- (void)saveWithSuccessCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue onSuccess:(SMSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Synchronous save method.
 
 This method works like the NSManagedObjectContext save: method, but pushes changes to private parent context which in turns saves to the persistent store.
 
 @param error Points to the error object if the save is unsuccessful.
 
 @return Whether the save was successful or not.
 */
- (BOOL)saveAndWait:(NSError *__autoreleasing*)error;

/**
 Asynchronous fetch method.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  Managed object IDs that are returned are converted to instances of NSManagedObject by the calling context.
 
 Callbacks are performed on the main thread.  Use <executeFetchRequest:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks on.
 
 @param request The fetch request to perform against the database.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context, containing an array of results as instances of NSManagedObject.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous fetch method with the option of returning instances of NSManagedObjectID.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  If returnIDs is YES, managed object IDs that are returned by the fetch are converted to instances of NSManagedObject by the calling context.
 
 Callbacks are performed on the main thread.  Use <executeFetchRequest:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks on.
 
 @param request The fetch request to perform against the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous fetch method.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  If returnIDs is YES, managed object IDs that are returned by the fetch are converted to instances of NSManagedObject by the calling context.
 
 Callbacks are performed on the main thread.  Use <executeFetchRequest:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks on.
 
 @param request The fetch request to perform against the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param successCallbackQueue Upon successful fetch, the queue to perform the success block on.
 @param failureCallbackQueue Upon unsuccessful fetch, the queue to perform the failure block on.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs successCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Synchronous fetch method.
 
 This method works like the NSManagedObjectContext executeFetchRequest:error: method, but executes fetch request on background context.  Managed object IDs that are returned are converted to managed objects on the calling context.
 
 @param request The fetch to perform on the database.
 @param error Points to the error object if the fetch is unsuccessful.
 
 @return An array of NSManagedObject instances matching the request, nil if there was an error.
 */
- (NSArray *)executeFetchRequestAndWait:(NSFetchRequest *)request error:(NSError *__autoreleasing *)error;

/**
 Synchronous fetch method with the option to return results as instances of NSManagedObjectID.
 
 This method works like the NSManagedObjectContext executeFetchRequest:error: method, but executes fetch request on background context.  Managed object IDs that are returned are converted to managed objects on the calling context.
 
 @param request The fetch to perform on the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param error Points to the error object if the fetch is unsuccessful.
 
 @return An array of NSManagedObject or NSManagedObjectID instances matching the request, nil if there was an error.
 */
- (NSArray *)executeFetchRequestAndWait:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs error:(NSError *__autoreleasing *)error;

/**
 Allows context to be notified when contextToObserve posts the NSManagedObjectContextDidSaveNotification notification.
 
 When notification arrives, mergeChangesFromContextDidSaveNotification: is called.
 
 @param contextToObserve The object to observe for notification posts.
 */
- (void)observeContext:(NSManagedObjectContext *)contextToObserve;

/**
 Removes context from observing NSManagedObjectContextDidSaveNotification notifications from contextToStopObserving.
 
 @param contextToStopObserving The object to stop observing for notification posts.
 */
- (void)stopObservingContext:(NSManagedObjectContext *)contextToStopObserving;

/**
 Adds/Removes observer for NSManagedObjectContextWillSaveNotification notification.
 
 When using the child / parent context pattern, child contexts should obtain permanent IDs for newly inserted objects before pushing save requests to parent contexts.  This method is automatically set to YES for mainThreadContext as well as private queue contexts that are created by contextForCurrentThread.
 
 @param value If YES, adds an observer for NSManagedObjectContextWillSaveNotification, which upon receiving a notification calls obtainPermanentIDsForObjects:, passing the context's inserted objects. 
 */
- (void)setContextShouldObtainPermanentIDsBeforeSaving:(BOOL)value;

@end