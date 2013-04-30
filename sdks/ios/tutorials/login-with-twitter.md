Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/twitter.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Login with Twitter and if necessary create a new user on StackMob and link to their Twitter account.  You can check their logged in status and logout a user.

<h3>Experience Level</h3>
Intermediate

<h3>Estimated time to complete</h3>
~15 minutes

<h3>Prerequisites</h3>

* XCode 4.5 and greater

* iOS SDK 6 and greater

* <a href="https://github.com/seancook/TWReverseAuthExample" target="_blank">Twitter Reverse OAuth</a> github project

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

<h3>Jump to:</h3>

* <a href="#addkey">Add your Public Key</a>
* <a href="#download">Download Twitter Reverse OAuth repo</a>
* <a href="#addreverseoauth">Add Reverse OAuth files to our project</a>
* <a href="#addframework">Add required Frameworks to our project</a>
* <a href="#editapp">Create a Twitter App</a>
* <a href="#addkeystomodule">Add Keys To Twitter Module</a>
* <a href="#createview">Edit Your View</a>
* <a href="#addcode">Add Login, Logout and Check Status code</a>
* <a href="#additional_methods">Additional Twitter Integration Methods</a>


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
<h2>Download Twitter Reverse OAuth project</h2>

iOS 6 introduced Social integration that allows users to store their login credentials for Twitter on an iOS device.  StackMob-Twitter login uses the Twitter consumer key and secret provided via the OAuth process.  We'll use a bit of code to perform a reverse OAuth to get the key and secret from Twitter based on the Twitter accounts setup on the device. 
If you haven't already,  <a href="https://github.com/seancook/TWReverseAuthExample" target="_blank">download the Twitter Reverse OAuth</a> github project.
<br/>
<br/>
For more background on Twitter Reverse OAuth, checkout the <a href="https://dev.twitter.com/docs/ios/using-reverse-auth" target="_blank">Twitter Developer docs</a>.


<a name="addreverseoauth"></a>
<h2>Add reverse OAuth files to our project</h2>

Drag and drop to copy the following files from the Twitter Reverse OAuth project into the **base-project** folder.

From the **Source** folder

* entire **Vendor** folder

From the **Source/Classes** folder

* TWSignedRequest.h 
* TWSignedRequest.m
* TWAPIManager.h 
* TWAPIManager.m 

<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-03.png">
</p>

Now, 3 files in the vendor file are not using automatic reference counting (ARC), so we'll need to tell ARC to ignore those 3 files.  Select your target and under the Build Phases > Compile Sources highlight the following 3 files.

* NSData+Base64.m
* OAuth+Additions.m
* OAuthCore.m

Double-click and enter **-fno-objc-arc** into the input box that appears.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-02.png">
</p>


If they are not already listed, add **TWSignedRequest.m** and **TWAPIManager.m** to the Compile Sources list.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-04.png">
</p>

<a name="addframework"></a>
<h2>Add required Frameworks to our project</h2>

In XCode, go to **Targets > Build Phases** and select **Link Binary with Libraries**
<br/>

Search for the **Social.framework** library and add it to your project.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-01.png">
</p>

Repeat this step for the **Twitter.framework** and **Accounts.framework** libraries.
<br/>

<a name="createapp"></a>
<h2>Create Twitter App</h2>
Head over to <a href="https://dev.twitter.com/" target="_blank">dev.twitter.com</a> and sign in. 

Select **my applications** under your account menu.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-05.png">
</p>


Click Create New Application and complete the details about your Twitter app.
<p class="screenshot">
<img width="600px" src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-06.png">
</p>

After your application is created, copy the **Consumer key** and **Consumer Secret**, return to your Xcode project and paste them into the **TWSignedRequest.m**.
<p class="screenshot">
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-07.png">
</p>

<h3>TWSignedRequest.m</h3> 

```obj-c,9,10
#import "OAuthCore.h"
#import "TWSignedRequest.h"

#define TW_HTTP_METHOD_GET @"GET"
#define TW_HTTP_METHOD_POST @"POST"
#define TW_HTTP_METHOD_DELETE @"DELETE"
#define TW_HTTP_HEADER_AUTHORIZATION @"Authorization"

#define kTWConsumerKey  @"TWITTER_CONSUMER_KEY"
#define kTWConsumerSecret @"TWITTER_CONSUMER_SECRET"

(code abbreviated)

@end
```

<a name="addkeystomodule"></a>
<h2>Add Keys To Twitter Module</h2>

Head over to the <a href="https://dashboard.stackmob.com/module/twittersm/settings" target="_blank">Twitter Module Settings</a> for your StackMob app and add the Twitter Consumer Key and Secret (Develpment section for now).  This allows StackMob to authenticate the request signature when you login with twitter credentials.

<a name="editview"></a>
<h2>Edit Your View</h2> 

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



<a name="addcode"></a>
<h2>Open ViewController.h</h2>

Add the following **highlighted** code to your ViewController.h file: 

```obj-c,3,7-8
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UIActionSheetDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@property (strong, nonatomic) NSString *oauthToken;
@property (strong, nonatomic) NSString *oauthTokenSecret;

- (IBAction)loginUser:(id)sender;
- (IBAction)checkStatus:(id)sender;
- (IBAction)logoutUser:(id)sender;

@end
```

<h2>Open ViewController.m</h2>

Add the following **highlighted** code to your ViewController.m file: 

```obj-c,4-6,10-13,15-18,25-26,35,45-46,48-52,60-63,73-76,81-91,96-103,108-115,118-167,172-211,214-230,235-266

#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import <Accounts/Accounts.h>
#import <Twitter/Twitter.h>
#import "TWAPIManager.h"

@interface ViewController ()

@property (nonatomic, strong) ACAccountStore *accountStore;
@property (nonatomic, strong) TWAPIManager *apiManager;
@property (nonatomic, strong) NSArray *accounts;
@property (nonatomic, strong) UIButton *reverseAuthBtn;

- (void)loginWithTwitterToken:(id)oauthToken secret:(id)oauthTokenSecret usernameForCreate:(NSString *)username;
- (void)obtainAccessToAccountsWithBlock:(void (^)(BOOL))block;
- (void)refreshTwitterAccounts:(id)sender;
- (void)performReverseAuth;

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize oauthToken = _oauthToken ;
@synthesize oauthTokenSecret = _oauthTokenSecret;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self refreshTwitterAccounts:self];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    _accountStore = [[ACAccountStore alloc] init];
    _apiManager = [[TWAPIManager alloc] init];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(refreshTwitterAccounts:)
     name:ACAccountStoreDidChangeNotification
     object:nil];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    
    [[NSNotificationCenter defaultCenter]
     removeObserver:self
     name:ACAccountStoreDidChangeNotification
     object:nil];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (IBAction)loginUser:(id)sender {
    
    /*
     Initiate Twitter login.
     */
    [self performReverseAuth];
}

- (IBAction)checkStatus:(id)sender {
    
    NSLog(@"%@",[[self.appDelegate client] isLoggedIn] ? @"Logged In" : @"Logged Out");
    
    /*
     StackMob method to grab the currently logged in user's Twitter information.
     This assumes the user was logged in user Twitter credentials.
     */
    [[self.appDelegate client] getLoggedInUserTwitterInfoOnSuccess:^(NSDictionary *result) {
        NSLog(@"Logged In User Twitter Info, %@", result);
    } onFailure:^(NSError *error) {
        NSLog(@"error %@", error);
    }];
}

- (IBAction)logoutUser:(id)sender {
    
    /*
     StackMob method to logout the currently logged in user.
     */
    [[self.appDelegate client] logoutOnSuccess:^(NSDictionary *result) {
        NSLog(@"Logged out.");
    } onFailure:^(NSError *error) {
        NSLog(@"error %@", error);
    }];
}

- (void)loginWithTwitterToken:(id)oauthToken secret:(id)oauthTokenSecret usernameForCreate:(NSString *)username {
    
    /*
     StackMob method to login with Twitter token and secret.  A StackMob user will be created with the username provided if one doesn't already exist attached to the provided credentials.
     */
    [[self.appDelegate client] loginWithTwitterToken:oauthToken twitterSecret:oauthTokenSecret createUserIfNeeded:YES usernameForCreate:username onSuccess:^(NSDictionary *result) {
        NSLog(@"successful login with twitter: %@", result);
    } onFailure:^(NSError *error) {
        NSLog(@"login user fail: %@", error);
    }];
}

- (void)actionSheet:(UIActionSheet *)actionSheet
clickedButtonAtIndex:(NSInteger)buttonIndex
{
    /*
     This code is called after you select your Twitter account from the popup.
     It will grab your keys and perform a createUser / login with Twitter through
     StackMob.
     */
    if (buttonIndex != (actionSheet.numberOfButtons - 1)) {
        [_apiManager
         performReverseAuthForAccount:_accounts[buttonIndex]
         withHandler:^(NSData *responseData, NSError *error) {
             if (responseData) {
                 NSString *responseStr = [[NSString alloc]
                                          initWithData:responseData
                                          encoding:NSUTF8StringEncoding];
                 
                 NSArray *parts = [responseStr
                                   componentsSeparatedByString:@"&"];
                 
                 
                 NSLog(@"parts: %@", parts.description);
                 
                 // Get oauth_token
                 NSString *oauth_tokenKV = [parts objectAtIndex:0];
                 NSArray *oauth_tokenArray = [oauth_tokenKV componentsSeparatedByString:@"="];
                 NSString *oauth_token = [oauth_tokenArray objectAtIndex:1];
                 
                 // Get oauth_token_secret
                 NSString *oauth_token_secretKV = [parts objectAtIndex:1];
                 NSArray *oauth_token_secretArray = [oauth_token_secretKV componentsSeparatedByString:@"="];
                 NSString *oauth_token_secret = [oauth_token_secretArray objectAtIndex:1];
                 
                 // Get screen name
                 NSString *twitter_screen_nameKV = [parts objectAtIndex:3];
                 NSArray *twitter_screen_nameArray = [twitter_screen_nameKV componentsSeparatedByString:@"="];
                 NSString *twitter_screen_name = [twitter_screen_nameArray objectAtIndex:1];
                 
                 /*
                  Login to StackMob using Twitter credentials.
                  */
                 [self loginWithTwitterToken:oauth_token secret:oauth_token_secret usernameForCreate:twitter_screen_name];
                 
             }
             else {
                 NSLog(@"Error!\n%@", [error localizedDescription]);
             }
         }];
    }
}



#pragma Twitter methods
- (void)obtainAccessToAccountsWithBlock:(void (^)(BOOL))block
{
    /*
     Get Twitter account info.
     */
    NSLog(@"obtain access");
    ACAccountType *twitterType = [_accountStore
                                  accountTypeWithAccountTypeIdentifier:
                                  ACAccountTypeIdentifierTwitter];
    
    ACAccountStoreRequestAccessCompletionHandler handler =
    ^(BOOL granted, NSError *error) {
        
        if (granted) {
            self.accounts = [_accountStore accountsWithAccountType:twitterType];
            NSLog(@"acct %@", self.accounts.description);
        } else {
            NSLog(@"error %@", error);
        }
        
        block(granted);
    };
    
    //  This method changed in iOS6.  If the new version isn't available, fall
    //  back to the original (which means that we're running on iOS5+).
    if ([_accountStore
         respondsToSelector:@selector(requestAccessToAccountsWithType:
                                      options:
                                      completion:)]) {
             
             [_accountStore requestAccessToAccountsWithType:twitterType
                                                    options:nil
                                                 completion:handler];
         }
    else {
        
        [_accountStore requestAccessToAccountsWithType:twitterType
                                 withCompletionHandler:handler];
    }
}


- (void)refreshTwitterAccounts:(id)sender
{
    /* 
     Initiate get Twitter account info method.
     */
    //  Get access to the user's Twitter account(s)
    [self obtainAccessToAccountsWithBlock:^(BOOL granted) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (granted) {
                //_reverseAuthBtn.enabled = YES;
            }
            else {
                NSLog(@"You were not granted access to the Twitter accounts.");
            }
        });
    }];
}




- (void)performReverseAuth
{
    /*
     Initiates a popup for you to select the Twitter account to sign in with.
     */
    if ([TWAPIManager isLocalTwitterAccountAvailable]) {
        UIActionSheet *sheet = [[UIActionSheet alloc]
                                initWithTitle:@"Choose an Account"
                                delegate:self
                                cancelButtonTitle:nil
                                destructiveButtonTitle:nil
                                otherButtonTitles:nil];
        NSLog(@"Local available ");
        for (ACAccount *acct in _accounts) {
            [sheet addButtonWithTitle:acct.username];
        }
        
        [sheet addButtonWithTitle:@"Cancel"];
        [sheet setDestructiveButtonIndex:[_accounts count]];
        [sheet showInView:self.view];
    }
    else {
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:@"No Accounts"
                              message:@"Please configure a Twitter "
                              "account in Settings.app"
                              delegate:nil
                              cancelButtonTitle:@"OK"
                              otherButtonTitles:nil];
        [alert show];
    }
}

@end


```


<h2>Build and Run!</h2>
You must have a Twitter account already set up on your device/simulator before you run your project.  To set one up go to Settings -> Twitter.

<p class="screenshot"> 
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/twitter/twitter-11.png">
</p>


Now, run your project and click the **Login with Twitter** button. An accounts panel will slide onto the screen.  Select your twitter account. If your user doesn't have an account in your StackMob app, one will be created, then you will be logged in automatically.  If one exists, the user will just be logged in.  Click **Check Status** to confirm you are now logged into Stackmob.  It will also return your Twitter Info into the debug console.  Lastly, click **Logout User** and check your status again to confirm you are logged out.

<a name="additional_methods"></a>
<h2>Additional Twitter Integration Methods</h2>

This tutorial has gone over the simplest way to create a StackMob user and log them in using Twitter credentials.  However, there may be cases when you want to create a user with Twitter credentials seperately, or unlink a Twitter token from a user so they can no longer login using that particular service.  Let's look at the other Twitter integration methods StackMob provides:

**Note:** The methods shown below have multiple signatures with additional parameters for providing request options, callback queues, etc.  All methods are listed in the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMClient.html" target="_blank">SMClient Class Reference</a>.

<h3>createUserWithTwitterToken:twitterSecret:username:onSuccess:onFailure:</h3>

This will create a new StackMob user and link them with the Twitter token provided.  An error will occur if a user linked to the provided token already exists.  Pass nil to the username parameter to default the username to the user ID provided by Twitter.

<h3>loginWithTwitterToken:twitterSecret:onSuccess:onFailure:</h3>

Login the user to StackMob linked with the Twitter token provided.  The method calls loginWithTwitterToken:twitterSecret:createUserIfNeeded:usernameForCreate:onSuccess:onFailure: with createUserIfNeeded:NO and usernameForCreate:nil.

<h3>linkLoggedInUserWithTwitterToken:twitterSecret:onSuccess:onFailure:</h3>

After logging in a user to StackMob using a StackMob username/password, this links the logged in user with the Twitter token provided.  Upon success, that user can then be logged in using the loginWithTwitterToken:twitterSecret:onSuccess:onFailure: method.

<h3>unlinkLoggedInUserFromTwitterOnSuccess:onFailure:</h3>

Removes any linked Twitter token from the currently logged in user.  Upon success, that user can no longer login using the loginWithTwitterToken:twitterSecret:onSuccess:onFailure: method.

<h3>updateTwitterStatusWithMessage:onSuccess:onFailure:</h3>

Updates the currently logged in Twitter user's status.  Requires write permissions.

<h3>getLoggedInUserTwitterInfoWithOnSuccess:onFailure:</h3>

Returns the Twitter user info for the currently logged in user.

<h2>Congratulations on completing this tutorial!</h2>

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/twitter.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>



