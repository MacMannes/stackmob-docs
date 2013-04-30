Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read-into-table.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Read all objects from an existing schema into a Table View

<h3>Experience Level</h3>
Intermediate

<h3>Estimated time to complete</h3>
~10 minutes

<h3>Prerequisites</h3>

* XCode 4.x and greater

* iOS SDK 5 and greater

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

<h3>Have you read through the <a href="http://stackmob.github.com/stackmob-ios-sdk/#coding_practices" target="_blank">StackMob <—> Core Data Coding Practices</a>?</h3>

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

<h1>Let's get started!</h1>

<h2>Open the Base Xcode Project</h2>

We’ve created an Xcode project for you as a starting place for this tutorial.  It has StackMob imported and the basic plumbing for Core Data.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the project, see <a href="https://developer.stackmob.com/tutorials/ios/Base-Xcode-Project-for-Tutorials" target="_blank">Base Xcode Project for Tutorials</a>.

Unzip the Base Project and open **base-project.xcodeproj**.

<h2>Add your Public Key</h2>
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

<h2>Create your TableViewController</h2>

Select the storyboard and drag and drop a TableViewController onto the storyboard.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-01.png"><br /><br />

<h2>Add the ListViewController</h2>

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
<h2>Delete the ViewController files</h2>
Delete the ViewController.h and ViewController.m files created with the project.

<h2>Update the TableViewController class</h2>
In the storyboard, select the TableViewController by clicking on the black-bar under it.  On the right side Utilities panel, select the **Identity Inspector** and change the **custom class** to **ListViewController**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-05.png">
<br/>
<br/>
<h2>Set Initial View Controller</h2>
Click on the **Attributes Inspector**, and check the box **Is Initial View Controller**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-06.png">
<br/>
<br/>
<h2>Configure the Cell</h2>
Select the cell in the TableViewController.  In the **Attributes Inspector** locate the **Identifier** and enter **Cell**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-07.png">
<br/>
<br/>
<h2>Delete original View Controller</h2>
In the storyboard, you can now delete the original View Controller created with your project, along with ViewController.h and ViewController.m.

<h2>Open ListViewController.h</h2>
Add the following **highlighted** code to the ListViewController.h file.  You’ll notice we’ve declared a fetchedResultsController.  We’ll use this to fetch our objects from StackMob and populate our TableView .


```obj-c,2,4,6-7
#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface ListViewController : UITableViewController <NSFetchedResultsControllerDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (strong, nonatomic) NSFetchedResultsController *fetchedResultsController;

@end
```

<h2>Open ListViewController.m</h2>

Add the following **highlighted** code to your ListViewController.m file:

```obj-c,2-3,8-10,25,38-68,72,77-78,86-87

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
}

- (void)viewDidUnload
{
    [super viewDidUnload];
}

-  (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

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
    NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Master"];
    aFetchedResultsController.delegate = self;
    self.fetchedResultsController = aFetchedResultsController;
    
  NSError *error = nil;
  if (![self.fetchedResultsController performFetch:&error]) {
      NSLog(@"An error %@, %@", error, [error userInfo]);
  }
    
    return _fetchedResultsController;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [[self.fetchedResultsController sections] count];
}

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

@end
```

<h2>Build and Run!</h2>

Run your project.  This will fetch all the objects from your todo schema on StackMob and display them in your table view.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/read-into-table.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>