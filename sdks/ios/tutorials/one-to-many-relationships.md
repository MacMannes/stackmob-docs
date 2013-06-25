One to Many Relationships
=========================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/one-to-many-relationship.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

How to create and save objects with a one to many relationship.  In this example we will have a single Todo object with many categories.

### Experience Level
Beginner

### Estimated time to complete
~10 minutes

### Prerequisites

* XCode 4.x and greater

* iOS SDK 5 and greater

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

**Have you read through the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#CodingPractices" target="_blank">Core Data Integration Coding Practices</a>?**

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

## Add the Category Entity

In the XCode project navigator, select **mydatamodel.xcdatamodelId**.  Click the **plus sign** next to **Add Entity** near the bottom of the screen.  **Name** your new entity **Category**. 
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-one/one-to-one-01.png">
<br/>
<br/>
Add two **attributes categoryId** and **name** and give them both the **data type string**.

<p class="alert">If you plan on using the cache, add the <b>createddate</b> and <b>lastmoddate</b> attributes of data type <b>Date</b> as well.</p>
<br />
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-one/one-to-one-02.png">
<br/>
<br/>

### Add Relationship on Category Entity

Select the **Category** entity. **Under relationships, click the plus icon** to added a new relationship attribute.  Since the Category entity has a one-to-one relationship with the Todo entity, call the **relationship todo** (sigular).  Set the **destination to Todo**.  Set the **inverse as No Inverse**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-one/one-to-one-03.png">
<br/>
<br/>

### Add Relationship on Todo

Select the **Todo** entity.  **Under relationships, click the plus icon** to added a new relationship attribute.  Since the Todo entity has a one-to-many relationship with the Category entity call the **relationship categories** (plural).  Set the **destination to Category**.  Set the **inverse as todo**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-many/one-to-many-02.png">
<br/>
<br/>
Now select the new categories attribute and look at the Data Model Inspector on the right.  Check the box for To Many Relationship.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-many/one-to-many-03.png">
<br/>
<br/>

## Create Subclasses

**Right click** on the **base-project** project folder and select **New File**. 
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-02.png">
<br/>
<br/>
Select **Core Data** on the template page and chose **NSManagedObject subclass** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-one/one-to-one-05.png">
<br/>
<br/>
Check the box next to  **mydatamodel** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-one/one-to-one-06.png">
<br/>
<br/>
Check the box next to **Todo** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/one-to-one/one-to-one-07.png">
<br/>
<br/>

** Repeat the above steps for the Category Entity. **


When you created the Todo NSManagedObject subcass, the Category subclass didn't exist so the Todo subclass files created could not reference it.  Instead it used the generic NSManagedObject class.  
<br/>
To fix this, **repeat the Create NSManagedObject subclass steps** one more time for the **Todo entity**.
<br/><br/>
When you are done, the files should look like the following:

<h5>Todo.h</h5>

```obj-c

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class Category;

@interface Todo : NSManagedObject

@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) NSString * todoId;
@property (nonatomic, retain) NSSet *categories;
@end

@interface Todo (CoreDataGeneratedAccessors)

- (void)addCategoriesObject:(Category *)value;
- (void)removeCategoriesObject:(Category *)value;
- (void)addCategories:(NSSet *)values;
- (void)removeCategories:(NSSet *)values;

@end
```

<h5>Todo.m</h5>

```obj-c

#import "Todo.h"
#import "Category.h"

@implementation Todo

@dynamic title;
@dynamic todoId;
@dynamic categories;

@end
```

<h5>Category.h</h5>

```obj-c

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class Todo;

@interface Category : NSManagedObject

@property (nonatomic, retain) NSString * categoryId;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) Todo *todo;

@end
```

<h5>Edit Category.m</h5>

```obj-c

#import "Category.h"
#import "Todo.h"

@implementation Category

@dynamic categoryId;
@dynamic name;
@dynamic todo;

@end
```


## Edit ViewController.m

Add the following **highlighted** code to your ViewController.m file:

```obj-c,4-5,29-65
#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "Todo.h"
#import "Category.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;


- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    
    Todo *newTodo = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
    [newTodo setValue:@"Hello One-To-Many" forKey:@"title"];
    [newTodo setValue:[newTodo assignObjectId] forKey:[newTodo primaryKeyField]];
    
    
    Category *newCategory = [NSEntityDescription insertNewObjectForEntityForName:@"Category" inManagedObjectContext:self.managedObjectContext];
    
    [newCategory setValue:@"Work" forKey:@"name"];
    [newCategory setValue:[newCategory assignObjectId] forKey:[newCategory primaryKeyField]];
    
    Category *newCategory2 = [NSEntityDescription insertNewObjectForEntityForName:@"Category" inManagedObjectContext:self.managedObjectContext];
    
    [newCategory2 setValue:@"Home" forKey:@"name"];
    [newCategory2 setValue:[newCategory2 assignObjectId] forKey:[newCategory primaryKeyField]];
    
    
    NSError *error = nil;
    if (![self.managedObjectContext saveAndWait:&error]) {
        NSLog(@"There was an error! %@", error);
    }
    else {
        NSLog(@"You created a Todo and two Category objects!");
    }
    
    [newTodo addCategoriesObject:newCategory];
    [newTodo addCategoriesObject:newCategory2];

    [self.managedObjectContext saveOnSuccess:^{
        
        NSLog(@"You created a relationship between the Todo and Category Objects!");
        
    } onFailure:^(NSError *error) {
        
        NSLog(@"There was an error! %@", error);
        
    }];

}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

@end

```

## Build and Run!

Run your project. A new Todo and two Category objects will be created when the view loads and they will have a one to many relationship.  You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a>.

Remember that your Core Data attributes **todoId** and **categoryId** will appear in the Object Browser as **todo_id** and **cateogry_id**, respectively. See the <a href="http://stackmob.github.com/stackmob-ios-sdk/CoreDataSupportSpecs.html" target="_blank">Entity and Property Naming Conventions</a> section of the Core Data Support Specifications for all the details.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/one-to-many-relationship.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>