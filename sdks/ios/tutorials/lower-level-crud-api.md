Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/datastore-crud.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Create, Read, Update and Delete objects from StackMob using the **data store API directly and WITHOUT Core Data**. 

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~15 minutes

<h3>Prerequisites</h3>

* XCode 4.x and greater

* iOS SDK 5 and greater

* **Note:** We will not use the Base Xcode Project for this tutorial

<h1>Let's Get Started</h1>

<h2>The XCode Project</h2>

The project itself is based off of the **Single View Application** template, found with File > New > Project and choosing Application > Single View Application.
<br><br>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/datastore-crud/datastore-crud-01a.png">

<h2>Importing StackMob</h2>

<a href="https://s3.amazonaws.com/static.stackmob.com/sdks/ios/stackmob-ios-sdk-v2.0.0.zip" class="button">Download StackMob v2.0.0</a>

<br/>
<br/>

Unzip stackmob-ios-sdk-vx.x.x.zip.

Drag and drop the unzipped folder into your project's root folder:
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Add-StackMob.png" alt="drag and drop image of project"/></p>

Check "Copy items...", select "Create groups for.." and "Finish"
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Added-StackMob.png" alt=""/></p>

Resulting in...
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Added-StackMob-2.png" alt=""/></p>

Now under "Project > Build Settings", using the Search bar, search for `Other Linker`, double click on the `Other Linker Flags` result, and set the value to `-ObjC`
    <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Add-Linker-Flag.png" alt="set objc"/></p>


<h2>Add CoreData, Security, SystemConfiguration, MobileCoreServices and CoreLocation Frameworks</h2>

**While you aren't using Core Data directly in this tutorial, the SDK still needs the CoreData.Framework to compile.**

Under "Targets > Build Phases", add the `CoreData.Framework`
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Add-CoreData.png" alt=""/></p>
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/gettingstarted/StackMob-Getting-Started-iOS-Search-for-CoreData.png" alt=""/></p>

Repeat and add the `Security.framework`, `SystemConfiguration.framework`, `MobileCoreServices.framework` and `CoreLocation.framework`.
<br/>
<br/>
<h2>Import SystemConfiguration and MobileCoreServices Frameworks</h2>

Somewhere in your project, such as your project's .pch file, import the SystemConfiguration and MobileCoreServices frameworks.
     <p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/base-project/pchFile.png" alt=""/></p>

<br/>
<h2>Appdelegate.h</h2>

Here we add the SMClient variable for application-wide usage:

```obj-c,3,8
#import <UIKit/UIKit.h>

@class SMClient;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) SMClient *client;

@end
```

**Important:** The reason we declare the StackMob client variable in AppDelegate, even though it will be used in ViewController, is because AppDelegate is a special file that allows us to access variables declared in its interface file from any other file, using a shared instance of AppDelegate. We declare that instance in ViewController.m.

<h2>Appdelegate.m</h2>

Here we add our StackMob client variable and initialize it. 
Go to <a href="https://dashboard.stackmob.com/settings" target="_blank">Manage App Info</a> in the StackMob Dashboard and copy the **Development Public Key** and paste it  into the **AppDelegate.m** file where is says **YOUR\_PUBLIC\_KEY** in the method:

```obj-c,2,6,11
#import "AppDelegate.h"
#import "StackMob.h"

@implementation AppDelegate

@synthesize client = _client;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    return YES;
}

// Other methods below remain unedited
```


<h2>Create Your View</h2> 

Select the storyboard file and drag and drop a **text field** and **4 buttons** on to your view.  Double click each button and change their labels to "create", "read", "update" and "delete".
<br />
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/datastore-crud/datastore-crud-01.png">
<br />
<br />
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the **text field** and drag to your **ViewController.h** file.  
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/datastore-crud/datastore-crud-02.png">
<br />
<br />
In the menu, set the Connection to **Outlet** and enter the name **myTextField**.
<br />
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/datastore-crud/datastore-crud-04.png">
<br />
<br />
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br />
In the menu, set the Connection to **Action** and enter the name **createObject**.


<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/datastore-crud/datastore-crud-05.png">
<br />
<br />

**REPEAT** the above steps for each of the buttons.  Enter "readObject", "updateObject" and "deleteObject" for each of the unique buttons.

<br />
<br />


<h2>Open ViewController.h</h2>


The ViewController.h file should look like the following. First, you'll add the UITextFieldDelegate, so when you hit the return button on your virtual keyboard, it will dismiss the keyboard.  Then, you'll add a property called myObjectId to hold the id of the object last created.

```obj-c,3,6
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) IBOutlet UITextField *myTextField;
@property (strong, nonatomic) NSString *myObjectId;

- (IBAction)create:(id)sender;
- (IBAction)read:(id)sender;
- (IBAction)update:(id)sender;
- (IBAction)delete:(id)sender;

@end
```

<h2>Open ViewController.m</h2>

Add the following **highlighted** code to your ViewController.m file:

* Lines 2 & 13-16 bring your AppDelegate into your view controller, so you can access the  StackMob client variable.
* Lines 11 & 21 implements the myObjectId property.
* Line 22 tells the text field to delegate messages to this file.
* Lines 38-42 Will allow the return button to dismiss the keyboard.
* Lines 46-53 creates a dictionary where you set the key-value pairs for your object.  Then perform a createObject method call passing in the schema name, arguments, and declaring a success and failure method handlers.
* Lines 58-62 Performs a readObjectWithId method call passing in the schema name, myObjectId, and declaring a success and failure method handlers.
* Lines 68-74 Performs an updateObjectWithId method call passing in the schema name, myObjectId, arguments with field to update and declaring a success and failure method handlers. 
* Lines 79-84 Performs an deleteObjectId method call passing in the schema name, myObjectId and declaring a success and failure method handlers. 

<h3>&nbsp;</h3>


```obj-c,2,3,11,13,14,15,16,21,22,38,39,40,41,42,46,47,48,49,50,51,52,53,58,59,60,61,62,68,69,70,71,72,73,74,79,80,81,82,83,84

#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize myTextField;
@synthesize myObjectId = _myObjectId;

- (AppDelegate *)appDelegate
{
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.myObjectId = nil;
    self.mytextField.delegate = self;
  // Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload
{
    [self setMyTextField:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
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

- (IBAction)create:(id)sender {
    
    NSDictionary *arguments = [NSDictionary dictionaryWithObject:[myTextField text] forKey:@"name"];
    
    [[[SMClient defaultClient] dataStore] createObject:arguments inSchema:@"todo" onSuccess:^(NSDictionary *theObject, NSString *schema) {
        NSLog(@"Created object %@ in schema %@", theObject, schema);
        self.myObjectId = [theObject objectForKey:@"todo_id"];
    } onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
        NSLog(@"Error creating object: %@", theError);
    }];
}

- (IBAction)read:(id)sender {
    
    [[[SMClient defaultClient] dataStore] readObjectWithId:self.myObjectId inSchema:@"todo" onSuccess:^(NSDictionary *theObject, NSString *schema) {
        NSLog(@"Result of read is %@", theObject);
    } onFailure:^(NSError *theError, NSString *theObjectId, NSString *schema) {
        NSLog(@"Error reading object: %@", theError);
    }];
    
}

- (IBAction)update:(id)sender {
    
    NSDictionary *arguments = [NSDictionary dictionaryWithObject:[myTextField text] forKey:@"name"];
    
    [[[SMClient defaultClient] dataStore] updateObjectWithId:self.myObjectId inSchema:@"todo" update:arguments onSuccess:^(NSDictionary *theObject, NSString *schema) {
        NSLog(@"Result of update is %@", theObject);
    } onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
        NSLog(@"Error updating object: %@", theError);
    }];
}

- (IBAction)delete:(id)sender {
    
    [[[SMClient defaultClient] dataStore] deleteObjectId:self.myObjectId inSchema:@"todo" onSuccess:^(NSString *theObjectId, NSString *schema) {
        NSLog(@"Deleted object %@", theObjectId);
        self.myObjectId = nil;
    } onFailure:^(NSError *theError, NSString *theObjectId, NSString *schema) {
        NSLog(@"Error deleting object: %@", theError);
    }];
}
@end
```

<h2>Build and Run!</h2>

Run your project, enter some text and click the create button.  You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a>.  You can also read your object, update and delete your object.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/datastore-crud.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
