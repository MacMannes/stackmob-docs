<div class="alert">Also check out our <a href="https://www.stackmob.com/product/facebook">technology partnership with Facebook!</a> and our <a href="https://developer.stackmob.com/facebook/ios">Facebook demo app</a></div>

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/facebook-login-logout.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Login with Facebook and if necessary create a new user on StackMob and link to their facebook account.  Log the user into StackMob and be able to check their logged in status.

<h3>Experience Level</h3>
Intermediate

<h3>Estimated time to complete</h3>
~15 minutes

<h3>Prerequisites</h3>

* XCode 4.5 and greater

* iOS SDK 6 and greater

* <a href="http://developers.facebook.com/ios/">Facebook iOS SDK</a> 3.1 and greater

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

<h3>Jump to:</h3>

* <a href="#addkey">Add your Public Key</a>
* <a href="#download">Download Facebook SDK</a>
* <a href="#addframework">Add required Frameworks to our project</a>
* <a href="#setflag">Set Other Linker Flag</a>
* <a href="#createview">Create Your View</a>
* <a href="#createapp">Create Facebook App</a>
* <a href="#addid">Add your Facebook AppID to your XCode project</a>
* <a href="#additional_methods">Additional Facebook Integration Methods</a>



<h3>Have you read through the <a href="http://stackmob.github.com/stackmob-ios-sdk/#coding_practices" target="_blank">StackMob <—> Core Data Coding Practices</a>?</h3>

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

<h1>Let's get started!</h1>

<h2>Open the Base Xcode Project</h2>

We’ve created an Xcode project for you as a starting place for this tutorial.  It has StackMob imported and the basic plumbing for Core Data.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the project, see <a href="https://developer.stackmob.com/tutorials/ios/Base-Xcode-Project-for-Tutorials" target="_blank">Base Xcode Project for Tutorials</a>.

Unzip the Base Project and open **base-project.xcodeproj**.


<a name="addkey"></a>
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


<a name="download"></a>
<h2>Download Facebook SDK</h2>

If you haven't already, <a href="http://developers.facebook.com/ios/"> download the Facebook iOS SDK</a>.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-03.png">
<br/>
<br/>
Double-click the **.pkg** file to install the SDK.  The default location is **~/Documents/FacebookSDK**.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-04.png">
<br/>
<br/>

<a name="addframework"></a>
<h2>Add required Frameworks to our project</h2>

In XCode, go to **Targets > Build Phases** and select **Link Binary with Libraries**
<br/>

Search for the **Social.framework** library and add it to your project.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-05.png">
<br/>
<br/>

Repeat this step for the **AdSupport.framework** and **Accounts.framework** libraries
<br/>
<br/>
Then click on **Add Other ...** button, and browse to the FacebookSDK folder, inside you'll find the **FacebookSDK.framework**.  Add this to your project.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-08.png">
<br/>
<br/>

<a name="setflag"></a>
<h2>Set Other Linker Flag</h2> 

In XCode, go to **Targets > Build Settings** and search for **Other Linker Flags**.  You should see the **-ObjC** flag,  Let's add one for the FacebookSDK.  **Double-click** and enter on a new line, **-lsqlite3.0**
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-10.png">
<br/>
<br/>

<a name="createview"></a>
<h2>Create Your View</h2> 

Select the storyboard file and drag and drop  **2 buttons** on to your view.  No need to change the label for the first button, we are going to dynamically set it to either "login" or "logout".  Change the label on the second button to **check status**
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the first **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Outlet** and enter the name **buttonLoginLogout**.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-12.png">
<br/>
<br/>
On the **same button** hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **buttonClickHandler**.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-13.png">
<br/>
<br/>
On the **second button** hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **checkStatus**.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-14.png">
<br/>


<h2>Open AppDelegate.h</h2>

Add the following **highlighted** code to your AppDelegate.h file: 

```obj-c,3,8,16
#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>
#import <FacebookSDK/FacebookSDK.h>

@class SMClient;
@class SMCoreDataStore;

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (strong, nonatomic) SMCoreDataStore *coreDataStore;
@property (strong, nonatomic) SMClient *client;
@property (strong, nonatomic) ViewController *viewController;

@end

```


<h2>Open AppDelegate.m</h2>

Add the following **highlighted** code to your AppDelegate.m file: 

```obj-c,6,8-14,54,59

@implementation AppDelegate
@synthesize client = _client;
@synthesize managedObjectModel = _managedObjectModel;
@synthesize coreDataStore = _coreDataStore;
@synthesize window = _window;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
    // attempt to extract a token from the url
    return [FBSession.activeSession handleOpenURL:url];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    [self.window makeKeyAndVisible];
    return YES;
}

- (NSManagedObjectModel *)managedObjectModel
{
    if (_managedObjectModel != nil) {
        return _managedObjectModel;
    }
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"mydatamodel" withExtension:@"momd"];
    _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return _managedObjectModel;
}
              
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [FBSession.activeSession handleDidBecomeActive];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [FBSession.activeSession close];
}

@end

```

<h2>Open ViewController.m</h2>

Add the following **highlighted** code to your ViewController.m file.  
<br>
The view will update based on the user's logged in status.  You'll login to StackMob using the Facebook access token of the current session.  If a user doesn't already exist StackMob linked with the provided token, one is created.  You can log a user out or check their status.



```obj-c,23,25,27-31,36-43,58-64,68,74-94,100-105,108-136


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
    
    [self updateView];
    
    self.client = [self.appDelegate client];
    
    if (FBSession.activeSession.state == FBSessionStateCreatedTokenLoaded) {
        [FBSession openActiveSessionWithReadPermissions:nil allowLoginUI:YES completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
            [self sessionStateChanged:session state:status error:error];
        }];
    }
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}

- (void)updateView {

    if ([self.client isLoggedIn]) {
        [self.buttonLoginLogout setTitle:@"Log out" forState:UIControlStateNormal];
    } else {
        [self.buttonLoginLogout setTitle:@"Login with Facebook" forState:UIControlStateNormal];
    }
}

- (void)viewDidUnload
{
    [self setButtonLoginLogout:nil];
    [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (IBAction)buttonClickHandler:(id)sender {

    if ([self.client isLoggedIn]) {
        [self logoutUser];
    } else {
        [FBSession openActiveSessionWithReadPermissions:nil allowLoginUI:YES completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
            [self sessionStateChanged:session state:status error:error];
        }];
    }
}

- (IBAction)checkStatus:(id)sender {
    NSLog(@"%@",[self.client isLoggedIn] ? @"Logged In" : @"Logged Out");
    
}

- (void)loginUser {
    
    /*
     Initiate a request for the current Facebook session user info, and apply the username to
     the StackMob user that might be created if one doesn't already exist.  Then login to StackMob with Facebook credentials.
     */
    [[FBRequest requestForMe] startWithCompletionHandler:
     ^(FBRequestConnection *connection,
       NSDictionary<FBGraphUser> *user,
       NSError *error) {
         if (!error) {
             [self.client loginWithFacebookToken:FBSession.activeSession.accessTokenData.accessToken createUserIfNeeded:YES usernameForCreate:user.username onSuccess:^(NSDictionary *result) {
                 NSLog(@"Logged in with StackMob");
                 [self updateView];
             } onFailure:^(NSError *error) {
                 NSLog(@"Error: %@", error);
             }];
         } else {
             // Handle error accordingly
             NSLog(@"Error getting current Facebook user data, %@", error);
         }
         
     }];
}


- (void)logoutUser {
    
    [self.client logoutOnSuccess:^(NSDictionary *result) {
        NSLog(@"Logged out of StackMob");
        [FBSession.activeSession closeAndClearTokenInformation];
    } onFailure:^(NSError *error) {
        NSLog(@"Error: %@", error);
    }];
}

- (void)sessionStateChanged:(FBSession *)session
                      state:(FBSessionState) state
                      error:(NSError *)error
{
    switch (state) {
        case FBSessionStateOpen:
            [self loginUser];
            break;
        case FBSessionStateClosed:
            [FBSession.activeSession closeAndClearTokenInformation];
            [self updateView];
            break;
        case FBSessionStateClosedLoginFailed:
            [FBSession.activeSession closeAndClearTokenInformation];
            break;
        default:
            break;
    }
    
    if (error) {
        UIAlertView *alertView = [[UIAlertView alloc]
                                  initWithTitle:@"Error"
                                  message:error.localizedDescription
                                  delegate:nil
                                  cancelButtonTitle:@"OK"
                                  otherButtonTitles:nil];
        [alertView show];
    }    
}



```

<a name="createapp"></a>
<h2>Create Facebook App</h2>
Go to <a href="https://developers.facebook.com/apps" target="_blank">developers.facebook.com</a> to create a Facebook App. Click the **create new app** button.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-16.png">
<br/>
<br/>

Give your Facebook app a name and click continue.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-17.png">
<br/>
<br/>

**Copy the App ID**, and return to XCode.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-18.png">
<br/>
<br/>

<a name="addid"></a>
<h2>Add your Facebook AppID to your XCode project</h2>

In the **Project Navigator**, look in the **supporting files folder** for your project **.plist** file.  Add your Facebook App ID, in the two places indicated in the screenshot below.
<br/>
<br/>

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-15.png">
<br/>

<h2>Build and Run!</h2>

Run your project, click the **Login with Facebook** button. A browser will open and you will be prompted to login to facebook and authorize your application.  When returned, the login button will update to a logout button.  Click **Check Status** to confirm you are now logged into Stackmob.


<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/facebook/fb-19.png">
<br/>
<br/>

<a name="additional_methods"></a>
<h2>Additional Facebook Integration Methods</h2>

This tutorial has gone over the simplest way to create a StackMob user and log them in using Facebook credentials.  However, there may be cases when you want to create a user with Facebook credentials seperately, or unlink a Facebook token from a user so they can no longer login using that particular service.  Let's look at the other Facebook integration methods StackMob provides:

**Note:** The methods shown below have multiple signatures with additional parameters for providing request options, callback queues, etc.  All methods are listed in the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMClient.html" target="_blank">SMClient Class Reference</a>.

<h3>createUserWithFacebookToken:username:onSuccess:onFailure:</h3>

This will create a new StackMob user and link them with the Facebook token provided.  An error will occur if a user linked to the provided token already exists.  Pass nil to the username parameter to default the username to the user ID provided by Facebook.

<h3>loginWithFacebookToken:onSuccess:onFailure:</h3>

Login the user to StackMob linked with the Facebook token provided.  The method calls loginWithFacebookToken:createUserIfNeeded:usernameForCreate:onSuccess:onFailure: with createUserIfNeeded:NO and usernameForCreate:nil.

<h3>linkLoggedInUserWithFacebookToken:onSuccess:onFailure:</h3>

After logging in a user to StackMob using a StackMob username/password, this links the logged in user with the Facebook token provided.  Upon success, that user can then be logged in using the loginWithFacebookToken:onSuccess:onFailure: method.

<h3>unlinkLoggedInUserFromFacebookOnSuccess:onFailure:</h3>

Removes any linked Facebook token from the currently logged in user.  Upon success, that user can no longer login using the loginWithFacebookToken:onSuccess:onFailure: method.

<h3>updateFacebookStatusWithMessage:onSuccess:onFailure:</h3>

Updates the currently logged in Facebook user's status.  Requires write permissions.

<h3>getLoggedInUserFacebookInfoWithOnSuccess:onFailure:</h3>

Returns the Facebook user info for the currently logged in user.

<h2>Congratulations on completing this tutorial!</h2>

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/facebook-login-logout.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
