Basic Queries
=============

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/basic-query.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Query your data and display results in a table view.

### Experience Level
Beginner

### Estimated time to complete
~5 minutes

### Prerequisites

* XCode 4.x and greater

* iOS SDK 5 and greater

* [Download StackMob Read into Table View App](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read-into-table.zip)

**Have you read through the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#CodingPractices" target="_blank">Core Data Integration Coding Practices</a>?**

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

## Open the StackMob Read to TableView

We’ll use the project from the Read into Table View Tutorial where we read all objects and display them in a table view.  It has StackMob imported, Core Data and a fetchRequest setup to display data in the table view.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the Read into Table View app, see <a href="https://developer.stackmob.com/ios-sdk/read-into-table-view-tutorial" target="_blank">StackMob iOS Read into Table View</a>.

Unzip the StackMob Read into Table View App and open **read-into-table.xcodeproj**.

## Add your Public Key
Go to <a href="https://dashboard.stackmob.com/settings" target="_blank">Manage App Info</a> in the StackMob Dashboard and copy the **Development Public Key** and paste it  into the **AppDelegate.m** file where is says **YOUR\_PUBLIC\_KEY** in the method:

```obj-c,4
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    return YES;
}
```

## Edit ListViewController.m

The NSPredicate class is used to define logical conditions used to constrain a search  for a fetch.  

Add the following **highlighted** code to your ListViewController.m file:

```obj-c,19-20


- (NSFetchedResultsController *)fetchedResultsController
{
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
    }
    
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    // Edit the entity name as appropriate.
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    [fetchRequest setEntity:entity];
    
    // Edit the sort key as appropriate.
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"title" ascending:YES];
    NSArray *sortDescriptors = [NSArray arrayWithObjects:sortDescriptor, nil];
    
    [fetchRequest setSortDescriptors:sortDescriptors];
    
    // EQUAL EXAMPLE
    NSPredicate *equalPredicate =[NSPredicate predicateWithFormat:@"title == %@", @"Hello World"];
    [fetchRequest setPredicate:equalPredicate];
    
    // Edit the section name key path and cache name if appropriate.
    // nil for section name key path means "no sections".
    NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Master"];
    aFetchedResultsController.delegate = self;
    self.fetchedResultsController = aFetchedResultsController;
    
    NSError *error = nil;
    if (![self.fetchedResultsController performFetch:&error]) {
        NSLog(@"An error %@, %@", error, [error userInfo]);
    }
    
    return _fetchedResultsController;
}
```

## Build and Run!

Replace "Hello World" with a Todo title that matches an object in your StackMob schema, then run your project.  You should only see results that match the query.

## Additional Predicates
Below are more examples of NSPredicates we support.


```obj-c

    // EQUAL ID Example
    NSPredicate *equalByIdPredicate =[NSPredicate predicateWithFormat:@"todoId == %@", @"3143B4F8-88C6-4E07-8ED4-74F0B9AF779C"];
    [fetchRequest setPredicate:equalByIdPredicate];
    
    // LESS THAN EXAMPLE
    NSPredicate *lessThanPredicate =[NSPredicate predicateWithFormat:@"count < %d", 4];
    [fetchRequest setPredicate:lessThanPredicate];
    
    // LESS THAN OR EQUAL THAN EXAMPLE
    NSPredicate *lessThanOrEqualPredicate =[NSPredicate predicateWithFormat:@"count <= %d", 5];
    [fetchRequest setPredicate:lessThanOrEqualPredicate];
    
    // GREATER THAN EXAMPLE
    NSPredicate *greaterThanPredicate =[NSPredicate predicateWithFormat:@"count > %d", 4];
    [fetchRequest setPredicate:greaterThanPredicate];
    
    // GREATER THAN OR EQUAL EXAMPLE
    NSPredicate *greaterThanOrEqualPredicate =[NSPredicate predicateWithFormat:@"count >= %d", 10];
    [fetchRequest setPredicate:greaterThanOrEqualPredicate];
    
    // NOT EQUAL EXAMPLE
    NSPredicate *notEqualPredicate =[NSPredicate predicateWithFormat:@"title != %@", @"One More"];
    [fetchRequest setPredicate:notEqualPredicate];
    
    // IN EXAMPLE
    NSArray *choice = [[NSArray alloc]initWithObjects:@"maybe",@"never",nil];
    NSPredicate *inPredicate =[NSPredicate predicateWithFormat:@"choice IN %@", choice];
    [fetchRequest setPredicate:inPredicate];
    
    //BETWEEN EXAMPLE
    NSArray *range = [[NSArray alloc]initWithObjects:@"6",@"10",nil];
    NSPredicate *betweenPredicate =[NSPredicate predicateWithFormat:@"count between %@", range];
    [fetchRequest setPredicate:betweenPredicate];
```

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/basic-query.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>