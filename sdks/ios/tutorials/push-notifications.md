iOS Push Notifications
================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/push-notifications.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

### Objective

Register your device for push notifications, create and upload a push certificate to StackMob and send a push notification from the StackMob dashboard to your iOS device.

### Experience Level
Intermediate

### Estimated time to complete
~20 minutes

### Prerequisites

* Push Module installed for your application, available from <a href="https://marketplace.stackmob.com/module/push" target="_blank">Marketplace</a>.

* XCode 4.x and greater

* iOS SDK 5 and greater

* iOS device (iPhone, iPad, etc) The iOS Simulator can not register for Push Notifications.

* Apple Developer Account.  You can register for $99/year at <a href="http://developer.apple.com">developer.apple.com</a>

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

### Using StackMob For Push Only

While the Push API comes built into the Core SDK, a separate Push SDK is available for those using StackMob only for push notifications.  You can download the library from [Github](https://github.com/downloads/stackmob/stackmob-ios-push-sdk/stackmob-ios-push-sdk-v1.0.2.zip).  Drag the StackMobPush-vx.x.x folder into your project with the **Copy items into destination group's folder** checkbox selected, and set your Target's **Other Linker Flags** build setting to **-ObjC**.  You can skip over the next section and start with the **AppDelegate.h** section.

## Open the Base Xcode Project

We’ve created an Xcode project for you as a starting place for this tutorial.  It has StackMob imported and the basic plumbing for Core Data.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the project, see <a href="https://developer.stackmob.com/ios-sdk/base-xcode-project-for-tutorials" target="_blank">Base Xcode Project for Tutorials</a>.

Unzip the Base Project and open **base-project.xcodeproj**.


## Edit AppDelegate.h

Here we reference the SMPushClient class and a SMPushClient variable for application-wide usage:

```obj-c,5,13

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@class SMClient;
@class SMPushClient;
@class SMCoreDataStore;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (strong, nonatomic) SMClient *client;
@property (strong, nonatomic) SMPushClient *pushClient;
@property (strong, nonatomic) SMCoreDataStore *coreDataStore;

@end
```

**Important:** The reason we declare the StackMob Push client variable in AppDelegate, even though it will be used in ViewController, is because AppDelegate is a special file that allows us to access variables declared in its interface file from any other file, using a shared instance of AppDelegate. We declare that instance in ViewController.m.

## Edit AppDelegate.m

Here we add our StackMob Push client variable and initialize it. 
Go to <a href="https://dashboard.stackmob.com/settings" target="_blank">Manage App Info</a> in the StackMob Dashboard and copy the **Development Public Key** and **Development Private Key** and paste it  into the **AppDelegate.m** file where is says **YOUR\_PUBLIC\_KEY** and **YOUR\_PRIVATE\_KEY** in the SMClient/SMPushClient init methods.  

<br/>
You'll also add three methods (**didRegisterForRemoteNotificationsWithDeviceToken** and **didFailToRegisterForRemoteNotificationsWithError**) to handle a success or error when registering a device with **Apple Push Notification Service** (APNS).  The third method **didReceiveRemoteNotification** is an extra method to trigger an Alert when you app is active.  Push notifications only appear for in-active apps.

```obj-c,3,8,15,20-34,36-38,40-56

#import "AppDelegate.h"
#import "StackMob.h"
#import "StackMobPush.h"

@implementation AppDelegate
@synthesize client = _client;
@synthesize coreDataStore = _coreDataStore;
@synthesize pushClient = _pushClient;
@synthesize managedObjectModel = _managedObjectModel;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.pushClient = [[SMPushClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY" privateKey:@"YOUR_PRIVATE_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    return YES;
}

- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
 
    NSString *token = [[deviceToken description] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    token = [[token componentsSeparatedByString:@" "] componentsJoinedByString:@""];
    
    NSLog(@"push token: %@",token);
    
     // Persist token here if you need.  User is an arbitrary string to associate with the token.
    [self.pushClient registerDeviceToken:token withUser:@"Person123" onSuccess:^{
        NSLog(@"successful registration");
    } onFailure:^(NSError *error) {
        NSLog(@"error: %@", [error userInfo]);
    }];
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err {
    NSLog(@"Error in registration. Error: %@", err);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    UIApplicationState state = [application applicationState];
    if (state == UIApplicationStateActive) {
        NSString *cancelTitle = @"Close";
        NSString *showTitle = @"Show";
        NSString *message = [[userInfo valueForKey:@"aps"] valueForKey:@"alert"];
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"StackMob Message"
                                                            message:message
                                                           delegate:self
                                                  cancelButtonTitle:cancelTitle
                                                  otherButtonTitles:showTitle, nil];
        [alertView show];

    } else {
        //Do stuff that you would do if the application was not active
    }
}

@end
// Other methods below remain unedited
```


## Create Your View 

Select the storyboard file and drag and drop a **button** on to your view.  Change the button's label to  **Register Device**.
<br />
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-03.png">

<br />
<br />
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

Hold down the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-04.png">

<br />
<br />
In the menu, set the Connection to **Action** and enter the name **registerDevice**.
<br />
<br />
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-05.png">

## Edit ViewController.m

Add the following **highlighted** code to the **registerDevice:** method in your ViewController.m file:

<p>&nbsp;</p>


```obj-c,5

// The rest of the file remains unedited

- (IBAction)registerDevice:(id)sender {
    
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:UIRemoteNotificationTypeAlert];
    
}

@end
```

## Creating your Push Certificate

Before you can send push notifications, you need to create a Push Certificate for your app and upload it to StackMob.
<br>

Login to the <a href="http://developer.apple.com" target="_blank">Apple Developer Site</a> and click on the iOS DevCenter

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-06.png">
<br><br>

Click on the iOS Provisioning Portal

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-07.png">
<br><br>
We are going to assume you have your Developer Certificate and Device configured for development.  If you haven't, follow the directions in the Provisioning Portal.
<br/><br/>

Click on **App IDs** on the left menu, then click **New App ID** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-08.png">
<br><br>
Give your app a common name (which can be anything).  Next, enter the unique identifier for you app, 
This needs to match the name when you created the app in XCode.  In our example, it's **com.stackmob.push-notification**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-09.png">
<br><br>
Now that you've created you app, let's configure it for push.  Click the **configure link**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-10.png">
<br><br>
Check the box for **Enable for Apple Push Notification Service** and click the **Configure** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-11.png">
<br><br>
You'll be prompted to generate a **Certificate Signing Request (CSR)** from your **Keychain Access** Application.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-12.png">

<br><br>
Launch the **Keychain Access application** found in Applications > Utilities. From the **Keychain Access** menu, select **Certificate Assistant > Request a Certificate from a Certificate Authority**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-14.png">
<br><br>
Save the certificate to disk.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-15.png">
<br><br>
Go back to the **Apple Developer site** and click the continue button, if you haven't already.
Click **choose file** and select the certificate you just created and click **Generate**

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-16.png">
<br><br>
Click **continue** and **Download the aps_development.cer file** to your computer.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-17.png">
<br><br>
**Double-click the aps_development.cer** file and it will be added to your Keychain.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-18.png">

## Export Push Certificate

Next, you'll export the Push Certificate (.p12) file for upload to StackMob.

In the Keychain Access application, 
**right-click** on your Push Service certificate and select **Export Apple Development iOS Push Services:...**

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-19.png">
<br><br>
Give the file a unique name and save it as a .p12 file.  You'll be prompted to give it a password.  Make sure you remember the password, you'll need it when you upload the file to StackMob.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-20.png">
<br><br>
Open a browser to the StackMob Dashboard and navigate to <a href="https://dashboard.stackmob.com/module/push/settings/ios" target="_blank">Manage Push Credentials</a>.  
Click on **Upload New Certs** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-23.png">
<br><br>
Choose the .p12 file you created and **enter the certificate password**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-24.png">


## Create Provisioning Profile

Click the **Provisioning** link on the left side and click the **New Profile** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-25.png">
<br><br>
Enter a **Profile Name**, Check the box for your developer certificate, select your newly created **App ID** and the device you will use for testing.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-26.png">
<br><br>
Click the **Download** button and save the Provisioning file on your computer.


<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-27.png">
<br><br>
**Open the XCode Organizer** (located under the Window menu in XCode) and drag and drop the **.mobileprovision** file onto the Provisioning Profiles area.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-28.png">
<br><br>
Return to your app in XCode and under **Build Settings**, search for "signing".  Under **Code Signing Identity** debug, you'll select the mobile provisioning with the bundle name that matches your app.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-29.png">


## Build and Run!

**Very Important:** You must build and run on your device NOT the iOS Simulator.

Run your project, click the Register Device button.  Look in the debugger output window to see the token for your device.


## Send Push to your iOS Device

**Close your app running on your device  Push notifications won't appear if your app is active.**

Go to the StackMob <a href="https://dashboard.stackmob.com/module/push/console" target="_blank">Test Console</a> and select the **Broadcast** method. Enter a message and click send.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-32.png">

## Development vs. Production

Once you set your version to 1 and use the production keys, StackMob will use Apple's production push server 
instead of the Apple sandbox.

You should only use the dev keys with version 0 and the production keys with version 1.

The user tokens don't change, so you would still test in the same manner.

Here is some additional info on <a href="http://developer.stackmob.com/tutorials/dashboard/API-Versioning">API versioning with development and production environments</a>.


Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/push-notifications.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
