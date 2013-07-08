Read into Table View
===========================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read-into-table.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Read all objects from an existing schema into a Table View

### Experience Level
Intermediate

### Estimated time to complete
~10 minutes

### Prerequisites

* XCode 4.x and greater

* iOS SDK 6 and greater

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

Have you read through the <a href="http://stackmob.github.com/stackmob-ios-sdk/#coding_practices" target="_blank">StackMob <—> Core Data Coding Practices</a>?

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

## Open the Base Xcode Project

We’ve created an Xcode project for you as a starting place for this tutorial.  It has StackMob imported and the basic plumbing for Core Data.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the project, see <a href="https://developer.stackmob.com/ios-sdk/base-xcode-project-for-tutorials" target="_blank">Base Xcode Project for Tutorials</a>.

Unzip the Base Project and open **base-project.xcodeproj**.

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

## Create your TableViewController

Select the storyboard and drag and drop a TableViewController onto the storyboard.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-01.png"><br /><br />

## Add the ListViewController

Right-click on the base-project folder and select **New File**.  
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-02.png">
<br/>
<br/>
Under the Cocoa-Touch templates select Objective-C class and click Next.  
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-03.png">
<br/>
<br/>
Name it **ListViewController** with **subclass UITableViewController** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-04.png">
<br/>
<br/>
## Delete the ViewController files
Delete the ViewController.h and ViewController.m files created with the project.

## Update the TableViewController class
In the storyboard, select the TableViewController by clicking on the black-bar under it.  On the right side Utilities panel, select the **Identity Inspector** and change the **custom class** to **ListViewController**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-05.png">
<br/>
<br/>
## Set Initial View Controller
Click on the **Attributes Inspector**, and check the box **Is Initial View Controller**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-06.png">
<br/>
<br/>
## Configure the Cell
Select the cell in the TableViewController.  In the **Attributes Inspector** locate the **Identifier** and enter **Cell**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-07.png">
<br/>
<br/>
## Delete original View Controller
In the storyboard, you can now delete the original View Controller created with your project, along with ViewController.h and ViewController.m.

## Edit ListViewController.h
Add the following **highlighted** code to the ListViewController.h file.  You’ll notice we’ve declared a NSArray.  We’ll use this to store our objects from StackMob and populate our TableView . Attached to the TableView is a RefreshControl, which will reload the fetchedResultsController when triggered.


```obj-c,2,4,6-7
#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface ListViewController : UITableViewController

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (strong, nonatomic) NSArray *objects;

@end
```
## Edit ListViewController.m

Add the following **highlighted** code to your ListViewController.m file:

```obj-c,2,3,8-10,25-35,54,59,67,68,73-96
#import "ListViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"


@implementation ListViewController

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    UIRefreshControl *refreshControl = [[UIRefreshControl alloc]
                                        init];
    [refreshControl addTarget:self action:@selector(refreshTable) forControlEvents:UIControlEventValueChanged];
    
    self.refreshControl  = refreshControl;
    
    [refreshControl beginRefreshing];
    
    [self refreshTable];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.objects count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    NSManagedObject *object = [self.objects objectAtIndex:indexPath.row];
    cell.textLabel.text = [object valueForKey:@"title"];
    
    return cell;
}

- (void) refreshTable {
    
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    // Edit the entity name as appropriate.
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    [fetchRequest setEntity:entity];
    
    // Edit the sort key as appropriate.
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"title" ascending:YES];
    NSArray *sortDescriptors = [NSArray arrayWithObjects:sortDescriptor, nil];
    
    [fetchRequest setSortDescriptors:sortDescriptors];
    
    [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
        [self.refreshControl endRefreshing];
        self.objects = results;
        [self.tableView reloadData];
        
    } onFailure:^(NSError *error) {
        
        [self.refreshControl endRefreshing];
        NSLog(@"An error %@, %@", error, [error userInfo]);
    }];
}

@end
```

## Build and Run!

Run your project.  This will fetch all the objects from your todo schema on StackMob and display them in your table view.

## Using NSFetchResultsController

Alternatively, you can use NSFetchResultsController to read your objects into a tableview. NSFetchResultsController is useful for monitoring an associated NSManagedObjectContext for local changes to individual objects, and updating the tableview accordingly. However, to properly get changes from a server you will still need to initiate manual refreshes.

If you not building for iOS 6, use an NSFetchResultsController without a refresh control.

With that in mind, here are the changes needed to add an NSFetchResultsController:

**Important:** SMPredicate, the class used for making Core Data geo-queries with StackMob, is incompatible with NSFetchResultsController.

In ListViewController.h, make the following changes:

```obj-c,4,7
#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface ListViewController : UITableViewController <NSFetchedResultsControllerDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (strong, nonatomic) NSFetchedResultsController *fetchedResultsController;

@end
``` 

Edit ListViewController.m from our earlier example. Add the following method:

```obj-c
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
    
    
    // Edit the section name key path and cache name if appropriate.
    // nil for section name key path means "no sections".
    NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:nil];
    aFetchedResultsController.delegate = self;
    _fetchedResultsController = aFetchedResultsController;

    NSError *error = nil;
    if (![_fetchedResultsController performFetch:&error]) {
        NSLog(@"An error %@, %@", error, [error userInfo]);
    }
    
    return _fetchedResultsController;
}
```

In ListViewController.m, edit the following methods:

```obj-c,3,4,12,13,20-29
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
     
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
     
    NSManagedObject *object = [self.fetchedResultsController objectAtIndexPath:indexPath];
    cell.textLabel.text = [object valueForKey:@"title"];
     
    return cell;
}

- (void) refreshTable {
    
    NSError *error = nil;
    if (![self.fetchedResultsController performFetch:&error]) {
        // Handle error
        NSLog(@"An error %@, %@", error, [error userInfo]);
    }
    else {
        [self.tableView reloadData];
    }
}
```

<p></p>

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read-into-table.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
