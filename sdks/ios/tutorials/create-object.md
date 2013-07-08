Create an Object
================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/create.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Create and save a new object.

### Experience Level
Beginner

### Estimated time to complete
~5 minutes

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

## Create Your View 

Select the storyboard file and drag and drop a **text field** and **button** to your view.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/create/create-01.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the **text field** and drag to your **ViewController.h** file.  
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/create/create-03.png">
<br/>
<br/>
In the menu, set the Connection to **Outlet** and enter the name **titleField**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/create/create-04.png">
<br/>
<br/>
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **createNewObject**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/create/create-05.png">
<br />
<br />

## Edit ViewController.h


Make the ViewController.h file the **text field delegate**:

```obj-c,3
#import <UIKit/UIKit.h>;

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@property (weak, nonatomic) IBOutlet UITextField *titleField;
- (IBAction)createNewObject:(id)sender;

@end
```

## Edit ViewController.m

Add the following **highlighted** code to your ViewController.m file:

```obj-c,3,22,36-40,44-53
#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize titleField = _titleField;

- (AppDelegate *)appDelegate {
  	return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewDidLoad
{
	[super viewDidLoad];  
   	self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    self.titleField.delegate = self;
}

- (void)viewDidUnload
{
   	[self setTitleField:nil];
   	[super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
   	return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (IBAction)createNewObject:(id)sender {
    
	NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
   
   	[newManagedObject setValue:self.titleField.text forKey:@"title"];
   	[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
   
   	[self.managedObjectContext saveOnSuccess:^{
        NSLog(@"You created a new object!");
    } onFailure:^(NSError *error) {
        NSLog(@"There was an error! %@", error);
    }];
}

@end
```

## Build and Run!

Run your project, enter some text and click the save button.  You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a>. 

Remember that your Core Data attribute **todoId** will appear in the Object Browser as **todo_id**. See the <a href="http://stackmob.github.com/stackmob-ios-sdk/CoreDataSupportSpecs.html" target="_blank">Entity and Property Naming Conventions</a> section of the Core Data Support Specifications for all the details.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/create.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
