iOS Twitter
===========

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/twitter.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Login with Twitter and if necessary create a new user on StackMob and link to their Twitter account.  You can check their logged in status and logout a user.

### Experience Level
Intermediate

### Estimated time to complete
~15 minutes

### Prerequisites

* XCode 4.5 and greater

* iOS SDK 6 and greater

* <a href="https://s3.amazonaws.com/static.stackmob.com/resources/ios/SMTwitterCredentials.zip" target="_blank">SMTwitterCredentials</a> files

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

## Download SMTwitterCredentials Files

iOS 6 introduced Social integration that allows users to store their login credentials for Twitter on an iOS device. StackMob-Twitter login uses the Twitter consumer key and secret provided via the OAuth process.  

We'll use some additional StackMob files to perform a reverse OAuth to get the key and secret from Twitter based on the Twitter accounts setup on the device.

The SMTwitterCredentials files provide the following workflow: You'll first declare an SMTwitterCredentials property. Just declaring an instance variable won't work because the workflow involves an action sheet pop-up, and the instance will be deallocated unless it's retained with "strong". When you initialize your instance, you'll pass in your Twitter consumer key and secret.

When you're ready to login in a user, you'll first open a session by requesting an session access token and secret from Twitter. This is all done using the retrieveTwitterCredentialsForAccount:onSuccess:onFailure: method. In the success block, you'll then be passed the session token and secret, which you can in turn pass to the StackMob loginWithTwitter:... method.

If you haven't already,  <a href="https://s3.amazonaws.com/static.stackmob.com/resources/ios/SMTwitterCredentials.zip" target="_blank">download the SMTwitterCredentials files</a>.
<br/>
<br/>
For more background on Twitter Reverse OAuth, checkout the <a href="https://dev.twitter.com/docs/ios/using-reverse-auth" target="_blank">Twitter Developer docs</a>.
<br/>
<br/>
<b>Unzip the SMTwitterCredentials files and copy the entire folder into your Xcode project.</b>

## Add required Frameworks

In XCode, go to **Targets > Build Phases** and select **Link Binary with Libraries**
<br/>

Search for the **Social.framework** library and add it to your project.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-01.png">
</p>

Repeat this step for the **Twitter.framework** and **Accounts.framework** libraries.
<br/>

## Create Twitter App
Head over to <a href="https://dev.twitter.com/" target="_blank">dev.twitter.com</a> and sign in. 

Select **my applications** under your account menu.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-05.png">
</p>


Click Create New Application and complete the details about your Twitter app.
<p class="screenshot">
<img width="600px" src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-06.png">
</p>

## Add Keys To Twitter Module

Head over to the <a href="https://dashboard.stackmob.com/module/twittersm/settings" target="_blank">Twitter Module Settings</a> for your StackMob app and add the Twitter Consumer Key and Secret (Develpment section for now).  This allows StackMob to authenticate the request signature when you login with twitter credentials.

You'll add your Twitter keys to your Xcode project in just a minute, so it might be wise to keep the settings page open so you can easily copy/paste them.

## Edit Your View 

Select the storyboard file and drag and drop  **3 buttons** on to your view.  Change the label on the  buttons to **Login with Twitter**, **Check Status** and **Logout User**
<p class="screenshot"> 
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-10.png">
</p>

Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the first **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **loginUser**.
<p class="screenshot"> 
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-09.png">
</p>
On the **other buttons** hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **checkStatus** and  **logoutUser**.


## Edit ViewController.m

Add the following **highlighted** code to your ViewController.m file: 

```obj-c,4,6-10,15,24,41-66,72,78-82,90-94

#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "SMTwitterCredentials.h"

@interface ViewController ()

@property (nonatomic, strong) SMTwitterCredentials *twitterCredentials;

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize twitterCredentials = _twitterCredentials;

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.managedObjectContext = [[[SMClient defaultClient] coreDataStore] contextForCurrentThread];
    
    self.twitterCredentials = [[SMTwitterCredentials alloc] initWithTwitterConsumerKey:@"TWITTER_APP_KEY" secret:@"TWITTER_APP_SECRET"];
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

- (IBAction)loginUser:(id)sender {
    
    // This will usually return true if you are using the simulator, even if there are no accounts
    if (self.twitterCredentials.twitterAccountsAvailable) {
        
        /*
         SMTwitterCredentials method for Twitter auth workflow.
         Pass nil for username to show a pop-up to the user and allow them to select from the available accounts.
         Pass an account username to search and use that account without any user interaction. Great technique for a "stay logged in" feature
         */
        [self.twitterCredentials retrieveTwitterCredentialsForAccount:nil onSuccess:^(NSString *token, NSString *secret, NSDictionary *fullResponse) {
            
            /*
             StackMob method to login with Twitter token and secret.  A StackMob user will be created with the username provided if one doesn't already exist attached to the provided credentials.
             */
            [[SMClient defaultClient] loginWithTwitterToken:token twitterSecret:secret createUserIfNeeded:YES usernameForCreate:fullResponse[@"screen_name"] onSuccess:^(NSDictionary *result) {
                NSLog(@"Successful Login with Twitter: %@", result);
            } onFailure:^(NSError *error) {
                NSLog(@"Login failed: %@", error);
            }];
            
        } onFailure:^(NSError *error) {
            NSLog(@"Twitter Auth Error: %@", error);
        }];
        
    } else {
        // Handle no Twitter accounts available on device
        NSLog(@"No Tiwtter accounts found on device.");
    }
    
}

- (IBAction)checkStatus:(id)sender {
    
    NSLog(@"%@",[[SMClient defaultClient] isLoggedIn] ? @"Logged In" : @"Logged Out");
    
    /*
     StackMob method to grab the currently logged in user's Twitter information.
     This assumes the user was logged in user Twitter credentials.
     */
    [[SMClient defaultClient] getLoggedInUserTwitterInfoOnSuccess:^(NSDictionary *result) {
        NSLog(@"Logged In User Twitter Info, %@", result);
    } onFailure:^(NSError *error) {
        NSLog(@"error %@", error);
    }];
}

- (IBAction)logoutUser:(id)sender {
    
    /*
     StackMob method to logout the currently logged in user.
     */
    [[SMClient defaultClient] logoutOnSuccess:^(NSDictionary *result) {
        NSLog(@"Logged out.");
    } onFailure:^(NSError *error) {
        NSLog(@"error %@", error);
    }];
}

@end
```


## Build and Run!
You must have a Twitter account already set up on your device/simulator before you run your project.  To set one up go to Settings -> Twitter.

<p class="screenshot"> 
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-11.png">
</p>


Now, run your project and click the **Login with Twitter** button. An accounts panel will slide onto the screen. Select your twitter account. If your user doesn't have an account in your StackMob app, one will be created, then you will be logged in automatically. If one exists, the user will just be logged in. Click **Check Status** to confirm you are now logged into Stackmob.  It will also return your Twitter Info into the debug console. Lastly, click **Logout User** and check your status again to confirm you are logged out.


## Additional Twitter Integration Methods

This tutorial has gone over the simplest way to create a StackMob user and log them in using Twitter credentials. However, there may be cases when you want to create a user with Twitter credentials seperately, or unlink a Twitter token from a user so they can no longer login using that particular service. Let's look at the other Twitter integration methods StackMob provides:

**Note:** The methods shown below have multiple signatures with additional parameters for providing request options, callback queues, etc.  All methods are listed in the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMClient.html" target="_blank">SMClient Class Reference</a>.

### createUserWithTwitterToken:twitterSecret:username:onSuccess:onFailure:

This will create a new StackMob user and link them with the Twitter token provided.  An error will occur if a user linked to the provided token already exists.  Pass nil to the username parameter to default the username to the user ID provided by Twitter.

### loginWithTwitterToken:twitterSecret:onSuccess:onFailure:

Login the user to StackMob linked with the Twitter token provided.  The method calls loginWithTwitterToken:twitterSecret:createUserIfNeeded:usernameForCreate:onSuccess:onFailure: with createUserIfNeeded:NO and usernameForCreate:nil.

### linkLoggedInUserWithTwitterToken:twitterSecret:onSuccess:onFailure:

After logging in a user to StackMob using a StackMob username/password, this links the logged in user with the Twitter token provided.  Upon success, that user can then be logged in using the loginWithTwitterToken:twitterSecret:onSuccess:onFailure: method.

### unlinkLoggedInUserFromTwitterOnSuccess:onFailure:

Removes any linked Twitter token from the currently logged in user.  Upon success, that user can no longer login using the loginWithTwitterToken:twitterSecret:onSuccess:onFailure: method.

### updateTwitterStatusWithMessage:onSuccess:onFailure:

Updates the currently logged in Twitter user's status.  Requires write permissions.

### getLoggedInUserTwitterInfoWithOnSuccess:onFailure:

Returns the Twitter user info for the currently logged in user.

## Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/twitter.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>



