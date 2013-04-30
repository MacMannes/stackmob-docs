Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/update.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Update an existing object.

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

Select the storyboard file and drag and drop a **text field** and **button** to your view.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/update/update-01.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the **text field** and drag to your **ViewController.h** file.  
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/update/update-02.png">
<br/>
<br/>
In the menu, set the Connection to **Outlet** and enter the name **titleField**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/update/update-03.png">
<br/>
<br/>
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **updateObject**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/update/update-04.png">

<br />
<br />
<h3>Open ViewController.h</h3>


Add the  **highlighted code** to your ViewController.h file. You'll notice we’ve declared an NSString instance to hold the ID of the managed object we will create so we can later retrieve and update it.  We have also made this file the **text field delegate**.

```obj-c,2,4,7-8
#import <UIKit/UIKit.h>
#import "StackMob.h"

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet UITextField *titleField;
@property (strong, nonatomic) NSString *todoId;

- (IBAction)updateObject:(id)sender;

@end
```

<h3>Open ViewController.m</h3>


Add the  **highlighted code** to your ViewController.m file.  

```obj-c,13,25-28,30-33,35-50,65-72,76-101
#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize titleField;
@synthesize todoId = _todoId;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    /*
     Set the instance of NSManagedObjectContext to the configured context from AppDelegate. 
     */
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    /*
     Set this controller to be the delegate for the text field.
     */
    self.titleField.delegate = self;
    
    /*
     Create a new todo, and record the value we assign to the primary key, todoId, for later reference.
     */
    NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
    [newManagedObject setValue:@"Hello World" forKey:@"title"];
    self.todoId = [newManagedObject assignObjectId];
    [newManagedObject setValue:self.todoId forKey:[newManagedObject primaryKeyField]];
    
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
    [self setTitleField:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

/*
 Resign the text field when the user presses 'Return'
 */
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (IBAction)updateObject:(id)sender {
    
    /*
     Use the ID of the todo object we created to fetch it.
     */
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
    NSPredicate *predicte = [NSPredicate predicateWithFormat:@"todoId == %@", self.todoId];
    [fetchRequest setPredicate:predicte];

    [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
        
        /*
          With the object in memory, update its title and save.
        */
        NSManagedObject *todoObject = [results objectAtIndex:0];
        [todoObject setValue:self.titleField.text forKey:@"title"];

        [self.managedObjectContext saveOnSuccess:^{
            NSLog(@"You updated the todo object!");
        } onFailure:^(NSError *error) {
            NSLog(@"There was an error! %@", error);
        }];
        
    } onFailure:^(NSError *error) {
        
        NSLog(@"Error fetching: %@", error);
        
    }];
    
}

@end
```

<h3>Build and Run!</h3>
Run your project. When ViewDidLoad is called a new object will be created for you.  Go to the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a> to view it.  Now, enter some text and click the update button.  Go back to the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a> and click the Query button to refresh your list.  You’ll notice the object has been updated.

Remember that your Core Data attribute **todoId** will appear in the Object Browser as **todo_id**. See the <a href="http://stackmob.github.com/stackmob-ios-sdk/CoreDataSupportSpecs.html" target="_blank">Entity and Property Naming Conventions</a> section of the Core Data Support Specifications for all the details.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/update.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>