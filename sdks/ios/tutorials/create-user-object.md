Create a User Object
====================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/user-object.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Create a new user.

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
**A Note about user schema defaults:** You must specify any changes to your user schema information. If the schema, primary key and password field names are different than the defaults @"user", @"username" and @"password", respectively, make sure to set those values on the client. While setters are available for all the properties, the easiest way to set all the values at once is using the extended SMClient initializer, like so:

```obj-c

/*
    Initializing the client. Note that for userPrimaryKeyField and userPasswordField, you should provide the names
    of the fields that correspond to the user object schema primary key and password fields, respectively.
*/
self.client = [[SMClient alloc] initWithAPIVersion:@"0" apiHost:@"api.stackmob.com" publicKey:@"YOUR_PUBLIC_KEY" 
    userSchema:@"nameofschema" userPrimaryKeyField:@"userprimarykeyfield" userPasswordField:@"userpasswordfield"]; 

``` 

<h2>Add the User Entity</h2>
In the XCode project navigator, select **mydatamodel.xcdatamodeld**.  Click the **plus sign** next to **Add Entity** near the bottom of the screen.  **Name** your new entity **User**. 
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-01.png">
<br/>
<br/>
Add a **username** attribute with data type **String**.  The username will be your primary key. 
<br />
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-02.png">
<br/>
<br/>
<h2>Create NSManagedObject subclass for the User entity</h2>
**Right click** on the **base-project** project folder and select **New File**. 
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/read-into-table/read-into-table-02.png">
<br/>
<br/>
Select **Core Data** on the template page and chose **NSManagedObject subclass** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-04.png">
<br/>
<br/>
Check the box next to  **mydatamodel** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-05.png">
<br/>
<br/>
Check the box next to **User** and click **Next**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-06.png">
<br/>
<br/>
<h2>User.h</h2>
Add the following **highlighted code** to your User.h file:<br>

```obj-c,5-9,13-16

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "StackMob.h"

/*
    We are going to inherit from SMUserManagedObject. This class gives us the necessary methods to 
    securely set a password for our user object without needing to store the value in a variable or Core Data attribute.
*/
@interface User : SMUserManagedObject

@property (nonatomic, retain) NSString * username;

/*
    We also declare an init method for easy initialization of our managed object.
*/
- (id)initIntoManagedObjectContext:(NSManagedObjectContext *)context;

@end
```

<h2>User.m</h2>
Add the following **highlighted code** to your User.m file:<br>      

```obj-c,7-19

#import "User.h"

@implementation User

@dynamic username;

/*
    In our init method we insert a new User managed object in the provided managed object context.
*/
- (id)initIntoManagedObjectContext:(NSManagedObjectContext *)context {
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"User" inManagedObjectContext:context];
    self = [super initWithEntity:entity insertIntoManagedObjectContext:context];
    
    if (self) {
        // assign local variables and do other init stuff here
    }
    
    return self;
}

@end
```

**Note:** Line 9 calls the **SMUserManagedObject** init method, an overridden method which will attach the default SMClient instance to the managed object.  This is so it can access the primary key and password field names for the user object, which are defined in your SMClient instance.  If you wish to reference a specific instance of SMClient other than what is returned by <i>[SMClient defaultClient]</i>, use <i>initWithEntity:client:insertIntoManagedObjectContext:</i>.

<h2>Create Your View</h2> 

Select the storyboard file and drag and drop  **two text fields** and a **button** to your view.  Under the **Attributes Inspector** on the right, you can set the **placeholder** value for the **username and password** if you like.

<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-07.png">

<br/>
<br/>

Select the **password text field** and under the **Attributes Inspector** on the right check the box for **Secure**.  This will cause the text to appear as dots, something suitable for a password field.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/secure_password.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the **username text field** and drag to your **ViewController.h** file.  
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-08.png">
<br/>
<br/>
In the menu, set the Connection to **Outlet** and enter the name **usernameField**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-09.png">
<br/>
<br/>
Hold down the **Control** key and click on the **password text field** and drag to your **ViewController.h** file.  
<br>
In the menu, set the Connection to **Outlet** and enter the name **passwordField**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-10.png">
<br/>
<br/>
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br>
In the menu, set the Connection to **Action** and enter the name **createUser**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-object/user-object-11.png">

<br />
<br />

<h2>Open ViewController.h</h2>


Add the following **highlighted code** to your ViewController.h file:

```obj-c,3

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;

- (IBAction)createUser:(id)sender;

@end

```

<h2>Open ViewController.m</h2>

Add the following **highlighted code** to your ViewController.m file:

```obj-c,3-7,29-33,48-55,58-61,63-67,69-73,75-92

#import "ViewController.h"
#import "AppDelegate.h"
/*
 Import the necessary header files.
 */
#import "StackMob.h"
#import "User.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize usernameField = _usernameField;
@synthesize passwordField = _passwordField;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	 
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    /*
     Set this controller file to be the delegate for the text fields.
     */
    self.usernameField.delegate = self;
    self.passwordField.delegate = self;
}

- (void)viewDidUnload
{
    [self setUsernameField:nil];
    [self setPasswordField:nil];
    [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

/*
 The textFieldShouldReturn: method will get called and dismiss the soft keyboard when the user presses the return button.
 */
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (IBAction)createUser:(id)sender {
    /* 
     We instantiate an instance of User using our custom init method.
     */
    User *newUser = [[User alloc] initIntoManagedObjectContext:self.managedObjectContext];
    
    /* 
     We set the value of the primary key field to what the user has typed in the usernameField text field. 
     [newUser sm_primaryKeyField] will return the userPrimaryKeyField value from the referenced SMClient instance.
     */ 
    [newUser setValue:self.usernameField.text forKey:[newUser primaryKeyField]];

    /* 
     SMUserManagedObject provides the setPassword: method and should be the only method used 
     to set a password for a user object.
     */
    [newUser setPassword:self.passwordField.text];
    
    [self.managedObjectContext saveOnSuccess:^{
        
        NSLog(@"You created a new user object!");
        
    } onFailure:^(NSError *error) {

        /*
        One optional way to handle an unsuccessful save of a managed object is to delete the object altogether 
        from the managed object context.  For example, you probably won't try to keep saving a user object that 
        returns a duplicate key error. If you delete a user managed object that hasn't been saved yet, you must 
        remove the password you originally set using the removePassword: method.
        */
        
        [self.managedObjectContext deleteObject:newUser];
        [newUser removePassword];
        NSLog(@"There was an error! %@", error);
        
    }];
    
}

@end
```

**Important:** To reiterate the comment starting on **Line 82**, One **optional** way to handle an unsuccessful save of a managed object is to delete the object altogether from the managed object context.  For example, you probably won't try to keep saving a user object that returns a duplicate key error. If you delete a user managed object that hasn't been saved yet, **you must remove the password you originally set using the <i>removePassword:</i> method.**

<h2>Build and Run!</h2>

Run your project, enter a username and password and click the create user button.  You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/user" target="_blank">StackMob Object Browser</a>.


<h2>A Note about creating user objects with the Datastore API</h2>

If you are using the Datastore API to create **user objects** directly, using methods like <i>createObject:inSchema:options:onSucess:onFailure:</i>, **make sure to pass in an SMRequestOptions instance that specifies that the request should be sent over secure https.** This ensures that the request will be encrypted, hiding your password, etc. You can easily specify that a request should be secure with the following:

```obj-c,1

SMRequestOptions *secureOptions = [SMRequestOptions optionsWithHTTPS];

[[[SMClient defaultClient] dataStore] createObject:object inSchema:@"schema" options:secureOptions 
    onSuccess:^{} onFailure:^{}];

```


Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/user-object.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
