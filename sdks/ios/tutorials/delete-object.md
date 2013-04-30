Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/delete.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Delete an existing object.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

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

<h3>Add your Public Key</h3>
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

<h3>Create Your View</h3> 

Select the storyboard file and drag and drop a **button** to your view.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/delete/delete-01.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/delete/delete-02.png">
<br/>
<br/>
In the menu, set the Connection to **Action** and enter the name **deleteObject**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/delete/delete-03.png">
<br />
<br />

<h3>Open ViewController.h</h3>


Add the following **highlighted** code to your ViewController.h file. You’ll notice we’ve declared a NSManagedObject called **aMangagedObject**.  We’ll use this to store the object we create in the ViewDidLoad method, so we have an object we can delete.

```obj-c,2,5-7
#import <UIKit/UIKit.h>
#import "StackMob.h"

@interface ViewController : UIViewController
{
    NSManagedObject *aManagedObject;
}

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
- (IBAction)deleteObject:(id)sender;

@end
```

<h3>Open ViewController.m</h3>

Add the following **highlighted** code to your ViewController.m file.  

```obj-c,24-37,53-63
#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

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
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    
    NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
    [newManagedObject setValue:@"Hello World" forKey:@"title"]; 
    [newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
    
    aManagedObject = newManagedObject;
    
    NSError *error = nil;
    if (![self.managedObjectContext saveAndWait:&error]) {
        NSLog(@"There was an error! %@", error);
    }
    else {
        NSLog(@"You created a new object!");
    }

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

- (IBAction)deleteObject:(id)sender {
    [self.managedObjectContext deleteObject:aManagedObject];
    
    [self.managedObjectContext saveOnSuccess:^{
        
        NSLog(@"You deleted the new object!");
        
    } onFailure:^(NSError *error) {
        
        NSLog(@"There was an error! %@", error);
        
    }];
    
}
@end
```

<h3>Build and Run!</h3>
Run your project.  When ViewDidLoad is called a new object will be created for you.   Go to the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a> to view it.  Now,  click the delete button.  Go back to the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a> and click the Query button to refresh your list.  You’ll notice the object has been deleted.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/delete.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>