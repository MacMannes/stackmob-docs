User Authentication
=============

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/user-authentication.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

User login, logout and check their logged in status.

### Experience Level
Beginner

### Estimated time to complete
~10 minutes

### Prerequisites

* XCode 4.x and greater

* iOS SDK 5 and greater

* [Download User Object App](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/user-object.zip)

**Have you read through the <a href="https://developer.stackmob.com/ios-sdk/core-data-guide#CodingPractices" target="_blank">Core Data Integration Coding Practices</a>?**

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

### Configure XCode Project

### Open the StackMob User Object App

We’ll use the project from the User Object Tutorial where we created a user object.  It has StackMob imported, Core Data and a User Entity defined for us.    This allows you to focus on the objective of this tutorial.

For more information on what's inside of the User Object app, see <a href="https://developer.stackmob.com/ios-sdk/create-user-object-tutorial" target="_blank">User Object Tutorial</a>.

If you haven't already, download the User Object App from the link under **Prerequisites** above. 

Unzip and open **user-object.xcodeproj**.

### Add your Public Key
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

## Login

Select the storyboard file and drag and drop  a **button** to your view.  Set the label of the button as **login**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-01.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-02.png">


<br>
In the menu, set the Connection to **Action** and enter the name **login**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-03.png">

<br />
<br />

### Login: Edit ViewController.h


Add the following **highlighted** code to your ViewController.h file:

```obj-c,3,10

#import <UIKit/UIKit.h>

@class SMClient;

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;
@property (strong, nonatomic) SMClient *client;

- (IBAction)createUser:(id)sender;
- (IBAction)login:(id)sender;

@end

```

### Login: Edit ViewController.m

Add the following **highlighted** code to your ViewController.m file. We set our local SMClient instance to the instance returned by the defaultClient class method.  By defualt this method returns the first initialized client,  which was done in our App Delegate.

```obj-c,15,26,70-93

#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "User.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize usernameField = _usernameField;
@synthesize passwordField = _passwordField;
@synthesize client = _client;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	 
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    self.client = [SMClient defaultClient];
    
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

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (IBAction)createUser:(id)sender {
    User *newUser = [[User alloc] initIntoManagedObjectContext:self.managedObjectContext];
    [newUser setValue:self.usernameField.text forKey:[newUser primaryKeyField]];
    [newUser setPassword:self.passwordField.text];
    
    [self.managedObjectContext saveOnSuccess:^{
        
        NSLog(@"You created a new user object!");
        
    } onFailure:^(NSError *error) {
        
        [self.managedObjectContext deleteObject:newUser];
        [newUser removePassword];
        NSLog(@"There was an error! %@", error);
        
    }];

}

- (IBAction)login:(id)sender {
    [self.client loginWithUsername:self.usernameField.text password:self.passwordField.text onSuccess:^(NSDictionary *results) {

            NSLog(@"Login Success %@",results);

            /* Uncomment the following if you are using Core Data integration and want to retrieve a managed object representation of the user object.  Store the resulting object or object ID for future use.
         
               Be sure to declare variables you are referencing in this block with the __block storage type modifier, including the managedObjectContext property.
            */
            /*
            // Edit entity name and predicate if you are not using the default user schema with username primary key field.
            NSFetchRequest *userFetch = [[NSFetchRequest alloc] initWithEntityName:@"User"];
            [userFetch setPredicate:[NSPredicate predicateWithFormat:@"username == %@", [results objectForKey:@"username"]]];
            [self.managedObjectContext executeFetchRequest:userFetch onSuccess:^(NSArray *results) {
                NSManagedObject *userObject = [results lastObject];
                // Store userObject somewhere for later use
                NSLog(@"Fetched user object: %@", userObject);
            } onFailure:^(NSError *error) {
                NSLog(@"Error fetching user object: %@", error);
            }];
            */

    } onFailure:^(NSError *error) {
            NSLog(@"Login Fail: %@",error);
    }];
}
@end
```

### Build and Run!

Run your project.  Create a user if you haven't already.  Then, enter the username and password and click the login button. In the XCode console log, you'll see the user object returned.


## Check the Login Status

Select the storyboard file and drag and drop a **label** and a **button** to your view.  Set the label text to **Status Unknown** and the button label as **Check Status**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-04.png">
<br>
<br>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)
<br>

Hold the **Control** key and click on the **label** and drag to your **ViewController.h** file.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-05.png">
<br/>
<br/>
In the menu, set the Connection to **Outlet** and enter the name **statusLabel**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-06.png">
<br><br>
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.<br>

In the menu, set the Connection to **Action** and enter the name **checkStatus**
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-07.png">

<br />
<br />

### Status: Edit ViewController.h

Your ViewController.h file should look like the following:

```obj-c

#import <UIKit/UIKit.h>

@class SMClient;

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;
@property (strong, nonatomic) SMClient *client;
@property (weak, nonatomic) IBOutlet UILabel *statusLabel;

- (IBAction)createUser:(id)sender;
- (IBAction)login:(id)sender;
- (IBAction)checkStatus:(id)sender;

@end
```

### Status: Edit ViewController.m

Add the following **highlighted** code to your ViewController.m file:

```obj-c,99-109

#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "User.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize usernameField = _usernameField;
@synthesize passwordField = _passwordField;
@synthesize client = _client;
@synthesize statusLabel = _statusLabel;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	 
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    self.client = [self.appDelegate client];
    
    self.usernameField.delegate = self;
    self.passwordField.delegate = self;
}

- (void)viewDidUnload
{
    [self setUsernameField:nil];
    [self setPasswordField:nil];
    [self setStatusLabel:nil];
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

- (IBAction)createUser:(id)sender {
    User *newUser = [[User alloc] initIntoManagedObjectContext:self.managedObjectContext];
    [newUser setValue:self.usernameField.text forKey:[newUser primaryKeyField]];
    [newUser setPassword:self.passwordField.text];
    
    [self.managedObjectContext saveOnSuccess:^{
        
        NSLog(@"You created a new user object!");
        
    } onFailure:^(NSError *error) {
        
        [self.managedObjectContext deleteObject:newUser];
        [newUser removePassword];
        NSLog(@"There was an error! %@", error);
        
    }];

}

- (IBAction)login:(id)sender {
    [self.client loginWithUsername:self.usernameField.text password:self.passwordField.text onSuccess:^(NSDictionary *results) {

            NSLog(@"Login Success %@",results);

            /* Uncomment the following if you are using Core Data integration and want to retrieve a managed object representation of the user object.  Store the resulting object or object ID for future use.
         
               Be sure to declare variables you are referencing in this block with the __block storage type modifier, including the managedObjectContext property.
            */
            /*
            // Edit entity name and predicate if you are not using the default user schema with username primary key field.
            NSFetchRequest *userFetch = [[NSFetchRequest alloc] initWithEntityName:@"User"];
            [userFetch setPredicate:[NSPredicate predicateWithFormat:@"username == %@", [results objectForKey:@"username"]]];
            [self.managedObjectContext executeFetchRequest:userFetch onSuccess:^(NSArray *results) {
                NSManagedObject *userObject = [results lastObject];
                // Store userObject somewhere for later use
                NSLog(@"Fetched user object: %@", userObject);
            } onFailure:^(NSError *error) {
                NSLog(@"Error fetching user object: %@", error);
            }];
            */

    } onFailure:^(NSError *error) {
            NSLog(@"Login Fail: %@",error);
    }];
}

- (IBAction)checkStatus:(id)sender {
    if([self.client isLoggedIn]) {
        
        [self.client getLoggedInUserOnSuccess:^(NSDictionary *result) {
            self.statusLabel.text = [NSString stringWithFormat:@"Hello, %@", [result objectForKey:@"username"]];
        } onFailure:^(NSError *error) {
            NSLog(@"No user found");
        }];
        
    } else {
        self.statusLabel.text = @"Nope, not Logged In";
    }
}
@end
```

### Build and Run!

Run your project.  Click the check status button, to see if anyone is logged in. You'll see the status label updated.


## Logout

Select the storyboard file and drag and drop a **button** to your view.  Set thebutton label as **logout**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-08.png">
<br>
<br>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)
<br>

Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.<br>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-09.png">
<br><br>

In the menu, set the Connection to **Action** and enter the name **logout**
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/user-authentication/user-authentication-10.png">

<br />
<br />

### Logout: Edit ViewController.h

Your ViewController.h file should look like the following:

```obj-c

#import <UIKit/UIKit.h>

@class SMClient;

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;
@property (strong, nonatomic) SMClient *client;
@property (weak, nonatomic) IBOutlet UILabel *statusLabel;

- (IBAction)createUser:(id)sender;
- (IBAction)login:(id)sender;
- (IBAction)checkStatus:(id)sender;
- (IBAction)logout:(id)sender;

@end

```

### Logout: Edit ViewController.m

Add the following **highlighted** code to your ViewController.m file:

```obj-c,112-116


#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "User.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize statusLabel = _statusLabel;

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
    self.client = [self.appDelegate client];
    
    self.usernameField.delegate = self;
    self.passwordField.delegate = self;
}

- (void)viewDidUnload
{
    [self setUsernameField:nil];
    [self setPasswordField:nil];
    [self setStatusLabel:nil];
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

- (IBAction)createUser:(id)sender {
    User *newUser = [[User alloc] initIntoManagedObjectContext:self.managedObjectContext];
    [newUser setValue:self.usernameField.text forKey:[newUser primaryKeyField]];
    [newUser setPassword:self.passwordField.text];
    
    [self.managedObjectContext saveOnSuccess:^{
        
        NSLog(@"You created a new user object!");
        
    } onFailure:^(NSError *error) {
        
        [self.managedObjectContext deleteObject:newUser];
        [newUser removePassword];
        NSLog(@"There was an error! %@", error);
        
    }];

}

- (IBAction)login:(id)sender {
    [self.client loginWithUsername:self.usernameField.text password:self.passwordField.text onSuccess:^(NSDictionary *results) {

            NSLog(@"Login Success %@",results);

            /* Uncomment the following if you are using Core Data integration and want to retrieve a managed object representation of the user object.  Store the resulting object or object ID for future use.
         
               Be sure to declare variables you are referencing in this block with the __block storage type modifier, including the managedObjectContext property.
            */
            /*
            // Edit entity name and predicate if you are not using the default user schema with username primary key field.
            NSFetchRequest *userFetch = [[NSFetchRequest alloc] initWithEntityName:@"User"];
            [userFetch setPredicate:[NSPredicate predicateWithFormat:@"username == %@", [results objectForKey:@"username"]]];
            [self.managedObjectContext executeFetchRequest:userFetch onSuccess:^(NSArray *results) {
                NSManagedObject *userObject = [results lastObject];
                // Store userObject somewhere for later use
                NSLog(@"Fetched user object: %@", userObject);
            } onFailure:^(NSError *error) {
                NSLog(@"Error fetching user object: %@", error);
            }];
            */

    } onFailure:^(NSError *error) {
            NSLog(@"Login Fail: %@",error);
    }];
}

- (IBAction)checkStatus:(id)sender {
    if([self.client isLoggedIn]) {
        
        [self.client getLoggedInUserOnSuccess:^(NSDictionary *result) {
            self.statusLabel.text = [NSString stringWithFormat:@"Hello, %@", [result objectForKey:@"username"]];
        } onFailure:^(NSError *error) {
            NSLog(@"No user found");
        }];
        
    } else {
        self.statusLabel.text = @"Nope, not Logged In";
    }
}

- (IBAction)logout:(id)sender {
    [self.client logoutOnSuccess:^(NSDictionary *result) {
        NSLog(@"Success, you are logged out");
    } onFailure:^(NSError *error) {
        NSLog(@"Logout Fail: %@",error);
    }];
}
@end
```

### Build and Run!

Run your project.  Click the logout button, then click check status.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/user-authentication.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>