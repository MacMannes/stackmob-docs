Advanced Queries
================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/advanced-query.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Create a complex query your data and display results in a table view.

### Experience Level
Beginner

### Estimated time to complete

~5 minutes

### Prerequisites

* XCode 4.x and greater

* iOS SDK 5 and greater

* StackMob iOS SDK 1.3.0+

* [Download StackMob Read into Table View App](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read-into-table.zip)

**Have you read through the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#CodingPractices" target="_blank">Core Data Integration Coding Practices</a>?**

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.


## Open the StackMob Read to TableView

We’ll use the project from the Read to TableView Tutorial where we read all objects and display them in a table view.  It has StackMob imported, Core Data and a fetchRequest setup to display data in the table view.  This allows you to focus on the objective of this tutorial.

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


## Modify our Todo Entity
We are going to modify our Todo Entity to add a new attribute.  Click on **mydatamodel.xcdatamodeld**, select the **Todo** Entity and click the plus sign to add a new attribute.
Give the new attribute the name **count** with a data type of **Integer16**


<br><br>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/adv-query/adv-query.png">
<br><br>


## Edit ListViewController.m

Let's add some code to our ViewDidLoad method to create a new object that has both a title and a count.  Make sure to **import StackMob** at the top of the file.


Add the following **highlighted** code to your ListViewController.m file:


```obj-c,3,13-23
#import "ListViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

// Other methods remain unedited

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
    [newManagedObject setValue:@"Hello World" forKey:@"title"];
    [newManagedObject setValue:[NSNumber numberWithInt:5] forKey:@"count"];
    [newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
    
    [self.managedObjectContext saveOnSuccess:^{
        NSLog(@"You created a new object!");
    } onFailure:^(NSError *error) {
        NSLog(@"There was an error! %@", error);
    }];
    
}
```

## Add an AND Compound NSPredicate

The NSPredicate class is used to define logical conditions used to constrain a search  for a fetch.  You can pass an array of NSPredicates to NSCompoundPredicate to create a predicate in the form "predicate1" AND "predicate2".

Add the following **highlighted** code to your ListViewController.m file:

```obj-c,14-23

- (NSFetchedResultsController *)fetchedResultsController
{
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
    }
    
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
    
    // Edit the sort key as appropriate.
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"title" ascending:YES];
    NSArray *sortDescriptors = [NSArray arrayWithObjects:sortDescriptor, nil];
    [fetchRequest setSortDescriptors:sortDescriptors];
    
    // EQUAL EXAMPLE
    NSPredicate *equalPredicate =[NSPredicate predicateWithFormat:@"title == %@", @"Hello World"];
    
    // LESS THAN EXAMPLE
    NSPredicate *lessThanPredicate =[NSPredicate predicateWithFormat:@"count < %d", 2];
    
    // COMPOUND PREDICATE using AND
    NSArray *predicates = [[NSArray alloc]initWithObjects:equalPredicate,lessThanPredicate,nil];
    NSPredicate *compoundPredicate =[NSCompoundPredicate andPredicateWithSubpredicates:predicates];
    [fetchRequest setPredicate:compoundPredicate];

    
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

Run your project.  You should only see results that match the query with a title of Hello World and a count greater than 2.

## Additional Predicates

Below are more examples of Comparison Predicates we support.


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
    
    // BETWEEN EXAMPLE
    NSArray *range = [[NSArray alloc]initWithObjects:@"6",@"10",nil];
    NSPredicate *betweenPredicate =[NSPredicate predicateWithFormat:@"count between %@", range];
    [fetchRequest setPredicate:betweenPredicate];

```

## OR Compound Predicate

Multiple predicates can be OR'ed together to create a fetch request with conditions "A" OR "B" OR "C".  Here's an example:

```obj-c

    // Fetch on Entity Person where "armor_class == 17 OR first_name == Jonah"
    NSFetchRequest *request = [[NSFetchRequest alloc] initWithEntityName:@"Person"];

    NSPredicate *allOrs = [NSCompoundPredicate orPredicateWithSubpredicates:[NSArray arrayWithObjects:[NSPredicate predicateWithFormat:@"armor_class == %@", [NSNumber numberWithInt:17]], [NSPredicate predicateWithFormat:@"first_name == %@", @"Jonah"], nil]];
    [request setPredicate:allOrs];

    [self.manangedObjectContext executeFetchRequest:request onSuccess:^(NSArray *results){
        // handle success
    } onFailure:^(NSError *error){
        // handle error
    }];         

```

Multiple predicates ANDed together can then be ORed i.e. ("A" AND "B") OR "C" OR "D".  Here's an example:

```obj-c

    // Fetch on Entity Person where "(first_name == 'Bob' AND last_name == 'Smith') OR first_name == 'John' OR company == 'StackMob'"  
    NSFetchRequest *request = [[NSFetchRequest alloc] initWithEntityName:@"Person"];

    NSPredicate *firstCondition = [NSCompoundPredicate andPredicateWithSubpredicates:
                                     [NSArray arrayWithObjects:
                                      [NSPredicate predicateWithFormat:@"first_name == %@", @"Bob"],
                                      [NSPredicate predicateWithFormat:@"last_name == %@", @"Smith"],
                                      nil]];
            
    NSPredicate *secondCondition = [NSPredicate predicateWithFormat:@"first_name == %@", @"John"];
                              
    NSPredicate *thirdCondition = [NSPredicate predicateWithFormat:@"company == %@", @"StackMob"];
    NSPredicate *allPredicates = [NSCompoundPredicate orPredicateWithSubpredicates:[NSArray arrayWithObjects:firstCondition, secondCondition, thirdCondition, nil]];
    
    [request setPredicate:allPredicates];

    [self.manangedObjectContext executeFetchRequest:request onSuccess:^(NSArray *results){
        // handle success
    } onFailure:^(NSError *error){
        // handle error
    }];         

```
 
Finally, the entire OR query can be chained together with any number of ANDs, like "A" AND "B" AND ( ("C1" AND "C2") OR "D" OR "E" ).
 
**Important:** Only one set of ORs can be used in a fetch.  For example:
 
Right: "A" AND "B" AND ( ("C" AND "D") OR "E" OR "F" )
 
Wrong: "A" AND "B" AND ( ("C" AND "D") OR "E" OR "F" ) AND ( "G" OR "H" )  // The second OR containing G/H is not allowed.

Here is a final, more complex example:

```obj-c

    // Fetch on Entity Person where "armor_class < 17 AND ( (first_name == 'Bob' AND last_name == 'Smith') OR (first_name == 'Jonh' AND last_name == 'Doe') OR company == 'StackMob' )" 
    NSFetchRequest *request = [[NSFetchRequest alloc] initWithEntityName:@"Person"];

    NSPredicate *firstCondition = [NSCompoundPredicate andPredicateWithSubpredicates:
                                     [NSArray arrayWithObjects:
                                      [NSPredicate predicateWithFormat:@"first_name == %@", @"Bob"],
                                      [NSPredicate predicateWithFormat:@"last_name == %@", @"Smith"],
                                      nil]];
            
    NSPredicate *secondCondition = [NSCompoundPredicate andPredicateWithSubpredicates:
                                     [NSArray arrayWithObjects:
                                      [NSPredicate predicateWithFormat:@"first_name == %@", @"John"],
                                      [NSPredicate predicateWithFormat:@"last_name == %@", @"Doe"],
                                      nil]];
                              
    NSPredicate *thirdCondition = [NSPredicate predicateWithFormat:@"company == %@", @"StackMob"];

    NSPredicate *orPredicates = [NSCompoundPredicate orPredicateWithSubpredicates:[NSArray arrayWithObjects:firstCondition, secondCondition, thirdCondition, nil]];

    NSPredicate *predicateForFetch = [NSCompoundPredicate andPredicateWithSubpredicates:[NSArray arrayWithObjects:[NSPredicate predicateWithFormat:@"armor_class < %@", [NSNumber numberWithInt:17]], orPredicates, nil]];

    [request setPredicate:predicateForFetch];

    [self.manangedObjectContext executeFetchRequest:request onSuccess:^(NSArray *results){
        // handle success
    } onFailure:^(NSError *error){
        // handle error
    }];         

```

## SMPredicate 

SMPredicate is a special subclass of NSPredicate that allows the querying of Geo Points.

```obj-c

    // Here's an SMGeoPoint
    NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
    NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];
    SMGeoPoint *geoPoint = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];

    // Here's another SMGeoPoint
    CLLocationCoordinate2D coordinate;
    coordinate.latitude = 37.755245;
    coordinate.longitude = -122.447741;  
    SMGeoPoint *geoPoint2 = [SMGeoPoint geoPointWithCoordinate:swCoordinate];

    // WITHIN MILES EXAMPLE
    SMPredicate *predicate = [SMPredicate predicateWhere:@"geopoint" isWithin:3.5 milesOfGeoPoint:geoPoint];

    // WITHIN KILOMETERS EXAMPLE
    SMPredicate *predicate = [SMPredicate predicateWhere:@"geopoint" isWithin:5.0 kilometersOfGeoPoint:geoPoint];

    // WITHIN BOUNDS EXAMPLE
    SMPredicate *predicate = [SMPredicate predicateWhere:@"geopoint" isWithinBoundsWithSWGeoPoint:geoPoint2 andNEGeoPoint:geoPoint];

    // NEAR EXAMPLE 
    SMPredicate *predicate = [SMPredicate predicateWhere:@"geopoint" nearGeoPoint:geoPoint];

```

You can use SMPredicate with NSCompoundPredicate. 

```obj-c

    // SMPredicate 
    SMPredicate *geoPredicate = [SMPredicate predicateWhere:@"geopoint" isWithin:10 milesOfGeoPoint:geoPoint];

    // NSPredicate
    NSPredicate *timePredicate = [NSPredicate predicateWithFormat:@"date == %@", lastmoddate];

    // Combine the two into an NSCompoundPredicate
    NSArray *predicates = [NSArray arrayWithObjects:geoPredicate, timePredicate, nil];       
    NSPredicate *compoundPredicate = [NSCompoundPredicate andPredicateWithSubpredicates:predicates];

```
Check out the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMPredicate.html">SMPredicate</a> class for more information.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/advanced-query.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
